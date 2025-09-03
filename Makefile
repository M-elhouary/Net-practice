
# ============================================================================
# NETWORK CALCULATOR - MAKEFILE
# ============================================================================
# 
# This Makefile builds the network calculator program with multiple source files
# for better organization and maintainability.
# 
# Source Files:
# - main.c: Main program and command line handling
# - bin_mask.c: Basic utility functions and subnet mask analysis
# - ip_conversion.c: IP address conversion using mathematical equations
# - network_analysis.c: Network range calculation and analysis functions
# - loopback_check.c: Loopback IP address detection and classification
# - enhanced_analysis.c: Advanced features (CIDR, class detection, validation)
# - advanced_features.c: Network scanning, IPv6 support, subnet splitting
# - output_formatter.c: Enhanced visual output with colors and formatting
# 
# Author: Network Tools Development Team
# ============================================================================

# Program name
NAME = net

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lm

# Source files (organized by functionality)
SRC = main.c \
      bin_mask.c \
      ip_conversion.c \
      network_analysis.c \
      loopback_check.c \
      enhanced_analysis.c \
      advanced_features.c \
      output_formatter.c

# Object files (automatically generated from source files)
OBJ = $(SRC:.c=.o)

# Header dependencies
HEADERS = net.h

# ============================================================================
# BUILD TARGETS
# ============================================================================

# Default target: build the program
all: $(NAME)
	@echo "✅ Build completed successfully!"
	@echo "💡 Usage: ./$(NAME) --help"
	@echo "📚 Documentation: cat USER_GUIDE.md"

# Link object files to create the final executable
$(NAME): $(OBJ)
	@echo "🔗 Linking object files..."
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
%.o: %.c $(HEADERS)
	@echo "🔨 Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# ============================================================================
# UTILITY TARGETS
# ============================================================================

# Clean build artifacts
clean:
	@echo "🧹 Cleaning build files..."
	rm -f $(OBJ) $(NAME)
	@echo "✅ Clean completed!"

fclean:
	@echo "🧹 Forcing clean of all build files..."
	rm -f $(OBJ) $(NAME)
	@echo "✅ Force clean completed!"

# Force rebuild everything
rebuild: clean all

# Install (copy to system directory)
install: $(NAME)
	@echo "📦 Installing $(NAME)..."
	cp $(NAME) /usr/local/bin/
	@echo "✅ Installation completed!"

# Show help
help:
	@echo "🛠️  Network Calculator Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  all      - Build the program (default)"
	@echo "  clean    - Remove build files"
	@echo "  rebuild  - Clean and build"
	@echo "  install  - Install to /usr/local/bin"
	@echo "  help     - Show this help"
	@echo ""
	@echo "Usage examples:"
	@echo "  make                    # Build the program"
	@echo "  make clean             # Clean build files"
	@echo "  make rebuild           # Force complete rebuild"

# ============================================================================
# SPECIAL TARGETS
# ============================================================================

.PHONY: all clean rebuild install help
