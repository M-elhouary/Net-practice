/*
 * ============================================================================
 * OUTPUT FORMATTER - ENHANCED VISUAL DISPLAY
 * ============================================================================
 * 
 * This file contains functions for beautiful, customizable output formatting
 * with colors, animations, and enhanced visual elements.
 * 
 * Features:
 * - ANSI color support with fallback for non-color terminals
 * - Progress bars and loading animations
 * - Beautiful box drawing and table formatting
 * - Customizable themes (dark/light mode)
 * - Eye-friendly spacing and typography
 * 
 * Author: Network Calculator Enhanced Edition
 * Purpose: Professional and beautiful terminal output
 */

#include "net.h"
#include <unistd.h>  // For usleep()
#include <stdarg.h>  // For va_list, va_start, va_end

/*
 * ============================================================================
 * COLOR DEFINITIONS AND THEME SUPPORT
 * ============================================================================
 */

// ANSI Color codes
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define ITALIC      "\033[3m"
#define UNDERLINE   "\033[4m"
#define BLINK       "\033[5m"

// Foreground colors
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// Bright foreground colors
#define BRIGHT_BLACK   "\033[90m"
#define BRIGHT_RED     "\033[91m"
#define BRIGHT_GREEN   "\033[92m"
#define BRIGHT_YELLOW  "\033[93m"
#define BRIGHT_BLUE    "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN    "\033[96m"
#define BRIGHT_WHITE   "\033[97m"

// Background colors
#define BG_BLACK    "\033[40m"
#define BG_RED      "\033[41m"
#define BG_GREEN    "\033[42m"
#define BG_YELLOW   "\033[43m"
#define BG_BLUE     "\033[44m"
#define BG_MAGENTA  "\033[45m"
#define BG_CYAN     "\033[46m"
#define BG_WHITE    "\033[47m"

// Global theme setting
static int use_colors = 1;
static int current_theme = 0; // 0 = default, 1 = dark, 2 = light, 3 = cyberpunk

/*
 * ============================================================================
 * UTILITY FUNCTIONS
 * ============================================================================
 */

/*
 * Check if terminal supports colors
 */
int terminal_supports_colors(void)
{
    char *term = getenv("TERM");
    if (!term) return 0;
    
    // Check for common color-supporting terminals
    if (strstr(term, "color") || strstr(term, "xterm") || 
        strstr(term, "screen") || strstr(term, "tmux") ||
        strcmp(term, "linux") == 0) {
        return 1;
    }
    return 0;
}

/*
 * Apply color with theme awareness and printf-style formatting
 */
void print_colored(const char *color, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    
    if (use_colors && terminal_supports_colors()) {
        printf("%s", color);
        vprintf(format, args);
        printf("%s", RESET);
    } else {
        vprintf(format, args);
    }
    
    va_end(args);
}

/*
 * Set output theme
 */
void set_theme(int theme)
{
    current_theme = theme;
    if (theme == -1) {
        use_colors = 0; // Disable colors
    } else {
        use_colors = 1;
    }
}

/*
 * ============================================================================
 * ENHANCED BOX DRAWING
 * ============================================================================
 */

/*
 * Draw a beautiful header box with gradient effect
 */
void draw_header_box(const char *title, const char *subtitle)
{
    int title_len = strlen(title);
    int subtitle_len = subtitle ? strlen(subtitle) : 0;
    int box_width = (title_len > subtitle_len ? title_len : subtitle_len) + 8;
    
    if (box_width < 60) box_width = 60;
    
    // Top border with gradient effect
    print_colored(BRIGHT_CYAN, "╔");
    for (int i = 0; i < box_width - 2; i++) {
        if (i % 4 == 0) print_colored(BRIGHT_BLUE, "═");
        else if (i % 4 == 1) print_colored(CYAN, "═");
        else if (i % 4 == 2) print_colored(BLUE, "═");
        else print_colored(BRIGHT_CYAN, "═");
    }
    print_colored(BRIGHT_CYAN, "╗\n");
    
    // Title line
    print_colored(BRIGHT_CYAN, "║");
    int padding = (box_width - title_len - 2) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    print_colored(BOLD BRIGHT_WHITE, title);
    for (int i = 0; i < box_width - title_len - 2 - padding; i++) printf(" ");
    print_colored(BRIGHT_CYAN, "║\n");
    
    // Subtitle line (if provided)
    if (subtitle) {
        print_colored(BRIGHT_CYAN, "║");
        padding = (box_width - subtitle_len - 2) / 2;
        for (int i = 0; i < padding; i++) printf(" ");
        print_colored(DIM BRIGHT_WHITE, subtitle);
        for (int i = 0; i < box_width - subtitle_len - 2 - padding; i++) printf(" ");
        print_colored(BRIGHT_CYAN, "║\n");
    }
    
    // Bottom border
    print_colored(BRIGHT_CYAN, "╚");
    for (int i = 0; i < box_width - 2; i++) {
        print_colored(BRIGHT_CYAN, "═");
    }
    print_colored(BRIGHT_CYAN, "╝\n");
    printf("\n");
}

