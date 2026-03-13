#!/bin/bash
set -e
echo "Building Raylib project..."
for file in *.cpp; do
    filename="${file%.cpp}"
    echo "Compiling $file..."
    g++ "$file" -o "$filename" -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    echo "Running $filename..."
    ./"$filename"
done
