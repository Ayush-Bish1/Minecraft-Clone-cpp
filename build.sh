#!/bin/bash

# Build script for Raylib project
# Usage: ./build.sh

set -e

echo "Building Raylib project..."

# Compile all .cpp files and create executables with same name
for file in *.cpp; do
    filename="${file%.cpp}"
    echo "Compiling $file..."
    g++ "$file" -o "$filename" -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    echo "Running $filename..."
    ./"$filename"
done

echo "Build and run complete!"