/*
 * ============================================================================
 * BASIC NETWORK UTILITIES AND SUBNET MASK ANALYSIS
 * ============================================================================
 * 
 * This file contains fundamental utility functions for network calculations:
 * - Binary conversion utilities
 * - String manipulation helpers
 * - Subnet mask parsing and analysis
 * - IP address classification and loopback detection
 * 
 * These are the core building blocks used by other modules.
 * 
 * Author: Network Tools Development Team
 * ============================================================================
 */

#include "net.h"

/*
 * ============================================================================
 * BINARY CONVERSION UTILITIES
 * ============================================================================
 */

/*
 * Converts a decimal number (0-255) to 8-bit binary string
 * 
 * This function uses mathematical division by 2 to extract binary digits.
 * It's the foundation for understanding how subnet masks work in binary.
 * 
 * Algorithm:
 * 1. Start with rightmost bit (index 7)
 * 2. While number > 0: bit = number % 2, number = number / 2
 * 3. Move to next bit position (left)
 * 
 * Example: 255 → "11111111", 192 → "11000000"
 * 
 * @param nb: Integer from 0-255 (representing one octet)
 * @return: Dynamically allocated 9-character string (8 bits + null terminator)
 */
char *dec_to_binary(int nb)
{
    int index = 7;  // Start from rightmost bit (bit 7)
    
    // Allocate memory for 8 bits + null terminator
    char *bin_nb = malloc(sizeof(char) * 9);
    if (!bin_nb) {
        printf("❌ Memory allocation failed for binary string\n");
        return NULL;
    }
    
    // Initialize all bits to '0' and add null terminator
    memset(bin_nb, '0', 8);
    bin_nb[8] = '\0';
    
    // Handle special case: 0 → "00000000"
    if (nb == 0) {
        printf("🔢 Binary conversion: %d → %s\n", nb, bin_nb);
        return bin_nb;
    }
    
    // Convert to binary using repeated division by 2
    while (nb > 0 && index >= 0)
    {
        bin_nb[index] = (nb % 2) + '0';  // Get remainder (0 or 1) and convert to char
        nb /= 2;                         // Integer division by 2
        index--;                         // Move to next bit position (leftward)
    }
    
    printf("🔢 Binary conversion: Original → %s\n", bin_nb);
    return bin_nb;
}

/*
 * ============================================================================
 * STRING MANIPULATION UTILITIES
 * ============================================================================
 */

/*
 * Creates a dynamic copy of input string for safe manipulation
 * 
 * This is essential because strtok() modifies the original string,
 * so we need a working copy to preserve the original.
 * 
 * @param input: Original string to copy
 * @return: Dynamically allocated copy (caller must free)
 */
char *copy_input(const char *input)
{
    if (!input) {
        printf("❌ Cannot copy NULL input string\n");
        return NULL;
    }
    
    size_t len = strlen(input);
    char *copy = malloc(len + 1);
    
    if (!copy) {
        printf("❌ Memory allocation failed for string copy\n");
        return NULL;
    }
    
    strcpy(copy, input);
    printf("📝 String copied: \"%s\" (length: %zu)\n", input, len);
    
    return copy;
}

/*
 * ============================================================================
 * SUBNET MASK PARSING FUNCTIONS
 * ============================================================================
 */

/*
 * Parses subnet mask string into array of 4 integers
 * 
 * Takes a dotted decimal string like "255.255.255.0" and splits it
 * into an array [255, 255, 255, 0] for mathematical processing.
 * 
 * Uses strtok() to split on '.' delimiter - this is why we need
 * a copy of the input string (strtok modifies the original).
 * 
 * @param input: Subnet mask string (WILL BE MODIFIED by strtok)
 * @return: Static array of 4 integers, or NULL if invalid format
 */
