/*
 * ============================================================================
 * NETWORK DIAGNOSTICS - LIVE CONNECTIVITY & SERVICE DISCOVERY
 * ============================================================================
 * 
 * This file implements live network connectivity testing and service discovery
 * using pure POSIX sockets. All operations are explained mathematically and
 * educationally, consistent with the base-256 philosophy of this tool.
 * 
 * Features:
 * 1. TCP Port Connectivity Check - Three-way handshake analysis
 * 2. ICMP Ping - Echo Request/Reply packet structure analysis
 * 3. Service Discovery - Common port scanning with state tracking
 * 4. Comprehensive Diagnostics Report - Network health assessment
 * 
 * Mathematical Foundation:
 * - TCP Sequence Numbers: Random initial value × state transitions
 * - ICMP Checksum: Sum of 16-bit words, then one's complement
 * - RTT Calculation: (end_time - start_time) in milliseconds
 * - Port Status: Based on socket connection result codes
 * 
 * Author: Network Tools Development Team
 * ============================================================================
 */

#include "net.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <limits.h>
#include <string.h>

/*
 * ============================================================================
 * TCP CONNECTIVITY CHECK
 * ============================================================================
 * 
 * Educational Purpose:
 * Demonstrates the TCP three-way handshake at the socket level.
 * Shows how kernel manages state transitions and sequence numbers.
 */

/*
 * Calculates elapsed time in milliseconds between two timevals
 * Mathematical Formula: (end.tv_sec - start.tv_sec) × 1000 + 
 *                       (end.tv_usec - start.tv_usec) / 1000
 * 
 * @param start: Starting time structure
 * @param end: Ending time structure
 * @return: Elapsed milliseconds
 */
static long calculate_elapsed_ms(struct timeval start, struct timeval end)
{
    long seconds = (end.tv_sec - start.tv_sec) * 1000;
    long microseconds = (end.tv_usec - start.tv_usec) / 1000;
    return seconds + microseconds;
}

/*
 * Performs TCP connection attempt with non-blocking socket and timeout
 * 
 * TCP Three-Way Handshake Explanation:
 * ═══════════════════════════════════════
 * 1. Client → Server: SYN packet (SEQ=X, Flags=0x02)
 *    - Kernel generates random initial sequence number X
 *    - Socket enters SYN_SENT state
 * 
 * 2. Server → Client: SYN-ACK packet (SEQ=Y, ACK=X+1, Flags=0x12)
 *    - Server acknowledges client's sequence with X+1
 *    - Server sends its own sequence Y
 * 
 * 3. Client → Server: ACK packet (SEQ=X+1, ACK=Y+1, Flags=0x10)
 *    - Client acknowledges server's sequence
 *    - Connection enters ESTABLISHED state
 * 
 * @param ip: Target IP address string
 * @param port: Target port (1-65535)
 * @param timeout_sec: Connection timeout in seconds
 * @return: 1 if connection successful, 0 if failed
 */
