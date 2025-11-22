# Simple Router Project

## 📋 Overview

This is a functional software router implementation that handles real IP packets in a Mininet virtual network environment. The router implements core networking functionality including ARP resolution, IP forwarding with longest prefix matching, and ICMP message handling.

---

## 🎯 Features Implemented

### ✅ Core Router Functionality
- **IP Packet Forwarding:** Routes packets between network interfaces based on routing table
- **Longest Prefix Match (LPM):** Efficient routing table lookup algorithm
- **Checksum Validation:** Verifies IP header checksums on incoming packets
- **TTL Handling:** Decrements TTL and generates ICMP time exceeded when TTL reaches 0
- **Checksum Recalculation:** Updates checksums after header modifications

### ✅ ARP (Address Resolution Protocol)
- **ARP Cache Management:** Maintains mapping between IP and MAC addresses
- **ARP Request Generation:** Sends ARP requests for unknown MAC addresses
- **ARP Reply Handling:** Processes ARP responses and updates cache
- **ARP Response Generation:** Responds to ARP requests for router interfaces
- **Packet Queueing:** Queues packets waiting for ARP resolution

### ✅ ICMP (Internet Control Message Protocol)
- **Echo Reply (Type 0):** Responds to ping requests
- **Destination Unreachable (Type 3):**
  - Network unreachable
  - Host unreachable
  - Port unreachable
- **Time Exceeded (Type 11):** Sent when TTL expires

---

## 🏗️ Architecture

```
┌──────────────────────────────────────────────────────────────┐
│                     Simple Router                            │
│                                                              │
│  ┌─────────────┐        ┌──────────────┐                    │
│  │  Interface  │        │ ARP Cache    │                    │
│  │  eth0       │◄───────┤ IP → MAC     │                    │
│  │  10.0.1.1   │        │ Timeout: 15s │                    │
│  └─────────────┘        └──────────────┘                    │
│                                                              │
│  ┌─────────────┐        ┌──────────────┐                    │
│  │  Interface  │        │ Routing Table│                    │
│  │  eth1       │◄───────┤ Prefix|Gw|If│                    │
│  │  10.0.2.1   │        │ (LPM Lookup) │                    │
│  └─────────────┘        └──────────────┘                    │
│                                                              │
│  ┌──────────────────────────────────────────────────────┐   │
│  │         Packet Processing Pipeline                   │   │
│  │                                                      │   │
│  │  1. Receive packet from interface                   │   │
│  │     ↓                                                │   │
│  │  2. Check if Ethernet frame is ARP or IP            │   │
│  │     ↓                                                │   │
│  │  3. If ARP: Process ARP request/reply               │   │
│  │     ↓                                                │   │
│  │  4. If IP: Validate checksum, check TTL             │   │
│  │     ↓                                                │   │
│  │  5. Check if packet is for router (ICMP echo)       │   │
│  │     ↓                                                │   │
│  │  6. Lookup routing table (longest prefix match)     │   │
│  │     ↓                                                │   │
│  │  7. Check ARP cache for next hop MAC                │   │
│  │     ↓                                                │   │
│  │  8. If MAC not found: send ARP request, queue pkt   │   │
│  │     ↓                                                │   │
│  │  9. Decrement TTL, recalculate checksum             │   │
│  │     ↓                                                │   │
│  │  10. Forward packet to next hop                     │   │
│  └──────────────────────────────────────────────────────┘   │
└──────────────────────────────────────────────────────────────┘
```

---

## 📁 File Structure

```
simple-router/
├── README.md              # This file
├── src/                   # Router source code
│   ├── sr_router.c        # Main router logic
│   ├── sr_router.h        # Router header
│   ├── sr_arpcache.c      # ARP cache implementation
│   ├── sr_arpcache.h      # ARP cache header
│   ├── sr_if.c            # Interface handling
│   ├── sr_if.h            # Interface header
│   ├── sr_rt.c            # Routing table
│   ├── sr_rt.h            # Routing table header
│   ├── sr_utils.c         # Utility functions
│   ├── sr_utils.h         # Utility headers
│   ├── sr_protocol.h      # Protocol definitions
│   └── Makefile           # Build configuration
│
├── docs/                  # Documentation
│   ├── design.md          # Design document
│   └── testing.md         # Testing procedures
│
├── rtable                 # Routing table configuration
├── config.sh              # Environment setup script
├── run_mininet.sh         # Start Mininet topology
└── run_pox.sh             # Start POX controller
```

