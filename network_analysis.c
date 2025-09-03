/*
 * ============================================================================
 * NETWORK ANALYSIS FUNCTIONS
 * ============================================================================
 * 
 * This file contains functions for analyzing networks, calculating ranges,
 * and displaying comprehensive network information.
 * 
 * Functions in this file:
 * - IP range calculation and display
 * - Network information analysis
 * - Subnet mask analysis and display
 * 
 * Author: Network Tools Development Team
 * ============================================================================
 */

#include "net.h"

/*
 * ============================================================================
 * COMPLETE NETWORK ANALYSIS WITH SPECIFIC IP
 * ============================================================================
 */

/*
 * Prints comprehensive network analysis for a specific IP and subnet mask
 * 
 * This function calculates and displays:
 * - Network address (first IP in range)
 * - Broadcast address (last IP in range)  
 * - First usable IP (network + 1)
 * - Last usable IP (broadcast - 1)
 * - Total available IP count
 * 
 * Special handling for:
 * - /32 networks (single host)
 * - /31 networks (point-to-point links)
 * - Normal networks (subtract network and broadcast)
 * 
 * @param network_ip: IP address string (any IP in the network)
 * @param mask_str: Subnet mask string like "255.255.255.0"
 */
void print_ip_range(const char *network_ip, const char *mask_str)
{
    printf("🎯 Analyzing specific network containing IP: %s\n", network_ip);
    
    // ========================================================================
    // STEP 1: CONVERT STRINGS TO INTEGERS FOR MATHEMATICAL OPERATIONS
    // ========================================================================
    
    unsigned int ip = ip_to_int(network_ip);
    unsigned int mask = mask_to_int(mask_str);
    
    // Validate conversions
    if (ip == 0 || mask == 0)
    {
        printf("❌ Invalid IP address or subnet mask format.\n");
        printf("   IP: %s → %u\n", network_ip, ip);
        printf("   Mask: %s → %u\n", mask_str, mask);
        return;
    }
    
    // ========================================================================
    // STEP 2: CALCULATE NETWORK AND BROADCAST ADDRESSES
    // ========================================================================
    
    unsigned int network = calculate_network_address(ip, mask);
    unsigned int broadcast = calculate_broadcast_address(network, mask);
    
    // ========================================================================
    // STEP 3: ANALYZE HOST BITS FOR SPECIAL CASES
    // ========================================================================
    
    // We need to count host bits to handle special cases (/31, /32)
    char *input_copy = copy_input(mask_str);
    if (!input_copy) {
        printf("❌ Memory allocation failed\n");
        return;
    }
    
    int *mask_array = prepar_mask(input_copy);
    if (!mask_array) {
        printf("❌ Failed to parse subnet mask\n");
        free(input_copy);
        return;
    }
    
    char *bin_mask = mask_bin_single(mask_array);
    if (!bin_mask) {
        printf("❌ Failed to convert mask to binary\n");
        free(input_copy);
        return;
    }
    
    // Count host bits (number of 0s from the right)
    int host_bits = 0;
    for (int i = 31; i >= 0; i--)
    {
        if (bin_mask[i] == '0')
            host_bits++;
        else
            break;  // Stop at first 1 bit from right
    }
    
    printf("📊 Host bits detected: %d (CIDR: /%d)\n", host_bits, 32 - host_bits);
    
    // ========================================================================
    // STEP 4: DISPLAY NETWORK INFORMATION
    // ========================================================================
    
    printf("\n📋 Network Information Summary:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ Input IP:      %-40s │\n", network_ip);
    printf("│ Subnet Mask:   %-40s │\n", mask_str);
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    // Convert addresses back to strings for display
    char *network_str = int_to_ip(network);
    char *broadcast_str = int_to_ip(broadcast);
    
    if (network_str && broadcast_str)
    {
        printf("\n🌐 Calculated Network Boundaries:\n");
        printf("┌─────────────────────────────────────────────────────────┐\n");
        printf("│ Network Address:   %-36s │\n", network_str);
        printf("│ Broadcast Address: %-36s │\n", broadcast_str);
        printf("└─────────────────────────────────────────────────────────┘\n");
        
        // ====================================================================
        // STEP 5: HANDLE SPECIAL CASES BASED ON HOST BITS
        // ====================================================================
        
        if (host_bits == 0) // /32 - Single host network
        {
            printf("\n🏠 Single Host Network (/32):\n");
            printf("   └─ Host Address: %s\n", network_str);
            printf("   └─ This is a host-specific route (no other IPs available)\n");
        }
        else if (host_bits == 1) // /31 - Point-to-point link
        {
            printf("\n🔗 Point-to-Point Link (/31):\n");
            printf("   ├─ First IP:  %s\n", network_str);
            printf("   └─ Second IP: %s\n", broadcast_str);
            printf("   └─ Used for router-to-router connections (RFC 3021)\n");
        }
        else // Normal network - subtract network and broadcast
        {
            // Calculate first and last usable IPs using mathematical addition/subtraction
            unsigned int first_ip = network + 1;    // First usable = Network + 1
            unsigned int last_ip = broadcast - 1;   // Last usable = Broadcast - 1
            
            char *first_ip_str = int_to_ip(first_ip);
            char *last_ip_str = int_to_ip(last_ip);
            
            if (first_ip_str && last_ip_str)
            {
                printf("\n💼 Usable IP Address Range:\n");
                printf("┌─────────────────────────────────────────────────────────┐\n");
                printf("│ First Usable IP:  %-36s │\n", first_ip_str);
                printf("│ Last Usable IP:   %-36s │\n", last_ip_str);
                printf("└─────────────────────────────────────────────────────────┘\n");
                
                // Calculate and display total available IPs
                int available = calculate_available_ips(bin_mask);
                printf("\n📈 Network Statistics:\n");
                printf("   ├─ Total Available IPs: %d\n", available);
                printf("   ├─ Network Size: %u total addresses\n", broadcast - network + 1);
                printf("   ├─ Unusable IPs: 2 (network + broadcast)\n");
                printf("   └─ Efficiency: %.1f%% usable\n", 
                       (float)available / (broadcast - network + 1) * 100);
            }
            
            // Clean up allocated strings
            if (first_ip_str) free(first_ip_str);
            if (last_ip_str) free(last_ip_str);
        }
    }
    else
    {
        printf("❌ Failed to convert network addresses to strings\n");
    }
    
    // ========================================================================
    // CLEANUP ALLOCATED MEMORY
    // ========================================================================
    
    if (network_str) free(network_str);
    if (broadcast_str) free(broadcast_str);
    free(bin_mask);
    free(input_copy);
}