int check_tcp_connectivity(const char *ip, int port, int timeout_sec)
{
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    
    // Validate port range
    if (port < 1 || port > 65535)
    {
        printf("❌ Invalid port number: %d (must be 1-65535)\n", port);
        return 0;
    }
    
    print_colored("\033[94m", "┌─ TCP CONNECTIVITY CHECK ─────────────────────────────\n");
    print_colored("\033[94m", "│ Target: ");
    print_colored("\033[97m", "%s:%d\n", ip, port);
    print_colored("\033[94m", "│ Timeout: ");
    print_colored("\033[97m", "%d seconds\n", timeout_sec);
    print_colored("\033[94m", "│ Method: Non-blocking socket with configurable timeout\n");
    print_colored("\033[94m", "└────────────────────────────────────────────────────────\n\n");
    
    // Step 1: Create socket (TCP, IPv4)
    print_colored("\033[96m", "📍 Step 1: Create TCP Socket\n");
    printf("   Creating socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)...\n");
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
    {
        print_colored("\033[91m", "   ❌ Socket creation failed: %s\n", strerror(errno));
        return 0;
    }
    printf("   ✅ Socket created (file descriptor: %d)\n\n", sock);
    
    // Step 2: Set non-blocking mode
    print_colored("\033[96m", "📍 Step 2: Set Non-Blocking Mode (fcntl)\n");
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    printf("   ✅ Non-blocking mode enabled for timeout control\n\n");
    
    // Step 3: Parse target IP and prepare sockaddr_in structure
    print_colored("\033[96m", "📍 Step 3: Prepare Connection Address\n");
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);  // Convert port to network byte order
    
    printf("   • Address Family: AF_INET (IPv4)\n");
    printf("   • Port Conversion: %d (decimal) → 0x%04x (network byte order)\n", 
           port, htons(port));
    
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
    {
        print_colored("\033[91m", "   ❌ Invalid IP address: %s\n", ip);
        close(sock);
        return 0;
    }
    printf("   • IP Address: %s → 0x%08x (32-bit)\n", ip, 
           (unsigned int)server_addr.sin_addr.s_addr);
    printf("   ✅ Address structure prepared\n\n");
    
    // Step 4: Initiate connection (non-blocking)
    print_colored("\033[96m", "📍 Step 4: Initiate TCP Connection (SYN)\n");
    printf("   Sending SYN packet with socket flags: 0x02\n");
    printf("   Kernel generates random initial sequence number (ISN)\n");
    printf("   Socket enters SYN_SENT state...\n");
    
    int connect_result = connect(sock, (struct sockaddr *)&server_addr, 
                                 sizeof(server_addr));
    
    if (connect_result < 0)
    {
        if (errno != EINPROGRESS)
        {
            print_colored("\033[91m", "   ❌ Connection failed: %s\n", strerror(errno));
            close(sock);
            return 0;
        }
    }
    
    // Step 5: Wait for connection with select() timeout
    print_colored("\033[96m", "📍 Step 5: Wait for SYN-ACK (Timeout Handling)\n");
    printf("   Using select() to wait for writeable socket\n");
    printf("   Timeout: %d seconds (user-configurable)\n", timeout_sec);
    
    fd_set write_set;
    FD_ZERO(&write_set);
    FD_SET(sock, &write_set);
    
    struct timeval timeout;
    timeout.tv_sec = timeout_sec;
    timeout.tv_usec = 0;
    
    int select_result = select(sock + 1, NULL, &write_set, NULL, &timeout);
    
    if (select_result <= 0)
    {
        print_colored("\033[91m", "   ⏱️  Connection timeout after %d seconds\n", timeout_sec);
        print_colored("\033[91m", "   ❌ No SYN-ACK received from server\n");
        close(sock);
        return 0;
    }
    
    // Step 6: Check for connection errors
    print_colored("\033[96m", "📍 Step 6: Verify Connection Status\n");
    int error = 0;
    socklen_t error_len = sizeof(error);
    getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &error_len);
    
    if (error != 0)
    {
        print_colored("\033[91m", "   ❌ Connection failed: %s (errno: %d)\n", 
                     strerror(error), error);
        close(sock);
        return 0;
    }
    
    printf("   ✅ Socket is writable - ACK received\n");
    printf("   ✅ Connection established (ESTABLISHED state)\n\n");
    
    // Step 7: Calculate RTT
    gettimeofday(&end_time, NULL);
    long rtt_ms = calculate_elapsed_ms(start_time, end_time);
    
    print_colored("\033[96m", "📍 Step 7: Measure Round-Trip Time\n");
    printf("   Start Time:  %ld.%06ld seconds\n", start_time.tv_sec, 
           start_time.tv_usec);
    printf("   End Time:    %ld.%06ld seconds\n", end_time.tv_sec, 
           end_time.tv_usec);
    printf("   RTT Formula: (end_sec - start_sec) × 1000 + (end_usec - start_usec) / 1000\n");
    printf("   RTT Calculation: (elapsed milliseconds) = %ld ms\n\n", rtt_ms);
    
    // Step 8: Print success summary
    print_colored("\033[92m", "┌─ TCP CONNECTION SUCCESSFUL ──────────────────────────\n");
    print_colored("\033[92m", "│ ✅ Target is reachable and responding\n");
    print_colored("\033[92m", "│ 📊 Response Time: %ld milliseconds\n", rtt_ms);
    print_colored("\033[92m", "│ 🔌 Port %d is OPEN\n", port);
    print_colored("\033[92m", "└────────────────────────────────────────────────────────\n\n");
    
    close(sock);
    return 1;
}

/*
 * ============================================================================
 * ICMP PING IMPLEMENTATION
 * ============================================================================
 * 
 * Educational Purpose:
 * Demonstrates ICMP Echo Request/Reply packet structure and checksum
 * calculation. Shows time measurement and statistics gathering.
 */