---

## 🚀 Setup & Installation

### Prerequisites

```bash
# Install Mininet
sudo apt-get install mininet

# Install POX controller
git clone http://github.com/noxrepo/pox
cd pox

# Install build tools
sudo apt-get install build-essential
```

### Building the Router

```bash
cd simple-router/src
make clean
make
```

This will produce an executable called `sr`.

---

## ▶️ Running the Router

### Step 1: Configure Environment
```bash
./config.sh
```
This script sets up the necessary environment variables and paths.

### Step 2: Start Mininet Topology
Open a terminal and run:
```bash
./run_mininet.sh
```

This creates a network topology with:
- Multiple hosts (h1, h2, h3, ...)
- Router interfaces connecting different subnets
- Default gateway configurations

### Step 3: Start POX Controller
Open a **new terminal** and run:
```bash
./run_pox.sh
```

The POX controller manages the OpenFlow switches in the Mininet topology.

### Step 4: Start the Router
Open **another terminal** and run:
```bash
cd simple-router/src
./sr
```

Optional flags:
```bash
# Verbose logging
./sr -v

# Load specific routing table
./sr -r rtable

# Specify server and topology
./sr -s localhost -t 1
```

---

## 🧪 Testing the Router

### Basic Connectivity Tests

#### 1. Test Ping (ICMP Echo)
```bash
# In Mininet CLI
mininet> h1 ping -c 3 10.0.1.1    # Ping router interface
mininet> h1 ping -c 3 h2           # Ping through router
```

**Expected behavior:**
- Router receives ICMP echo request
- Router sends ICMP echo reply
- Round-trip time displayed

#### 2. Test ARP Resolution
```bash
# Clear ARP cache and ping
mininet> h1 ip -s -s neigh flush all
mininet> h1 ping -c 1 h2

# View ARP cache
mininet> h1 arp -n
```

**Expected behavior:**
- ARP request sent for next hop
- Router responds with ARP reply
- Packet forwarded after MAC resolution

#### 3. Test Routing Between Subnets
```bash
mininet> h1 ping h3
mininet> pingall
```

**Expected behavior:**
- Packets routed through multiple interfaces
- Routing table lookups work correctly
- All hosts can communicate

#### 4. Test Traceroute
```bash
mininet> h1 traceroute h3
```

**Expected behavior:**
- Shows router as intermediate hop
- TTL decremented at each hop
- Complete path displayed

### Advanced Tests

#### 5. Test TTL Expiry
```bash
# Ping with TTL=1 (should get time exceeded)
mininet> h1 ping -t 1 h2
```

**Expected behavior:**
- Router decrements TTL to 0
- Router sends ICMP time exceeded (Type 11)

#### 6. Test Invalid Destinations
```bash
# Ping non-existent host
mininet> h1 ping 192.168.99.99
```

**Expected behavior:**
- Router has no matching route
- Router sends ICMP destination unreachable (Type 3)

#### 7. Monitor with Wireshark
```bash
# In separate terminal
sudo wireshark &

# Select interface to monitor (e.g., eth0, s1-eth1)
# Filter: arp or icmp or ip
```

**What to observe:**
- ARP request/reply exchanges
- ICMP echo request/reply
- TTL changes in IP headers
- Checksum recalculation

---

## 📊 Routing Table Format

The routing table (`rtable`) contains static routes in the following format:

```
# Destination    Gateway         Netmask         Interface
10.0.1.0        10.0.1.1        255.255.255.0   eth0
10.0.2.0        10.0.2.1        255.255.255.0   eth1
0.0.0.0         10.0.1.100      0.0.0.0         eth0     # Default route
```

### Longest Prefix Match Algorithm
1. Convert destination IP to binary
2. For each route entry:
   - Apply netmask with bitwise AND
   - Compare result with destination prefix
