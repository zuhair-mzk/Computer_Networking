# CSCD58 — Computer Networks (Fall 2025)

> **Author:** Zuhair Khan  
> **Institution:** University of Toronto Scarborough  
> **Instructor:** Prof. Marcelo Ponce

---

## 📚 Course Description

This repository contains coursework, lab exercises, and projects from **CSCD58: Computer Networks**, covering fundamental concepts in computer networking including:

- **Network Protocol Stack**: Physical, Data Link, Network, Transport, and Application layers
- **Core Protocols**: Ethernet, IP, ARP, ICMP, TCP, UDP, HTTP
- **Routing & Switching**: Static routing, longest prefix matching, forwarding tables
- **Network Simulation**: Hands-on experience with Mininet and NS3
- **Performance Analysis**: Throughput measurement, latency analysis, congestion control
- **Router Implementation**: Building a functional software router with ARP and ICMP support

This repository showcases practical implementations, simulations, and analysis of networking concepts learned throughout the course.

---

## 📂 Repository Structure

```
CSCD58/
│
├── README.md
│
├── assignments/          # Course assignments
│   ├── assignment1/     # Error detection and correction
│   ├── assignment2/     # Network protocols and packet analysis
│   ├── assignment3/     # Routing algorithms and forwarding
│   ├── assignment4/     # Transport layer and congestion control
│
├── labs/                # Laboratory exercises
│   ├── lab03-mininet-basics/
│   ├── lab05-routing-and-switching/
│   ├── lab07-network-simulators/
│   ├── lab09-simple-router/
│
├── simple-router/       # Software router implementation
│   ├── src/            # Router source code
│   ├── docs/           # Documentation
│   └── README.md       # Project-specific instructions
│
├── ns3/                # NS3 network simulator work
│   ├── example-scripts/
│   └── build-instructions.md
│
└── resources/          # Course materials and references
    ├── slides/
    └── reference-material/
```

---

## 📝 Assignments

### Assignment 1: Error Detection and Data Encoding
Explores fundamental concepts in reliable data transmission, including:
- Hamming codes for error detection and correction
- Parity bits and syndrome calculation
- Data encoding techniques for ensuring data integrity

**Technologies:** Binary arithmetic, error correction algorithms, mathematical proofs

---

### Assignment 2: Network Protocols and Packet Analysis
Investigates packet-level behavior in networks:
- Protocol header analysis (Ethernet, IP, TCP/UDP)
- Packet fragmentation and reassembly
- Network address translation and subnetting
- Protocol behavior under various network conditions

**Technologies:** Wireshark, packet analysis tools, protocol specifications

---

### Assignment 3: Routing Algorithms and Forwarding
Focuses on network layer functionality:
- Static and dynamic routing algorithms
- Longest prefix matching (LPM)
- Forwarding table construction and lookup
- Distance vector and link-state routing protocols

**Technologies:** Routing table algorithms, graph theory, network simulators

---

### Assignment 4: Transport Layer and Network Performance
Examines end-to-end communication and performance:
- TCP congestion control mechanisms (slow start, congestion avoidance, fast recovery)
- Flow control and sliding window protocols
- Exponential backoff algorithms
- Network performance metrics and optimization

**Technologies:** TCP analysis, throughput calculations, congestion window modeling

---

## 🔬 Labs

### Lab 03: Mininet Basics
**Overview:** Introduction to Mininet, a network emulation platform for creating virtual networks.

**Topics Covered:**
- Setting up virtual network topologies
- Running basic connectivity tests (ping, traceroute)
- Understanding network namespaces and virtual switches
- Measuring link bandwidth with iperf

**Key Commands:**
```bash
# Start Mininet with basic topology
sudo mn --topo single,3

# Test connectivity
mininet> pingall

# Measure bandwidth
mininet> iperf h1 h2
```

**Tools:** Mininet, iperf, ping, Wireshark

---

### Lab 05: Routing and Switching
**Overview:** Deep dive into Layer 2 and Layer 3 networking concepts.

**Topics Covered:**
- MAC address learning and switching
- ARP protocol operation and cache management
- Static routing configuration
- VLAN configuration and trunk ports
- Network performance testing under various topologies

**Key Experiments:**
- Building custom topologies with multiple switches
- Observing ARP request/reply behavior
- Testing routing table configurations
- Analyzing packet flows through Wireshark

**Tools:** Mininet, POX controller, Wireshark, custom Python scripts

---

### Lab 07: Network Simulators
**Overview:** Introduction to NS3 (Network Simulator 3) for discrete-event network simulation.

