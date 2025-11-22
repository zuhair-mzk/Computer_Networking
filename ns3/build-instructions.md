# NS3 Network Simulator - Build & Usage Guide

## 📋 Overview

NS3 (Network Simulator 3) is a discrete-event network simulator designed for research and educational use. It provides a flexible, modular framework for simulating various network protocols, topologies, and scenarios.

This directory contains example simulation scripts and instructions for working with NS3 in the CSCD58 course.

---

## 🎯 What is NS3?

NS3 is used to:
- **Simulate network protocols** (TCP, UDP, IP, WiFi, LTE, etc.)
- **Model network behavior** without physical hardware
- **Analyze performance metrics** (throughput, latency, packet loss)
- **Visualize network topologies** with NetAnim
- **Test algorithms** in controlled environments

### Key Features
- Written in C++ with Python bindings
- Modular architecture with independent components
- Extensive protocol library
- Support for wired and wireless networks
- Integrated trace and statistical output
- Animation and visualization support

---

## 💻 System Requirements

### Supported Platforms
- Ubuntu/Debian Linux (recommended)
- macOS
- Windows (via WSL2 or Cygwin)

### Prerequisites

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install g++ python3 python3-dev pkg-config sqlite3
sudo apt-get install cmake python3-setuptools git
sudo apt-get install qt5-default  # For NetAnim visualizer (optional)
```

**macOS:**
```bash
# Install Homebrew first: https://brew.sh
brew install cmake python@3 qt@5
```

---

## 📥 Installation

### Option 1: Download Official Release (Recommended)

```bash
# Download NS3 (replace 3.xx with latest version)
cd ~/
wget https://www.nsnam.org/releases/ns-allinone-3.38.tar.bz2
tar xjf ns-allinone-3.38.tar.bz2
cd ns-allinone-3.38/ns-3.38/
```

### Option 2: Clone from GitLab

```bash
git clone https://gitlab.com/nsnam/ns-3-dev.git
cd ns-3-dev/
```

---

## 🔨 Building NS3

NS3 supports two build systems: **ns3** (wrapper around Waf) and **CMake**.

### Method 1: Using ns3 (Waf-based - Recommended)

```bash
# Configure NS3 with examples and tests enabled
./ns3 configure --enable-examples --enable-tests

# Build NS3
./ns3 build

# Verify installation
./ns3 run first
```

**Configuration Options:**
```bash
# Debug build (slower, with debugging symbols)
./ns3 configure --enable-examples --build-profile=debug

# Optimized build (faster execution)
./ns3 configure --enable-examples --build-profile=optimized

# Show all configuration options
./ns3 configure --help
```

### Method 2: Using CMake

```bash
# Configure with CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build -j$(nproc)

# Run example
./build/examples/tutorial/first
```

---

## 🚀 Running Simulations

### Built-in Examples

NS3 includes numerous example simulations in the `examples/` directory.

#### 1. First Example (Point-to-Point Network)
```bash
./ns3 run first
```

**What it does:**
- Creates two nodes connected by point-to-point link
- Sends UDP packets between nodes
- Generates pcap trace files

**Output:**
```
At time +2s client sent 1024 bytes to 10.1.1.2 port 9
At time +2.00369s server received 1024 bytes from 10.1.1.1 port 49153
```

#### 2. Second Example (CSMA Network)
```bash
./ns3 run "second --nCsma=5"
```

**What it does:**
- Creates CSMA/CD network with configurable number of nodes
- Simulates Ethernet-like shared medium

#### 3. Third Example (WiFi Network)
```bash
./ns3 run "third --nWifi=10 --verbose=true"
```

**What it does:**
- Creates WiFi access point with multiple stations
- Demonstrates wireless network simulation

### Running with Arguments
```bash
# Pass command-line arguments
./ns3 run "example-name --arg1=value1 --arg2=value2"

# Show help for simulation
./ns3 run "example-name --help"
```

---

## 📝 Creating Your Own Simulation

### Step 1: Create Simulation File

Place your simulation in the `scratch/` directory:

```bash
cd ns-3.38/scratch/
nano my-simulation.cc
```

### Step 2: Basic Simulation Template

```cpp
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MySimulation");

