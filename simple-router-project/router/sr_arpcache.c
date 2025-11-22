#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>
#include "sr_arpcache.h"
#include "sr_router.h"
#include "sr_if.h"
#include "sr_rt.h"
#include "sr_protocol.h"
#include "sr_utils.h"

/* Forward declarations for helper functions */
static void handle_arpreq(struct sr_instance *sr, struct sr_arpreq *req);
static void send_arp_request_for_req(struct sr_instance *sr, struct sr_arpreq *req);
static void send_icmp_host_unreachable(struct sr_instance *sr, struct sr_arpreq *req);

/* Helper to print IP without newline */
static void print_ip_addr(uint32_t ip) {
    uint32_t octet = ip >> 24;
    fprintf(stderr, "%d.", octet);
    octet = (ip << 8) >> 24;
    fprintf(stderr, "%d.", octet);
    octet = (ip << 16) >> 24;
    fprintf(stderr, "%d.", octet);
    octet = (ip << 24) >> 24;
    fprintf(stderr, "%d", octet);
}

/* 
  This function gets called every second. For each request sent out, we keep
  checking whether we should resend an request or destroy the arp request.
  See the comments in the header file for an idea of what it should look like.
*/
void sr_arpcache_sweepreqs(struct sr_instance *sr) { 
    struct sr_arpreq *req = sr->cache.requests;
    struct sr_arpreq *next;
    
    while (req) {
        next = req->next;
        handle_arpreq(sr, req);
        req = next;
    }
}

/*---------------------------------------------------------------------
 * Method: handle_arpreq
 * Scope:  Static helper
 *
 * Handle an ARP request - resend if needed or timeout
 *
 *---------------------------------------------------------------------*/
static void handle_arpreq(struct sr_instance *sr, struct sr_arpreq *req)
{
    time_t now = time(NULL);
    
    if (difftime(now, req->sent) >= 1.0) {
        if (req->times_sent >= 5) {
            /* Timeout - send ICMP host unreachable to all waiting packets */
            send_icmp_host_unreachable(sr, req);
            sr_arpreq_destroy(&sr->cache, req);
        } else {
            /* Resend ARP request */
            send_arp_request_for_req(sr, req);
            req->sent = now;
            req->times_sent++;
        }
    }
}

/*---------------------------------------------------------------------
 * Method: send_arp_request_for_req
 * Scope:  Static helper
 *
 * Send ARP request for a queued request
 *
 *---------------------------------------------------------------------*/
static void send_arp_request_for_req(struct sr_instance *sr, struct sr_arpreq *req)
{
    if (!req->packets) {
        return;
    }
    
    /* Use the first packet's interface to send the ARP request */
    struct sr_packet *pkt = req->packets;
    struct sr_if *iface = sr_get_interface(sr, pkt->iface);
    
    if (!iface) {
        return;
    }
    
    unsigned int len = sizeof(sr_ethernet_hdr_t) + sizeof(sr_arp_hdr_t);
    uint8_t *arp_req = (uint8_t *)malloc(len);
    
    sr_ethernet_hdr_t *eth_hdr = (sr_ethernet_hdr_t *)arp_req;
    sr_arp_hdr_t *arp_hdr = (sr_arp_hdr_t *)(arp_req + sizeof(sr_ethernet_hdr_t));
    
    /* Ethernet header */
    memset(eth_hdr->ether_dhost, 0xFF, ETHER_ADDR_LEN);
    memcpy(eth_hdr->ether_shost, iface->addr, ETHER_ADDR_LEN);
    eth_hdr->ether_type = htons(ethertype_arp);
    
    /* ARP header */
    arp_hdr->ar_hrd = htons(arp_hrd_ethernet);
    arp_hdr->ar_pro = htons(ethertype_ip);
    arp_hdr->ar_hln = ETHER_ADDR_LEN;
    arp_hdr->ar_pln = 4;
    arp_hdr->ar_op = htons(arp_op_request);
    memcpy(arp_hdr->ar_sha, iface->addr, ETHER_ADDR_LEN);
    arp_hdr->ar_sip = iface->ip;
    memset(arp_hdr->ar_tha, 0x00, ETHER_ADDR_LEN);
    arp_hdr->ar_tip = req->ip;
    
    sr_send_packet(sr, arp_req, len, iface->name);
    free(arp_req);
}