**Topics Covered:**
- NS3 architecture and core concepts
- Building simulation scripts in C++
- Simulating various network protocols
- Collecting and analyzing simulation data
- Performance comparison: wired vs. wireless networks

**Key Simulations:**
- Point-to-point network throughput
- WiFi network capacity analysis
- TCP congestion control behavior
- Queue management and packet drops

**Running Simulations:**
```bash
# Build NS3
./ns3 configure --enable-examples
./ns3 build

# Run example simulation
./ns3 run first
./ns3 run "second --nCsma=4"
```

**Tools:** NS3, NetAnim, trace analysis tools

---

### Lab 09: Simple Router Implementation
**Overview:** Practical implementation of router functionality in a Mininet environment.

**Topics Covered:**
- Router architecture and packet processing pipeline
- ARP request/reply handling
- ICMP echo (ping) response generation
- IP forwarding with longest prefix match
- TTL decrement and checksum recalculation

**Testing Procedures:**
- Verifying ARP cache population
- Testing ping between hosts
- Traceroute functionality
- Handling invalid packets and ICMP errors

**Tools:** Mininet, POX, simple router code (C/C++), Wireshark

See the [simple-router/](./simple-router/) directory for full implementation details.

---

## 🌐 Simple Router Project

### Overview
The Simple Router project is a hands-on implementation of a functional software router capable of handling real network traffic in a Mininet environment. This project demonstrates core router functionality including packet forwarding, ARP resolution, and ICMP handling.

### Key Features
- **Static Routing:** Forwarding packets based on routing table with longest prefix matching
- **ARP Cache Management:** Maintaining and updating ARP cache for efficient Layer 2 address resolution
- **ARP Request/Reply Handling:** Generating ARP requests for unknown MAC addresses and responding to ARP queries
- **ICMP Support:**
  - Echo reply (ping response)
  - Time exceeded (TTL expiry)
  - Destination unreachable messages
- **IP Packet Processing:**
  - Checksum verification and recalculation
  - TTL decrement and expiry handling
  - Packet forwarding to next hop

### Architecture
```
┌─────────────────────────────────────────┐
│          Simple Router                  │
│                                         │
│  ┌────────────┐    ┌──────────────┐   │
│  │ ARP Cache  │    │ Routing Table │   │
│  └────────────┘    └──────────────┘   │
│         │                  │            │
│  ┌──────▼──────────────────▼─────────┐ │
│  │   Packet Processing Pipeline      │ │
│  │                                   │ │
│  │  1. Receive packet                │ │
│  │  2. Validate checksums            │ │
│  │  3. Check routing table (LPM)     │ │
│  │  4. ARP lookup / queue request    │ │
│  │  5. Forward to next hop           │ │
│  └───────────────────────────────────┘ │
└─────────────────────────────────────────┘
```

### Running the Simple Router

**Prerequisites:**
- Mininet installed
- POX controller
- Compiled router binary

**Setup Steps:**

1. **Configure the environment:**
   ```bash
   cd simple-router/
   ./config.sh
   ```

2. **Start the Mininet topology:**
   ```bash
   ./run_mininet.sh
   ```

3. **Start the POX controller** (in a new terminal):
   ```bash
   ./run_pox.sh
   ```

4. **Run the simple router** (in another terminal):
   ```bash
   ./sr
   ```

5. **Test connectivity:**
   ```bash
   # In Mininet CLI
   mininet> pingall
   mininet> h1 ping h2
   mininet> h1 traceroute h3
   ```

### Testing Checklist
- ✅ Router receives and processes ARP requests
- ✅ Router responds to ping (ICMP echo)
- ✅ Packets are forwarded between hosts through router
- ✅ TTL is decremented properly
- ✅ ICMP time exceeded sent when TTL reaches 0
- ✅ Checksums are recalculated correctly

For detailed implementation documentation, see [simple-router/README.md](./simple-router/README.md).

---

## 🖥️ NS3 Network Simulator

### Overview
NS3 is a discrete-event network simulator used for research and education in computer networks. This section contains simulation scripts and experiments performed as part of the course.

### Installation & Build

**System Requirements:**
- C++ compiler (g++ 7+)
- Python 3.6+
- CMake or Waf build system

**Building NS3:**

```bash
# Download NS3
wget https://www.nsnam.org/releases/ns-allinone-3.xx.tar.bz2
tar xjf ns-allinone-3.xx.tar.bz2
cd ns-allinone-3.xx/ns-3.xx/

# Configure with examples enabled
./ns3 configure --enable-examples --enable-tests

# Build
./ns3 build

# Verify installation
./ns3 run first
```

**Alternative: Using CMake:**
```bash
cmake -B build
cmake --build build
./build/scratch/my-simulation
```

### Example Simulations

