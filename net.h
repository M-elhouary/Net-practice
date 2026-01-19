#ifndef NET_H
#define NET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ============================================================================
 * NETWORK CALCULATOR LIBRARY
 * ============================================================================
 * 
 * This library provides comprehensive network analysis tools using pure
 * mathematical equations instead of bit shifting operations. This makes
 * the code educational and easy to understand.
 * 
 * Mathematical Approach:
 * - IP addresses are converted using base-256 number system
 * - IP = A×256³ + B×256² + C×256¹ + D×256⁰
 * - Network calculations use boolean algebra (AND/OR operations)
 * 
 * Author: Network Tools Development Team
 * Version: 2.0
 * ============================================================================
 */

// ============================================================================
// BASIC CONVERSION FUNCTIONS
// ============================================================================

// Converts decimal number (0-255) to 8-bit binary string
// Input: Integer 0-255
// Output: Null-terminated string like "11111111" or "00000000"
char *dec_to_binary(int nb);

// Parses subnet mask string (e.g., "255.255.255.0") into integer array
// Input: String like "255.255.255.0"
// Output: Array of 4 integers [255, 255, 255, 0] or NULL if invalid
int *prepar_mask(char *input);

// Converts integer mask array to a single 32-bit binary string
// Input: Array of 4 integers representing octets
// Output: 32-character binary string like "11111111111111111111111100000000"
char *mask_bin_single(int *mask);

// Creates a dynamic copy of input string for safe manipulation
// Input: Constant string to copy
// Output: Dynamically allocated copy (must be freed by caller)
char *copy_input(const char *input);

// ============================================================================
// IP ADDRESS MATHEMATICAL CONVERSION FUNCTIONS
// ============================================================================

// Converts IP address string to 32-bit integer using mathematical equation
// Formula: IP = A×256³ + B×256² + C×256¹ + D×256⁰
// Input: String like "192.168.1.1"
// Output: 32-bit integer representation or 0 if invalid
unsigned int ip_to_int(const char *ip_str);

// Converts 32-bit integer to IP address string using mathematical division
// Formula: A = IP/256³, B = (IP%256³)/256², C = (IP%256²)/256¹, D = IP%256¹
// Input: 32-bit integer
// Output: Dynamically allocated string like "192.168.1.1" (must be freed)
char *int_to_ip(unsigned int ip);

// Converts subnet mask string to 32-bit integer using mathematical equation
// Input: String like "255.255.255.0"
// Output: 32-bit integer representation or 0 if invalid
unsigned int mask_to_int(const char *mask_str);

// ============================================================================
// NETWORK CALCULATION FUNCTIONS
// ============================================================================

// Calculates network address using mathematical AND operation
// Formula: Network = IP AND Mask
// Input: IP and mask as 32-bit integers
// Output: Network address as 32-bit integer
unsigned int calculate_network_address(unsigned int ip, unsigned int mask);

// Calculates broadcast address using mathematical equation
// Formula: Broadcast = Network OR (~Mask)
// Input: Network address and mask as 32-bit integers  
// Output: Broadcast address as 32-bit integer
unsigned int calculate_broadcast_address(unsigned int network, unsigned int mask);

// ============================================================================
// ANALYSIS AND DISPLAY FUNCTIONS
// ============================================================================

// Calculates number of available IP addresses from binary mask string
// Counts host bits (0s from right) and calculates 2^host_bits - 2
// Special cases: /31 returns 2, /32 returns 1
// Input: 32-character binary string
// Output: Number of usable IP addresses or -1 if error
int calculate_available_ips(const char *bin_mask);

// Prints comprehensive binary mask analysis and CIDR information
// Shows: decimal mask, binary representation, available IPs, CIDR notation
// Input: Subnet mask string like "255.255.255.0"
void print_binary_mask(const char *mask_str);

// Enhanced version of print_binary_mask with additional information
// Input: Subnet mask string like "255.255.255.0"
void print_mask_info(const char *mask_str);

// Prints complete IP range analysis for specific network
// Shows: network, broadcast, first/last usable IPs, total count
// Input: Network IP string and subnet mask string
void print_ip_range(const char *network_ip, const char *mask_str);

// Prints IP range information using 0.0.0.0 as base network
// Useful for understanding subnet sizing without specific network
// Input: Subnet mask string
void print_ip_range_from_mask(const char *mask_str);

// ============================================================================
// SPECIAL IP ADDRESS ANALYSIS
// ============================================================================

// Comprehensive loopback address detection and analysis
// Detects 127.0.0.0/8 range and provides detailed information
// Also identifies other IP types (private, public, link-local)
// Input: IP address string
void check_loopback_ip(const char *ip_str);

