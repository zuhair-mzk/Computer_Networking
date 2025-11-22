# Lab 09: Simple Router Implementation

## Overview
Practical implementation and testing of a functional software router with ARP and ICMP support.

## Learning Objectives
- Understanding router architecture
- Implementing packet forwarding logic
- Handling ARP protocol
- Generating ICMP messages

## Files
- `D58_Lab09.pdf` - Lab specification and requirements

## Key Components
1. Routing table with LPM lookup
2. ARP cache management
3. ICMP echo reply
4. TTL handling and checksum recalculation

## Testing
```bash
# Start router environment
./run_mininet.sh  # Terminal 1
./run_pox.sh      # Terminal 2
./sr              # Terminal 3

# Test connectivity
mininet> pingall
mininet> h1 ping h2
mininet> h1 traceroute h3
```

## Implementation Checklist
- [ ] ARP request/reply handling
- [ ] IP packet forwarding
- [ ] ICMP echo reply
- [ ] TTL decrement
- [ ] Checksum recalculation
- [ ] LPM routing table lookup

See also: [../../simple-router/](../../simple-router/) for full project details.
