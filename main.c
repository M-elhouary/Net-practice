/*
 * =================================================================    // Check if user wants CIDR analysis (format: ./net --cidr <cidr_notation>)
    if (argc == 3 && strcmp(argv[1], "--cidr") == 0)
    {
        show_loading_animation("🔍 Initializing CIDR Analysis", 800);
        draw_header_box("📡 CIDR Network Analysis", argv[2]);
        analyze_cidr_network(argv[2]);
        return 0;
    }===
 * NETWORK CALCULATOR - MAIN PROGRAM
 * ============================================================================
 * 
 * A comprehensive network analysis tool that provides:
 * - Subnet mask analysis with binary representation
 * - IP range calculation using mathematical equations
 * - Loopback address detection and classification
 * - Educational approach using pure math instead of bit operations
 * 
 * Usage Modes:
 * 1. Subnet Analysis: ./net <subnet_mask>
 * 2. Network Analysis: ./net <ip> <subnet_mask>  
 * 3. Loopback Check: ./net -l <ip_address>
 * 
 * Examples:
 * ./net 255.255.255.0                    # Analyze /24 subnet
 * ./net 192.168.1.100 255.255.255.0      # Analyze specific network
 * ./net -l 127.0.0.1                     # Check if IP is loopback
 * 
 * Author: Network Tools Development Team
 * Version: 2.0
 * ============================================================================
 */

#include "net.h"

/*
 * Main program entry point
 * Handles command line arguments and directs to appropriate analysis functions
 */
