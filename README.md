# Network Calculator - User Guide

## Table of Contents
1. [Overview](#overview)
2. [Installation](#installation)
3. [Basic Usage](#basic-usage)
4. [Features](#features)
5. [Examples](#examples)
6. [Tips & Best Practices](#tips--best-practices)

## Overview

This network calculator is a comprehensive tool for network analysis, subnet calculations, and IP address management. It uses pure mathematical equations (no bit shifting) to perform all calculations, making it educational and easy to understand.

## Installation

```bash
# Compile the program
make

# Run the program
./net [options] [arguments]
```

## Basic Usage

The program supports multiple modes of operation:

### 1. Subnet Mask Analysis (Basic Mode)
```bash
./net <subnet_mask>
```
**Purpose**: Analyze a subnet mask and show IP range for 0.0.0.0 network
**Example**: `./net 255.255.255.0`

### 2. Network Analysis (Advanced Mode)
```bash
./net <network_ip> <subnet_mask>
```
**Purpose**: Analyze a specific network with given IP and subnet mask
**Example**: `./net 192.168.1.100 255.255.255.0`

### 3. Loopback Detection
```bash
./net -l <ip_address>
```
**Purpose**: Check if an IP address is a loopback address
**Example**: `./net -l 127.0.0.1`

### 4. CIDR Network Analysis
```bash
./net --cidr <cidr_notation>
```
**Purpose**: Analyze a network using CIDR notation (e.g., 192.168.1.0/24)
**Example**: `./net --cidr 192.168.1.0/24`

### 5. Network Class Detection
```bash
./net --class <ip_address>
```
**Purpose**: Determine historical network class (A, B, C) of an IP address
**Example**: `./net --class 10.0.0.1`

### 6. IP Range Validation
```bash
./net --check <ip_address> <cidr_network>
```
**Purpose**: Validate whether an IP address falls within a CIDR network
**Example**: `./net --check 192.168.1.100 192.168.1.0/24`

### 7. Multi-Format IP Converter
```bash
./net --convert <ip_address>
```
**Purpose**: Show IP address in multiple formats (decimal, binary, hex, integer)
**Example**: `./net --convert 192.168.1.1`

## Features

### ✅ Current Features

#### 1. **Subnet Mask Analysis**
- Converts subnet mask to binary representation
- Shows CIDR notation (e.g., /24, /28)
- Calculates available IP addresses
- Mathematical approach for educational value

#### 2. **IP Range Calculation**
- Network address calculation
- Broadcast address calculation
- First and last usable IP addresses
- Total network size information

#### 3. **Loopback Detection**
- Detects 127.0.0.0/8 range (all 127.x.x.x addresses)
- Special recognition for 127.0.0.1 (localhost)
- Identifies other IP address types (private, public, link-local)

#### 4. **Mathematical Approach**
- Uses base-256 number system for IP conversion
- IP = A×256³ + B×256² + C×256¹ + D×256⁰
- Educational formulas instead of cryptic bit operations

#---

## � PRACTICAL EXAMPLES

### Network Administration Scenarios

**Scenario 1: Office Network Planning**
```bash
# Analyze company's Class B network
./net --class 172.16.0.1

# Split into 4 departments
./net --split 172.16.0.0/16 4

# Scan a department subnet
./net --scan 172.16.0.0/18
```

**Scenario 2: Home Network Setup**
```bash
# Analyze home router network
./net --cidr 192.168.1.0/24

# Check if device is in range
./net --check 192.168.1.100 192.168.1.0/24

# Convert to different formats
./net --convert 192.168.1.1
```

**Scenario 3: IPv6 Migration Planning**
```bash
# Analyze IPv6 documentation prefix
./net --ipv6 2001:db8::1

# Check link-local address
./net --ipv6 fe80::1

# Learn format conversion
./net --ipv6-convert 2001:db8::1234
```

**Scenario 4: Subnet Design Exercise**
```bash
# Design 8 equal subnets from Class C
./net --split 192.168.100.0/24 8

# Validate the design
./net --scan 192.168.100.0/27

# Check a specific IP assignment
./net --check 192.168.100.15 192.168.100.0/27
```

---

### 🔍 Network Scanner (--scan)

Enumerates all IP addresses within a network range. Educational tool for understanding network topology.

```bash
# Scan a small network
./net --scan 192.168.1.0/28

# Scan larger networks (shows summary)
./net --scan 10.0.0.0/24
```

**Educational Value:**
- Shows network vs broadcast addresses
- Demonstrates IP enumeration concepts  
- Explains usable vs unusable IP addresses
- Mathematical approach to address calculation

### 🔀 Subnet Splitter (--split)

Divides a network into smaller equal-sized subnets using VLSM principles.

```bash
# Split 10.0.0.0/16 into 4 subnets
./net --split 10.0.0.0/16 4

# Split 192.168.1.0/24 into 8 subnets
./net --split 192.168.1.0/24 8
```

**Requirements:**
- Number of subnets must be a power of 2 (2, 4, 8, 16, etc.)
- Resulting prefix length must be ≤ /30

**Educational Value:**
- VLSM (Variable Length Subnet Masking) concepts
- Efficient IP address allocation
- Subnet design for network efficiency
- Mathematical subnet calculation

### 🌐 IPv6 Address Analysis (--ipv6)

Comprehensive analysis of IPv6 addresses with educational information.

```bash
# Analyze documentation address
./net --ipv6 2001:db8::1

# Analyze link-local address
./net --ipv6 fe80::1

# Analyze loopback
./net --ipv6 ::1
```

**Detects:**
- Address type (unicast, multicast, special-purpose)
- Scope (global, link-local, loopback, etc.)
- Special ranges (documentation, reserved, etc.)

### 🔄 IPv6 Format Converter (--ipv6-convert)

Educational IPv6 format converter and structure explanation.

```bash
# Convert and explain IPv6 formats
./net --ipv6-convert 2001:db8::1
./net --ipv6-convert fe80::1234:5678
```

**Educational Value:**
- IPv6 vs IPv4 comparison
- Compression rules (:: usage)
- Hexadecimal notation explanation
- Structure and formatting best practices

---
- CIDR notation input support
- Network calculator with gateway/DNS suggestions
- Subnet splitting tools
- Binary/Hex conversion display
- Wildcard mask calculator
- Next/Previous network finder
- Route summarization
- Interactive mode

## Examples

### Example 1: Basic Subnet Analysis
```bash
$ ./net 255.255.255.0

=== Subnet Mask Analysis ===
Subnet mask: 255.255.255.0
Binary mask (32 bits):
11111111111111111111111100000000
Available IP addresses: 254
CIDR notation: /24

=== IP Range Analysis ===
IP Range Information (based on 0.0.0.0 network):
Subnet Mask: 255.255.255.0
Network Address: 0.0.0.0
Broadcast Address: 0.0.0.255
First available IP: 0.0.0.1
Last available IP: 0.0.0.254
Total available IPs: 254

Range Information:
Each /24 network contains 254 usable IP addresses
Network size: 256 total addresses
```

### Example 2: Specific Network Analysis
```bash
$ ./net 192.168.1.100 255.255.255.0

=== Subnet Mask Analysis ===
Subnet mask: 255.255.255.0
Binary mask (32 bits):
11111111111111111111111100000000
Available IP addresses: 254
CIDR notation: /24

=== IP Range Analysis ===
Network Information:
Network IP: 192.168.1.100
Subnet Mask: 255.255.255.0
Network Address: 192.168.1.0
Broadcast Address: 192.168.1.255
First available IP: 192.168.1.1
Last available IP: 192.168.1.254
Total available IPs: 254

=== Loopback IP Analysis ===
IP Address: 192.168.1.100
✗ This is NOT a loopback address
Loopback Range: 127.0.0.0 - 127.255.255.255 (127.0.0.0/8)
ℹ️  This is a Private IP address (192.168.0.0/16)
```

### Example 3: Loopback Detection
```bash
$ ./net -l 127.0.0.1

=== Loopback IP Analysis ===
IP Address: 127.0.0.1
✓ This is a LOOPBACK address!
Loopback Range: 127.0.0.0 - 127.255.255.255 (127.0.0.0/8)
Address breakdown: 127.0.0.1
⭐ Special: This is 127.0.0.1 (localhost)
   - Most commonly used loopback address
   - Points to the local machine itself

Loopback Properties:
- Traffic never leaves the local machine
- No physical network interface required
- Maximum speed (no network latency)
- Used for local services and testing
```

### Example 4: CIDR Network Analysis
```bash
$ ./net --cidr 192.168.1.0/24

🌐 Comprehensive CIDR Network Analysis
CIDR parsed successfully:
   📍 Network IP: 192.168.1.0
   📏 Prefix Length: /24
   🧮 Network Bits: 24, Host Bits: 8

CIDR Network Summary:
│ Original CIDR:     192.168.1.0/24                       │
│ Network IP:        192.168.1.0                          │
│ Subnet Mask:       255.255.255.0                        │

Network Boundaries:
│ Network Address:   192.168.1.0                          │
│ Broadcast Address: 192.168.1.255                        │
│ First Usable IP:   192.168.1.1                          │
│ Last Usable IP:    192.168.1.254                        │
```

### Example 5: Network Class Detection
```bash
$ ./net --class 10.0.0.1

📋 Network Class Analysis
📍 Analyzing: 10.0.0.1 (decimal: 167772161)
🏷️  Network Class: Class A

Class A Network Information:
│ Range: 1.0.0.0 - 126.255.255.255                       │
│ Default Mask: 255.0.0.0 (/8)                           │
│ Networks: 126 possible networks                        │
│ Hosts per Network: 16,777,214                          │
│ Usage: Large organizations, ISPs                       │
```

### Example 6: IP Range Validation
```bash
$ ./net --check 192.168.1.100 192.168.1.0/24

🎯 IP Range Validation
Validation Summary:
│ Target IP:         192.168.1.100                        │
│ Network:           192.168.1.0/24                       │
│ Result:            ✅ IP IS in network                 │
```

### Example 7: Multi-Format IP Converter
```bash
$ ./net --convert 192.168.1.1

🔄 Multi-Format IP Address Converter
Multiple Format Representations:
│ Decimal Dotted:    192.168.1.1                          │
│ 32-bit Integer:    3232235777                           │
│ Hexadecimal:       0xC0A80101                           │
│ Binary (grouped):  11000000 10101000 00000001 00000001 │

Mathematical Breakdown:
│ Octet A (192): 192 × 256³ = 3221225472                  │
│ Octet B (168): 168 × 256² = 11010048                    │
│ Octet C (  1):   1 × 256¹ = 256                         │
│ Octet D (  1):   1 × 256⁰ = 1                           │
│ Total Sum:      = 3232235777                            │
```

## Common Subnet Masks & Their Uses

| Subnet Mask     | CIDR | Available IPs | Common Use                    |
|-----------------|------|---------------|-------------------------------|
| 255.255.255.252 | /30  | 2             | Point-to-point links          |
| 255.255.255.248 | /29  | 6             | Very small networks           |
| 255.255.255.240 | /28  | 14            | Small office networks         |
| 255.255.255.224 | /27  | 30            | Small department networks     |
| 255.255.255.192 | /26  | 62            | Medium office networks        |
| 255.255.255.128 | /25  | 126           | Large department networks     |
| 255.255.255.0   | /24  | 254           | Standard home/office networks |
| 255.255.254.0   | /23  | 510           | Large office networks         |
| 255.255.252.0   | /22  | 1022          | Very large networks           |
| 255.255.0.0     | /16  | 65534         | Enterprise networks           |
| 255.0.0.0       | /8   | 16777214      | ISP/Large enterprise          |

## Mathematical Formulas Used

### IP Address Conversion
```
IP to Integer: IP = A×256³ + B×256² + C×256¹ + D×256⁰
Integer to IP: 
  A = IP ÷ 256³
  B = (IP mod 256³) ÷ 256²
  C = (IP mod 256²) ÷ 256¹
  D = IP mod 256¹
```

### Network Calculations
```
Network Address = IP AND Subnet_Mask
Broadcast Address = Network + (2^host_bits - 1)
First Usable IP = Network + 1
Last Usable IP = Broadcast - 1
Available IPs = 2^host_bits - 2 (except for /31 and /32)
```

## Tips & Best Practices

### 1. **Understanding CIDR Notation**
- /24 = 255.255.255.0 (24 network bits, 8 host bits)
- /28 = 255.255.255.240 (28 network bits, 4 host bits)
- Smaller CIDR number = Larger network

### 2. **Private IP Ranges** (RFC 1918)
- 10.0.0.0/8 (10.0.0.0 - 10.255.255.255)
- 172.16.0.0/12 (172.16.0.0 - 172.31.255.255)
- 192.168.0.0/16 (192.168.0.0 - 192.168.255.255)

### 3. **Special Addresses**
- Network Address: First IP in range (not usable for hosts)
- Broadcast Address: Last IP in range (not usable for hosts)
- Gateway: Usually .1 (first usable IP)
- DNS Servers: Often .1, .2, or .8 (.8.8.8 for Google)

### 4. **Subnet Planning**
- Always plan for growth (use larger subnets than immediately needed)
- Document your IP allocation scheme
- Reserve ranges for different purposes (servers, printers, DHCP)

### 5. **Common Networking Commands**
```bash
# Test connectivity
ping 192.168.1.1

# Show network configuration
ip addr show        # Linux
ifconfig           # macOS/older Linux

# Show routing table
ip route           # Linux
route -n           # macOS/older systems
```

## Troubleshooting

### Common Issues:
1. **"Invalid IP address format"**: Check IP format (must be x.x.x.x with 0-255 per octet)
2. **"Invalid subnet mask format"**: Ensure proper dotted decimal notation
3. **Wrong network calculation**: Verify subnet mask is valid (contiguous 1s followed by 0s)

### Getting Help:
```bash
# Show usage information
./net

# Test with known good values
./net 192.168.1.1 255.255.255.0
./net -l 127.0.0.1
```

---
**Note**: This program uses mathematical equations instead of bit operations for educational purposes. All calculations show the underlying math, making it perfect for learning networking concepts.