/*
 * Draw a beautiful information box
 */
void draw_info_box(const char *title, const char **lines, int line_count)
{
    int max_width = strlen(title);
    
    // Find maximum line width
    for (int i = 0; i < line_count; i++) {
        int len = strlen(lines[i]);
        if (len > max_width) max_width = len;
    }
    
    int box_width = max_width + 6;
    if (box_width < 50) box_width = 50;
    
    // Header
    print_colored(BRIGHT_GREEN, "┌─");
    print_colored(BOLD BRIGHT_WHITE, title);
    for (int i = strlen(title); i < box_width - 4; i++) {
        print_colored(BRIGHT_GREEN, "─");
    }
    print_colored(BRIGHT_GREEN, "─┐\n");
    
    // Content lines
    for (int i = 0; i < line_count; i++) {
        print_colored(BRIGHT_GREEN, "│ ");
        printf("%-*s", box_width - 4, lines[i]);
        print_colored(BRIGHT_GREEN, " │\n");
    }
    
    // Footer
    print_colored(BRIGHT_GREEN, "└");
    for (int i = 0; i < box_width - 2; i++) {
        print_colored(BRIGHT_GREEN, "─");
    }
    print_colored(BRIGHT_GREEN, "┘\n");
}

/*
 * ============================================================================
 * PROGRESS AND LOADING ANIMATIONS
 * ============================================================================
 */

/*
 * Show a progress bar with percentage
 */
void show_progress_bar(const char *label, int progress, int total)
{
    int bar_width = 40;
    int filled = (progress * bar_width) / total;
    int percentage = (progress * 100) / total;
    
    printf("\r");
    print_colored(BRIGHT_YELLOW, label);
    printf(" [");
    
    // Draw progress bar
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) {
            print_colored(BRIGHT_GREEN, "█");
        } else if (i == filled && progress < total) {
            print_colored(YELLOW, "▓");
        } else {
            print_colored(DIM, "░");
        }
    }
    
    printf("] ");
    print_colored(BRIGHT_WHITE, "%d%%", percentage);
    fflush(stdout);
    
    if (progress >= total) {
        printf("\n");
    }
}

/*
 * Loading animation with dots
 */
void show_loading_animation(const char *message, int duration_ms)
{
    const char *spinner = "⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏";
    int spinner_len = strlen(spinner) / 3; // Each Unicode char is 3 bytes
    
    printf("\r");
    print_colored(BRIGHT_CYAN, message);
    printf(" ");
    
    for (int i = 0; i < duration_ms / 100; i++) {
        printf("\r");
        print_colored(BRIGHT_CYAN, message);
        printf(" ");
        
        // Print current spinner character
        int char_index = (i % spinner_len) * 3;
        printf("%.3s", &spinner[char_index]);
        
        fflush(stdout);
        usleep(100000); // 100ms
    }
    
    printf("\r");
    print_colored(BRIGHT_GREEN, message);
    print_colored(BRIGHT_GREEN, " ✓");
    printf("\n");
}

/*
 * ============================================================================
 * ENHANCED TABLE FORMATTING
 * ============================================================================
 */

/*
 * Draw a beautiful table with alternating row colors
 */
