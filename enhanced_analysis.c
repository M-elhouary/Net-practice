/*
 * ============================================================================
 * ENHANCED NETWORK ANALYSIS FEATURES
 * ============================================================================
 * 
 * This file contains advanced network analysis functions:
 * - CIDR notation support and parsing
 * - Network class detection (A, B, C)
 * - IP range validation and checking
 * - Multi-format IP address conversion
 * 
 * Educational Focus:
 * - Uses mathematical calculations for all operations
 * - Provides detailed explanations of networking concepts
 * - Shows step-by-step calculations for learning
 * 
 * Author: Network Calculator Enhanced Edition
 * Purpose: Advanced educational network analysis tools
 */

#include "net.h"

/*
 * ============================================================================
 * CIDR NOTATION SUPPORT
 * ============================================================================
 */

/*
 * Parses CIDR notation string into IP address and prefix length
 * 
 * CIDR (Classless Inter-Domain Routing) notation combines an IP address
 * with a prefix length, like "192.168.1.0/24".
 * 
 * Mathematical background:
 * - /24 means first 24 bits are network bits
 * - Remaining 8 bits (32-24) are host bits
 * - This equals subnet mask 255.255.255.0
 * 
 * @param cidr_str: CIDR string like "192.168.1.0/24"
 * @param ip_out: Buffer to store extracted IP (must be at least 16 chars)
 * @param prefix_len: Pointer to store prefix length
 * @return: 1 if valid CIDR format, 0 if invalid
 */
int parse_cidr_notation(const char *cidr_str, char *ip_out, int *prefix_len)
{
    printf("🔍 Parsing CIDR notation: %s\n", cidr_str);
    
    if (!cidr_str || !ip_out || !prefix_len) {
        printf("❌ Invalid input parameters\n");
        return 0;
    }
    
    // Find the '/' separator
    char *slash_pos = strchr(cidr_str, '/');
    if (!slash_pos) {
        printf("❌ No '/' found in CIDR notation\n");
        return 0;
    }
    
    // Calculate IP address length (before the '/')
    size_t ip_len = slash_pos - cidr_str;
    if (ip_len >= 16) {  // Max IP length is "255.255.255.255" = 15 chars
        printf("❌ IP address part too long\n");
        return 0;
    }
    
    // Extract IP address part
    strncpy(ip_out, cidr_str, ip_len);
    ip_out[ip_len] = '\0';
    
    // Extract and validate prefix length
    *prefix_len = atoi(slash_pos + 1);
    
    if (*prefix_len < 0 || *prefix_len > 32) {
        printf("❌ Invalid prefix length: %d (must be 0-32)\n", *prefix_len);
        return 0;
    }
    
    printf("✅ CIDR parsed successfully:\n");
    printf("   📍 Network IP: %s\n", ip_out);
    printf("   📏 Prefix Length: /%d\n", *prefix_len);
    printf("   🧮 Network Bits: %d, Host Bits: %d\n", *prefix_len, 32 - *prefix_len);
    
    return 1;
}

/*
 * Converts CIDR prefix length to subnet mask using mathematical calculation
 * 
 * Mathematical approach:
 * 1. Create mask with 'prefix_len' number of 1s followed by 0s
 * 2. Convert to decimal octets using base-256 division
 * 
 * Examples:
 * - /24: 24 ones = 11111111 11111111 11111111 00000000 = 255.255.255.0
 * - /28: 28 ones = 11111111 11111111 11111111 11110000 = 255.255.255.240
 * 
 * @param prefix_len: CIDR prefix length (0-32)
 * @return: Dynamically allocated subnet mask string (caller must free)
 */
