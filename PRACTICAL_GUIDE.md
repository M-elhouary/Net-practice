# PRACTICAL USAGE GUIDE & EXAMPLES

## Quick Start

### Installation
```bash
cd /home/mel-houa/githube/net
make clean
make
```

### Basic Commands

```bash
# Test if a port is open
./net --tcp 8.8.8.8 53

# Ping a host
./net --ping 192.168.1.1 3

# Find open services
./net --discover 192.168.1.1

# Full diagnostics
./net --diagnose 8.8.8.8
```

---

## Real-World Scenarios

### Scenario 1: Checking if DNS Server is Reachable

```bash
$ ./net --tcp 8.8.8.8 53 5
```

**Output:**
```
┌─ TCP CONNECTIVITY CHECK ─────────────────────────────
│ Target: 8.8.8.8:53
│ Timeout: 5 seconds
│ Method: Non-blocking socket with configurable timeout
└────────────────────────────────────────────────────────

📍 Step 1: Create TCP Socket
   Creating socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)...
   ✅ Socket created (file descriptor: 3)

📍 Step 2: Set Non-Blocking Mode (fcntl)
   ✅ Non-blocking mode enabled for timeout control

📍 Step 3: Prepare Connection Address
   • Address Family: AF_INET (IPv4)
   • Port Conversion: 53 (decimal) → 0x0035 (network byte order)
   • IP Address: 8.8.8.8 → 0x08080808 (32-bit)
   ✅ Address structure prepared

📍 Step 4: Initiate TCP Connection (SYN)
   Sending SYN packet with socket flags: 0x02
   Kernel generates random initial sequence number (ISN)
   Socket enters SYN_SENT state...

📍 Step 5: Wait for SYN-ACK (Timeout Handling)
   Using select() to wait for writeable socket
   Timeout: 5 seconds (user-configurable)
   ✅ Socket is writable - ACK received
   ✅ Connection established (ESTABLISHED state)

📍 Step 6: Verify Connection Status
   ✅ Socket is writable - ACK received
   ✅ Connection established (ESTABLISHED state)

📍 Step 7: Measure Round-Trip Time
   Start Time:  1234567890.234567 seconds
   End Time:    1234567890.261234 seconds
   RTT Formula: (end_sec - start_sec) × 1000 + (end_usec - start_usec) / 1000
   RTT Calculation: (elapsed milliseconds) = 26 ms

┌─ TCP CONNECTION SUCCESSFUL ──────────────────────────
│ ✅ Target is reachable and responding
│ 📊 Response Time: 26 milliseconds
│ 🔌 Port 53 is OPEN
└────────────────────────────────────────────────────────
```

**What This Tells You:**
- ✅ Google's DNS server (8.8.8.8) is reachable
- ✅ Port 53 (DNS) is open and accepting connections
- ✅ Network latency to Google is ~26 milliseconds
- ✅ Your firewall allows outbound connections to port 53

---

### Scenario 2: Testing SSH Access

```bash
$ ./net --tcp 192.168.1.100 22 3
```

**Expected Output (if SSH is running):**
```
┌─ TCP CONNECTION SUCCESSFUL ──────────────────────────
│ ✅ Target is reachable and responding
│ 📊 Response Time: 2 milliseconds
│ 🔌 Port 22 is OPEN
└────────────────────────────────────────────────────────
```

**If SSH is not running:**
```
❌ Connection failed: Connection refused
```

**What This Tells You:**
- SSH is running on 192.168.1.100 (if success)
- SSH is not running or blocked (if failure)
- Can try: `ssh -p 22 user@192.168.1.100`

---

### Scenario 3: Checking Internet Connectivity

```bash
$ ./net --ping 8.8.8.8 4 5
```

