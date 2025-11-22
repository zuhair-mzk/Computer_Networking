# Lab 05: Routing and Switching

## Overview
Comprehensive hands-on exploration of Layer 2 switching and Layer 3 routing in software-defined networks. This lab focused on understanding how packets are forwarded through switches and routers, and how network devices learn and maintain forwarding information.

## Learning Objectives
- Understanding MAC address learning in switches
- Implementing and observing ARP protocol operation
- Configuring static routing tables
- Analyzing multi-hop packet forwarding
- Working with SDN controllers (POX)
- Measuring network performance in complex topologies

## Topics Covered

### 1. Layer 2 Switching
- **MAC Address Learning**: How switches build forwarding tables
- **Frame Forwarding**: Unicast, broadcast, and multicast forwarding
- **Switch Flooding**: Behavior when destination MAC is unknown
- **MAC Address Tables**: Viewing and understanding switch CAM tables

### 2. Address Resolution Protocol (ARP)
- **ARP Request/Reply Mechanism**: Mapping IP addresses to MAC addresses
- **ARP Cache Management**: Viewing and clearing ARP tables
- **ARP Table Timeouts**: Understanding cache aging
- **Broadcast Domain Limitations**: ARP within Layer 2 segments

### 3. Layer 3 Routing
- **Static Routing Configuration**: Manually configuring routing tables
- **Routing Table Entries**: Destination, gateway, netmask, interface
- **Longest Prefix Match (LPM)**: How routers select best routes
- **Default Gateway**: Handling packets for unknown networks
- **Multi-hop Forwarding**: Packets traversing multiple routers

### 4. Software-Defined Networking (SDN)
- **POX Controller**: OpenFlow controller for switch management
- **L2 Learning Switch**: Implementing MAC learning in software
- **Flow Tables**: OpenFlow rules and packet matching

## Experiments Performed

### Experiment 1: Switch MAC Learning
Created topology with multiple hosts and switches, observed MAC learning process including initial flooding and subsequent unicast forwarding.

### Experiment 2: ARP Cache Analysis
Examined ARP requests (broadcast) and replies (unicast), observed cache building and timeout behavior.

### Experiment 3: Static Routing Configuration
Configured routing tables on multiple hosts, set up multiple subnets, and tested cross-subnet connectivity.

### Experiment 4: Multi-Hop Packet Forwarding
Created linear topologies with multiple routers, used traceroute to visualize packet paths, analyzed delay accumulation.

### Experiment 5: POX Controller Integration
Ran POX L2 learning controller, observed dynamic flow rule installation, and analyzed controller-switch communication.

## Key Commands Used

```bash
# Mininet topology creation
sudo mn --topo single,3
sudo mn --topo tree,depth=2,fanout=2
sudo mn --custom topology.py --topo mytopo

# ARP operations
arp -n                    # View ARP cache
ip neigh show            # View neighbor table
ip neigh flush all       # Clear ARP cache

# Routing operations
route -n                 # View routing table
ip route show            # Show routing information

# Network testing
ping <destination>
traceroute <destination>
iperf <host1> <host2>
```

## Key Concepts Learned

1. **MAC Learning**: Switches dynamically learn host locations
2. **ARP Resolution**: Critical for IP-to-MAC translation
3. **Routing Decisions**: Based on longest prefix matching
4. **Packet Forwarding**: Different at Layer 2 vs Layer 3
5. **SDN Architecture**: Separation of control and data planes

## Tools Used
- **Mininet**: Network emulation platform
- **POX Controller**: Python-based OpenFlow controller
- **Open vSwitch**: Virtual switch with OpenFlow support
- **Wireshark**: Protocol analyzer for traffic inspection
- **iperf**: Network performance measurement
