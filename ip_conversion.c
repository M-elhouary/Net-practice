/*
 * ============================================================================
 * IP CONVERSION UTILITIES
 * ============================================================================
 * 
 * This file contains mathematical functions for converting between different
 * IP address representations using pure mathematical equations.
 * 
 * Mathematical Foundation:
 * - Base-256 number system for IP addresses
 * - IP = A×256³ + B×256² + C×256¹ + D×256⁰ (where A,B,C,D are octets)
 * - Reverse: A = IP÷256³, B = (IP%256³)÷256², etc.
 * 
 * Why Mathematical Approach:
 * - Educational: Shows the underlying math
 * - Readable: No cryptic bit operations
 * - Maintainable: Easy to understand and modify
 * 
 * Author: Network Tools Development Team
 * ============================================================================
 */

#include "net.h"

/*
 * ============================================================================
 * IP ADDRESS STRING TO INTEGER CONVERSION
 * ============================================================================
 */

/*
 * Converts IP address string to 32-bit integer using mathematical equation
 * 
 * Mathematical Formula:
 * IP = A×256³ + B×256² + C×256¹ + D×256⁰
 * where: 256³ = 16,777,216, 256² = 65,536, 256¹ = 256, 256⁰ = 1
 * 
 * Example: 192.168.1.1
 * = 192×16,777,216 + 168×65,536 + 1×256 + 1×1
 * = 3,221,225,472 + 11,010,048 + 256 + 1
 * = 3,232,235,777
 * 
 * @param ip_str: IP address string like "192.168.1.1"
 * @return: 32-bit integer representation, 0 if invalid
 */
unsigned int ip_to_int(const char *ip_str)
{
    unsigned int result = 0;
    
    // Create a working copy of the input string (strtok modifies the original)
    char *ip_copy = copy_input(ip_str);
    if (!ip_copy) {
        printf("❌ Memory allocation failed for IP string copy\n");
        return 0;
    }
    
    // Parse the IP address into 4 octets using dot as delimiter
    char *token = strtok(ip_copy, ".");
    int octets[4];  // Array to store the 4 octets (A, B, C, D)
    int i = 0;
    
    // Extract each octet and validate it's in range 0-255
    while (token != NULL && i < 4)
    {
        unsigned int octet = (unsigned int)atoi(token);
        
        // Validate octet range (0-255)
        if (octet > 255) {
            printf("❌ Invalid octet value %u (must be 0-255)\n", octet);
            free(ip_copy);
            return 0;
        }
        
        octets[i] = octet;
        token = strtok(NULL, ".");
        i++;
    }
    
    // Ensure we got exactly 4 octets
    if (i == 4) {
        // Apply mathematical formula: IP = A×256³ + B×256² + C×256¹ + D×256⁰
        result = octets[0] * 16777216 +  // A × 256³ (first octet × 16,777,216)
                 octets[1] * 65536 +     // B × 256² (second octet × 65,536)
                 octets[2] * 256 +       // C × 256¹ (third octet × 256)
                 octets[3];              // D × 256⁰ (fourth octet × 1)
        
        printf("🔢 IP Conversion: %s → %u\n", ip_str, result);
        printf("   Math: %d×16777216 + %d×65536 + %d×256 + %d = %u\n", 
               octets[0], octets[1], octets[2], octets[3], result);
    } else {
        printf("❌ Invalid IP format: expected 4 octets, got %d\n", i);
    }
    
    free(ip_copy);
    return (i == 4) ? result : 0;
}

/*
 * ============================================================================
 * INTEGER TO IP ADDRESS STRING CONVERSION
 * ============================================================================
 */

/*
 * Converts 32-bit integer to IP address string using mathematical division
 * 
 * Mathematical Formula (reverse of ip_to_int):
 * A = IP ÷ 256³         (extract first octet)
 * B = (IP % 256³) ÷ 256² (extract second octet)
 * C = (IP % 256²) ÷ 256¹ (extract third octet)
 * D = IP % 256¹          (extract fourth octet)
 * 
 * Example: 3,232,235,777
 * A = 3,232,235,777 ÷ 16,777,216 = 192
 * B = (3,232,235,777 % 16,777,216) ÷ 65,536 = 11,010,305 ÷ 65,536 = 168
 * C = (11,010,305 % 65,536) ÷ 256 = 257 ÷ 256 = 1
 * D = 257 % 256 = 1
 * Result: "192.168.1.1"
 * 
 * @param ip: 32-bit integer representation of IP
 * @return: Dynamically allocated string like "192.168.1.1" (caller must free)
 */