**Output (successful):**
```
┌─ ICMP PING CONNECTIVITY CHECK ────────────────────────
│ Target: 8.8.8.8
│ Packets: 4
│ Timeout: 5 seconds
└────────────────────────────────────────────────────────

📍 ICMP Packet Structure Analysis
   ICMP Header (8+ bytes):
   • Type Field:     1 byte  (Echo Request = 8, Echo Reply = 0)
   • Code Field:     1 byte  (0 for echo operations)
   • Checksum:       2 bytes (one's complement of 16-bit words)
   • Identifier:     2 bytes (process ID)
   • Sequence:       2 bytes (packet number, starts at 0)
   • Payload:        variable (usually 56 bytes of timestamp data)

📍 Sending ICMP Echo Requests

   Packet 1/4: Type=8(Echo Req), Seq=0, ID=0x1a2b
   Checksum Calculation:
   • Sum all 16-bit words in packet
   • Result: 0x1234 (before one's complement)
   • Checksum: 0xedcb (one's complement)
   ✅ Sent 64 bytes to 8.8.8.8
   
   ✅ Echo Reply received from 8.8.8.8
   📊 RTT Calculation:
      Send Time:  1234567890.123456 seconds
      Recv Time:  1234567890.151234 seconds
      RTT = (recv_sec - send_sec)×1000 + (recv_usec - send_usec)/1000
      RTT = 27 milliseconds

   [Packets 2, 3, 4 similar...]

📊 ICMP Statistics
   Packets Sent: 4
   Packets Received: 4
   Packet Loss: 0.0%
   Min RTT: 25 ms
   Max RTT: 31 ms
   Avg RTT: 27 ms

✅ Host is reachable
```

**What This Tells You:**
- ✅ Internet connection is working
- ✅ Google's server is reachable
- ✅ Network is stable (0% packet loss)
- ✅ Average latency is 27ms

**If output shows timeouts:**
```
❌ Host is unreachable (0% success rate)
```
Indicates:
- Possible no internet connection
- Firewall blocking ICMP
- Target host is down

---

### Scenario 4: Finding Services on a Network Device

```bash
$ ./net --discover 192.168.1.1 3
```

**Output:**
```
┌─ SERVICE DISCOVERY SCAN ──────────────────────────────
│ Target: 192.168.1.1
│ Common Ports: 13
│ Timeout: 3 seconds
└────────────────────────────────────────────────────────

📍 Service Discovery Algorithm
   1. Create non-blocking socket for each port
   2. Initiate connection attempt (SYN packet)
   3. Monitor all sockets with select()
   4. Classify result: OPEN = connected, CLOSED = refused

📍 Initiating Connections

   ✅ Port    22 SSH            : Connection attempt initiated
   ✅ Port    23 Telnet         : Connection attempt initiated
   ✅ Port    25 SMTP           : Connection attempt initiated
   ... [more ports] ...
   ✅ Port   443 HTTPS          : Connection attempt initiated

📍 Waiting for Responses (select)

📍 Results

   ✅ Port    22 SSH            : OPEN
   ❌ Port    23 Telnet         : CLOSED/FILTERED
   ❌ Port    25 SMTP           : CLOSED/FILTERED
   ❌ Port    53 DNS            : CLOSED/FILTERED
   ✅ Port    80 HTTP           : OPEN
   ❌ Port   110 POP3           : CLOSED/FILTERED
   ❌ Port   143 IMAP           : CLOSED/FILTERED
   ✅ Port   443 HTTPS          : OPEN
   ❌ Port  3306 MySQL          : CLOSED/FILTERED
   ❌ Port  5432 PostgreSQL     : CLOSED/FILTERED
   ❌ Port  6379 Redis          : CLOSED/FILTERED
   ❌ Port 27017 MongoDB        : CLOSED/FILTERED
   ❌ Port  3389 RDP            : CLOSED/FILTERED

📊 Summary
   Total Ports Scanned: 13
   Open Ports: 3
   Closed/Filtered: 10
```

**What This Tells You:**
- Device at 192.168.1.1 has SSH (22), HTTP (80), HTTPS (443) open
- Other services are not running or blocked
- Likely a router or access point with web management interface

---

### Scenario 5: Complete Network Diagnostics

```bash
$ ./net --diagnose 192.168.1.1
```

