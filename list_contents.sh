#!/bin/bash
if [ -z "$1" ]; then
    echo "Usage: $0 <directory_path>"
    exit 1
fi

DIR="$1"
if [ ! -d "$DIR" ]; then
    echo "Error: '$DIR' is not a directory"
    exit 1
fi

format_size() {
    local size=$1
    local units=("B" "KB" "MB" "GB" "TB")
    local i=0
    
    while (( size > 1024 && i < ${#units[@]} - 1 )); do
        size=$(awk "BEGIN {printf \"%.2f\", $size/1024}")
        ((i++))
    done
    echo "$size ${units[$i]}"
}

echo "Contents of directory: $DIR"
echo "============================"

for entry in "$DIR"/*; do
    if [ -d "$entry" ]; then
        count=$(find "$entry" -type f 2>/dev/null | wc -l)
        echo "Directory: $(basename "$entry") - files inside: $count"
    elif [ -f "$entry" ]; then
        size=$(stat -c%s "$entry" 2>/dev/null)
        readable_size=$(format_size "$size")
        echo "File: $(basename "$entry") - size: $readable_size"
    fi
done