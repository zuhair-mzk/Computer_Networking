# Lab 05: Routing and Switching

## Overview
Hands-on exploration of Layer 2 switching and Layer 3 routing in software-defined networks.

## Learning Objectives
- Understanding MAC address learning
- ARP protocol operation
- Static routing configuration
- Network performance testing

## Files
- `lab05_SR.pdf` - Lab specification and procedures

## Key Experiments
1. MAC address table observation
2. ARP cache analysis
3. Routing table configuration
4. Multi-hop packet forwarding

## Setup
```bash
# Start Mininet with custom topology
sudo mn --custom topo.py --topo mytopo

# Run POX controller
./pox.py forwarding.l2_learning
```

## Tools Used
- Mininet
- POX controller
- Wireshark
- Python networking scripts