/*---------------------------------------------------------------------
 * Method: send_icmp_host_unreachable
 * Scope:  Static helper
 *
 * Send ICMP host unreachable to all packets waiting on an ARP request
 *
 *---------------------------------------------------------------------*/
static void send_icmp_host_unreachable(struct sr_instance *sr, struct sr_arpreq *req)
{
    struct sr_packet *pkt = req->packets;
    
    while (pkt) {
        /* Extract IP header from queued packet */
        sr_ip_hdr_t *orig_ip = (sr_ip_hdr_t *)(pkt->buf + sizeof(sr_ethernet_hdr_t));
        
        /* Build ICMP host unreachable message */
        unsigned int icmp_len = sizeof(sr_ethernet_hdr_t) + sizeof(sr_ip_hdr_t) + sizeof(sr_icmp_t3_hdr_t);
        uint8_t *icmp_pkt = (uint8_t *)malloc(icmp_len);
        memset(icmp_pkt, 0, icmp_len);
        
        sr_ethernet_hdr_t *eth = (sr_ethernet_hdr_t *)icmp_pkt;
        sr_ip_hdr_t *ip = (sr_ip_hdr_t *)(icmp_pkt + sizeof(sr_ethernet_hdr_t));
        sr_icmp_t3_hdr_t *icmp = (sr_icmp_t3_hdr_t *)(icmp_pkt + sizeof(sr_ethernet_hdr_t) + sizeof(sr_ip_hdr_t));
        
        /* Find interface and route back to original sender */
        struct sr_if *out_iface = sr_get_interface(sr, pkt->iface);
        if (!out_iface) {
            free(icmp_pkt);
            pkt = pkt->next;
            continue;
        }
        
        /* IP header */
        ip->ip_v = 4;
        ip->ip_hl = 5;
        ip->ip_tos = 0;
        ip->ip_len = htons(sizeof(sr_ip_hdr_t) + sizeof(sr_icmp_t3_hdr_t));
        ip->ip_id = 0;
        ip->ip_off = 0;
        ip->ip_ttl = 64;
        ip->ip_p = ip_protocol_icmp;
        ip->ip_src = out_iface->ip;
        ip->ip_dst = orig_ip->ip_src;
        ip->ip_sum = 0;
        ip->ip_sum = cksum(ip, sizeof(sr_ip_hdr_t));
        
        /* ICMP header - type 3, code 1 (host unreachable) */
        icmp->icmp_type = 3;
        icmp->icmp_code = 1;
        icmp->unused = 0;
        icmp->next_mtu = 0;
        memcpy(icmp->data, orig_ip, ICMP_DATA_SIZE);
        icmp->icmp_sum = 0;
        icmp->icmp_sum = cksum(icmp, sizeof(sr_icmp_t3_hdr_t));
        
        /* Look up how to reach the original sender */
        struct sr_rt *rt = sr->routing_table;
        struct sr_rt *best = NULL;
        uint32_t best_mask = 0;
        
        while (rt) {
            uint32_t mask = rt->mask.s_addr;
            if ((orig_ip->ip_src & mask) == (rt->dest.s_addr & mask)) {
                if (ntohl(mask) > ntohl(best_mask)) {
                    best = rt;
                    best_mask = mask;
                }
            }
            rt = rt->next;
        }
        
        if (!best) {
            free(icmp_pkt);
            pkt = pkt->next;
            continue;
        }
        
        struct sr_if *send_iface = sr_get_interface(sr, best->interface);
        if (!send_iface) {
            free(icmp_pkt);
            pkt = pkt->next;
            continue;
        }
        
        /* Determine next hop */
        uint32_t next_hop = (best->gw.s_addr) ? best->gw.s_addr : orig_ip->ip_src;
        
        /* Check ARP cache */
        struct sr_arpentry *arp_entry = sr_arpcache_lookup(&sr->cache, next_hop);
        
        if (arp_entry) {
            memcpy(eth->ether_dhost, arp_entry->mac, ETHER_ADDR_LEN);
            memcpy(eth->ether_shost, send_iface->addr, ETHER_ADDR_LEN);
            eth->ether_type = htons(ethertype_ip);
            
            sr_send_packet(sr, icmp_pkt, icmp_len, send_iface->name);
            free(arp_entry);
            free(icmp_pkt);
        } else {
            /* Queue it - but don't create infinite loop */
            free(icmp_pkt);
        }
        
        pkt = pkt->next;
    }
}