int *prepar_mask(char *input)
{
    static int mask[4];  // Static array - persists after function returns
    int i = 0;
    
    printf("🔍 Parsing mask string: \"%s\"\n", input);
    
    // Split string on '.' delimiter
    char *token = strtok(input, ".");
    
    while (token != NULL && i < 4)
    {
        mask[i] = atoi(token);  // Convert string to integer
        
        // Validate octet range (0-255)
        if (mask[i] < 0 || mask[i] > 255) {
            printf("❌ Invalid octet value: %d (must be 0-255)\n", mask[i]);
            return NULL;
        }
        
        printf("   Octet %d: %s → %d\n", i+1, token, mask[i]);
        
        i++;
        token = strtok(NULL, ".");  // Get next token
    }
    
    // Ensure we got exactly 4 octets
    if (i != 4) {
        printf("❌ Invalid mask format: expected 4 octets, got %d\n", i);
        return NULL;
    }
    
    printf("✅ Mask parsed successfully: [%d, %d, %d, %d]\n", 
           mask[0], mask[1], mask[2], mask[3]);
    
    return mask;
}

/*
 * Converts integer mask array to single 32-bit binary string
 * 
 * Takes array [255, 255, 255, 0] and converts to:
 * "11111111111111111111111100000000"
 * 
 * This is crucial for understanding how subnet masks work and
 * for calculating host bits.
 * 
 * @param mask: Array of 4 integers (octets)
 * @return: 33-character string (32 bits + null terminator)
 */
char *mask_bin_single(int *mask)
{
    // Allocate memory for 32 bits + null terminator
    char *res = malloc(33);
    if (!res) {
        printf("❌ Memory allocation failed for binary mask\n");
        return NULL;
    }
    
    int pos = 0;  // Position in result string
    
    printf("🔄 Converting mask to binary...\n");
    
    // Convert each octet to binary and concatenate
    for (int i = 0; i < 4; i++)
    {
        char *bin_oct = dec_to_binary(mask[i]);
        if (!bin_oct) {
            printf("❌ Failed to convert octet %d (%d) to binary\n", i, mask[i]);
            free(res);
            return NULL;
        }
        
        // Copy 8 bits to result string
        memcpy(res + pos, bin_oct, 8);
        pos += 8;
        
        printf("   Octet %d (%d) → %s\n", i+1, mask[i], bin_oct);
        
        free(bin_oct);  // Clean up individual octet binary string
    }
    
    res[32] = '\0';  // Null terminate the 32-bit string
    
    printf("✅ Complete binary mask: %s\n", res);
    
    return res;
}

/*
 * ============================================================================
 * SUBNET MASK ANALYSIS FUNCTIONS
 * ============================================================================
 */

/*
 * Basic function to print binary mask (legacy compatibility)
 * For detailed analysis, use print_mask_info() instead.
 * 
 * @param mask_str: Subnet mask string like "255.255.255.0"
 */
void print_binary_mask(const char *mask_str)
{
    printf("🔍 Basic binary mask analysis for: %s\n", mask_str);
    
    char *input_copy = copy_input(mask_str);
    if (!input_copy)
    {
        printf("❌ Memory allocation failed.\n");
        return;
    }

    int *mask = prepar_mask(input_copy);
    if (!mask)
    {
        printf("❌ Invalid subnet mask format.\n");
        free(input_copy);
        return;
    }

    char *bin_mask = mask_bin_single(mask);
    if (!bin_mask)
    {
        printf("❌ Memory allocation error.\n");
        free(input_copy);
        return;
    }

    printf("📊 Binary mask (32 bits):\n%s\n", bin_mask);

    free(bin_mask);
    free(input_copy);
}

/*
 * ============================================================================
 * IP CALCULATION FUNCTIONS
 * ============================================================================
 */

/*
 * Calculates number of available IP addresses from binary mask string
 * 
 * Algorithm:
 * 1. Count host bits (0s from right side of binary mask)
 * 2. Calculate total IPs = 2^host_bits
 * 3. Subtract 2 for network and broadcast (except /31 and /32)
 * 
 * Special cases:
 * - /32 (host_bits=0): Returns 1 (single host)
 * - /31 (host_bits=1): Returns 2 (point-to-point link, RFC 3021)
 * - Others: Returns 2^host_bits - 2
 * 
 * @param bin_mask: 32-character binary string like "11111111111111111111111100000000"
 * @return: Number of usable IP addresses, -1 if error
 */