char *int_to_ip(unsigned int ip)
{
    // Allocate memory for IP string (max "255.255.255.255" = 15 chars + null terminator)
    char *ip_str = malloc(16);
    if (!ip_str) {
        printf("❌ Memory allocation failed for IP string\n");
        return NULL;
    }
    
    // Mathematical extraction of octets using division and modulo
    unsigned int octet_a = ip / 16777216;           // 256³ = 16,777,216
    unsigned int octet_b = (ip % 16777216) / 65536; // 256² = 65,536
    unsigned int octet_c = (ip % 65536) / 256;      // 256¹ = 256
    unsigned int octet_d = ip % 256;                // 256⁰ = 1
    
    // Format the result as dotted decimal notation
    sprintf(ip_str, "%u.%u.%u.%u", octet_a, octet_b, octet_c, octet_d);
    
    printf("🔢 Integer Conversion: %u → %s\n", ip, ip_str);
    printf("   Math: A=%u, B=%u, C=%u, D=%u\n", octet_a, octet_b, octet_c, octet_d);
    
    return ip_str;
}

/*
 * ============================================================================
 * SUBNET MASK STRING TO INTEGER CONVERSION
 * ============================================================================
 */

/*
 * Converts subnet mask string to 32-bit integer using mathematical equation
 * Same mathematical approach as ip_to_int, but specifically for subnet masks
 * 
 * @param mask_str: Subnet mask string like "255.255.255.0"
 * @return: 32-bit integer representation, 0 if invalid
 */
unsigned int mask_to_int(const char *mask_str)
{
    // Create working copy for parsing
    char *input_copy = copy_input(mask_str);
    if (!input_copy) {
        printf("❌ Memory allocation failed for mask string copy\n");
        return 0;
    }
    
    // Parse mask into integer array
    int *mask = prepar_mask(input_copy);
    if (!mask) {
        printf("❌ Invalid subnet mask format: %s\n", mask_str);
        free(input_copy);
        return 0;
    }
    
    // Apply mathematical formula: Mask = A×256³ + B×256² + C×256¹ + D×256⁰
    unsigned int result = mask[0] * 16777216 +  // 256³ = 16,777,216
                          mask[1] * 65536 +     // 256² = 65,536
                          mask[2] * 256 +       // 256¹ = 256
                          mask[3];              // 256⁰ = 1
    
    printf("🔢 Mask Conversion: %s → %u\n", mask_str, result);
    printf("   Math: %d×16777216 + %d×65536 + %d×256 + %d = %u\n", 
           mask[0], mask[1], mask[2], mask[3], result);
    
    free(input_copy);
    return result;
}

/*
 * ============================================================================
 * NETWORK CALCULATION HELPER FUNCTIONS
 * ============================================================================
 */

/*
 * Calculates network address using mathematical AND operation
 * Network Address = IP AND Subnet_Mask
 * 
 * This is one of the few places where we use bitwise AND (&) because
 * it's the standard definition of network calculation in networking.
 * The mathematical equivalent would be much more complex.
 * 
 * @param ip: IP address as 32-bit integer
 * @param mask: Subnet mask as 32-bit integer
 * @return: Network address as 32-bit integer
 */
unsigned int calculate_network_address(unsigned int ip, unsigned int mask)
{
    unsigned int network = ip & mask;
    
    printf("🌐 Network Calculation: IP (%u) AND Mask (%u) = %u\n", ip, mask, network);
    
    return network;
}

/*
 * Calculates broadcast address using mathematical equation
 * Broadcast = Network + (Total_IPs_in_subnet - 1)
 * Or equivalently: Broadcast = Network OR (~Mask)
 * 
 * @param network: Network address as 32-bit integer
 * @param mask: Subnet mask as 32-bit integer
 * @return: Broadcast address as 32-bit integer
 */
unsigned int calculate_broadcast_address(unsigned int network, unsigned int mask)
{
    // Calculate inverse mask (flip all bits)
    // Mathematical equivalent: 2³² - 1 - mask = 4,294,967,295 - mask
    unsigned int inverse_mask = 4294967295U - mask;
    
    // Broadcast = Network OR Inverse_mask
    unsigned int broadcast = network | inverse_mask;
    
    printf("📡 Broadcast Calculation: Network (%u) OR InverseMask (%u) = %u\n", 
           network, inverse_mask, broadcast);
    
    return broadcast;
}