**Output:**
```
🔧 NETWORK DIAGNOSTICS REPORT
   Target: 192.168.1.1

📋 Test Suite
   1. ICMP Ping (Reachability Test)
   2. TCP Port Scanning (Open Services Discovery)
   3. Statistics & Analysis

═════════════════════════════════════════════════════════════
TEST 1: ICMP PING (Reachability)
═════════════════════════════════════════════════════════════

[ICMP ping output for 3 packets...]

📊 ICMP Statistics
   Packets Sent: 3
   Packets Received: 3
   Packet Loss: 0.0%
   Min RTT: 1 ms
   Max RTT: 2 ms
   Avg RTT: 1 ms

✅ Host is reachable

═════════════════════════════════════════════════════════════
TEST 2: SERVICE DISCOVERY (Common Ports)
═════════════════════════════════════════════════════════════

[Service scan output...]

📊 Summary
   Total Ports Scanned: 13
   Open Ports: 3
   Closed/Filtered: 10

═════════════════════════════════════════════════════════════
DIAGNOSTICS SUMMARY
═════════════════════════════════════════════════════════════

✅ Host Reachability: ONLINE

📌 Educational Notes
   • ICMP Echo uses Type=8 for request, Type=0 for reply
   • TCP uses three-way handshake: SYN → SYN-ACK → ACK
   • Open ports accept connections; Closed ports send RST
   • Filtered ports don't respond (likely behind firewall)
```

---

## Advanced Usage Patterns

### Bash Script: Network Health Monitor

```bash
#!/bin/bash

# File: monitor_network.sh

TARGET_HOST="192.168.1.1"

echo "🔍 Network Health Monitor"
echo "Target: $TARGET_HOST"
echo ""

# Test 1: Reachability
echo "📍 Test 1: ICMP Reachability"
if /home/mel-houa/githube/net/net --ping $TARGET_HOST 1 3 >/dev/null 2>&1; then
    echo "✅ Host is reachable"
else
    echo "❌ Host is unreachable - ALERT!"
    exit 1
fi
echo ""

# Test 2: SSH Access
echo "📍 Test 2: SSH Access (Port 22)"
if /home/mel-houa/githube/net/net --tcp $TARGET_HOST 22 3 >/dev/null 2>&1; then
    echo "✅ SSH is accessible"
else
    echo "⚠️  SSH is not accessible"
fi
echo ""

# Test 3: Web Access
echo "📍 Test 3: Web Access (Port 80)"
if /home/mel-houa/githube/net/net --tcp $TARGET_HOST 80 3 >/dev/null 2>&1; then
    echo "✅ HTTP is accessible"
else
    echo "⚠️  HTTP is not accessible"
fi
echo ""

echo "✅ Health check complete!"
```

**Usage:**
```bash
chmod +x monitor_network.sh
./monitor_network.sh
```

---

### Bash Script: Port Availability Checker

```bash
#!/bin/bash

# File: check_port.sh

if [ $# -ne 2 ]; then
    echo "Usage: $0 <ip> <port>"
    exit 1
fi

IP=$1
PORT=$2

echo "Checking port $PORT on $IP..."

if /home/mel-houa/githube/net/net --tcp $IP $PORT 5 >/dev/null 2>&1; then
    echo "✅ Port $PORT is OPEN on $IP"
    exit 0
else
    echo "❌ Port $PORT is CLOSED/UNREACHABLE on $IP"
    exit 1
fi
```

**Usage:**
```bash
chmod +x check_port.sh
./check_port.sh 8.8.8.8 443
./check_port.sh 192.168.1.1 22
```

---

### Bash Script: Multi-Host Diagnostics

```bash
#!/bin/bash

# File: multi_host_diag.sh

HOSTS=(
    "8.8.8.8"
    "192.168.1.1"
    "1.1.1.1"
)

echo "🔧 Multi-Host Diagnostics Report"
echo "=================================="
echo ""

for host in "${HOSTS[@]}"; do
    echo "Analyzing: $host"
    /home/mel-houa/githube/net/net --diagnose $host
    echo ""
    echo "---"
    echo ""
done
```

**Usage:**
```bash
chmod +x multi_host_diag.sh
./multi_host_diag.sh
```

---

## Troubleshooting Guide

### Problem: "Raw socket creation failed"

```bash
$ ./net --ping 8.8.8.8
❌ Failed to create raw socket
   Note: Raw sockets require root/administrator privileges
```

**Solution**: Use sudo for ICMP operations
```bash
sudo ./net --ping 8.8.8.8
```