/* You should not need to touch the rest of this code. */

/* Checks if an IP->MAC mapping is in the cache. IP is in network byte order.
   You must free the returned structure if it is not NULL. */
struct sr_arpentry *sr_arpcache_lookup(struct sr_arpcache *cache, uint32_t ip) {
    pthread_mutex_lock(&(cache->lock));
    
    struct sr_arpentry *entry = NULL, *copy = NULL;
    
    int i;
    for (i = 0; i < SR_ARPCACHE_SZ; i++) {
        if ((cache->entries[i].valid) && (cache->entries[i].ip == ip)) {
            entry = &(cache->entries[i]);
        }
    }
    
    /* Must return a copy b/c another thread could jump in and modify
       table after we return. */
    if (entry) {
        copy = (struct sr_arpentry *) malloc(sizeof(struct sr_arpentry));
        memcpy(copy, entry, sizeof(struct sr_arpentry));
    }
        
    pthread_mutex_unlock(&(cache->lock));
    
    return copy;
}

/* Adds an ARP request to the ARP request queue. If the request is already on
   the queue, adds the packet to the linked list of packets for this sr_arpreq
   that corresponds to this ARP request. You should free the passed *packet.
   
   A pointer to the ARP request is returned; it should not be freed. The caller
   can remove the ARP request from the queue by calling sr_arpreq_destroy. */
struct sr_arpreq *sr_arpcache_queuereq(struct sr_arpcache *cache,
                                       uint32_t ip,
                                       uint8_t *packet,           /* borrowed */
                                       unsigned int packet_len,
                                       char *iface)
{
    pthread_mutex_lock(&(cache->lock));
    
    struct sr_arpreq *req;
    for (req = cache->requests; req != NULL; req = req->next) {
        if (req->ip == ip) {
            break;
        }
    }
    
    /* If the IP wasn't found, add it */
    if (!req) {
        req = (struct sr_arpreq *) calloc(1, sizeof(struct sr_arpreq));
        req->ip = ip;
        req->next = cache->requests;
        cache->requests = req;
    }
    
    /* Add the packet to the list of packets for this request */
    if (packet && packet_len && iface) {
        struct sr_packet *new_pkt = (struct sr_packet *)malloc(sizeof(struct sr_packet));
        
        new_pkt->buf = (uint8_t *)malloc(packet_len);
        memcpy(new_pkt->buf, packet, packet_len);
        new_pkt->len = packet_len;
		new_pkt->iface = (char *)malloc(sr_IFACE_NAMELEN);
        strncpy(new_pkt->iface, iface, sr_IFACE_NAMELEN);
        new_pkt->next = req->packets;
        req->packets = new_pkt;
    }
    
    pthread_mutex_unlock(&(cache->lock));
    
    return req;
}

/* This method performs two functions:
   1) Looks up this IP in the request queue. If it is found, returns a pointer
      to the sr_arpreq with this IP. Otherwise, returns NULL.
   2) Inserts this IP to MAC mapping in the cache, and marks it valid. */