char *cidr_to_subnet_mask(int prefix_len)
{
    printf("🧮 Converting CIDR /%d to subnet mask...\n", prefix_len);
    
    if (prefix_len < 0 || prefix_len > 32) {
        printf("❌ Invalid prefix length: %d\n", prefix_len);
        return NULL;
    }
    
    // Calculate the 32-bit mask value
    // For /24: we want 24 ones followed by 8 zeros
    unsigned int mask_value;
    
    if (prefix_len == 0) {
        mask_value = 0;  // 0.0.0.0
    } else if (prefix_len == 32) {
        mask_value = 0xFFFFFFFF;  // 255.255.255.255
    } else {
        // Mathematical calculation: create mask with prefix_len ones
        // Shift 1 left by (32 - prefix_len) positions, then subtract 1
        // This creates the host bits pattern, then we invert it
        mask_value = 0xFFFFFFFF << (32 - prefix_len);
    }
    
    printf("🔢 Calculated mask value: %u (0x%08X)\n", mask_value, mask_value);
    
    // Convert to dotted decimal using mathematical division
    unsigned int octet_a = (mask_value >> 24) & 0xFF;  // Extract bits 31-24
    unsigned int octet_b = (mask_value >> 16) & 0xFF;  // Extract bits 23-16
    unsigned int octet_c = (mask_value >> 8) & 0xFF;   // Extract bits 15-8
    unsigned int octet_d = mask_value & 0xFF;          // Extract bits 7-0
    
    printf("🔍 Mathematical breakdown:\n");
    printf("   Octet A: (mask >> 24) & 0xFF = %u\n", octet_a);
    printf("   Octet B: (mask >> 16) & 0xFF = %u\n", octet_b);
    printf("   Octet C: (mask >> 8) & 0xFF = %u\n", octet_c);
    printf("   Octet D: mask & 0xFF = %u\n", octet_d);
    
    // Allocate and format result string
    char *result = malloc(16);  // "255.255.255.255" + null terminator
    if (!result) {
        printf("❌ Memory allocation failed\n");
        return NULL;
    }
    
    sprintf(result, "%u.%u.%u.%u", octet_a, octet_b, octet_c, octet_d);
    
    printf("✅ CIDR /%d → Subnet Mask: %s\n", prefix_len, result);
    
    return result;
}

/*
 * Complete CIDR network analysis with comprehensive information
 * 
 * This function provides full analysis of a CIDR network including:
 * - CIDR parsing and validation
 * - Subnet mask conversion
 * - Network range calculation
 * - Available IP count
 * 
 * @param cidr_str: CIDR string like "192.168.1.0/24"
 */
