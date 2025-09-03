/*
 * ============================================================================
 * LOOPBACK IP ADDRESS DETECTION AND ANALYSIS
 * ============================================================================
 * 
 * This file contains functions for detecting and analyzing loopback IP addresses.
 * 
 * Educational Focus:
 * - Uses mathematical calculations instead of bit manipulation
 * - Shows clear step-by-step IP address analysis
 * - Explains networking concepts in detail
 * 
 * Author: Network Calculator
 * Purpose: Educational IP address classification and loopback detection
 */

#include "net.h"

/*
 * ============================================================================
 * LOOPBACK DETECTION FUNCTION
 * ============================================================================
 */

/*
 * Comprehensive loopback IP address detection and analysis
 * 
 * This function checks if an IP address falls within the loopback range
 * and provides detailed information about the address type and properties.
 * 
 * Loopback Range: 127.0.0.0/8 (127.0.0.0 to 127.255.255.255)
 * Mathematical boundaries:
 * - Start: 127 × 256³ = 127 × 16,777,216 = 2,130,706,432
 * - End:   127 × 256³ + 255×256² + 255×256¹ + 255 = 2,147,483,647
 * 
 * @param ip_str: IP address string (e.g., "127.0.0.1")
 */
void check_loopback_ip(const char *ip_str)
{
    printf("🔍 Comprehensive IP Address Analysis\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    // Convert IP string to integer using mathematical base-256 conversion
    unsigned int ip = ip_to_int(ip_str);
    
    if (ip == 0)
    {
        printf("❌ Invalid IP address format: %s\n", ip_str);
        printf("   Expected format: A.B.C.D where each octet is 0-255\n");
        return;
    }
    
    printf("📍 Analyzing IP Address: %s\n", ip_str);
    printf("🔢 Mathematical conversion: %u (32-bit integer)\n", ip);
    
    /*
     * Loopback Range Calculation (127.0.0.0/8):
     * 
     * Mathematical approach:
     * - Start: 127.0.0.0 = 127 × 256³ + 0 × 256² + 0 × 256¹ + 0 × 256⁰
     * - Start: 127 × 16,777,216 = 2,130,706,432
     * 
     * - End: 127.255.255.255 = 127 × 256³ + 255 × 256² + 255 × 256¹ + 255 × 256⁰
     * - End: 127 × 16,777,216 + 255 × 65,536 + 255 × 256 + 255
     * - End: 2,130,706,432 + 16,711,680 + 65,280 + 255 = 2,147,483,647
     */
    
    unsigned int loopback_start = 127 * 16777216;  // 127.0.0.0 = 2,130,706,432
    unsigned int loopback_end = loopback_start + 16777215;  // Add all possible host addresses
    
    printf("\n📊 Range Analysis:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ Loopback Range: 127.0.0.0 - 127.255.255.255 (/8)      │\n");
    printf("│ Mathematical Range: %u - %u         │\n", loopback_start, loopback_end);
    printf("│ Your IP (decimal): %-32u │\n", ip);
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    // Perform the loopback check
    if (ip >= loopback_start && ip <= loopback_end)
    {
        printf("\n✅ LOOPBACK ADDRESS DETECTED!\n");
        printf("🏠 This IP address is a loopback address\n");
        
        // Extract octets for detailed analysis using mathematical division
        unsigned int octet_a = ip / 16777216;           // 256³ = 16,777,216
        unsigned int octet_b = (ip % 16777216) / 65536; // 256² = 65,536
        unsigned int octet_c = (ip % 65536) / 256;      // 256¹ = 256
        unsigned int octet_d = ip % 256;                // 256⁰ = 1
        
        printf("\n🧮 Mathematical Breakdown:\n");
        printf("┌─────────────────────────────────────────────────────────┐\n");
        printf("│ Octet A (127): %u × 256³ = %u              │\n", octet_a, octet_a * 16777216);
        printf("│ Octet B (%3u): %u × 256² = %u                  │\n", octet_b, octet_b, octet_b * 65536);
        printf("│ Octet C (%3u): %u × 256¹ = %u                    │\n", octet_c, octet_c, octet_c * 256);
        printf("│ Octet D (%3u): %u × 256⁰ = %u                      │\n", octet_d, octet_d, octet_d);
        printf("│ Total: %u.%u.%u.%u = %u                     │\n", octet_a, octet_b, octet_c, octet_d, ip);
        printf("└─────────────────────────────────────────────────────────┘\n");
        
        // Analyze specific loopback addresses
        if (ip == 2130706433) // 127.0.0.1
        {
            printf("\n⭐ SPECIAL ADDRESS: 127.0.0.1 (localhost)\n");
            printf("📌 Properties:\n");
            printf("   • Most commonly used loopback address\n");
            printf("   • Default localhost in most systems\n");
            printf("   • Standard for local development and testing\n");
            printf("   • Always points to the local machine\n");
        }
        else if (octet_b == 0 && octet_c == 0 && octet_d == 0) // 127.0.0.0
        {
            printf("\n⚠️  NETWORK ADDRESS: 127.0.0.0\n");
            printf("📌 Properties:\n");
            printf("   • This is the network identifier\n");
            printf("   • Not typically used for actual traffic\n");
            printf("   • Represents the entire 127.0.0.0/8 network\n");
            printf("   • Reserved for network identification\n");
        }
        else if (octet_b == 255 && octet_c == 255 && octet_d == 255) // 127.255.255.255
        {
            printf("\n📡 BROADCAST ADDRESS: 127.255.255.255\n");
            printf("📌 Properties:\n");
            printf("   • Theoretical broadcast for loopback network\n");
            printf("   • Not used in practice (loopback doesn't broadcast)\n");
            printf("   • Marks the end of the 127.0.0.0/8 range\n");
        }
        else
        {
            printf("\n✨ VALID LOOPBACK ADDRESS\n");
            printf("📌 Properties:\n");
            printf("   • Can be used for local testing and development\n");
            printf("   • Functionally equivalent to 127.0.0.1\n");
            printf("   • Part of the 127.0.0.0/8 reserved range\n");
            printf("   • Traffic stays within the local machine\n");
        }
        
        printf("\n🌐 Loopback Network Properties:\n");
        printf("┌─────────────────────────────────────────────────────────┐\n");
        printf("│ • Traffic NEVER leaves the local machine               │\n");
        printf("│ • No physical network interface required               │\n");
        printf("│ • Maximum speed (no network latency)                   │\n");
        printf("│ • Used for local services and testing                  │\n");
        printf("│ • Secure (no external network exposure)                │\n");
        printf("│ • Available even when network is disconnected          │\n");
        printf("└─────────────────────────────────────────────────────────┘\n");
    }
    else
    {
        printf("\n❌ NOT a loopback address\n");
        printf("🌍 This IP address is outside the loopback range\n");
        
        printf("\n🔍 Let's check what type of address this is:\n");
        
        // Check other well-known IP ranges using mathematical boundaries
        if (ip >= 167772160 && ip <= 184549375) // 10.0.0.0/8
        {
            printf("🏠 PRIVATE IP ADDRESS: Class A (10.0.0.0/8)\n");
            printf("   Range: 10.0.0.0 - 10.255.255.255\n");
            printf("   Use: Large private networks (16,777,214 hosts)\n");
        }
        else if (ip >= 2886729728U && ip <= 2887778303U) // 172.16.0.0/12
        {
            printf("🏢 PRIVATE IP ADDRESS: Class B (172.16.0.0/12)\n");
            printf("   Range: 172.16.0.0 - 172.31.255.255\n");
            printf("   Use: Medium private networks (1,048,574 hosts)\n");
        }
        else if (ip >= 3232235520U && ip <= 3232301055U) // 192.168.0.0/16
        {
            printf("🏡 PRIVATE IP ADDRESS: Class C (192.168.0.0/16)\n");
            printf("   Range: 192.168.0.0 - 192.168.255.255\n");
            printf("   Use: Small private networks (65,534 hosts)\n");
        }
        else if (ip >= 2851995648U && ip <= 2852061183U) // 169.254.0.0/16
        {
            printf("🔗 LINK-LOCAL ADDRESS (169.254.0.0/16)\n");
            printf("   Range: 169.254.0.0 - 169.254.255.255\n");
            printf("   Use: Automatic IP when DHCP fails (APIPA)\n");
        }
        else if (ip >= 3758096384U && ip <= 4026531839U) // 224.0.0.0/4
        {
            printf("📡 MULTICAST ADDRESS (224.0.0.0/4)\n");
            printf("   Range: 224.0.0.0 - 239.255.255.255\n");
            printf("   Use: One-to-many communication\n");
        }
        else if (ip >= 4026531840U) // 240.0.0.0/4
        {
            printf("🚫 RESERVED ADDRESS (240.0.0.0/4)\n");
            printf("   Range: 240.0.0.0 - 255.255.255.255\n");
            printf("   Use: Experimental/Reserved for future use\n");
        }
        else
        {
            printf("🌐 PUBLIC IP ADDRESS\n");
            printf("   This appears to be a routable public internet address\n");
            printf("   Can be accessed from anywhere on the internet\n");
        }
        
        printf("\n📚 Educational Note:\n");
        printf("┌─────────────────────────────────────────────────────────┐\n");
        printf("│ Loopback addresses are special because they:               │\n");
        printf("│ • Always exist on every IP-capable device                  │\n");
        printf("│ • Don't require network hardware to function               │\n");
        printf("│ • Provide fastest possible local communication             │\n");
        printf("│ • Are essential for many system services                   │\n");
        printf("└─────────────────────────────────────────────────────────┘\n");
    }
    
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}
