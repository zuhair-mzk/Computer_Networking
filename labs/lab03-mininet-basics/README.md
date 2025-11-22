# Lab 03: Mininet Basics

## Overview
Introduction to Mininet network emulation platform for creating and testing virtual networks. This lab provided hands-on experience with network emulation, allowing us to create virtual network topologies and test network protocols without physical hardware.

## Learning Objectives
- Understanding Mininet architecture and virtual network namespaces
- Creating and manipulating virtual network topologies
- Running connectivity and performance tests
- Measuring network bandwidth and latency
- Using Wireshark for packet analysis in virtual networks

## Topics Covered

### 1. Mininet Installation and Setup
- Installing Mininet on Ubuntu/Linux systems
- Understanding the Mininet CLI (Command Line Interface)
- Basic Mininet commands and topology creation

### 2. Network Topologies
- **Single topology**: One switch connected to multiple hosts
- **Linear topology**: Hosts connected in a line through switches
- **Tree topology**: Hierarchical network structure
- Creating custom topologies with Python scripts

### 3. Connectivity Testing
- Using `ping` to test host-to-host connectivity
- Running `pingall` to test full mesh connectivity
- Understanding ping output (latency, packet loss)
- Testing reachability across different network segments

### 4. Bandwidth Measurement
- Using `iperf` to measure TCP and UDP throughput
- Client-server bandwidth testing between hosts
- Understanding bandwidth vs. throughput
- Analyzing network performance metrics

### 5. Packet Analysis
- Capturing packets with tcpdump in Mininet
- Using Wireshark to analyze network traffic
- Observing protocol behavior (ARP, ICMP, TCP)
- Understanding packet headers and encapsulation

## Key Commands

```bash
# Start Mininet with basic topology
sudo mn --topo single,3

# Test connectivity
mininet> pingall
mininet> h1 ping h2

# Measure bandwidth
mininet> iperf h1 h2

# Run commands on specific hosts
mininet> h1 ifconfig
mininet> h2 ping 10.0.0.1

# Dump network topology
mininet> dump
mininet> net

# Exit Mininet
mininet> exit
```

## Experiments Performed

### Experiment 1: Basic Connectivity
- Created a simple topology with 3 hosts and 1 switch
- Tested ping between all hosts
- Observed ARP requests and replies
- Measured round-trip time (RTT)

### Experiment 2: Bandwidth Testing
- Set up iperf server on one host
- Measured TCP throughput between hosts
- Tested UDP bandwidth with different packet sizes
- Compared theoretical vs. actual bandwidth

### Experiment 3: Custom Topology
- Created custom network topology using Python
- Configured link parameters (bandwidth, delay, loss)
- Tested performance with different link characteristics
- Analyzed impact of network parameters on performance

### Experiment 4: Protocol Analysis
- Captured packets using tcpdump
- Analyzed traffic with Wireshark
- Observed TCP three-way handshake
- Examined ICMP echo request/reply packets

## Key Concepts Learned

- **Network Namespaces**: How Mininet isolates hosts in separate network spaces
- **Virtual Switches**: Software switches connecting virtual hosts
- **Link Emulation**: Simulating real network conditions (delay, bandwidth, loss)
- **Performance Metrics**: RTT, throughput, bandwidth, packet loss
- **Protocol Behavior**: How protocols operate in a controlled environment

## Tools Used
- **Mininet**: Network emulation platform
- **iperf/iperf3**: Network bandwidth measurement
- **ping**: Connectivity and latency testing
- **Wireshark**: Network protocol analyzer
- **tcpdump**: Command-line packet capture