/*
 * Calculates ICMP checksum using one's complement arithmetic
 * 
 * Checksum Algorithm:
 * 1. Treat ICMP data as 16-bit words
 * 2. Sum all 16-bit words (with carry propagation)
 * 3. Take one's complement (flip all bits) of the result
 * 
 * Formula: ICMP_Checksum = ~(sum of all 16-bit words)
 * 
 * @param data: ICMP packet data pointer
 * @param len: Length of data in bytes
 * @return: 16-bit checksum value
 */
static unsigned short calculate_icmp_checksum(void *data, int len)
{
    unsigned short *ptr = (unsigned short *)data;
    unsigned int sum = 0;
    unsigned short result;
    
    // Sum all 16-bit words
    while (len > 1)
    {
        sum += *ptr++;
        len -= 2;
    }
    
    // Add remaining byte if odd length
    if (len == 1)
    {
        sum += *(unsigned char *)ptr;
    }
    
    // Add carry to sum
    while (sum >> 16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    
    // Take one's complement
    result = ~sum;
    return result;
}

/*
 * Performs ICMP Echo (ping) to check IP reachability
 * 
 * ICMP Echo Request/Reply Structure:
 * ═════════════════════════════════════
 * 
 * ICMP Header (8 bytes minimum):
 * [Type (1 byte)] [Code (1 byte)] [Checksum (2 bytes)] 
 * [ID (2 bytes)]  [Sequence (2 bytes)] [Payload (variable)]
 * 
 * Echo Request: Type=8,  Code=0
 * Echo Reply:   Type=0,  Code=0
 * 
 * Educational Trace:
 * - Shows packet construction step-by-step
 * - Calculates checksum with formula
 * - Measures RTT using gettimeofday() precision
 * - Displays time differences mathematically
 * 
 * @param ip: Target IP address string
 * @param packet_count: Number of ICMP packets to send
 * @param timeout_sec: Timeout for receiving reply
 * @return: 1 if at least one reply received, 0 if all failed
 */
int perform_icmp_ping(const char *ip, int packet_count, int timeout_sec)
{
    print_colored("\033[94m", "┌─ ICMP PING CONNECTIVITY CHECK ────────────────────────\n");
    print_colored("\033[94m", "│ Target: ");
    print_colored("\033[97m", "%s\n", ip);
    print_colored("\033[94m", "│ Packets: ");
    print_colored("\033[97m", "%d\n", packet_count);
    print_colored("\033[94m", "│ Timeout: ");
    print_colored("\033[97m", "%d seconds\n", timeout_sec);
    print_colored("\033[94m", "└────────────────────────────────────────────────────────\n\n");
    
    // Create raw socket for ICMP
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0)
    {
        print_colored("\033[91m", "❌ Failed to create raw socket\n");
        print_colored("\033[93m", "   Note: Raw sockets require root/administrator privileges\n");
        return 0;
    }
    
    // Prepare target address
    struct sockaddr_in target_addr;
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    
    if (inet_pton(AF_INET, ip, &target_addr.sin_addr) <= 0)
    {
        print_colored("\033[91m", "❌ Invalid IP address: %s\n", ip);
        close(sock);
        return 0;
    }
    
    print_colored("\033[96m", "📍 ICMP Packet Structure Analysis\n");
    printf("   ICMP Header (8+ bytes):\n");
    printf("   • Type Field:     1 byte  (Echo Request = 8, Echo Reply = 0)\n");
    printf("   • Code Field:     1 byte  (0 for echo operations)\n");
    printf("   • Checksum:       2 bytes (one's complement of 16-bit words)\n");
    printf("   • Identifier:     2 bytes (process ID)\n");
    printf("   • Sequence:       2 bytes (packet number, starts at 0)\n");
    printf("   • Payload:        variable (usually 56 bytes of timestamp data)\n\n");
    
    uint16_t icmp_id = (uint16_t)(getpid() & 0xFFFF);
    int success_count = 0;
    long min_rtt = LLONG_MAX, max_rtt = 0;
    long total_rtt = 0;
    
    print_colored("\033[96m", "📍 Sending ICMP Echo Requests\n\n");
    
    for (int i = 0; i < packet_count; i++)
    {
        // ICMP Echo Request structure (portable)
        struct {
            uint8_t type;      // Type = 8 for echo request
            uint8_t code;      // Code = 0
            uint16_t checksum; // Checksum (calculated)
            uint16_t id;       // Identifier (usually process ID)
            uint16_t seq;      // Sequence number
            uint8_t data[56];  // Payload data
        } icmp_packet;
        
        char send_buffer[64];
        struct timeval send_time, recv_time;
        
        // Clear packet buffer
        memset(&icmp_packet, 0, sizeof(icmp_packet));
        memset(send_buffer, 0, sizeof(send_buffer));
        
        // Set ICMP header fields
        icmp_packet.type = 8;          // Echo Request
        icmp_packet.code = 0;          // Code 0
        icmp_packet.id = htons(icmp_id);
        icmp_packet.seq = htons((uint16_t)i);
        icmp_packet.checksum = 0;      // Will calculate after filling data
        
        // Store current time as payload for RTT calculation
        gettimeofday(&send_time, NULL);
        memcpy(&icmp_packet.data, &send_time, sizeof(send_time));
        
        // Copy to send buffer
        memcpy(send_buffer, &icmp_packet, sizeof(icmp_packet));
        
        // Calculate checksum
        uint16_t checksum = calculate_icmp_checksum(send_buffer, sizeof(icmp_packet));
        
        // Put checksum back in packet
        ((uint16_t *)send_buffer)[1] = checksum;
        
        printf("   Packet %d/%d: ", i + 1, packet_count);
        printf("Type=8(Echo Req), Seq=%d, ID=0x%04x\n", i, icmp_id);
        printf("   Checksum Calculation:\n");
        printf("   • Sum all 16-bit words in packet\n");
        printf("   • Result: 0x%04x (before one's complement)\n", 
               ~checksum);
        printf("   • Checksum: 0x%04x (one's complement)\n", checksum);
        
        // Send ICMP packet
        int bytes_sent = sendto(sock, send_buffer, sizeof(icmp_packet), 0,
                               (struct sockaddr *)&target_addr, sizeof(target_addr));
        
        if (bytes_sent < 0)
        {
            print_colored("\033[91m", "   ❌ Failed to send packet\n");
            continue;
        }
        
        printf("   ✅ Sent %d bytes to %s\n\n", bytes_sent, ip);
        
        // Set socket timeout for receiving
        struct timeval timeout;
        timeout.tv_sec = timeout_sec;
        timeout.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        
        // Receive ICMP reply
        struct sockaddr_in reply_addr;
        socklen_t reply_addr_len = sizeof(reply_addr);
        char recv_buffer[256];
        
        int bytes_recv = recvfrom(sock, recv_buffer, sizeof(recv_buffer), 0,
                                  (struct sockaddr *)&reply_addr, &reply_addr_len);
        
        if (bytes_recv > 0)
        {
            gettimeofday(&recv_time, NULL);
            long rtt_ms = calculate_elapsed_ms(send_time, recv_time);
            
            printf("   ✅ Echo Reply received from %s\n", ip);
            printf("   📊 RTT Calculation:\n");
            printf("      Send Time:  %ld.%06ld seconds\n", send_time.tv_sec, 
                   send_time.tv_usec);
            printf("      Recv Time:  %ld.%06ld seconds\n", recv_time.tv_sec, 
                   recv_time.tv_usec);
            printf("      RTT = (recv_sec - send_sec)×1000 + (recv_usec - send_usec)/1000\n");
            printf("      RTT = %ld milliseconds\n\n", rtt_ms);
            
            success_count++;
            total_rtt += rtt_ms;
            if (rtt_ms < min_rtt) min_rtt = rtt_ms;
            if (rtt_ms > max_rtt) max_rtt = rtt_ms;
        }
        else
        {
            print_colored("\033[91m", "   ⏱️  No reply received (timeout after %d seconds)\n\n", 
                         timeout_sec);
        }
    }
    
    close(sock);
    
    // Print statistics
    print_colored("\033[96m", "📊 ICMP Statistics\n");
    printf("   Packets Sent: %d\n", packet_count);
    printf("   Packets Received: %d\n", success_count);
    
    if (success_count > 0)
    {
        float loss = ((float)(packet_count - success_count) / packet_count) * 100;
        long avg_rtt = total_rtt / success_count;
        
        printf("   Packet Loss: %.1f%%\n", loss);
        printf("   Min RTT: %ld ms\n", min_rtt);
        printf("   Max RTT: %ld ms\n", max_rtt);
        printf("   Avg RTT: %ld ms\n\n", avg_rtt);
        
        print_colored("\033[92m", "✅ Host is reachable\n\n");
        return 1;
    }
    else
    {
        print_colored("\033[91m", "❌ Host is unreachable (0%% success rate)\n\n");
        return 0;
    }
}