/*
 * ============================================================================
 * THEORETICAL NETWORK ANALYSIS (0.0.0.0 BASE)
 * ============================================================================
 */

/*
 * Prints IP range information using 0.0.0.0 as the base network
 * This is useful for understanding subnet sizing without a specific network
 * 
 * @param mask_str: Subnet mask string like "255.255.255.0"
 */
void print_ip_range_from_mask(const char *mask_str)
{
    printf("🧮 Analyzing theoretical network with base 0.0.0.0\n");
    printf("   (This shows the pattern for ANY network with this mask)\n");
    
    // ========================================================================
    // MATHEMATICAL CALCULATION FOR 0.0.0.0 NETWORK
    // ========================================================================
    
    unsigned int mask = mask_to_int(mask_str);
    
    if (mask == 0)
    {
        printf("❌ Invalid subnet mask format: %s\n", mask_str);
        return;
    }
    
    // Use network 0.0.0.0 as base (mathematical zero)
    unsigned int network = 0;
    
    // Calculate broadcast address for this theoretical network
    unsigned int broadcast = calculate_broadcast_address(network, mask);
    
    // ========================================================================
    // ANALYZE HOST BITS FOR PATTERN UNDERSTANDING
    // ========================================================================
    
    char *input_copy = copy_input(mask_str);
    if (!input_copy) return;
    
    int *mask_array = prepar_mask(input_copy);
    if (!mask_array) {
        free(input_copy);
        return;
    }
    
    char *bin_mask = mask_bin_single(mask_array);
    if (!bin_mask) {
        free(input_copy);
        return;
    }
    
    // Count host bits
    int host_bits = 0;
    for (int i = 31; i >= 0; i--)
    {
        if (bin_mask[i] == '0')
            host_bits++;
        else
            break;
    }
    
    // ========================================================================
    // DISPLAY THEORETICAL NETWORK INFORMATION
    // ========================================================================
    
    printf("\n📚 Theoretical Network Analysis:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ Subnet Mask:       %-36s │\n", mask_str);
    printf("│ CIDR Notation:     %-36s │\n", (host_bits == 0) ? "/32" : 
           (host_bits == 1) ? "/31" : (host_bits == 2) ? "/30" :
           (host_bits == 3) ? "/29" : (host_bits == 4) ? "/28" :
           (host_bits == 5) ? "/27" : (host_bits == 6) ? "/26" :
           (host_bits == 7) ? "/25" : (host_bits == 8) ? "/24" : "Other");
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    char *network_str = int_to_ip(network);
    char *broadcast_str = int_to_ip(broadcast);
    
    if (network_str && broadcast_str)
    {
        printf("\n🎯 Address Pattern (for 0.0.0.0 example):\n");
        printf("┌─────────────────────────────────────────────────────────┐\n");
        printf("│ Network Address:   %-36s │\n", network_str);
        printf("│ Broadcast Address: %-36s │\n", broadcast_str);
        printf("└─────────────────────────────────────────────────────────┘\n");
        
        if (host_bits == 0) // /32
        {
            printf("\n🏠 Single Host Network (/32):\n");
            printf("   └─ Each IP is its own network\n");
        }
        else if (host_bits == 1) // /31
        {
            printf("\n🔗 Point-to-Point Networks (/31):\n");
            printf("   ├─ First IP:  %s\n", network_str);
            printf("   └─ Second IP: %s\n", broadcast_str);
        }
        else // Normal networks
        {
            // Calculate first and last usable IPs
            unsigned int first_ip = network + 1;
            unsigned int last_ip = broadcast - 1;
            
            char *first_ip_str = int_to_ip(first_ip);
            char *last_ip_str = int_to_ip(last_ip);
            
            if (first_ip_str && last_ip_str)
            {
                printf("\n💼 Usable IP Pattern:\n");
                printf("┌─────────────────────────────────────────────────────────┐\n");
                printf("│ First Usable IP:  %-36s │\n", first_ip_str);
                printf("│ Last Usable IP:   %-36s │\n", last_ip_str);
                printf("└─────────────────────────────────────────────────────────┘\n");
                
                int available = calculate_available_ips(bin_mask);
                unsigned int total_addresses = broadcast - network + 1;
                
                printf("\n📊 Network Pattern Statistics:\n");
                printf("┌─────────────────────────────────────────────────────────┐\n");
                printf("│ Total Available IPs:    %-27d │\n", available);
                printf("│ Network Size:           %-27u │\n", total_addresses);
                printf("│ Host Bits:              %-27d │\n", host_bits);
                printf("│ Possible Networks:      %-27d │\n", (int)(4294967296ULL / total_addresses));
                printf("└─────────────────────────────────────────────────────────┘\n");
                
                printf("\n💡 Usage Examples with this mask:\n");
                printf("   ├─ 192.168.1.0/%d → 192.168.1.%d - 192.168.1.%d\n", 
                       32-host_bits, 1, (int)last_ip - (int)first_ip + 1);
                printf("   ├─ 10.0.0.0/%d → 10.0.0.%d - 10.0.0.%d\n", 
                       32-host_bits, 1, (int)last_ip - (int)first_ip + 1);
                printf("   └─ 172.16.0.0/%d → 172.16.0.%d - 172.16.0.%d\n", 
                       32-host_bits, 1, (int)last_ip - (int)first_ip + 1);
            }
            
            if (first_ip_str) free(first_ip_str);
            if (last_ip_str) free(last_ip_str);
        }
    }
    
    // ========================================================================
    // CLEANUP
    // ========================================================================
    
    if (network_str) free(network_str);
    if (broadcast_str) free(broadcast_str);
    free(bin_mask);
    free(input_copy);
}