#### 1. First Example: Point-to-Point Network
```bash
./ns3 run first
```
Creates two nodes connected by a point-to-point link and sends packets between them.

#### 2. CSMA Network
```bash
./ns3 run "second --nCsma=5"
```
Simulates a CSMA/CD network with multiple nodes.

#### 3. WiFi Network Simulation
```bash
./ns3 run "third --nWifi=10"
```
Creates a wireless network with multiple WiFi stations.

### Creating Custom Simulations

Place your simulation scripts in:
- `ns3/example-scripts/` for standalone examples
- Or in `scratch/` directory within NS3 installation

**Basic Simulation Template:**
```cpp
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

int main(int argc, char *argv[]) {
    // Simulation code here
    NodeContainer nodes;
    nodes.Create(2);
    
    // Configure network topology
    // Install protocol stack
    // Set up applications
    // Run simulation
    
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
```

For detailed build instructions, see [ns3/build-instructions.md](./ns3/build-instructions.md).

---

## 📖 Resources & References

### Course Materials
- **Course Website:** [CSCD58 - Prof. Marcelo Ponce](https://cmsweb.utsc.utoronto.ca/marcelo-ponce/teaching/d94--qc/index.html)
- **Slides & Tutorials:** Available in `resources/slides/`

### Official Documentation
- **Mininet:** [http://mininet.org/](http://mininet.org/)
  - Walkthrough: [http://mininet.org/walkthrough/](http://mininet.org/walkthrough/)
  - API Documentation: [http://mininet.org/api/](http://mininet.org/api/)

- **NS3 Network Simulator:** [https://www.nsnam.org/](https://www.nsnam.org/)
  - Tutorial: [https://www.nsnam.org/docs/tutorial/html/](https://www.nsnam.org/docs/tutorial/html/)
  - Manual: [https://www.nsnam.org/docs/manual/html/](https://www.nsnam.org/docs/manual/html/)

- **POX Controller:** [https://github.com/noxrepo/pox](https://github.com/noxrepo/pox)

### Network Protocol References
- **Wireshark User Guide:** [https://www.wireshark.org/docs/wsug_html_chunked/](https://www.wireshark.org/docs/wsug_html_chunked/)
- **TCP/IP Illustrated** by W. Richard Stevens
- **Computer Networks: A Systems Approach** by Peterson & Davie

### Useful Tools
- **Wireshark:** Network protocol analyzer
- **iperf/iperf3:** Network bandwidth measurement tool
- **tcpdump:** Command-line packet analyzer
- **NetAnim:** Network animator for NS3 simulations

---

## 🚀 Getting Started

### Prerequisites
```bash
# Install Mininet
sudo apt-get install mininet

# Install NS3 dependencies
sudo apt-get install g++ python3 python3-dev cmake

# Install Wireshark
sudo apt-get install wireshark

# Install iperf
sudo apt-get install iperf iperf3
```

### Quick Start
1. Clone this repository
2. Navigate to specific lab or project directory
3. Follow the README instructions in each subdirectory
4. Refer to assignment PDFs for detailed requirements

---

## 📋 Lab & Assignment Locations

| Component | Location | Description |
|-----------|----------|-------------|
| Assignment 1 | `assignments/assignment1/` | Error detection and correction |
| Assignment 2 | `assignments/assignment2/` | Protocol analysis |
| Assignment 3 | `assignments/assignment3/` | Routing algorithms |
| Assignment 4 | `assignments/assignment4/` | Transport layer |
| Lab 03 | `labs/lab03-mininet-basics/` | Mininet introduction |
| Lab 05 | `labs/lab05-routing-and-switching/` | Routing & switching |
| Lab 07 | `labs/lab07-network-simulators/` | NS3 simulations |
| Lab 09 | `labs/lab09-simple-router/` | Router implementation |
| Simple Router | `simple-router/` | Complete router project |
| NS3 Scripts | `ns3/` | Simulation scripts |

---

## ⚠️ Academic Integrity Notice

This repository contains coursework completed for academic purposes at the University of Toronto Scarborough. 

**Please note:**
- This work is for reference and portfolio purposes only
- Do not copy or submit any part of this work as your own
- Refer to your institution's academic integrity policies
- Course materials and official assignment specifications are not redistributed here

---

## 📧 Contact

**Zuhair Khan**  
University of Toronto Scarborough  
Computer Science - CSCD58

For questions about the course content, please refer to the official course website or contact Prof. Marcelo Ponce.

---

## 📄 License

This repository is for educational purposes. All course content and materials belong to the University of Toronto Scarborough and Prof. Marcelo Ponce.

---

**Last Updated:** November 2025
