# nanoDB

A lightweight, high-performance database framework designed for Electronic Design Automation (EDA) applications.

**Status:** Under Active Development  
**License:** Non-commercial

## Overview

nanoDB is a modern C++ database framework specifically built for EDA workflows. It provides efficient data structures and interfaces for managing chip design data, including Liberty timing libraries, Verilog netlists, parasitic information, package models, and power analysis.

## Features

- **Liberty Library Support**: Parse and manage Liberty timing library files (.lib)
- **Verilog Support**: Handle Verilog netlist data structures
- **Chip Design Data**: Comprehensive chip design database with efficient storage
- **Package Modeling**: Support for package design and analysis
- **Parasitic Analysis**: Manage parasitic resistance and capacitance data
- **Power Analysis**: Power consumption calculation and optimization
- **Serialization**: Boost serialization support for database persistence
- **Thread-Safe**: Multi-threaded operations support with OpenMP
- **Memory Efficiency**: Optional jemalloc/tcmalloc support for optimized memory management

## Requirements

### System Requirements
- CMake 3.14 or higher
- C++23 compatible compiler:
  - GCC (recommended)
  - AppleClang (macOS)

### Dependencies

- **Boost** (1.83.0 or higher)
  - boost_serialization
- **Eigen3**: Linear algebra library
- **libpng**: PNG image support
- **OpenMP**: Parallel computing (optional)
- **jemalloc** or **tcmalloc**: Memory allocator (optional)

### Environment Variables

Set the `BOOST_PATH` environment variable to your Boost installation directory:
```bash
export BOOST_PATH=/path/to/boost
```

For custom Eigen installation, set:
```bash
export EIGEN_PATH=/path/to/eigen
```

## Building

### Basic Build

```bash
# Configure
cmake -Bbuild -G Ninja

# Build
cmake --build build
```

### Build Options

```bash
# Enable assertions
cmake -Bbuild -DENABLE_ASSERTION=ON

# Enable/disable exceptions
cmake -Bbuild -DENABLE_EXCEPTION=OFF

# Use jemalloc
cmake -Bbuild -DNS_MALLOC_LIB=jemalloc

# Use tcmalloc
cmake -Bbuild -DNS_MALLOC_LIB=tcmalloc

# Enable address sanitizer (for debugging)
cmake -Bbuild -DENABLE_ASAN=ON

# Enable thread sanitizer (for debugging)
cmake -Bbuild -DENABLE_TSAN=ON

# Enable profiling
cmake -Bbuild -DENABLE_PROFILING=ON

# Build without tests
cmake -Bbuild -DBUILD_NANO_TEST=OFF
```

### macOS Build

On macOS with Homebrew:
```bash
# Install dependencies
brew install boost eigen libpng cmake ninja

# Optional: Enable Apple Accelerate framework
cmake -Bbuild -DENABLE_APPLE_ACCELERATE=ON -G Ninja
cmake --build build
```

### Ubuntu/Debian Build

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install build-essential cmake ninja-build \
    libeigen3-dev libpng-dev libjemalloc-dev

# Install Boost (or use install-boost action in CI)
# Build with jemalloc
export BOOST_PATH=/path/to/boost
cmake -Bbuild -DNS_MALLOC_LIB=jemalloc -G Ninja
cmake --build build
```

## Testing

Run the unit tests:
```bash
build/bin/nanoTest.exe
```

## Project Structure

```
nanoDB/
├── include/nano/          # Public headers
│   ├── db                 # Main database header
│   ├── core/              # Core module headers
│   │   ├── basic/         # Basic data structures
│   │   ├── chip/          # Chip design data
│   │   ├── liberty/       # Liberty library
│   │   ├── package/       # Package modeling
│   │   ├── parasitic/     # Parasitic data
│   │   ├── power/         # Power analysis
│   │   └── verilog/       # Verilog support
│   └── common/            # Common utilities
├── src/                   # Source code implementation
│   ├── basic/             # Database core implementation
│   ├── chip/              # Chip module
│   ├── liberty/           # Liberty parser and data structures
│   ├── package/           # Package module
│   ├── parasitic/         # Parasitic module
│   ├── power/             # Power module
│   └── verilog/           # Verilog module
├── test/                  # Unit tests
├── 3rdparty/             # Third-party dependencies
├── reference/            # Reference documentation
└── tools/                # Utility tools
```

## Usage

Include the main header in your C++ code:

```cpp
#include <nano/db>

// Use the nano namespace
using namespace nano;

// Create and use database objects
// (Detailed API documentation coming soon)
```

## Contributing

This project is currently under active development. Contributions, issues, and feature requests are welcome.

## License

**Non-commercial use only.** This software is provided for educational and research purposes.

## References

- Liberty User Guides and Reference Manual Suite (Version 2017.06) included in `reference/` directory