int main(int argc, char *argv[])
{
    // Check for color preference
    if (getenv("NO_COLOR") != NULL) {
        set_theme(-1); // Disable colors
    }
    
    // Check for theme selection
    if (argc >= 3 && strcmp(argv[1], "--theme") == 0) {
        int theme = atoi(argv[2]);
        set_theme(theme);
        argc -= 2;  // Remove theme arguments
        for (int i = 1; i < argc; i++) {
            argv[i] = argv[i + 2];
        }
    }
    
    // ========================================================================
    // MODE 0: HELP DISPLAY
    // ========================================================================
    
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "--help") == 0))
    {
        // Beautiful help display
        draw_header_box("🌟 NETWORK CALCULATOR v3.0 🌟", "Educational Network Analysis Tool");
        
        const char *help_lines[] = {
            "🎯 BASIC MODES:",
            "  ./net <subnet_mask>                 → Basic subnet analysis",
            "  ./net <ip> <subnet_mask>            → Network analysis", 
            "  ./net -l <ip_address>               → Loopback detection",
            "",
            "🚀 ADVANCED MODES:",
            "  ./net --cidr <cidr_notation>        → CIDR analysis",
            "  ./net --class <ip_address>          → Network class detection",
            "  ./net --check <ip> <cidr_network>   → IP range validation",
            "  ./net --convert <ip_address>        → Multi-format converter",
            "  ./net --scan <cidr_network>         → Network IP scanner",
            "  ./net --split <cidr> <num_subnets>  → Subnet splitter (VLSM)",
            "  ./net --ipv6 <ipv6_address>         → IPv6 address analysis",
            "  ./net --ipv6-convert <ipv6_address> → IPv6 format converter",
            "",
            "� CONNECTIVITY & DIAGNOSTICS:",
            "  ./net --ping <ip> [count] [timeout] → ICMP Echo test (ping)",
            "  ./net --tcp <ip> <port> [timeout]   → TCP port connectivity",
            "  ./net --discover <ip> [timeout]     → Service discovery scan",
            "  ./net --diagnose <ip>               → Comprehensive diagnostics",
            "",
            "💡 EXAMPLES:",
            "  ./net 255.255.255.0                 → Shows 0.0.0.0/24 range",
            "  ./net 192.168.1.100 255.255.255.0   → Shows 192.168.1.0/24",
            "  ./net --scan 192.168.1.0/24         → Scan network IPs",
            "  ./net --ping 8.8.8.8 4              → Ping Google DNS",
            "  ./net --tcp 192.168.1.1 22          → Check SSH port",
            "  ./net --discover 192.168.1.1        → Find open services",
            "  ./net --diagnose 8.8.8.8            → Full network diagnostics",
            "",
            "✨ FEATURES:",
            "  • Mathematical IP conversion (base-256 system)",
            "  • Educational binary/hex representation",
            "  • Live connectivity testing (ICMP & TCP)",
            "  • Service discovery with pedagogical trace",
            "  • Beautiful color output and formatting",
            "  • Comprehensive network analysis",
            "  • IPv6 support with modern features",
            "  • Network scanning and VLSM calculator"
        };
        
        draw_info_box("📚 Usage Guide", help_lines, sizeof(help_lines)/sizeof(help_lines[0]));
        
        print_colored("\033[96m", "📖 For detailed documentation: ");
        print_colored("\033[97m", "cat USER_GUIDE.md\n");
        print_colored("\033[96m", "🎨 To disable colors: ");
        print_colored("\033[97m", "export NO_COLOR=1\n\n");
        
        return 0;
    }
    
    // Check for valid number of arguments (2-4 allowed, excluding help)
    if (argc < 2 || argc > 4)
    {
        printf("❌ Invalid number of arguments!\n\n");
        printf("Usage: %s [--help | <subnet_mask> | <ip> <subnet_mask> | -l <ip> | --cidr <cidr> | --class <ip> | --check <ip> <cidr> | --convert <ip>]\n", argv[0]);
        printf("For detailed help: %s --help\n", argv[0]);
        return 1;
    }

    // ========================================================================
    // MODE 1: LOOPBACK DETECTION (-l flag)
    // ========================================================================
    
    // Check if user wants loopback analysis (format: ./net -l <ip>)
    if (argc == 3 && strcmp(argv[1], "-l") == 0)
    {
        printf("🔍 Starting Loopback Analysis...\n");
        printf("Target IP: %s\n\n", argv[2]);
        check_loopback_ip(argv[2]);
        return 0;
    }

    // ========================================================================
    // MODE 2: CIDR ANALYSIS (--cidr flag)
    // ========================================================================
    
    // Check if user wants CIDR analysis (format: ./net --cidr <cidr>)
    if (argc == 3 && strcmp(argv[1], "--cidr") == 0)
    {
        printf("📡 Starting CIDR Network Analysis...\n");
        printf("Target CIDR: %s\n\n", argv[2]);
        analyze_cidr_network(argv[2]);
        return 0;
    }

    // ========================================================================
    // MODE 3: NETWORK CLASS DETECTION (--class flag)
    // ========================================================================
    
    // Check if user wants class analysis (format: ./net --class <ip>)
    if (argc == 3 && strcmp(argv[1], "--class") == 0)
    {
        printf("🏷️  Starting Network Class Analysis...\n");
        printf("Target IP: %s\n\n", argv[2]);
        classify_ip_address(argv[2]);
        return 0;
    }

    // ========================================================================
    // MODE 4: IP RANGE VALIDATION (--check flag)
    // ========================================================================
    
    // Check if user wants range validation (format: ./net --check <ip> <cidr>)
    if (argc == 4 && strcmp(argv[1], "--check") == 0)
    {
        printf("🎯 Starting IP Range Validation...\n");
        printf("Target IP: %s, Network: %s\n\n", argv[2], argv[3]);
        validate_ip_in_range(argv[2], argv[3]);
        return 0;
    }

    // ========================================================================
    // MODE 5: MULTI-FORMAT CONVERTER (--convert flag)
    // ========================================================================
    
    // Check if user wants format conversion (format: ./net --convert <ip>)
    if (argc == 3 && strcmp(argv[1], "--convert") == 0)
    {
        printf("🔄 Starting Multi-Format Conversion...\n");
        printf("Target IP: %s\n\n", argv[2]);
        convert_ip_formats(argv[2]);
        return 0;
    }

    // ========================================================================
    // MODE 6: NETWORK SCANNER (--scan flag)
    // ========================================================================
    
    // Check if user wants network scanning (format: ./net --scan <cidr>)
    if (argc == 3 && strcmp(argv[1], "--scan") == 0)
    {
        show_loading_animation("🔍 Preparing Network Scanner", 600);
        draw_header_box("🌐 Network Range Scanner", argv[2]);
        scan_network_range(argv[2]);
        return 0;
    }

    // ========================================================================
    // MODE 7: SUBNET SPLITTER (--split flag)
    // ========================================================================
    
    // Check if user wants subnet splitting (format: ./net --split <cidr> <num>)
    if (argc == 4 && strcmp(argv[1], "--split") == 0)
    {
        show_loading_animation("🔀 Initializing Subnet Splitter", 700);
        draw_header_box("🔀 VLSM Subnet Calculator", argv[2]);
        int num_subnets = atoi(argv[3]);
        split_network(argv[2], num_subnets);
        return 0;
    }

    // ========================================================================
    // MODE 8: IPv6 ANALYZER (--ipv6 flag)
    // ========================================================================
    
    // Check if user wants IPv6 analysis (format: ./net --ipv6 <ipv6_address>)
    if (argc == 3 && strcmp(argv[1], "--ipv6") == 0)
    {
        printf("🌐 Starting IPv6 Analyzer...\n");
        printf("Target IPv6: %s\n\n", argv[2]);
        analyze_ipv6_address(argv[2]);
        return 0;
    }

    // ========================================================================
    // MODE 9: IPv6 FORMAT CONVERTER (--ipv6-convert flag)
    // ========================================================================
    
    // Check if user wants IPv6 format conversion (format: ./net --ipv6-convert <ipv6>)
    if (argc == 3 && strcmp(argv[1], "--ipv6-convert") == 0)
    {
        printf("🔄 Starting IPv6 Format Converter...\n");
        printf("Target IPv6: %s\n\n", argv[2]);
        convert_ipv6_formats(argv[2]);
        return 0;
    }

    // ========================================================================
    // MODE 10: TCP CONNECTIVITY CHECK (--tcp flag)
    // ========================================================================
    
    // Check if user wants TCP connectivity test (format: ./net --tcp <ip> <port> [timeout])
    if (argc >= 4 && strcmp(argv[1], "--tcp") == 0)
    {
        int port = atoi(argv[3]);
        int timeout = (argc == 5) ? atoi(argv[4]) : 5;  // Default 5 second timeout
        check_tcp_connectivity(argv[2], port, timeout);
        return 0;
    }

    // ========================================================================
    // MODE 11: ICMP PING TEST (--ping flag)
    // ========================================================================
    
    // Check if user wants ICMP ping test (format: ./net --ping <ip> [count] [timeout])
    if (argc >= 3 && strcmp(argv[1], "--ping") == 0)
    {
        int count = (argc >= 4) ? atoi(argv[3]) : 4;     // Default 4 packets
        int timeout = (argc >= 5) ? atoi(argv[4]) : 5;   // Default 5 second timeout
        perform_icmp_ping(argv[2], count, timeout);
        return 0;
    }

    // ========================================================================
    // MODE 12: SERVICE DISCOVERY SCAN (--discover flag)
    // ========================================================================
    
    // Check if user wants service discovery (format: ./net --discover <ip> [timeout])
    if (argc >= 3 && strcmp(argv[1], "--discover") == 0)
    {
        int timeout = (argc == 4) ? atoi(argv[3]) : 3;  // Default 3 second timeout
        scan_services_in_range(argv[2], timeout);
        return 0;
    }

    // ========================================================================
    // MODE 13: COMPREHENSIVE DIAGNOSTICS (--diagnose flag)
    // ========================================================================
    
    // Check if user wants full diagnostics (format: ./net --diagnose <ip>)
    if (argc == 3 && strcmp(argv[1], "--diagnose") == 0)
    {
        generate_diagnostics_report(argv[2]);
        return 0;
    }

    // ========================================================================
    // MODE 6: BASIC SUBNET ANALYSIS (subnet mask only)
    // ========================================================================
    
    if (argc == 2)
    {
        printf("📊 Starting Basic Subnet Analysis...\n");
        printf("Input: Subnet Mask = %s\n", argv[1]);
        printf("Network Base: 0.0.0.0 (for demonstration)\n\n");
        
        // Analyze the subnet mask properties
        printf("=== Subnet Mask Analysis ===\n");
        print_mask_info(argv[1]);
        
        // Show IP range for theoretical 0.0.0.0 network
        printf("\n=== IP Range Analysis ===\n");
        print_ip_range_from_mask(argv[1]);
    }
    
    // ========================================================================
    // MODE 7: COMPLETE NETWORK ANALYSIS (IP + subnet mask)
    // ========================================================================
    
    else if (argc == 3)
    {
        printf("🌐 Starting Complete Network Analysis...\n");
        printf("Input: IP = %s, Subnet Mask = %s\n\n", argv[1], argv[2]);
        
        // First, analyze the subnet mask properties
        printf("=== Subnet Mask Analysis ===\n");
        print_mask_info(argv[2]);
        
        // Then, analyze the specific network containing the given IP
        printf("\n=== IP Range Analysis ===\n");
        print_ip_range(argv[1], argv[2]);
        
        // Finally, check if the provided IP has special properties
        printf("\n=== IP Classification Analysis ===\n");
        check_loopback_ip(argv[1]);
    }

    // ========================================================================
    // PROGRAM COMPLETION
    // ========================================================================
    
    printf("\n✅ Analysis completed successfully!\n");
    printf("💡 Tip: Use 'cat USER_GUIDE.md' for detailed documentation\n");
    
    return 0;
}
