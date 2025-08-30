#!/bin/bash

# SU1 Display Server Build Script
# Ultimate Reality Edition

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m' # No Color

# ASCII Art Banner
print_banner() {
    echo -e "${PURPLE}"
    echo "███████╗██╗   ██╗ ██╗"
    echo "██╔════╝██║   ██║███║"
    echo "███████╗██║   ██║╚██║"
    echo "╚════██║██║   ██║ ██║"
    echo "███████║╚██████╔╝ ██║"
    echo "╚══════╝ ╚═════╝  ╚═╝"
    echo -e "${WHITE}Display Server - Ultimate Reality Edition${NC}"
    echo -e "${CYAN}Building the future of human-computer consciousness integration${NC}"
    echo ""
}

# Logging functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

log_step() {
    echo -e "${PURPLE}[STEP]${NC} $1"
}

# Configuration
BUILD_TYPE="Release"
BUILD_JOBS=$(nproc)
INSTALL_PREFIX="/usr/local"
BUILD_TESTS=false
BUILD_EXAMPLES=true
BUILD_DOCS=false
ENABLE_CONSCIOUSNESS=true
ENABLE_QUANTUM_EFFECTS=true
ENABLE_TRANSCENDENT_UI=true
ENABLE_ULTIMATE_OPTIMIZATION=true

# Parse command line arguments
parse_arguments() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            --debug)
                BUILD_TYPE="Debug"
                log_info "Debug build enabled"
                shift
                ;;
            --release)
                BUILD_TYPE="Release"
                log_info "Release build enabled"
                shift
                ;;
            --jobs=*)
                BUILD_JOBS="${1#*=}"
                log_info "Using $BUILD_JOBS build jobs"
                shift
                ;;
            --prefix=*)
                INSTALL_PREFIX="${1#*=}"
                log_info "Install prefix set to $INSTALL_PREFIX"
                shift
                ;;
            --tests)
                BUILD_TESTS=true
                log_info "Tests enabled"
                shift
                ;;
            --no-examples)
                BUILD_EXAMPLES=false
                log_info "Examples disabled"
                shift
                ;;
            --docs)
                BUILD_DOCS=true
                log_info "Documentation generation enabled"
                shift
                ;;
            --disable-consciousness)
                ENABLE_CONSCIOUSNESS=false
                log_warning "Consciousness features disabled"
                shift
                ;;
            --disable-quantum)
                ENABLE_QUANTUM_EFFECTS=false
                log_warning "Quantum effects disabled"
                shift
                ;;
            --disable-transcendent)
                ENABLE_TRANSCENDENT_UI=false
                log_warning "Transcendent UI disabled"
                shift
                ;;
            --basic)
                ENABLE_CONSCIOUSNESS=false
                ENABLE_QUANTUM_EFFECTS=false
                ENABLE_TRANSCENDENT_UI=false
                ENABLE_ULTIMATE_OPTIMIZATION=false
                log_warning "Basic build mode - advanced features disabled"
                shift
                ;;
            --help)
                show_help
                exit 0
                ;;
            *)
                log_error "Unknown option $1"
                show_help
                exit 1
                ;;
        esac
    done
}

show_help() {
    echo "SU1 Display Server Build Script"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --debug                   Build in debug mode"
    echo "  --release                 Build in release mode (default)"
    echo "  --jobs=N                  Use N parallel jobs (default: $(nproc))"
    echo "  --prefix=PATH             Set install prefix (default: /usr/local)"
    echo "  --tests                   Enable building tests"
    echo "  --no-examples             Disable building examples"
    echo "  --docs                    Enable documentation generation"
    echo "  --disable-consciousness   Disable consciousness-based features"
    echo "  --disable-quantum         Disable quantum effects"
    echo "  --disable-transcendent    Disable transcendent UI features"
    echo "  --basic                   Basic build without advanced features"
    echo "  --help                    Show this help message"
    echo ""
    echo "Advanced Features:"
    echo "  - Consciousness Integration: AI-driven interface adaptation"
    echo "  - Quantum Effects: Physics-inspired visual elements"
    echo "  - Transcendent UI: Sacred geometry and divine proportions"
    echo "  - Ultimate Optimization: Maximum performance compilation"
    echo ""
}