// ============================================================================
// ENHANCED NETWORK ANALYSIS FUNCTIONS
// ============================================================================

// CIDR notation parsing and conversion functions
// Parses CIDR string like "192.168.1.0/24" into IP and prefix length
// Input: CIDR string, output buffers for IP and prefix length
// Output: 1 if valid, 0 if invalid
int parse_cidr_notation(const char *cidr_str, char *ip_out, int *prefix_len);

// Converts CIDR prefix length to subnet mask string
// Input: Prefix length (0-32)
// Output: Dynamically allocated subnet mask string (must be freed)
char *cidr_to_subnet_mask(int prefix_len);

// Complete CIDR network analysis
// Input: CIDR string like "192.168.1.0/24"
void analyze_cidr_network(const char *cidr_str);

// Network class detection and analysis functions
// Analyzes IP address class (A, B, C) and provides historical context
// Input: IP address string
void classify_ip_address(const char *ip_str);

// Returns network class string for given IP integer
// Input: IP as 32-bit integer
// Output: Constant string like "Class A", "Class B", etc.
const char *get_network_class(unsigned int ip);

// IP range validation functions
// Checks if an IP address falls within a specific network range
// Input: IP string, network string, mask string
// Output: 1 if IP is in network, 0 if not
int is_ip_in_network(const char *ip_str, const char *network_str, const char *mask_str);

// Validates if IP is within CIDR network range
// Input: IP string and CIDR string
void validate_ip_in_range(const char *ip_str, const char *cidr_str);

// Multi-format IP converter (binary, hex, decimal)
// Shows IP address in all formats for educational purposes
// Input: IP address string
void convert_ip_formats(const char *ip_str);

// Network scanning and enumeration functions
// Displays all possible IP addresses in a given network range
// Input: CIDR network string
void scan_network_range(const char *cidr_str);

// Subnet splitting calculator
// Splits a network into smaller equal-sized subnets
// Input: CIDR network string and number of desired subnets
void split_network(const char *cidr_str, int num_subnets);

// IPv6 address analysis and conversion functions
// Analyzes IPv6 addresses and provides detailed information
// Input: IPv6 address string
void analyze_ipv6_address(const char *ipv6_str);

// IPv6 address compression/expansion
// Shows compressed and expanded forms of IPv6 addresses
// Input: IPv6 address string
void convert_ipv6_formats(const char *ipv6_str);

// Enhanced output formatting functions (output_formatter.c)
// Terminal color and theme support
int terminal_supports_colors(void);
void print_colored(const char *color, const char *format, ...);
void set_theme(int theme);

// Beautiful box drawing and headers
void draw_header_box(const char *title, const char *subtitle);
void draw_info_box(const char *title, const char **lines, int line_count);

// Progress bars and animations
void show_progress_bar(const char *label, int progress, int total);
void show_loading_animation(const char *message, int duration_ms);

// Enhanced table and diagram formatting
void draw_data_table(const char *title, const char **headers, const char ***data, int cols, int rows);
void draw_network_diagram(const char *network, const char *mask, int host_count);
void display_ip_info_enhanced(const char *label, const char *ip, const char *description);
void show_calculation_steps(const char *title, const char **steps, int step_count);

// ============================================================================
// NETWORK DIAGNOSTICS - LIVE CONNECTIVITY & SERVICE DISCOVERY
// ============================================================================

// TCP connectivity check with three-way handshake analysis
// Tests if a specific IP:port pair is reachable and responding
// Shows detailed educational trace of TCP handshake process
// Input: IP address string, port number (1-65535), timeout in seconds
// Output: 1 if connected successfully, 0 if connection failed
int check_tcp_connectivity(const char *ip, int port, int timeout_sec);

// ICMP Echo (Ping) connectivity check with packet structure analysis
// Sends ICMP Echo Request packets and measures Round-Trip Time (RTT)
// Shows ICMP packet format and checksum calculation mathematically
// Input: IP address string, number of packets to send, timeout in seconds
// Output: 1 if at least one reply received, 0 if all failed
int perform_icmp_ping(const char *ip, int packet_count, int timeout_sec);

// Service discovery scanner - tests common ports for open services
// Uses non-blocking sockets to rapidly scan well-known service ports
// Educational trace shows TCP handshake states and port status classification
// Input: IP address string, timeout in seconds for each connection
void scan_services_in_range(const char *ip, int timeout_sec);

// Comprehensive network diagnostics report generator
// Combines ICMP ping, TCP scanning, and statistics into one report
// Shows complete health check of target IP with detailed analysis
// Input: Target IP address string
void generate_diagnostics_report(const char *ip);

#endif // NET_H
