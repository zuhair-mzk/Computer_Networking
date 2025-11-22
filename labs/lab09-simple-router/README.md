# Lab 09: Simple Router Implementation

## Overview
Comprehensive hands-on implementation of a functional software router in C/C++. This lab involved building a router from scratch that handles real IP packets in a Mininet environment, implementing core networking functionality including packet forwarding, ARP resolution, and ICMP handling.

## Learning Objectives
- Understanding router architecture and packet processing pipeline
- Implementing IP packet forwarding with longest prefix matching
- Handling ARP protocol for Layer 2 address resolution
- Generating appropriate ICMP messages for network events
- Managing router data structures (routing table, ARP cache)
- Debugging network protocols at a low level
- Testing router functionality in a virtual network environment

## Implementation Components

### 1. Packet Reception and Processing
- Receiving raw Ethernet frames from network interfaces
- Parsing Ethernet headers to determine frame type (ARP or IP)
- Validating packet integrity and minimum length requirements
- Dispatching packets to appropriate handlers based on protocol type

### 2. ARP (Address Resolution Protocol)
**ARP Request Handling**:
- Receive ARP requests for router's interfaces
- Construct and send ARP replies with appropriate MAC address
- Update ARP cache with sender's information

**ARP Reply Handling**:
- Process incoming ARP replies
- Update ARP cache with IP-to-MAC mappings
- Forward queued packets waiting for ARP resolution

**ARP Cache Management**:
- Maintain cache of IP-to-MAC address mappings with timeouts (typically 15s)
- Send ARP requests for unknown MAC addresses
- Queue packets while waiting for ARP replies
- Clean up expired cache entries

### 3. IP Packet Forwarding
**Packet Validation**:
- Minimum length check (20 bytes for header)
- IPv4 version check
- Header checksum verification
- TTL check (must be > 1 to forward)

**Forwarding Logic**:
1. Check if packet is destined for router (ICMP echo)
2. Perform longest prefix match on destination IP
3. Look up next hop MAC address in ARP cache
4. If MAC unknown, send ARP request and queue packet
5. Decrement TTL and recalculate IP checksum
6. Update Ethernet header with new source/dest MAC
7. Send packet out appropriate interface

**Longest Prefix Match Algorithm**:
- Iterate through routing table entries
- Apply netmask to destination IP
- Compare with route prefix
- Select route with longest matching prefix (most specific)

### 4. ICMP (Internet Control Message Protocol)
**ICMP Echo Reply (Type 0)**:
- Respond to ping requests destined for router's IP addresses
- Swap source/destination IPs
- Change ICMP type from 8 (request) to 0 (reply)
- Recalculate ICMP checksum

**ICMP Time Exceeded (Type 11)**:
- Sent when packet TTL reaches 0 or 1
- Include original IP header + 8 bytes of data in payload

**ICMP Destination Unreachable (Type 3)**:
- Net unreachable (Code 0): No matching route
- Host unreachable (Code 1): No ARP reply after retries
- Port unreachable (Code 3): UDP/TCP to router's IP

### 5. Checksum Calculation
**IP Checksum**:
- Implemented RFC 791 checksum algorithm
- Sum all 16-bit words in IP header
- Add carry bits back into sum
- Take one's complement of result
- Recalculated after TTL decrement

**ICMP Checksum**:
- Calculated over ICMP header and payload
- Set checksum field to 0 before calculation
- Verified with Wireshark packet captures

## Testing Procedures

### Test Environment
Mininet topology with multiple hosts connected through the router on different subnets.

### Test Cases Performed

**Test 1: ARP Request/Reply**
- Cleared ARP cache, sent ping to router interface
- Verified ARP request is broadcast (ff:ff:ff:ff:ff:ff)
- Verified ARP reply has correct MAC and is unicast

**Test 2: ICMP Echo (Ping to Router)**
- Pinged router's IP addresses
- Verified router generates ICMP echo replies
- Checked correct ICMP type (0) and code (0)
- Confirmed proper checksum calculation

**Test 3: Packet Forwarding Between Hosts**
- Pinged between hosts on different subnets through router
- Verified TTL decremented by 1 at each hop
- Confirmed checksums recalculated correctly
- Tested bidirectional communication

**Test 4: TTL Expiry**
- Sent packets with TTL=1 to trigger expiry
- Verified router sends ICMP time exceeded
- Confirmed original IP header included in ICMP payload

**Test 5: Traceroute**
- Ran traceroute from h1 to h2
- Verified router appears as intermediate hop
- Confirmed ICMP time exceeded sent for incremental TTL values

**Test 6: Destination Unreachable**
- Pinged non-existent network (no matching route)
- Verified router sends ICMP destination net unreachable
- Tested ICMP type 3, code 0

**Test 7: Full Connectivity**
- Ran pingall in Mininet
- Verified 100% reachability between all hosts
- Confirmed no packet loss

## Debugging Techniques

1. **Verbose Logging**: Enabled detailed packet processing logs
2. **Wireshark Packet Capture**: Analyzed Ethernet/IP/ICMP headers
3. **Print Debugging**: Added debug prints throughout code
4. **Data Structure Inspection**: Printed routing tables and ARP cache
5. **Incremental Testing**: Tested each component separately before integration

## Implementation Checklist

### Core Functionality
- ✅ Receive and parse Ethernet frames
- ✅ Handle ARP requests (respond with router's MAC)
- ✅ Handle ARP replies (update cache, forward queued packets)
- ✅ Maintain ARP cache with timeouts
- ✅ Send ARP requests for unknown destinations
- ✅ Queue packets waiting for ARP resolution
- ✅ Validate IP packets (checksum, length, version, TTL)
- ✅ Perform longest prefix match on routing table
- ✅ Forward IP packets to correct next hop
- ✅ Decrement TTL and recalculate checksums

### ICMP Implementation
- ✅ ICMP echo reply (Type 0) - respond to pings
- ✅ ICMP time exceeded (Type 11) - TTL expired
- ✅ ICMP destination unreachable (Type 3, various codes)
- ✅ Include original IP header + 8 bytes in ICMP payload
- ✅ Calculate ICMP checksums correctly

## Key Challenges and Solutions

**Challenge 1: Checksum Calculation**
- **Solution**: Carefully implemented RFC 791 algorithm, ensured proper byte ordering

**Challenge 2: ARP Cache Race Conditions**
- **Solution**: Implemented packet queue per pending ARP request, only one ARP request per IP

**Challenge 3: ICMP Payload Construction**
- **Solution**: Read RFC 792 carefully, included full IP header + 8 data bytes

**Challenge 4: Longest Prefix Match**
- **Solution**: Drew binary representations, tested with various routing configurations

**Challenge 5: Interface Management**
- **Solution**: Verified routing table specifies correct interfaces, checked interface lookups

## Key Concepts Learned

1. **Protocol Details Matter**: Small header mistakes cause silent failures
2. **Checksums Are Critical**: Incorrect checksums lead to packet drops
3. **State Management**: ARP cache and packet queues require careful design
4. **Debugging Tools**: Wireshark and logging essential for finding bugs
5. **Incremental Development**: Testing each component separately saves time
6. **Standards Compliance**: Following RFCs ensures interoperability

## Related Documentation

For complete implementation documentation, build instructions, and additional details:
- [../../simple-router/](../../simple-router/) - Full Simple Router project documentation

## Practical Skills Developed

- Low-level network programming in C
- Understanding router internals
- Protocol implementation from RFCs
- Network debugging techniques
- Systems programming (pointers, memory management, data structures)
- Applicable to SDN, embedded networking, firewalls, and troubleshooting