int calculate_available_ips(const char *bin_mask)
{
    // Validate input
    if (!bin_mask || strlen(bin_mask) != 32) {
        printf("❌ Invalid binary mask: %s\n", bin_mask ? bin_mask : "NULL");
        return -1;
    }
    
    int host_bits = 0;
    
    printf("🔢 Counting host bits in mask: %s\n", bin_mask);
    
    // Count the number of 0 bits (host bits) from right to left
    for (int i = 31; i >= 0; i--)
    {
        if (bin_mask[i] == '0') {
            host_bits++;
            printf("   Bit %d: 0 (host bit #%d)\n", i, host_bits);
        } else {
            printf("   Bit %d: 1 (network bit) - stopping count\n", i);
            break; // Stop when we hit the first 1 bit from the right
        }
    }
    
    printf("📈 Total host bits: %d (CIDR: /%d)\n", host_bits, 32 - host_bits);
    
    // Calculate total IPs using mathematical exponentiation: 2^host_bits
    int total_ips = 1;
    for (int i = 0; i < host_bits; i++)
    {
        total_ips *= 2;
    }
    
    printf("🧮 Total possible IPs: 2^%d = %d\n", host_bits, total_ips);
    
    // Handle special cases according to networking standards
    if (host_bits == 0) // /32 - single host
    {
        printf("🏠 Special case: /32 single host network\n");
        return 1;
    }
    else if (host_bits == 1) // /31 - point-to-point link
    {
        printf("🔗 Special case: /31 point-to-point link (RFC 3021)\n");
        return 2;
    }
    else // Normal network - subtract network and broadcast
    {
        int available = total_ips - 2;
        printf("💼 Normal network: %d total - 2 (network + broadcast) = %d usable\n", 
               total_ips, available);
        return available;
    }
}

/*
 * Enhanced subnet mask analysis with comprehensive information
 * 
 * Shows:
 * - Original subnet mask in decimal notation
 * - Binary representation (educational)
 * - CIDR notation (/24, /28, etc.)
 * - Available IP count
 * 
 * @param mask_str: Subnet mask string like "255.255.255.0"
 */
void print_mask_info(const char *mask_str)
{
    printf("🎯 Comprehensive subnet mask analysis\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    char *input_copy = copy_input(mask_str);
    if (!input_copy)
    {
        printf("❌ Memory allocation failed.\n");
        return;
    }

    int *mask = prepar_mask(input_copy);
    if (!mask)
    {
        printf("❌ Invalid subnet mask format.\n");
        free(input_copy);
        return;
    }

    char *bin_mask = mask_bin_single(mask);
    if (!bin_mask)
    {
        printf("❌ Memory allocation error.\n");
        free(input_copy);
        return;
    }

    // Display basic information
    printf("\n📊 Subnet Mask Information:\n");
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│ Decimal Notation:  %-36s │\n", mask_str);
    printf("│ Binary Notation:   %-36s │\n", bin_mask);
    printf("└─────────────────────────────────────────────────────────┘\n");
    
    // Calculate and show available IPs
    int available_ips = calculate_available_ips(bin_mask);
    if (available_ips >= 0)
    {
        printf("\n📈 Network Capacity:\n");
        printf("┌─────────────────────────────────────────────────────────┐\n");
        printf("│ Available IP addresses: %-27d │\n", available_ips);
        
        // Calculate and show CIDR notation
        int network_bits = 0;
        for (int i = 0; i < 32; i++)
        {
            if (bin_mask[i] == '1')
                network_bits++;
            else
                break;
        }
        printf("│ CIDR notation:          %-27s │\n", 
               network_bits == 32 ? "/32" : network_bits == 31 ? "/31" :
               network_bits == 30 ? "/30" : network_bits == 29 ? "/29" :
               network_bits == 28 ? "/28" : network_bits == 27 ? "/27" :
               network_bits == 26 ? "/26" : network_bits == 25 ? "/25" :
               network_bits == 24 ? "/24" : "Other");
        printf("│ Network bits:           %-27d │\n", network_bits);
        printf("│ Host bits:              %-27d │\n", 32 - network_bits);
        printf("└─────────────────────────────────────────────────────────┘\n");
    }
    else
    {
        printf("❌ Error calculating available IPs.\n");
    }

    // Clean up allocated memory
    free(bin_mask);
    free(input_copy);
    
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}