void analyze_cidr_network(const char *cidr_str)
{
    printf("🌐 Comprehensive CIDR Network Analysis\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    char ip_str[16];
    int prefix_len;
    
    // Parse CIDR notation
    if (!parse_cidr_notation(cidr_str, ip_str, &prefix_len)) {
        printf("❌ Invalid CIDR format: %s\n", cidr_str);
        return;
    }
    
    // Convert to subnet mask
    char *mask_str = cidr_to_subnet_mask(prefix_len);
    if (!mask_str) {
        printf("❌ Failed to convert CIDR to subnet mask\n");
        return;
    }
    
    char prefix_str[5];  // buffer for "/xx"
    snprintf(prefix_str, sizeof(prefix_str), "/%d", prefix_len);

    printf("\n📊 CIDR Network Summary:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ Original CIDR:     %-36s │\n", cidr_str);
    printf("│ Network IP:        %-36s │\n", ip_str);
    printf("│ Prefix Length:     %-36s │\n", prefix_str);
    printf("│ Subnet Mask:       %-36s │\n", mask_str);
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    // Perform complete network analysis
    printf("\n=== Detailed Network Analysis ===\n");
    print_ip_range(ip_str, mask_str);
    
    free(mask_str);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}


/*
 * ============================================================================
 * NETWORK CLASS DETECTION
 * ============================================================================
 */

/*
 * Returns network class for a given IP address
 * 
 * Historical IP address classes (before CIDR):
 * - Class A: 1.0.0.0 - 126.255.255.255 (/8 networks)
 * - Class B: 128.0.0.0 - 191.255.255.255 (/16 networks)  
 * - Class C: 192.0.0.0 - 223.255.255.255 (/24 networks)
 * - Class D: 224.0.0.0 - 239.255.255.255 (Multicast)
 * - Class E: 240.0.0.0 - 255.255.255.255 (Reserved)
 * 
 * @param ip: IP address as 32-bit integer
 * @return: Constant string describing the class
 */
const char *get_network_class(unsigned int ip)
{
    // Extract first octet using mathematical division
    unsigned int first_octet = ip / 16777216;  // ip / 256^3
    
    printf("🔍 Class determination: First octet = %u\n", first_octet);
    
    if (first_octet >= 1 && first_octet <= 126) {
        return "Class A";
    } else if (first_octet >= 128 && first_octet <= 191) {
        return "Class B";
    } else if (first_octet >= 192 && first_octet <= 223) {
        return "Class C";
    } else if (first_octet >= 224 && first_octet <= 239) {
        return "Class D (Multicast)";
    } else if (first_octet >= 240 && first_octet <= 255) {
        return "Class E (Reserved)";
    } else if (first_octet == 127) {
        return "Loopback";
    } else {
        return "Reserved/Invalid";
    }
}

/*
 * Comprehensive IP address classification analysis
 * 
 * Provides detailed information about IP address class,
 * historical context, and modern usage.
 * 
 * @param ip_str: IP address string
 */
void classify_ip_address(const char *ip_str)
{
    printf("📋 Network Class Analysis\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    unsigned int ip = ip_to_int(ip_str);
    if (ip == 0) {
        printf("❌ Invalid IP address: %s\n", ip_str);
        return;
    }
    
    const char *class_name = get_network_class(ip);
    
    printf("📍 Analyzing: %s (decimal: %u)\n", ip_str, ip);
    printf("🏷️  Network Class: %s\n", class_name);
    
    // Provide detailed class information
    if (strncmp(class_name, "Class A", 7) == 0) {
        printf("\n📚 Class A Network Information:\n");
        printf("┌─────────────────────────────────────────────────────────┐\n");
        printf("│ Range: 1.0.0.0 - 126.255.255.255                       │\n");
        printf("│ Default Mask: 255.0.0.0 (/8)                           │\n");
        printf("│ Networks: 126 possible networks                        │\n");
        printf("│ Hosts per Network: 16,777,214                          │\n");
        printf("│ Usage: Large organizations, ISPs                       │\n");
        printf("└─────────────────────────────────────────────────────────┘\n");
    }
    else if (strncmp(class_name, "Class B", 7) == 0) {
        printf("\n📚 Class B Network Information:\n");
        printf("┌─────────────────────────────────────────────────────────┐\n");
        printf("│ Range: 128.0.0.0 - 191.255.255.255                     │\n");
        printf("│ Default Mask: 255.255.0.0 (/16)                        │\n");
        printf("│ Networks: 16,384 possible networks                     │\n");
        printf("│ Hosts per Network: 65,534                              │\n");
        printf("│ Usage: Medium-sized organizations                      │\n");
        printf("└─────────────────────────────────────────────────────────┘\n");
    }
    else if (strncmp(class_name, "Class C", 7) == 0) {
        printf("\n📚 Class C Network Information:\n");
        printf("┌─────────────────────────────────────────────────────────┐\n");
        printf("│ Range: 192.0.0.0 - 223.255.255.255                     │\n");
        printf("│ Default Mask: 255.255.255.0 (/24)                      │\n");
        printf("│ Networks: 2,097,152 possible networks                  │\n");
        printf("│ Hosts per Network: 254                                 │\n");
        printf("│ Usage: Small networks, home/office                     │\n");
        printf("└─────────────────────────────────────────────────────────┘\n");
    }
    else if (strstr(class_name, "Multicast")) {
        printf("\n📡 Multicast Address Information:\n");
        printf("┌─────────────────────────────────────────────────────────┐\n");
        printf("│ Range: 224.0.0.0 - 239.255.255.255                     │\n");
        printf("│ Purpose: One-to-many communication                     │\n");
        printf("│ Usage: Streaming, conferencing, routing protocols      │\n");
        printf("│ Note: Not assigned to individual hosts                 │\n");
        printf("└─────────────────────────────────────────────────────────┘\n");
    }
    
    printf("\n🏛️  Historical Context:\n");
    printf("   • Classes were used before CIDR (1993)\n");
    printf("   • Modern networks use CIDR for flexibility\n");
    printf("   • Class system led to IP address waste\n");
    printf("   • CIDR allows variable-length subnet masks\n");
    
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

/*
 * ============================================================================
 * IP RANGE VALIDATION
 * ============================================================================
 */

/*
 * Checks if an IP address is within a specific network range
 * 
 * Mathematical approach:
 * 1. Convert IP, network, and mask to integers
 * 2. Calculate network address: network AND mask
 * 3. Calculate IP's network: ip AND mask  
 * 4. Compare: if equal, IP is in network
 * 
 * @param ip_str: IP to check
 * @param network_str: Network address
 * @param mask_str: Subnet mask
 * @return: 1 if IP is in network, 0 if not
 */
int is_ip_in_network(const char *ip_str, const char *network_str, const char *mask_str)
{
    unsigned int ip = ip_to_int(ip_str);
    unsigned int network = ip_to_int(network_str);
    unsigned int mask = mask_to_int(mask_str);
    
    if (ip == 0 || network == 0 || mask == 0) {
        printf("❌ Invalid IP address or network parameters\n");
        return 0;
    }
    
    // Calculate the network address for the given IP
    unsigned int ip_network = calculate_network_address(ip, mask);
    unsigned int actual_network = calculate_network_address(network, mask);
    
    printf("🧮 Network validation calculation:\n");
    printf("   IP (%s) network: %u\n", ip_str, ip_network);
    printf("   Given network: %u\n", actual_network);
    printf("   Match: %s\n", (ip_network == actual_network) ? "YES" : "NO");
    
    return (ip_network == actual_network);
}

/*
 * Validates IP within CIDR network range with detailed output
 * 
 * @param ip_str: IP address to validate
 * @param cidr_str: CIDR network string
 */
void validate_ip_in_range(const char *ip_str, const char *cidr_str)
{
    printf("🎯 IP Range Validation\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    char network_ip[16];
    int prefix_len;
    
    // Parse CIDR
    if (!parse_cidr_notation(cidr_str, network_ip, &prefix_len)) {
        printf("❌ Invalid CIDR format: %s\n", cidr_str);
        return;
    }
    
    // Convert to subnet mask
    char *mask_str = cidr_to_subnet_mask(prefix_len);
    if (!mask_str) {
        return;
    }
    
    printf("\n🔍 Checking if %s is in network %s\n", ip_str, cidr_str);
    
    int is_in_network = is_ip_in_network(ip_str, network_ip, mask_str);
    
    printf("\n📋 Validation Summary:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ Target IP:         %-36s │\n", ip_str);
    printf("│ Network:           %-36s │\n", cidr_str);
    printf("│ Network IP:        %-36s │\n", network_ip);
    printf("│ Subnet Mask:       %-36s │\n", mask_str);
    printf("│ Result:            %-36s │\n", is_in_network ? "✅ IP IS in network" : "❌ IP NOT in network");
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    free(mask_str);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

/*
 * ============================================================================
 * MULTI-FORMAT IP CONVERTER
 * ============================================================================
 */

/*
 * Shows IP address in multiple formats for educational purposes
 * 
 * Displays:
 * - Decimal dotted notation (192.168.1.1)
 * - 32-bit integer (3232235777)
 * - Hexadecimal (0xC0A80101)
 * - Binary with octet separation
 * 
 * @param ip_str: IP address string
 */
void convert_ip_formats(const char *ip_str)
{
    printf("🔄 Multi-Format IP Address Converter\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    unsigned int ip = ip_to_int(ip_str);
    if (ip == 0) {
        printf("❌ Invalid IP address: %s\n", ip_str);
        return;
    }
    
    // Extract octets using mathematical division
    unsigned int octet_a = ip / 16777216;           // ip / 256^3
    unsigned int octet_b = (ip % 16777216) / 65536; // (ip % 256^3) / 256^2
    unsigned int octet_c = (ip % 65536) / 256;      // (ip % 256^2) / 256^1
    unsigned int octet_d = ip % 256;                // ip % 256^1
    
    printf("📍 Converting: %s\n", ip_str);
    
    printf("\n🎨 Multiple Format Representations:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ Decimal Dotted:    %-36s │\n", ip_str);
    printf("│ 32-bit Integer:    %-36u │\n", ip);
    printf("│ Hexadecimal:       0x%08X                               │\n", ip);
    printf("│ Binary (grouped):  ");
    
    // Show binary representation with octet grouping
    for (int i = 0; i < 4; i++) {
        unsigned int octet = (i == 0) ? octet_a : (i == 1) ? octet_b : (i == 2) ? octet_c : octet_d;
        for (int bit = 7; bit >= 0; bit--) {
            printf("%d", (octet >> bit) & 1);
        }
        if (i < 3) printf(" ");
    }
    printf(" │\n");
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    printf("\n🧮 Mathematical Breakdown:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ Octet A (%3u): %3u × 256³ = %u                     │\n", octet_a, octet_a, octet_a * 16777216);
    printf("│ Octet B (%3u): %3u × 256² = %u                       │\n", octet_b, octet_b, octet_b * 65536);
    printf("│ Octet C (%3u): %3u × 256¹ = %u                         │\n", octet_c, octet_c, octet_c * 256);
    printf("│ Octet D (%3u): %3u × 256⁰ = %u                           │\n", octet_d, octet_d, octet_d);
    printf("│ Total Sum:      = %u                        │\n", ip);
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    printf("\n💡 Educational Notes:\n");
    printf("   • Each octet represents 8 bits (0-255)\n");
    printf("   • IP = A×256³ + B×256² + C×256¹ + D×256⁰\n");
    printf("   • Hexadecimal is base-16 representation\n");
    printf("   • Binary shows actual bits used by computers\n");
    
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}