void draw_data_table(const char *title, const char **headers, const char ***data, 
                     int cols, int rows)
{
    // Calculate column widths
    int *col_widths = malloc(cols * sizeof(int));
    
    // Initialize with header widths
    for (int c = 0; c < cols; c++) {
        col_widths[c] = strlen(headers[c]);
    }
    
    // Check data widths
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int len = strlen(data[r][c]);
            if (len > col_widths[c]) {
                col_widths[c] = len;
            }
        }
    }
    
    // Add padding
    for (int c = 0; c < cols; c++) {
        col_widths[c] += 2;
    }
    
    // Table title
    printf("\n");
    print_colored(BOLD BRIGHT_MAGENTA, "📊 %s\n", title);
    
    // Top border
    print_colored(BRIGHT_BLUE, "┌");
    for (int c = 0; c < cols; c++) {
        for (int i = 0; i < col_widths[c]; i++) {
            print_colored(BRIGHT_BLUE, "─");
        }
        if (c < cols - 1) print_colored(BRIGHT_BLUE, "┬");
    }
    print_colored(BRIGHT_BLUE, "┐\n");
    
    // Headers
    print_colored(BRIGHT_BLUE, "│");
    for (int c = 0; c < cols; c++) {
        print_colored(BOLD BRIGHT_WHITE, "%-*s", col_widths[c], headers[c]);
        if (c < cols - 1) print_colored(BRIGHT_BLUE, "│");
    }
    print_colored(BRIGHT_BLUE, "│\n");
    
    // Header separator
    print_colored(BRIGHT_BLUE, "├");
    for (int c = 0; c < cols; c++) {
        for (int i = 0; i < col_widths[c]; i++) {
            print_colored(BRIGHT_BLUE, "─");
        }
        if (c < cols - 1) print_colored(BRIGHT_BLUE, "┼");
    }
    print_colored(BRIGHT_BLUE, "┤\n");
    
    // Data rows with alternating colors
    for (int r = 0; r < rows; r++) {
        print_colored(BRIGHT_BLUE, "│");
        for (int c = 0; c < cols; c++) {
            if (r % 2 == 0) {
                print_colored(BRIGHT_WHITE, "%-*s", col_widths[c], data[r][c]);
            } else {
                print_colored(WHITE, "%-*s", col_widths[c], data[r][c]);
            }
            if (c < cols - 1) print_colored(BRIGHT_BLUE, "│");
        }
        print_colored(BRIGHT_BLUE, "│\n");
    }
    
    // Bottom border
    print_colored(BRIGHT_BLUE, "└");
    for (int c = 0; c < cols; c++) {
        for (int i = 0; i < col_widths[c]; i++) {
            print_colored(BRIGHT_BLUE, "─");
        }
        if (c < cols - 1) print_colored(BRIGHT_BLUE, "┴");
    }
    print_colored(BRIGHT_BLUE, "┘\n\n");
    
    free(col_widths);
}

/*
 * ============================================================================
 * ENHANCED NETWORK VISUALIZATION
 * ============================================================================
 */

/*
 * Draw network topology diagram
 */
void draw_network_diagram(const char *network, const char *mask, int host_count)
{
    printf("\n");
    print_colored(BOLD BRIGHT_CYAN, "🌐 Network Topology Visualization\n");
    printf("\n");
    
    // Network box
    print_colored(BRIGHT_GREEN, "    ┌─────────────────────────────────┐\n");
    print_colored(BRIGHT_GREEN, "    │ ");
    print_colored(BOLD BRIGHT_WHITE, "Network: %-19s", network);
    print_colored(BRIGHT_GREEN, " │\n");
    print_colored(BRIGHT_GREEN, "    │ ");
    print_colored(BRIGHT_WHITE, "Mask:    %-19s", mask);
    print_colored(BRIGHT_GREEN, " │\n");
    print_colored(BRIGHT_GREEN, "    │ ");
    print_colored(BRIGHT_YELLOW, "Hosts:   %-19d", host_count);
    print_colored(BRIGHT_GREEN, " │\n");
    print_colored(BRIGHT_GREEN, "    └─────────────────────────────────┘\n");
    
    // Connection lines
    print_colored(BRIGHT_BLUE, "                    │\n");
    print_colored(BRIGHT_BLUE, "            ┌───────┼───────┐\n");
    print_colored(BRIGHT_BLUE, "            │       │       │\n");
    
    // Host representations
    print_colored(BRIGHT_MAGENTA, "        ┌─────┐ ┌─────┐ ┌─────┐\n");
    print_colored(BRIGHT_MAGENTA, "        │Host1│ │Host2│ │Host3│\n");
    print_colored(BRIGHT_MAGENTA, "        └─────┘ └─────┘ └─────┘\n");
    
    if (host_count > 3) {
        print_colored(DIM, "            ... and %d more hosts\n", host_count - 3);
    }
    printf("\n");
}

/*
 * Enhanced IP address display with visual formatting
 */
void display_ip_info_enhanced(const char *label, const char *ip, const char *description)
{
    print_colored(BRIGHT_CYAN, "  🔸 ");
    print_colored(BOLD BRIGHT_WHITE, "%-20s", label);
    print_colored(BRIGHT_GREEN, "%s", ip);
    
    if (description) {
        print_colored(DIM, " (%s)", description);
    }
    printf("\n");
}

/*
 * Display calculation steps with beautiful formatting
 */
void show_calculation_steps(const char *title, const char **steps, int step_count)
{
    printf("\n");
    print_colored(BOLD BRIGHT_YELLOW, "🧮 %s\n", title);
    print_colored(BRIGHT_YELLOW, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    for (int i = 0; i < step_count; i++) {
        print_colored(BRIGHT_CYAN, "  Step %d: ", i + 1);
        print_colored(BRIGHT_WHITE, "%s\n", steps[i]);
        
        if (i < step_count - 1) {
            print_colored(DIM, "           ↓\n");
        }
    }
    
    print_colored(BRIGHT_YELLOW, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("\n");
}