# Check dependencies
check_dependencies() {
    log_step "Checking dependencies..."
    
    local missing_deps=()
    
    # Check for required build tools
    if ! command -v cmake &> /dev/null; then
        missing_deps+=("cmake")
    fi
    
    if ! command -v pkg-config &> /dev/null; then
        missing_deps+=("pkg-config")
    fi
    
    if ! command -v gcc &> /dev/null && ! command -v clang &> /dev/null; then
        missing_deps+=("gcc or clang")
    fi
    
    # Check for required libraries using pkg-config
    local required_libs=(
        "libdrm" "gbm" "egl" "glesv2" "libinput" "libudev"
        "cairo" "pango" "pangoft2" "freetype2" "harfbuzz"
        "pixman-1" "librsvg-2.0" "json-glib-1.0" "glib-2.0"
        "gio-2.0" "gobject-2.0" "gdk-pixbuf-2.0" "gtk+-3.0"
    )
    
    for lib in "${required_libs[@]}"; do
        if ! pkg-config --exists "$lib"; then
            missing_deps+=("$lib")
        fi
    done
    
    if [ ${#missing_deps[@]} -ne 0 ]; then
        log_error "Missing dependencies: ${missing_deps[*]}"
        echo ""
        echo "Please install the missing dependencies:"
        echo ""
        echo "Ubuntu/Debian:"
        echo "sudo apt install cmake pkg-config build-essential"
        echo "sudo apt install libdrm-dev libgbm-dev libegl1-mesa-dev libgles2-mesa-dev"
        echo "sudo apt install libinput-dev libudev-dev libcairo2-dev libpango1.0-dev"
        echo "sudo apt install libfreetype6-dev libharfbuzz-dev libpixman-1-dev"
        echo "sudo apt install librsvg2-dev libjson-glib-dev libgtk-3-dev"
        echo ""
        echo "Fedora/RHEL:"
        echo "sudo dnf install cmake pkgconf-pkg-config gcc-c++"
        echo "sudo dnf install libdrm-devel mesa-libgbm-devel mesa-libEGL-devel mesa-libGLES-devel"
        echo "sudo dnf install libinput-devel systemd-devel cairo-devel pango-devel"
        echo "sudo dnf install freetype-devel harfbuzz-devel pixman-devel"
        echo "sudo dnf install librsvg2-devel json-glib-devel gtk3-devel"
        echo ""
        echo "Arch Linux:"
        echo "sudo pacman -S cmake pkgconf base-devel"
        echo "sudo pacman -S libdrm mesa libinput systemd cairo pango freetype2 harfbuzz"
        echo "sudo pacman -S pixman librsvg json-glib gtk3"
        echo ""
        exit 1
    fi
    
    log_success "All dependencies satisfied"
}

# Configure build
configure_build() {
    log_step "Configuring build system..."
    
    # Create build directory
    mkdir -p build
    cd build
    
    # Prepare CMake arguments
    local cmake_args=(
        "-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
        "-DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX"
        "-DSU1_BUILD_EXAMPLES=$BUILD_EXAMPLES"
        "-DSU1_BUILD_TESTS=$BUILD_TESTS"
        "-DSU1_BUILD_DOCS=$BUILD_DOCS"
        "-DSU1_ENABLE_CONSCIOUSNESS=$ENABLE_CONSCIOUSNESS"
        "-DSU1_ENABLE_QUANTUM_EFFECTS=$ENABLE_QUANTUM_EFFECTS"
        "-DSU1_ENABLE_TRANSCENDENT_UI=$ENABLE_TRANSCENDENT_UI"
        "-DSU1_ENABLE_ULTIMATE_OPTIMIZATION=$ENABLE_ULTIMATE_OPTIMIZATION"
    )
    
    # Add advanced optimization flags for release builds
    if [ "$BUILD_TYPE" = "Release" ] && [ "$ENABLE_ULTIMATE_OPTIMIZATION" = "true" ]; then
        cmake_args+=(
            "-DCMAKE_CXX_FLAGS_RELEASE=-O3 -march=native -mtune=native -flto"
            "-DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON"
        )
        log_info "Ultimate optimization enabled"
    fi
    
    # Run CMake
    log_info "Running CMake with: ${cmake_args[*]}"
    if ! cmake "${cmake_args[@]}" ..; then
        log_error "CMake configuration failed"
        exit 1
    fi
    
    log_success "Build system configured successfully"
}

# Build the project
build_project() {
    log_step "Building SU1 Display Server..."
    
    # Show build configuration
    echo -e "${CYAN}Build Configuration:${NC}"
    echo "  Build Type: $BUILD_TYPE"
    echo "  Parallel Jobs: $BUILD_JOBS"
    echo "  Install Prefix: $INSTALL_PREFIX"
    echo "  Examples: $BUILD_EXAMPLES"
    echo "  Tests: $BUILD_TESTS"
    echo "  Documentation: $BUILD_DOCS"
    echo ""
    echo -e "${PURPLE}Advanced Features:${NC}"
    echo "  Consciousness Integration: $ENABLE_CONSCIOUSNESS"
    echo "  Quantum Effects: $ENABLE_QUANTUM_EFFECTS"
    echo "  Transcendent UI: $ENABLE_TRANSCENDENT_UI"
    echo "  Ultimate Optimization: $ENABLE_ULTIMATE_OPTIMIZATION"
    echo ""
    
    # Start build
    local start_time=$(date +%s)
    
    if ! make -j"$BUILD_JOBS"; then
        log_error "Build failed"
        exit 1
    fi
    
    local end_time=$(date +%s)
    local build_time=$((end_time - start_time))
    
    log_success "Build completed in ${build_time} seconds"
}

# Run tests if enabled
run_tests() {
    if [ "$BUILD_TESTS" = "true" ]; then
        log_step "Running tests..."
        
        if ! ctest --output-on-failure; then
            log_error "Tests failed"
            exit 1
        fi
        
        log_success "All tests passed"
    fi
}

# Generate documentation if enabled
generate_docs() {
    if [ "$BUILD_DOCS" = "true" ]; then
        log_step "Generating documentation..."
        
        if ! make docs; then
            log_warning "Documentation generation failed"
        else
            log_success "Documentation generated"
        fi
    fi
}

# Show build summary
show_summary() {
    echo ""
    echo -e "${GREEN}╔══════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║${NC}                    ${WHITE}BUILD SUCCESSFUL${NC}                        ${GREEN}║${NC}"
    echo -e "${GREEN}╚══════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${CYAN}Build artifacts:${NC}"
    echo "  Binaries: $(pwd)/bin/"
    echo "  Libraries: $(pwd)/lib/"
    if [ "$BUILD_EXAMPLES" = "true" ]; then
        echo "  Examples: $(pwd)/bin/su1_demo_display_server"
    fi
    echo ""
    echo -e "${YELLOW}Next steps:${NC}"
    echo "  1. Run the demo: make run_demo"
    echo "  2. Install system-wide: sudo make install"
    echo "  3. Read documentation: README.md"
    echo ""
    echo -e "${PURPLE}Experience the future of consciousness-driven computing!${NC}"
    echo ""
}

# Main execution
main() {
    print_banner
    parse_arguments "$@"
    check_dependencies
    configure_build
    build_project
    run_tests
    generate_docs
    show_summary
}

# Execute main function with all arguments
main "$@"