struct sr_arpreq *sr_arpcache_insert(struct sr_arpcache *cache,
                                     unsigned char *mac,
                                     uint32_t ip)
{
    pthread_mutex_lock(&(cache->lock));
    
    struct sr_arpreq *req, *prev = NULL, *next = NULL; 
    for (req = cache->requests; req != NULL; req = req->next) {
        if (req->ip == ip) {            
            if (prev) {
                next = req->next;
                prev->next = next;
            } 
            else {
                next = req->next;
                cache->requests = next;
            }
            
            break;
        }
        prev = req;
    }
    
    int i;
    for (i = 0; i < SR_ARPCACHE_SZ; i++) {
        if (!(cache->entries[i].valid))
            break;
    }
    
    if (i != SR_ARPCACHE_SZ) {
        memcpy(cache->entries[i].mac, mac, 6);
        cache->entries[i].ip = ip;
        cache->entries[i].added = time(NULL);
        cache->entries[i].valid = 1;
    }
    
    pthread_mutex_unlock(&(cache->lock));
    
    return req;
}

/* Frees all memory associated with this arp request entry. If this arp request
   entry is on the arp request queue, it is removed from the queue. */
void sr_arpreq_destroy(struct sr_arpcache *cache, struct sr_arpreq *entry) {
    pthread_mutex_lock(&(cache->lock));
    
    if (entry) {
        struct sr_arpreq *req, *prev = NULL, *next = NULL; 
        for (req = cache->requests; req != NULL; req = req->next) {
            if (req == entry) {                
                if (prev) {
                    next = req->next;
                    prev->next = next;
                } 
                else {
                    next = req->next;
                    cache->requests = next;
                }
                
                break;
            }
            prev = req;
        }
        
        struct sr_packet *pkt, *nxt;
        
        for (pkt = entry->packets; pkt; pkt = nxt) {
            nxt = pkt->next;
            if (pkt->buf)
                free(pkt->buf);
            if (pkt->iface)
                free(pkt->iface);
            free(pkt);
        }
        
        free(entry);
    }
    
    pthread_mutex_unlock(&(cache->lock));
}

/* Prints out the ARP table. */
void sr_arpcache_dump(struct sr_arpcache *cache) {
    fprintf(stderr, "\nMAC            IP         ADDED                      VALID\n");
    fprintf(stderr, "-----------------------------------------------------------\n");
    
    int i;
    for (i = 0; i < SR_ARPCACHE_SZ; i++) {
        struct sr_arpentry *cur = &(cache->entries[i]);
        unsigned char *mac = cur->mac;
        fprintf(stderr, "%.1x%.1x%.1x%.1x%.1x%.1x   %.8x   %.24s   %d\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], ntohl(cur->ip), ctime(&(cur->added)), cur->valid);
    }
    
    fprintf(stderr, "\n");
}

/* Initialize table + table lock. Returns 0 on success. */
int sr_arpcache_init(struct sr_arpcache *cache) {  
    /* Seed RNG to kick out a random entry if all entries full. */
    srand(time(NULL));
    
    /* Invalidate all entries */
    memset(cache->entries, 0, sizeof(cache->entries));
    cache->requests = NULL;
    
    /* Acquire mutex lock */
    pthread_mutexattr_init(&(cache->attr));
    pthread_mutexattr_settype(&(cache->attr), PTHREAD_MUTEX_RECURSIVE);
    int success = pthread_mutex_init(&(cache->lock), &(cache->attr));
    
    return success;
}

/* Destroys table + table lock. Returns 0 on success. */
int sr_arpcache_destroy(struct sr_arpcache *cache) {
    return pthread_mutex_destroy(&(cache->lock)) && pthread_mutexattr_destroy(&(cache->attr));
}

/* Thread which sweeps through the cache and invalidates entries that were added
   more than SR_ARPCACHE_TO seconds ago. */
void *sr_arpcache_timeout(void *sr_ptr) {
    struct sr_instance *sr = sr_ptr;
    struct sr_arpcache *cache = &(sr->cache);
    
    while (1) {
        sleep(1.0);
        
        pthread_mutex_lock(&(cache->lock));
    
        time_t curtime = time(NULL);
        
        int i;    
        for (i = 0; i < SR_ARPCACHE_SZ; i++) {
            if ((cache->entries[i].valid) && (difftime(curtime,cache->entries[i].added) > SR_ARPCACHE_TO)) {
                cache->entries[i].valid = 0;
            }
        }
        
        sr_arpcache_sweepreqs(sr);

        pthread_mutex_unlock(&(cache->lock));
    }
    
    return NULL;
}

