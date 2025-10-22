#!/bin/bash
echo "Counting executable and non-executable commands in PATH..."

IFS=':' read -ra DIRS <<< "$PATH"
executable_count=0
non_executable_count=0

for dir in "${DIRS[@]}"; do
    if [ -d "$dir" ]; then
        for file in "$dir"/*; do
            # Check if it's a regular file
            if [ -f "$file" ]; then
                if [ -x "$file" ]; then
                    ((executable_count++))
                else
                    ((non_executable_count++))
                fi
            fi
        done
    fi
done

echo "Executable commands: $executable_count"
echo "Non-executable files: $non_executable_count"