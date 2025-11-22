# Simple Router  
**CSCD58 – Computer Networks**  
**Assignment 3 (Fall 2025)**  

---

## Authors
- **Zuhair Khan** — 1008882933 — zuhair.khan@mail.utoronto.ca  
- **Muhammad Bilal** — 1009041010 — mb.bilal@mail.utoronto.ca  

---

## Table of Contents
1. [Contributions](#contributions)  
2. [Descriptions of Functions](#descriptions-of-functions)  
3. [Test Cases & Results](#test-cases--results)  

---

## Contributions

### **Zuhair Khan**
Implemented all **IP-layer functionality** of the router, including forwarding logic, ICMP generation, and helper utilities.  

#### Main Responsibilities
- **IP Packet Handling & Forwarding (`sr_router.c`)**  
  - Validate IP version, header length, and checksum.  
  - Distinguish between local-delivery and forwarding cases.  
  - Decrement TTL, recompute checksum, and perform **Longest Prefix Match (LPM)** routing.  
  - Generate *ICMP Time Exceeded* or *Network Unreachable* when appropriate.  

- **ICMP Protocol Handling**  
  - Implemented **Echo Reply** (Type 0) for router interfaces.  
  - Implemented **Error Messages** (Type 3 & Type 11) — net/host/port unreachable and TTL expired.  
  - Built helper routines for IP + ICMP checksum computation (RFC 791/792 compliant).  

- **Utility & Helper Functions**  
  - `ip_checksum()` — 16-bit one’s-complement checksum.  
  - `lpm_lookup()` — returns best route entry.  
  - `sr_get_interface_by_ip()` — maps router IP to interface struct.  
  - `forward_ip_packet()` — central forwarding path integrating ARP cache lookup.  

---

### **Muhammad Bilal**
Implemented all **ARP-layer functionality** and **ARP cache maintenance** with timeout and retry logic.  

#### Main Responsibilities
- **ARP Packet Processing (`sr_router.c`)**  
  - Handle incoming **ARP Requests** and **Replies**.  
  - Send replies when target IP matches any router interface.  
  - Insert mappings into ARP cache and release queued packets.  

- **ARP Cache & Request Queue (`sr_arpcache.c`)**  
  - Maintain 15-second cache expiry.  
  - Retry unsatisfied ARP Requests every 1 s (up to 5 times).  
  - Send *ICMP Host Unreachable* to all queued packets after final timeout.  

- **Key Functions**
  - `sr_arpcache_sweepreqs()` – periodic queue maintenance.  
  - `handle_arpreq()` – retry/timeout handling for one entry.  
  - `send_arp_request()` / `send_arp_reply()` – build & transmit ARP packets.  

---

## Descriptions of Functions

### **Packet Dispatch**
- **`sr_handlepacket(sr, packet, len, iface)`**  
  Entry point for all frames. Checks EtherType → delegates to `handle_arp_packet` or `handle_ip_packet`.

### **IP Path**
- **`handle_ip_packet()`**  
  Parses and validates IP header, verifies checksum, and decides whether the packet is destined for the router or needs forwarding.  
  - If ICMP echo to router → reply.  
  - If TCP/UDP to router → port unreachable.  
  - Else → forward.  

- **`forward_ip_packet()`**  
  Handles TTL and LPM; sends *Time Exceeded* or *Net Unreachable* as needed; interacts with ARP cache for next-hop resolution.  

- **`send_icmp_echo_reply()` / `send_icmp_t3()`**  
  Build and send ICMP reply or error messages; recompute checksums and swap headers.  

### **ARP Path**
- **`handle_arp_packet()`**  
  Processes requests and replies.  
  - On Request: replies if target IP matches router interface.  
  - On Reply: updates cache and flushes waiting packets.  

- **`send_arp_request()` / `send_arp_reply()`**  
  Constructs broadcast requests and unicast replies with proper Ethernet headers.  

### **Cache Maintenance**
- **`sr_arpcache_sweepreqs()` + `handle_arpreq()`**  
  Retry unsatisfied ARP requests every second, destroy and notify after 5 failures.  
- **`send_icmp_host_unreachable()`**  
  Called when ARP fails → sends ICMP type 3 code 1 to original sender.  

---

## Test Cases & Results

All tests executed inside the provided **Mininet VM** using the `run_mininet.sh` (topology) and `run_pox.sh` (controller) scripts.

### **Setup**
```bash
# Terminal A
./run_mininet.sh

# Terminal B
./run_pox.sh

# Terminal C
cd ~/cs144_lab3/router
make clean && make
./sr
```

---

## Additional Test Details & Build Instructions

### 1️⃣ Ping Router Interfaces
```
client ping -c 3 10.0.1.1
client ping -c 3 192.168.2.1
client ping -c 3 172.64.3.1
```
**Expected:** All three interfaces respond with ttl=64, 0% loss.
**Validates:** ICMP echo reply generation (Type 0).

### 2️⃣ Ping Across Router (Network Forwarding + ARP)
```
client ping -c 3 192.168.2.2
client ping -c 3 172.64.3.10
```
**Expected:** Initial packet triggers ARP, subsequent pings succeed.
**Validates:** Forwarding path, ARP request/reply logic, cache updates.

### 3️⃣ Traceroute Through Router
```
client traceroute -n 192.168.2.2
```
**Expected:** Hop 1 = 10.0.1.1, Hop 2 = 192.168.2.2.
**Validates:** TTL decrement, ICMP Time Exceeded (Type 11 Code 0).

### 4️⃣ HTTP Download
```
client wget http://192.168.2.2
```
**Expected:** 200 OK, file saved as index.html.
**Validates:** End-to-end forwarding for TCP flows.

### 5️⃣ No Route → Destination Network Unreachable
```
client ping -c 3 1.2.3.4
```
**Expected:** From 10.0.1.1 Destination Net Unreachable.
**Validates:** ICMP Type 3 Code 0.

### 6️⃣ Port Unreachable (TCP/UDP to Router)
```
client telnet 10.0.1.1 23
```
**Expected:** Connection refused.
**Validates:** ICMP Type 3 Code 3 for closed ports.

### 7️⃣ ARP Timeout → Destination Host Unreachable
```
sw0 ifconfig sw0-eth1 down
client ping -c 3 192.168.2.2
sw0 ifconfig sw0-eth1 up
```
**Expected:** After 5 ARP retries, ICMP Type 3 Code 1 messages.
**Validates:** Retry interval and timeout logic.

### 8️⃣ Stress / Concurrency Test
```
client ping -c 10 192.168.2.2 &
client ping -c 10 172.64.3.10 &
```
**Expected:** All flows succeed, no packet loss.
**Validates:** Thread-safe ARP cache and robust forwarding.

### Build Instructions
```
cd router
make clean && make
./sr
```
Produces executable sr with debug prints matching reference format.

### Key Features Implemented
- IPv4 forwarding with LPM and TTL management.
- Complete ARP stack (request, reply, cache, timeouts).
- ICMP echo reply and error generation.
- Packet queueing during ARP resolution.
- Thread-safe ARP cache sweeper (1 Hz).
- Clean packet logging for debug consistency.