int main(int argc, char *argv[])
{
    // Enable logging
    LogComponentEnable("MySimulation", LOG_LEVEL_INFO);
    
    // Create nodes
    NodeContainer nodes;
    nodes.Create(2);
    
    // Create point-to-point link
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
    
    // Install devices on nodes
    NetDeviceContainer devices;
    devices = pointToPoint.Install(nodes);
    
    // Install Internet stack
    InternetStackHelper stack;
    stack.Install(nodes);
    
    // Assign IP addresses
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign(devices);
    
    // Create UDP echo server on node 1
    UdpEchoServerHelper echoServer(9);
    ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));
    
    // Create UDP echo client on node 0
    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));
    
    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));
    
    // Run simulation
    Simulator::Run();
    Simulator::Destroy();
    
    return 0;
}
```

### Step 3: Build and Run

```bash
# Build your simulation
./ns3 build

# Run your simulation
./ns3 run my-simulation
```

---

## 📊 Analyzing Results

### Trace Files

NS3 can generate trace files for detailed analysis:

```cpp
// Enable pcap tracing
pointToPoint.EnablePcapAll("my-simulation");

// Enable ASCII tracing
AsciiTraceHelper ascii;
pointToPoint.EnableAsciiAll(ascii.CreateFileStream("my-simulation.tr"));
```

**Opening pcap files:**
```bash
# View with Wireshark
wireshark my-simulation-0-0.pcap

# View with tcpdump
tcpdump -r my-simulation-0-0.pcap -nn
```

### Flow Monitor

Measure network performance metrics:

```cpp
#include "ns3/flow-monitor-module.h"

// Install Flow Monitor
FlowMonitorHelper flowmon;
Ptr<FlowMonitor> monitor = flowmon.InstallAll();

// Run simulation
Simulator::Stop(Seconds(10.0));
Simulator::Run();

// Print statistics
monitor->CheckForLostPackets();
Ptr<Ipv4FlowClassifier> classifier = 
    DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();

for (auto const &i : stats)
{
    std::cout << "Flow " << i.first << "\n";
    std::cout << "  Tx Packets: " << i.second.txPackets << "\n";
    std::cout << "  Rx Packets: " << i.second.rxPackets << "\n";
    std::cout << "  Throughput: " << i.second.rxBytes * 8.0 / 9.0 / 1000 / 1000 
              << " Mbps\n";
}

Simulator::Destroy();
```

---

## 🎨 Visualization with NetAnim

NetAnim provides graphical animation of network simulations.

### Installing NetAnim

```bash
cd ns-allinone-3.38/netanim-3.108/
make clean
qmake NetAnim.pro
make
```

### Generating Animation

Add to your simulation code:

```cpp
#include "ns3/netanim-module.h"

// Before Simulator::Run()
AnimationInterface anim("my-simulation.xml");
anim.SetConstantPosition(nodes.Get(0), 10.0, 10.0);
anim.SetConstantPosition(nodes.Get(1), 20.0, 10.0);
```

### Viewing Animation

```bash
./NetAnim
# Open my-simulation.xml file
```

---

## 📚 Common Simulation Scenarios

### 1. TCP Throughput Measurement

```cpp
// TCP bulk send application
BulkSendHelper bulk("ns3::TcpSocketFactory",
                    InetSocketAddress(interfaces.GetAddress(1), 9));
bulk.SetAttribute("MaxBytes", UintegerValue(0)); // Unlimited
ApplicationContainer bulkApp = bulk.Install(nodes.Get(0));
bulkApp.Start(Seconds(0.0));
bulkApp.Stop(Seconds(10.0));

// TCP sink receiver
PacketSinkHelper sink("ns3::TcpSocketFactory",
                      InetSocketAddress(Ipv4Address::GetAny(), 9));
ApplicationContainer sinkApp = sink.Install(nodes.Get(1));
sinkApp.Start(Seconds(0.0));
sinkApp.Stop(Seconds(10.0));
```

### 2. WiFi Network with Multiple Stations

```cpp
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"

// Create WiFi nodes
NodeContainer wifiStaNodes;
wifiStaNodes.Create(10);
NodeContainer wifiApNode;
wifiApNode.Create(1);

