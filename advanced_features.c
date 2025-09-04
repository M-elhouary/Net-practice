/*
 * ============================================================================
 * ADVANCED NETWORK FEATURES
 * ============================================================================
 * 
 * This file contains advanced networking features:
 * - Network scanning and IP enumeration
 * - IPv6 address analysis and conversion
 * - Subnet splitting calculator (VLSM)
 * 
 * Educational Focus:
 * - Mathematical approach to all calculations
 * - Step-by-step explanations for learning
 * - Comprehensive IPv6 support for modern networking
 * 
 * Author: Network Calculator Advanced Edition
 * Purpose: Professional network analysis and IPv6 support
 */

#include "net.h"

/*
 * ============================================================================
 * NETWORK SCANNING FUNCTIONS
 * ============================================================================
 */

/*
 * Network range scanner - displays all possible IP addresses in a network
 * 
 * This function is educational and shows network enumeration concepts.
 * It demonstrates how network scanning tools work mathematically.
 * 
 * Mathematical approach:
 * 1. Parse CIDR notation to get network and prefix length
 * 2. Calculate network address and broadcast address
 * 3. Enumerate all IPs from network+1 to broadcast-1
 * 
 * @param cidr_str: CIDR network string like "192.168.1.0/24"
 */
void scan_network_range(const char *cidr_str)
{
    printf("🔍 Network Range Scanner\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    char network_ip[16];
    int prefix_len;
    
    // Parse CIDR notation
    if (!parse_cidr_notation(cidr_str, network_ip, &prefix_len)) {
        printf("❌ Invalid CIDR format: %s\n", cidr_str);
        return;
    }
    
    // Convert to subnet mask
    char *mask_str = cidr_to_subnet_mask(prefix_len);
    if (!mask_str) {
        return;
    }
    
    unsigned int network = ip_to_int(network_ip);
    unsigned int mask = mask_to_int(mask_str);
    
    if (network == 0 || mask == 0) {
        printf("❌ Invalid network or mask\n");
        free(mask_str);
        return;
    }
    
    // Calculate network boundaries
    unsigned int network_addr = calculate_network_address(network, mask);
    unsigned int broadcast_addr = calculate_broadcast_address(network_addr, mask);
    
    // Calculate host count
    int host_bits = 32 - prefix_len;
    int total_ips = 1;
    for (int i = 0; i < host_bits; i++) {
        total_ips *= 2;
    }
    
    printf("📊 Network Scan Summary:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ Target Network:    %-36s │\n", cidr_str);
    printf("│ Network Address:   %-36s │\n", int_to_ip(network_addr));
    printf("│ Broadcast Address: %-36s │\n", int_to_ip(broadcast_addr));
    printf("│ Subnet Mask:       %-36s │\n", mask_str);
    printf("│ Host Bits:         %-36d │\n", host_bits);
    printf("│ Total IPs:         %-36d │\n", total_ips);
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    printf("\n🎯 IP Address Enumeration:\n");
    
    // Handle special cases
    if (host_bits == 0) // /32 - single host
    {
        printf("📍 Single Host Network (/32):\n");
        char *ip_str = int_to_ip(network_addr);
        printf("   └─ %s (single host)\n", ip_str);
        free(ip_str);
    }
    else if (host_bits == 1) // /31 - point-to-point
    {
        printf("🔗 Point-to-Point Link (/31):\n");
        char *ip1 = int_to_ip(network_addr);
        char *ip2 = int_to_ip(broadcast_addr);
        printf("   ├─ %s (first endpoint)\n", ip1);
        printf("   └─ %s (second endpoint)\n", ip2);
        free(ip1);
        free(ip2);
    }
    else if (total_ips <= 64) // Small networks - show all IPs
    {
        printf("📋 Complete IP Listing:\n");
        
        char *net_str = int_to_ip(network_addr);
        char *bc_str = int_to_ip(broadcast_addr);
        
        printf("   ├─ %s (network address) ❌\n", net_str);
        
        // Show all usable IPs
        for (unsigned int ip = network_addr + 1; ip < broadcast_addr; ip++) {
            char *ip_str = int_to_ip(ip);
            printf("   ├─ %s (usable host)\n", ip_str);
            free(ip_str);
        }
        
        printf("   └─ %s (broadcast address) ❌\n", bc_str);
        
        free(net_str);
        free(bc_str);
    }
    else // Large networks - show summary
    {
        printf("📈 Large Network Summary (showing first/last 5 IPs):\n");
        
        char *net_str = int_to_ip(network_addr);
        char *bc_str = int_to_ip(broadcast_addr);
        
        printf("   ├─ %s (network address) ❌\n", net_str);
        
        // Show first 5 usable IPs
        printf("   ├─ First 5 usable IPs:\n");
        for (int i = 1; i <= 5; i++) {
            char *ip_str = int_to_ip(network_addr + i);
            printf("   │  ├─ %s\n", ip_str);
            free(ip_str);
        }
        
        printf("   │  └─ ... (%d more IPs) ...\n", total_ips - 12);
        
        // Show last 5 usable IPs
        printf("   ├─ Last 5 usable IPs:\n");
        for (int i = 5; i >= 1; i--) {
            char *ip_str = int_to_ip(broadcast_addr - i);
            printf("   │  ├─ %s\n", ip_str);
            free(ip_str);
        }
        
        printf("   └─ %s (broadcast address) ❌\n", bc_str);
        
        free(net_str);
        free(bc_str);
    }
    
    printf("\n💡 Network Scanning Notes:\n");
    printf("   • ❌ = Not usable for hosts (network/broadcast)\n");
    printf("   • This is for educational purposes only\n");
    printf("   • Real network scanning requires proper authorization\n");
    printf("   • Total usable hosts: %d\n", (total_ips > 2) ? total_ips - 2 : total_ips);
    
    free(mask_str);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

/*
 * ============================================================================
 * SUBNET SPLITTING CALCULATOR
 * ============================================================================
 */

/*
 * Subnet splitting calculator - divides a network into smaller equal subnets
 * 
 * This function demonstrates VLSM (Variable Length Subnet Masking) concepts
 * and shows how to mathematically split networks for efficient IP allocation.
 * 
 * Mathematical approach:
 * 1. Calculate required subnet bits: log2(num_subnets)
 * 2. New prefix length = original + subnet_bits
 * 3. Each subnet size = original_size / num_subnets
 * 
 * @param cidr_str: Original CIDR network string
 * @param num_subnets: Number of desired subnets (must be power of 2)
 */
void split_network(const char *cidr_str, int num_subnets)
{
    printf("🔀 Subnet Splitting Calculator\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    char network_ip[16];
    int prefix_len;
    
    // Parse CIDR notation
    if (!parse_cidr_notation(cidr_str, network_ip, &prefix_len)) {
        printf("❌ Invalid CIDR format: %s\n", cidr_str);
        return;
    }
    
    // Validate number of subnets (must be power of 2)
    if (num_subnets <= 1) {
        printf("❌ Number of subnets must be greater than 1\n");
        return;
    }
    
    // Check if num_subnets is power of 2
    int temp = num_subnets;
    int subnet_bits = 0;
    while (temp > 1) {
        if (temp % 2 != 0) {
            printf("❌ Number of subnets must be a power of 2 (2, 4, 8, 16, etc.)\n");
            return;
        }
        temp /= 2;
        subnet_bits++;
    }
    
    // Calculate new prefix length
    int new_prefix = prefix_len + subnet_bits;
    if (new_prefix > 30) {
        printf("❌ Cannot split: would result in /%d (maximum practical is /30)\n", new_prefix);
        return;
    }
    
    // Format new prefix string
    char prefix_str[5];  // enough for "/32" + null terminator
    snprintf(prefix_str, sizeof(prefix_str), "/%d", new_prefix);
    
    printf("📊 Splitting Analysis:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ Original Network:  %-36s │\n", cidr_str);
    printf("│ Requested Subnets: %-36d │\n", num_subnets);
    printf("│ Required Bits:     %-36d │\n", subnet_bits);
    printf("│ New Prefix:        %-36s │\n", prefix_str);
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    // Calculate original network details
    unsigned int network = ip_to_int(network_ip);
    char *orig_mask_str = cidr_to_subnet_mask(prefix_len);
    char *new_mask_str = cidr_to_subnet_mask(new_prefix);
    
    if (!orig_mask_str || !new_mask_str) {
        printf("❌ Failed to calculate subnet masks\n");
        if (orig_mask_str) free(orig_mask_str);
        if (new_mask_str) free(new_mask_str);
        return;
    }
    
    // Calculate subnet size
    int host_bits = 32 - new_prefix;
    int subnet_size = 1 << host_bits;  // 2^host_bits
    int usable_ips = (host_bits > 1) ? subnet_size - 2 : subnet_size;
    
    printf("\n📈 Subnet Details:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ Original Subnet Mask: %-32s │\n", orig_mask_str);
    printf("│ New Subnet Mask:      %-32s │\n", new_mask_str);
    printf("│ IPs per Subnet:       %-32d │\n", subnet_size);
    printf("│ Usable IPs per Subnet: %-31d │\n", usable_ips);
    printf("│ Host Bits per Subnet:  %-31d │\n", host_bits);
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    printf("\n🎯 Generated Subnets:\n");
    
    // Calculate network address (masked)
    unsigned int network_addr = calculate_network_address(network, mask_to_int(orig_mask_str));
    
    for (int i = 0; i < num_subnets; i++) {
        unsigned int subnet_addr = network_addr + (i * subnet_size);
        unsigned int broadcast_addr = subnet_addr + subnet_size - 1;
        
        char *subnet_str = int_to_ip(subnet_addr);
        char *broadcast_str = int_to_ip(broadcast_addr);
        
        printf("   ├─ Subnet %d: %s/%d\n", i + 1, subnet_str, new_prefix);
        printf("   │  ├─ Network:   %s\n", subnet_str);
        printf("   │  ├─ Broadcast: %s\n", broadcast_str);
        
        if (usable_ips > 0) {
            char *first_ip = int_to_ip(subnet_addr + 1);
            char *last_ip = int_to_ip(broadcast_addr - 1);
            printf("   │  ├─ First IP:  %s\n", first_ip);
            printf("   │  └─ Last IP:   %s (%d usable)\n", last_ip, usable_ips);
            free(first_ip);
            free(last_ip);
        } else {
            printf("   │  └─ Single host subnet\n");
        }
        
        if (i < num_subnets - 1) printf("   │\n");
        
        free(subnet_str);
        free(broadcast_str);
    }
    
    printf("\n💡 VLSM Educational Notes:\n");
    printf("   • VLSM allows efficient IP address allocation\n");
    printf("   • Each subnet is %s in size\n", new_mask_str);
    printf("   • Total IPs allocated: %d (original had %d)\n", 
           num_subnets * subnet_size, 1 << (32 - prefix_len));
    printf("   • This technique reduces IP address waste\n");
    
    free(orig_mask_str);
    free(new_mask_str);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}


/*
 * ============================================================================
 * IPv6 ADDRESS ANALYSIS
 * ============================================================================
 */

/*
 * IPv6 address analysis and educational information
 * 
 * Provides comprehensive analysis of IPv6 addresses including:
 * - Address type detection (unicast, multicast, etc.)
 * - Scope identification (link-local, global, etc.)
 * - Educational information about IPv6 addressing
 * 
 * @param ipv6_str: IPv6 address string
 */
void analyze_ipv6_address(const char *ipv6_str)
{
    printf("🌐 IPv6 Address Analysis\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    printf("📍 Analyzing IPv6 Address: %s\n", ipv6_str);
    
    // Basic IPv6 validation (simplified)
    int colons = 0;
    int double_colon = 0;
    const char *ptr = ipv6_str;
    
    while (*ptr) {
        if (*ptr == ':') {
            colons++;
            if (*(ptr + 1) == ':') {
                double_colon++;
                ptr++; // Skip the second colon
            }
        }
        ptr++;
    }
    
    printf("\n🔍 Address Structure Analysis:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ Format:           IPv6 (128-bit addressing)             │\n");
    printf("│ Colon Count:      %-36d │\n", colons);
    printf("│ Double Colon (::): %-35s │\n", double_colon > 0 ? "Present (compression)" : "Not present");
    printf("│ Address Length:   %-36zu │\n", strlen(ipv6_str));
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    // Analyze address type based on prefix
    printf("\n🏷️  IPv6 Address Type Classification:\n");
    
    if (strncmp(ipv6_str, "fe80:", 5) == 0 || strncmp(ipv6_str, "FE80:", 5) == 0) {
        printf("🔗 Link-Local Address (fe80::/10)\n");
        printf("   • Scope: Link-local (not routed)\n");
        printf("   • Use: Automatic configuration, neighbor discovery\n");
        printf("   • Valid only on local network segment\n");
    }
    else if (strncmp(ipv6_str, "fc", 2) == 0 || strncmp(ipv6_str, "fd", 2) == 0 ||
             strncmp(ipv6_str, "FC", 2) == 0 || strncmp(ipv6_str, "FD", 2) == 0) {
        printf("🏠 Unique Local Address (fc00::/7)\n");
        printf("   • Scope: Local (similar to IPv4 private addresses)\n");
        printf("   • Use: Private networks, not globally routed\n");
        printf("   • Locally administered\n");
    }
    else if (strncmp(ipv6_str, "ff", 2) == 0 || strncmp(ipv6_str, "FF", 2) == 0) {
        printf("📡 Multicast Address (ff00::/8)\n");
        printf("   • Scope: Multicast (one-to-many communication)\n");
        printf("   • Use: Group communication, routing protocols\n");
        printf("   • No broadcast in IPv6, only multicast\n");
    }
    else if (strncmp(ipv6_str, "2001:db8:", 9) == 0) {
        printf("📚 Documentation Address (2001:db8::/32)\n");
        printf("   • Scope: Documentation and examples only\n");
        printf("   • Use: RFC 3849 - reserved for documentation\n");
        printf("   • Should never appear in real networks\n");
    }
    else if (strncmp(ipv6_str, "::1", 3) == 0 && strlen(ipv6_str) == 3) {
        printf("🏠 Loopback Address (::1)\n");
        printf("   • Scope: Loopback (equivalent to 127.0.0.1)\n");
        printf("   • Use: Local machine communication\n");
        printf("   • Only one loopback address in IPv6\n");
    }
    else if (strncmp(ipv6_str, "::", 2) == 0 && strlen(ipv6_str) == 2) {
        printf("🚫 Unspecified Address (::)\n");
        printf("   • Scope: Unspecified (equivalent to 0.0.0.0)\n");
        printf("   • Use: Indicates absence of address\n");
        printf("   • Used in address configuration\n");
    }
    else if (ipv6_str[0] == '2' || ipv6_str[0] == '3') {
        printf("🌍 Global Unicast Address (2000::/3)\n");
        printf("   • Scope: Global (routable on internet)\n");
        printf("   • Use: Public IPv6 addresses\n");
        printf("   • Equivalent to IPv4 public addresses\n");
    }
    else {
        printf("❓ Other/Reserved Address Type\n");
        printf("   • May be reserved or special-purpose\n");
        printf("   • Check current IPv6 allocation standards\n");
    }
    
    printf("\n📊 IPv6 vs IPv4 Comparison:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ IPv4 Address Space:  32 bits (4.3 billion addresses)    │\n");
    printf("│ IPv6 Address Space:  128 bits (340 undecillion)         │\n");
    printf("│ IPv4 Subnetting:     Variable (CIDR)                    │\n");
    printf("│ IPv6 Subnetting:     Usually /64 for LANs               │\n");
    printf("│ IPv4 Special:        Many special ranges                │\n");
    printf("│ IPv6 Special:        Simplified special addressing      │\n");
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    printf("\n💡 IPv6 Educational Notes:\n");
    printf("   • IPv6 uses hexadecimal notation (0-9, A-F)\n");
    printf("   • :: can compress consecutive zero groups\n");
    printf("   • Standard subnet size is /64 (64-bit interface ID)\n");
    printf("   • No broadcast - uses multicast instead\n");
    printf("   • Mandatory IPSec support built-in\n");
    printf("   • Simplified header structure vs IPv4\n");
    
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

/*
 * IPv6 address format converter and educational display
 * 
 * Shows IPv6 addresses in different formats for educational purposes:
 * - Compressed form (using ::)
 * - Expanded form (full 128 bits)
 * - Binary representation
 * 
 * @param ipv6_str: IPv6 address string
 */
void convert_ipv6_formats(const char *ipv6_str)
{
    printf("🔄 IPv6 Format Converter\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    printf("📍 Converting IPv6 Address: %s\n", ipv6_str);
    
    // This is a simplified example - real IPv6 parsing is more complex
    printf("\n🎨 Multiple Format Representations:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ Original:          %-36s │\n", ipv6_str);
    
    // Show different possible representations (educational examples)
    if (strstr(ipv6_str, "::") != NULL) {
        printf("│ Format Type:       Compressed (using ::)                │\n");
        printf("│ Compression:       Zero groups omitted                  │\n");
    } else {
        printf("│ Format Type:       Full/Expanded form                   │\n");
        printf("│ Compression:       None (all groups shown)              │\n");
    }
    
    printf("│ Address Family:    IPv6 (Internet Protocol version 6)   │\n");
    printf("│ Bit Length:        128 bits (16 bytes)                  │\n");
    printf("│ Group Count:       8 groups of 16 bits each             │\n");
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    printf("\n📚 Educational Examples:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ Compressed:    2001:db8::1                              │\n");
    printf("│ Expanded:      2001:0db8:0000:0000:0000:0000:0000:0001  │\n");
    printf("│ Mixed:         ::ffff:192.168.1.1 (IPv4-mapped)        │\n");
    printf("│ Loopback:      ::1 (equivalent to 127.0.0.1)            │\n");
    printf("│ Unspecified:   :: (equivalent to 0.0.0.0)               │\n");
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    printf("\n🧮 IPv6 Structure Breakdown:\n");
    printf("   • Each group represents 16 bits (4 hex digits)\n");
    printf("   • Leading zeros in groups can be omitted\n");
    printf("   • :: represents one or more groups of zeros\n");
    printf("   • :: can only appear once in an address\n");
    printf("   • Total: 8 groups × 16 bits = 128 bits\n");
    
    printf("\n💡 IPv6 Best Practices:\n");
    printf("   • Use lowercase for hex digits (recommended)\n");
    printf("   • Compress longest run of zeros with ::\n");
    printf("   • Standard LAN subnet: /64 prefix\n");
    printf("   • Document your IPv6 addressing scheme\n");
    printf("   • Plan for dual-stack (IPv4 + IPv6) operation\n");
    
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}
