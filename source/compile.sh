#!/bin/bash

# Rocky 8 with openCV4 installed via mamba environment cp4: 
# g++ -std=c++11 `pkg-config opencv --cflags ` -I/hpc/local/CentOS7/dhl_ec/software/Anaconda3_2021_05/envs/cp4/include/ -o ../binaries/sideNormalize_CentOS7 slideNormalize.cpp
# macOS Sequoia with openCV4 installed via brew:
# https://stackoverflow.com/questions/49943161/opencv-4-x-requires-enabled-c11-support-compilation-darknet-fatal-error
# g++ -std=c++11 slideNormalize.cpp `pkg-config opencv --cflags --libs` -o ../binaries/slideNormalize_macOS_Sequoia150 slideNormalize.cpp


# This script compiles the slideNormalize.cpp file based on the provided operating system.

# Function to display help
function display_help {
    echo "Usage: $0 [-os <operating_system>] [-v] [-h]"
    echo
    echo "Options:"
    echo "  -os     Specify the operating system (linux or macos)"
    echo "  -v      Enable verbose output"
    echo "  -h      Display this help message"
    exit 0
}

# Default values
VERBOSE=0
OS=""

# Parse command-line arguments
while getopts "os:vh" option; do
    case $option in
        os)
            OS=$OPTARG
            ;;
        v)
            VERBOSE=1
            ;;
        h)
            display_help
            ;;
        *)
            display_help
            ;;
    esac
done

# Ensure OS is provided
if [ -z "$OS" ]; then
    echo "Error: Operating system (-os) is required."
    display_help
fi

# Check if g++ is installed
if ! [ -x "$(command -v g++)" ]; then
    echo "Error: g++ is not installed. Please install g++ and try again."
    exit 1
fi

# Check if pkg-config is installed
if ! [ -x "$(command -v pkg-config)" ]; then
    echo "Error: pkg-config is not installed. Please install pkg-config and try again."
    exit 1
fi

# Check if OpenCV is installed
if ! pkg-config --exists opencv; then
    echo "Error: OpenCV is not installed. Please install OpenCV and try again."
    exit 1
fi

# Compile based on the provided OS
if [ "$OS" = "linux" ]; then
    if [ $VERBOSE -eq 1 ]; then
        echo "Compiling for Rocky 8 with OpenCV4..."
    fi
    g++ -std=c++11 `pkg-config opencv --cflags` \
        -I/hpc/local/CentOS7/dhl_ec/software/Anaconda3_2021_05/envs/cp4/include/ \
        -o ../binaries/slideNormalize_linux slideNormalize.cpp

elif [ "$OS" = "macos" ]; then
    if [ $VERBOSE -eq 1 ]; then
        echo "Compiling for macOS Mojave 10.14+, Catalina 10.15+, Big Sur 11+, Monterey 12+, Ventura 13+, Sonoma 14+, Sequoia 15+ with OpenCV4..."
    fi
    g++ -std=c++11 slideNormalize.cpp `pkg-config opencv --cflags --libs` \
        -o ../binaries/slideNormalize_macOS slideNormalize.cpp
else
    echo "Error: Unsupported operating system. Use 'linux' or 'macos'."
    exit 1
fi

# Final message if verbose is enabled
if [ $VERBOSE -eq 1 ]; then
    echo "Compilation complete."
fi