/*
 * ============================================================================
 * SERVICE DISCOVERY - COMMON PORT SCANNING
 * ============================================================================
 * 
 * Educational Purpose:
 * Demonstrates rapid port scanning using non-blocking sockets
 * and state monitoring. Shows which ports are open/closed/filtered.
 */

typedef struct
{
    const char *name;
    int port;
} CommonPort;

static const CommonPort COMMON_PORTS[] = {
    {"SSH", 22},
    {"Telnet", 23},
    {"SMTP", 25},
    {"DNS", 53},
    {"HTTP", 80},
    {"POP3", 110},
    {"IMAP", 143},
    {"HTTPS", 443},
    {"MySQL", 3306},
    {"PostgreSQL", 5432},
    {"Redis", 6379},
    {"MongoDB", 27017},
    {"RDP", 3389}
};

#define NUM_COMMON_PORTS (sizeof(COMMON_PORTS) / sizeof(COMMON_PORTS[0]))

/*
 * Scans common service ports on target IP with parallel non-blocking approach
 * 
 * Service Discovery Algorithm:
 * 1. Create socket for each port
 * 2. Set to non-blocking mode
 * 3. Initiate connections in parallel
 * 4. Use select() to check which sockets are connected
 * 5. Classify as OPEN/CLOSED based on connection result
 * 
 * @param ip: Target IP address string
 * @param timeout_sec: Connection timeout
 */