3. Select route with longest matching prefix (most specific)
4. Return gateway and outgoing interface

**Example:**
```
Destination: 10.0.1.5
Routes:
  10.0.1.0/24  → Matches (24 bits)
  10.0.0.0/16  → Matches (16 bits)
  0.0.0.0/0    → Matches (0 bits, default)
  
Selected: 10.0.1.0/24 (most specific, longest prefix)
```

---

## 🐛 Debugging Tips

### Enable Verbose Logging
```bash
./sr -v
```
Shows detailed packet processing information.

### Common Issues

#### 1. **Router not forwarding packets**
- **Check:** Routing table loaded correctly
- **Solution:** Verify `rtable` file path and format
- **Command:** Print routing table in router output

#### 2. **ARP requests timing out**
- **Check:** ARP cache timeout settings
- **Solution:** Ensure ARP request function is being called
- **Debug:** Print ARP cache contents periodically

#### 3. **Checksum errors**
- **Check:** Checksum calculation after TTL decrement
- **Solution:** Recalculate checksum for modified IP headers
- **Tool:** Verify with Wireshark

#### 4. **ICMP not working**
- **Check:** ICMP type and code values
- **Solution:** Ensure proper ICMP header construction
- **Verify:** Compare with RFC 792 specifications

### Useful Debug Commands

```bash
# View router interfaces
mininet> router ifconfig

# Check connectivity
mininet> pingall

# View ARP cache on host
mininet> h1 arp -n

# Monitor specific interface
mininet> h1 tcpdump -i h1-eth0 -n

# Test with specific TTL
mininet> h1 ping -t <ttl> h2
```

---

## 📚 Protocol Specifications

- **ARP:** RFC 826 - Address Resolution Protocol
- **ICMP:** RFC 792 - Internet Control Message Protocol
- **IP:** RFC 791 - Internet Protocol
- **Ethernet:** IEEE 802.3

---

## ✅ Testing Checklist

Use this checklist to verify router functionality:

- [ ] Router initializes and loads routing table
- [ ] Router interfaces are configured with correct IP addresses
- [ ] ARP request sent for unknown MAC addresses
- [ ] ARP replies are processed and cache is updated
- [ ] Router responds to ARP requests for its interfaces
- [ ] Router responds to ping (ICMP echo request)
- [ ] IP packets are forwarded to correct interface
- [ ] TTL is decremented on forwarded packets
- [ ] Checksums are recalculated correctly
- [ ] ICMP time exceeded sent when TTL = 0
- [ ] ICMP destination unreachable sent for invalid routes
- [ ] Longest prefix match works correctly
- [ ] Multiple hosts can communicate through router
- [ ] Traceroute shows router as intermediate hop

---

## 🔍 Performance Considerations

### ARP Cache Timeout
- Default: 15 seconds
- Reduces unnecessary ARP traffic
- Balances cache freshness vs. network overhead

### Packet Queue Management
- Queues packets awaiting ARP resolution
- Prevents packet drops during ARP lookup
- FIFO queue with timeout mechanism

### Routing Table Lookup
- O(n) linear search with prefix comparison
- Can be optimized with trie or radix tree for large tables
- Static routing table for simplicity

---

## 📖 Additional Resources

- **Mininet Walkthrough:** http://mininet.org/walkthrough/
- **POX Documentation:** https://github.com/noxrepo/pox
- **RFC 791 (IP):** https://tools.ietf.org/html/rfc791
- **RFC 792 (ICMP):** https://tools.ietf.org/html/rfc792
- **RFC 826 (ARP):** https://tools.ietf.org/html/rfc826

---

## 👨‍💻 Implementation Notes

This router implementation:
- Written in C for performance and low-level control
- Uses raw sockets for packet capture and transmission
- Implements basic router functionality for educational purposes
- Does not include advanced features like NAT, firewall rules, or dynamic routing

For production use, consider:
- Adding IPv6 support
- Implementing dynamic routing protocols (OSPF, BGP)
- Adding security features (firewall, ACLs)
- Performance optimizations (multi-threading, zero-copy)

---

**Last Updated:** November 2025