// Configure WiFi
YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
YansWifiPhyHelper phy;
phy.SetChannel(channel.Create());

WifiHelper wifi;
wifi.SetRemoteStationManager("ns3::AarfWifiManager");

WifiMacHelper mac;
Ssid ssid = Ssid("ns-3-ssid");
mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid));

NetDeviceContainer staDevices;
staDevices = wifi.Install(phy, mac, wifiStaNodes);

mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
NetDeviceContainer apDevices;
apDevices = wifi.Install(phy, mac, wifiApNode);

// Set mobility
MobilityHelper mobility;
mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                              "MinX", DoubleValue(0.0),
                              "MinY", DoubleValue(0.0),
                              "DeltaX", DoubleValue(5.0),
                              "DeltaY", DoubleValue(10.0),
                              "GridWidth", UintegerValue(3),
                              "LayoutType", StringValue("RowFirst"));
mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
mobility.Install(wifiStaNodes);
mobility.Install(wifiApNode);
```

### 3. Network with Congestion

```cpp
// Create bottleneck link with limited bandwidth
PointToPointHelper bottleneck;
bottleneck.SetDeviceAttribute("DataRate", StringValue("1Mbps"));
bottleneck.SetChannelAttribute("Delay", StringValue("10ms"));
bottleneck.SetQueue("ns3::DropTailQueue", "MaxSize", StringValue("10p"));
```

---

## 🔧 Debugging Tips

### Enable Logging

```cpp
// In C++ code
NS_LOG_COMPONENT_DEFINE("MyComponent");
LogComponentEnable("MyComponent", LOG_LEVEL_INFO);
```

```bash
# Via environment variable
export NS_LOG=MyComponent=level_info
./ns3 run my-simulation
```

**Log Levels:**
- `LOG_ERROR`: Error messages only
- `LOG_WARN`: Warnings and errors
- `LOG_INFO`: Informational messages
- `LOG_DEBUG`: Detailed debugging info
- `LOG_ALL`: Everything

### Common Build Errors

**Error: "command not found"**
```bash
# Ensure ns3 is executable
chmod +x ns3
```

**Error: "python not found"**
```bash
# Ensure Python 3 is installed
sudo apt-get install python3
```

**Error: "waf: command not found"**
```bash
# Reconfigure
./ns3 configure --enable-examples
```

---

## 📖 Additional Resources

### Official Documentation
- **NS3 Homepage:** https://www.nsnam.org/
- **Tutorial:** https://www.nsnam.org/docs/tutorial/html/
- **Manual:** https://www.nsnam.org/docs/manual/html/
- **API Documentation:** https://www.nsnam.org/docs/doxygen/

### Example Scripts
- Built-in examples: `ns-3.xx/examples/`
- Tutorial examples: `ns-3.xx/examples/tutorial/`
- Wireless examples: `ns-3.xx/examples/wireless/`

### Community Resources
- **Mailing List:** https://groups.google.com/g/ns-3-users
- **GitLab Issues:** https://gitlab.com/nsnam/ns-3-dev/-/issues
- **Stack Overflow:** Tag `ns3`

---

## 📁 Example Scripts in This Directory

The `example-scripts/` folder contains simulation scripts used in the course:

```
example-scripts/
├── first.cc              # Basic point-to-point example
├── second.cc             # CSMA network example
├── third.cc              # WiFi network example
├── tcp-bulk-send.cc      # TCP throughput test
└── wireless-animation.cc # WiFi with NetAnim
```

*(Note: Place your actual simulation files here after creating them)*

---

## 🎓 Course-Specific Notes

For CSCD58 lab exercises:
1. Start with built-in examples (`first`, `second`, `third`)
2. Modify parameters to observe behavior changes
3. Use Flow Monitor to collect statistics
4. Generate pcap files for Wireshark analysis
5. Create NetAnim visualizations for presentations

**Common Lab Tasks:**
- Measure TCP throughput with different window sizes
- Compare UDP vs TCP performance
- Simulate network congestion and packet loss
- Analyze WiFi performance with varying node counts
- Test routing protocols in multi-hop networks

---

**Last Updated:** November 2025
