# Lab 07: Network Simulators

## Overview
Introduction to NS3 network simulator for discrete-event network simulation and analysis.

## Learning Objectives
- Understanding NS3 architecture
- Writing simulation scripts
- Collecting performance metrics
- Analyzing simulation results

## Files
- `D58-lab07.pdf` - Lab specification
- Simulation scripts (`.cc` files)
- Results and analysis

## Key Simulations
1. Point-to-point throughput
2. WiFi network capacity
3. TCP congestion behavior
4. Protocol performance comparison

## Building and Running
```bash
# Configure NS3
./ns3 configure --enable-examples

# Build
./ns3 build

# Run simulation
./ns3 run first
```

## Tools Used
- NS3 simulator
- NetAnim visualizer
- Flow Monitor
- Wireshark (for trace analysis)