void scan_services_in_range(const char *ip, int timeout_sec)
{
    print_colored("\033[94m", "┌─ SERVICE DISCOVERY SCAN ──────────────────────────────\n");
    print_colored("\033[94m", "│ Target: ");
    print_colored("\033[97m", "%s\n", ip);
    print_colored("\033[94m", "│ Common Ports: %lu\n", NUM_COMMON_PORTS);
    print_colored("\033[94m", "│ Timeout: ");
    print_colored("\033[97m", "%d seconds\n", timeout_sec);
    print_colored("\033[94m", "└────────────────────────────────────────────────────────\n\n");
    
    print_colored("\033[96m", "📍 Service Discovery Algorithm\n");
    printf("   1. Create non-blocking socket for each port\n");
    printf("   2. Initiate connection attempt (SYN packet)\n");
    printf("   3. Monitor all sockets with select()\n");
    printf("   4. Classify result: OPEN = connected, CLOSED = refused\n\n");
    
    struct sockaddr_in target_addr;
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    
    if (inet_pton(AF_INET, ip, &target_addr.sin_addr) <= 0)
    {
        print_colored("\033[91m", "❌ Invalid IP address: %s\n", ip);
        return;
    }
    
    int sockets[NUM_COMMON_PORTS];
    int open_count = 0;
    
    // Create non-blocking sockets and initiate connections
    print_colored("\033[96m", "📍 Initiating Connections\n\n");
    
    for (size_t i = 0; i < NUM_COMMON_PORTS; i++)
    {
        target_addr.sin_port = htons(COMMON_PORTS[i].port);
        
        sockets[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockets[i] < 0)
        {
            printf("   ⚠️  Port %5d: Socket creation failed\n", 
                   COMMON_PORTS[i].port);
            sockets[i] = -1;
            continue;
        }
        
        // Set non-blocking mode
        int flags = fcntl(sockets[i], F_GETFL, 0);
        fcntl(sockets[i], F_SETFL, flags | O_NONBLOCK);
        
        // Attempt connection
        int result = connect(sockets[i], (struct sockaddr *)&target_addr, 
                            sizeof(target_addr));
        
        if (result < 0 && errno != EINPROGRESS)
        {
            printf("   ❌ Port %5d %-15s: Connection error\n", 
                   COMMON_PORTS[i].port, COMMON_PORTS[i].name);
            close(sockets[i]);
            sockets[i] = -1;
        }
    }
    
    printf("\n");
    
    // Wait for connections with select()
    print_colored("\033[96m", "📍 Waiting for Responses (select)\n\n");
    
    fd_set write_set;
    FD_ZERO(&write_set);
    
    int max_fd = 0;
    for (size_t i = 0; i < NUM_COMMON_PORTS; i++)
    {
        if (sockets[i] >= 0)
        {
            FD_SET(sockets[i], &write_set);
            if (sockets[i] > max_fd)
                max_fd = sockets[i];
        }
    }
    
    struct timeval timeout;
    timeout.tv_sec = timeout_sec;
    timeout.tv_usec = 0;
    
    select(max_fd + 1, NULL, &write_set, NULL, &timeout);
    
    // Check which ports are open
    print_colored("\033[96m", "📍 Results\n\n");
    
    for (size_t i = 0; i < NUM_COMMON_PORTS; i++)
    {
        if (sockets[i] < 0)
            continue;
        
        int error = 0;
        socklen_t error_len = sizeof(error);
        getsockopt(sockets[i], SOL_SOCKET, SO_ERROR, &error, &error_len);
        
        if (error == 0 && FD_ISSET(sockets[i], &write_set))
        {
            print_colored("\033[92m", "   ✅ Port %5d %-15s: OPEN\n", 
                         COMMON_PORTS[i].port, COMMON_PORTS[i].name);
            open_count++;
        }
        else
        {
            printf("   ❌ Port %5d %-15s: CLOSED/FILTERED\n", 
                   COMMON_PORTS[i].port, COMMON_PORTS[i].name);
        }
        
        close(sockets[i]);
    }
    
    printf("\n");
    print_colored("\033[96m", "📊 Summary\n");
    printf("   Total Ports Scanned: %lu\n", NUM_COMMON_PORTS);
    printf("   Open Ports: %d\n", open_count);
    printf("   Closed/Filtered: %lu\n\n", NUM_COMMON_PORTS - open_count);
}