**Alternative**: Use TCP port check instead (no root needed)
```bash
./net --tcp 8.8.8.8 53  # Check DNS instead
```

---

### Problem: "Invalid IP address"

```bash
$ ./net --tcp 999.999.999.999 22
❌ Invalid IP address: 999.999.999.999
```

**Solution**: Use valid IPv4 address (dotted decimal, 0-255 per octet)
```bash
./net --tcp 8.8.8.8 22      # ✅ Valid
./net --tcp 192.168.1.1 22  # ✅ Valid
./net --tcp 8.8.8 22        # ❌ Invalid (missing octet)
```

---

### Problem: "Connection timeout"

```bash
⏱️  Connection timeout after 5 seconds
❌ No SYN-ACK received from server
```

**Possible Causes:**
1. Host is offline
2. Port is not listening
3. Firewall blocking the traffic
4. Network unreachable

**Solutions:**
- Verify host is online: `ping 192.168.1.1`
- Check if service is running: `ssh 192.168.1.1`
- Increase timeout: `./net --tcp 192.168.1.1 22 10`
- Verify firewall rules

---

## Performance Tips

### For Fastest Service Scanning
```bash
# Default timeout (3 seconds per port × 13 ports)
./net --discover 192.168.1.1      # ~39 seconds maximum

# Faster with shorter timeout
./net --discover 192.168.1.1 1    # ~13 seconds maximum

# Slower with longer timeout (for unreliable networks)
./net --discover 192.168.1.1 5    # ~65 seconds maximum
```

### For Minimal Network Traffic
```bash
# Single ping (1 packet instead of 4)
./net --ping 8.8.8.8 1 5

# Combined diagnostics (less repetitive)
./net --diagnose 192.168.1.1      # Uses 3 pings + 13 ports
```

### For Maximum Reliability
```bash
# More packets for better statistics
./net --ping 8.8.8.8 10 5

# Longer timeouts for unstable networks
./net --tcp 192.168.1.1 22 10
./net --discover 192.168.1.1 5
```

---

## Educational Use Cases

### Teaching Network Concepts

**Concept: TCP Three-Way Handshake**
```bash
$ ./net --tcp 8.8.8.8 53 5
# Shows students:
# - Socket creation (AF_INET, SOCK_STREAM)
# - Non-blocking I/O (fcntl O_NONBLOCK)
# - SYN flag (0x02)
# - State machine (SYN_SENT → ESTABLISHED)
# - RTT calculation
```

**Concept: ICMP Packet Structure**
```bash
$ sudo ./net --ping 8.8.8.8 1 5
# Shows students:
# - ICMP header format (Type, Code, Checksum, ID, Sequence)
# - One's complement checksum calculation
# - Timestamp embedding
# - RTT measurement
```

**Concept: Service Discovery**
```bash
$ ./net --discover 192.168.1.1
# Shows students:
# - Port numbering (22=SSH, 80=HTTP, 443=HTTPS)
# - Connection classification (OPEN, CLOSED, FILTERED)
# - Parallel socket monitoring with select()
# - Network services identification
```

---

## Integration with Other Tools

### With Traditional ping
```bash
# Compare educational ping vs standard ping
ping -c 4 8.8.8.8              # Standard tool
sudo ./net --ping 8.8.8.8 4    # Educational version
```

### With netstat
```bash
# Check local services
netstat -tlnp | grep LISTEN

# Test if ports are accessible externally
./net --tcp 192.168.1.1 22
./net --discover 192.168.1.1
```

### With curl/wget
```bash
# Test if HTTP service is running
./net --tcp example.com 80

# If port 80 is open, try HTTP request
curl http://example.com
```

---

## Summary

The connectivity features provide:

✅ **Practical Network Testing**
- TCP port checks
- ICMP reachability tests
- Service discovery scanning

✅ **Educational Explanations**
- Mathematical formulas
- Packet structure diagrams
- Step-by-step operations

✅ **Scripting Integration**
- Return codes for automation
- Plain text output
- Consistent interfaces

✅ **Real-World Applications**
- Network troubleshooting
- System administration
- Network monitoring

Happy networking! 🚀
