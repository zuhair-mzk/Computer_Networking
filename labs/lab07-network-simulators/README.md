# Lab 07: Network Simulators (NS3)

## Overview
Comprehensive introduction to NS3 (Network Simulator 3) for discrete-event network simulation and performance analysis. This lab focused on building, running, and analyzing network simulations to understand protocol behavior and network performance under various conditions.

## Learning Objectives
- Understanding NS3 architecture and simulation concepts
- Writing and modifying simulation scripts in C++
- Configuring network topologies and protocol parameters
- Collecting and analyzing performance metrics with Flow Monitor
- Using visualization tools (NetAnim) for network animations
- Comparing simulation results with theoretical predictions

## Topics Covered

### 1. NS3 Fundamentals
- Discrete-event simulation concepts
- NS3 architecture: nodes, devices, channels, protocols, applications
- Event scheduling and simulation time management
- Module system (core, network, internet, applications)

### 2. Network Components
- **Nodes**: Creating network nodes (hosts, routers, switches)
- **Channels**: Point-to-point, CSMA, WiFi channel configuration
- **Net Devices**: Installing network interfaces on nodes
- **Protocol Stack**: IPv4, TCP, UDP installation
- **Applications**: Echo server/client, bulk send, on-off applications

### 3. Topology Configuration
- Point-to-point links between nodes
- CSMA networks (shared Ethernet-like medium)
- WiFi networks with access points and stations
- Mixed wired/wireless topologies
- Mobility models for wireless scenarios

### 4. Performance Metrics
- **Throughput**: Measuring data transfer rates
- **Latency/Delay**: End-to-end packet delays  
- **Packet Loss**: Drop rates and reliability
- **Jitter**: Variation in packet arrival times
- **TCP Congestion Window**: cwnd evolution over time

## Simulations Performed

### Simulation 1: Basic Point-to-Point Network
Created two nodes with P2P link, tested UDP echo and TCP bulk transfer, measured throughput with different data rates, observed packet delays.

**Key Findings**: Verified throughput matches configured data rate, RTT correlates with link delay, observed TCP handshake in packet traces.

### Simulation 2: CSMA (Ethernet) Network
Simulated shared medium network with multiple hosts sending simultaneously, observed collision detection and bandwidth sharing.

**Key Findings**: Bandwidth shared among active nodes, increased nodes decreased per-node throughput, collision overhead impacts performance.

### Simulation 3: WiFi Network with Multiple Stations
Created wireless network with AP and varying numbers of stations (1, 5, 10, 20), tested different distances and mobility patterns.

**Key Findings**: Throughput degrades with more stations, signal strength affects rate selection, mobility impacts stability, observed hidden terminal problem.

### Simulation 4: TCP Congestion Control
Analyzed TCP behavior with bottleneck links, measured congestion window evolution, compared TCP variants (NewReno, Cubic).

**Key Findings**: Observed saw-tooth cwnd pattern, verified slow start and congestion avoidance, analyzed fast retransmit/recovery, compared fairness.

### Simulation 5: Network Performance Under Load
Stress tested network with varying traffic loads (10% to 150% capacity), measured throughput, delay, and packet loss at saturation.

**Key Findings**: Throughput plateaus at capacity, delay increases exponentially near saturation, packet loss rises sharply beyond capacity.

## Visualization and Analysis

### NetAnim Integration
Added animation support to visualize node positions, packet transmissions, queue lengths, and throughput graphs in real-time.

### Trace File Analysis
Generated pcap traces for Wireshark analysis, examined protocol headers, timing, retransmissions, and state machines.

### Flow Monitor Statistics
Collected detailed flow statistics including tx/rx packets, bytes transferred, throughput, average delay, jitter, and packet loss per flow.

## Key Commands

```bash
# NS3 build system
./ns3 configure --enable-examples --enable-tests
./ns3 build

# Running simulations
./ns3 run first
./ns3 run "second --nCsma=5"
./ns3 run "third --nWifi=10 --verbose=true"

# Running with arguments
./ns3 run "simulation --PrintHelp"

# Building custom simulations
./ns3 run scratch/my-simulation
```

## Performance Metrics Collected

- **Throughput**: Measured via Flow Monitor and packet counters
- **Latency**: End-to-end delay from packet timestamps
- **Packet Loss**: Comparison of tx vs rx packets
- **Jitter**: Delay variation between consecutive packets
- **Congestion Window**: TCP cwnd size tracing
- **Queue Length**: Router buffer occupancy
- **Collision Rate**: CSMA medium access statistics

## Key Concepts Learned

1. **Event-Driven Simulation**: Discrete-event scheduling vs real-time
2. **Protocol Stack Configuration**: Building complete network stacks
3. **Performance Modeling**: Predicting network behavior
4. **Wireless Simulation**: WiFi-specific modeling challenges
5. **TCP Dynamics**: Congestion control mechanisms in detail
6. **Statistical Analysis**: Interpreting simulation results
7. **Validation**: Comparing simulations with analytical models

## Tools and Technologies

- **NS3 Core**: C++ simulation framework
- **NetAnim**: Network animation and visualization tool
- **Flow Monitor**: Performance metrics collection
- **Wireshark**: Pcap trace analysis
- **Python bindings**: Alternative to C++ for simpler scripts
- **Gnuplot**: Graphing simulation results

## Comparison: Simulation vs Theory

- **Throughput**: Simulation matched theoretical data rate with <5% variation
- **Delay**: Queuing delay dominant under high load, matched M/M/1 queue model
- **TCP Throughput**: Formula MSS/(RTT × √Loss) accurate for low loss rates

## Practical Applications

- **Protocol Development**: Testing new protocols before hardware implementation
- **Network Planning**: Capacity planning and design validation
- **Research**: Studying protocol behavior under controlled conditions
- **Education**: Understanding networking concepts through experimentation
- **Performance Prediction**: Estimating network performance before deployment