/*
 * ============================================================================
 * COMPREHENSIVE DIAGNOSTICS REPORT
 * ============================================================================
 * 
 * Combines all connectivity tests into one comprehensive health report
 */

/*
 * Generates comprehensive network diagnostics for a single IP
 * Tests: Ping reachability, common open ports, estimated latency
 * 
 * @param ip: Target IP address string
 */
void generate_diagnostics_report(const char *ip)
{
    draw_header_box("🔧 NETWORK DIAGNOSTICS REPORT", ip);
    
    printf("\n");
    print_colored("\033[96m", "📋 Test Suite\n");
    printf("   1. ICMP Ping (Reachability Test)\n");
    printf("   2. TCP Port Scanning (Open Services Discovery)\n");
    printf("   3. Statistics & Analysis\n\n");
    
    // Test 1: ICMP Ping
    print_colored("\033[96m", "═════════════════════════════════════════════════════════\n");
    print_colored("\033[96m", "TEST 1: ICMP PING (Reachability)\n");
    print_colored("\033[96m", "═════════════════════════════════════════════════════════\n\n");
    
    int ping_result = perform_icmp_ping(ip, 3, 5);
    
    // Test 2: TCP Port Scan
    print_colored("\033[96m", "═════════════════════════════════════════════════════════\n");
    print_colored("\033[96m", "TEST 2: SERVICE DISCOVERY (Common Ports)\n");
    print_colored("\033[96m", "═════════════════════════════════════════════════════════\n\n");
    
    scan_services_in_range(ip, 3);
    
    // Test 3: Summary
    print_colored("\033[96m", "═════════════════════════════════════════════════════════\n");
    print_colored("\033[96m", "DIAGNOSTICS SUMMARY\n");
    print_colored("\033[96m", "═════════════════════════════════════════════════════════\n\n");
    
    if (ping_result)
    {
        print_colored("\033[92m", "✅ Host Reachability: ONLINE\n");
    }
    else
    {
        print_colored("\033[91m", "❌ Host Reachability: OFFLINE/UNREACHABLE\n");
    }
    
    printf("\n");
    print_colored("\033[96m", "📌 Educational Notes\n");
    printf("   • ICMP Echo uses Type=8 for request, Type=0 for reply\n");
    printf("   • TCP uses three-way handshake: SYN → SYN-ACK → ACK\n");
    printf("   • Open ports accept connections; Closed ports send RST\n");
    printf("   • Filtered ports don't respond (likely behind firewall)\n\n");
}
