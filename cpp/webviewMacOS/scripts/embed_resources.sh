#!/bin/bash
# embed_resources.sh - Convert web files to C++ byte arrays

INPUT_DIR="$1"
OUTPUT_FILE="$2"

if [ -z "$INPUT_DIR" ] || [ -z "$OUTPUT_FILE" ]; then
    echo "Usage: $0 <input_dir> <output_file>"
    exit 1
fi

echo "// Auto-generated embedded resources" > "$OUTPUT_FILE"
echo "#include \"resource_manager.h\"" >> "$OUTPUT_FILE"
echo "" >> "$OUTPUT_FILE"

# Function to convert file to byte array
embed_file() {
    local file_path="$1"
    local var_name="$2"
    local web_path="$3"
    local mime_type="$4"
    
    echo "// Embedded: $web_path" >> "$OUTPUT_FILE"
    echo "const unsigned char ${var_name}_data[] = {" >> "$OUTPUT_FILE"
    
    # Convert file to hex bytes
    xxd -i < "$file_path" >> "$OUTPUT_FILE"
    
    echo "};" >> "$OUTPUT_FILE"
    echo "const size_t ${var_name}_size = sizeof(${var_name}_data);" >> "$OUTPUT_FILE"
    echo "" >> "$OUTPUT_FILE"
}

# Function to get MIME type
get_mime_type() {
    local file="$1"
    case "${file##*.}" in
        html) echo "text/html" ;;
        css) echo "text/css" ;;
        js) echo "application/javascript" ;;
        json) echo "application/json" ;;
        png) echo "image/png" ;;
        jpg|jpeg) echo "image/jpeg" ;;
        svg) echo "image/svg+xml" ;;
        *) echo "application/octet-stream" ;;
    esac
}

# Process all files in the directory
find "$INPUT_DIR" -type f | while read -r file; do
    # Get relative path from input directory
    rel_path="${file#$INPUT_DIR}"
    
    # Create variable name (replace special chars with underscores)
    var_name=$(echo "$rel_path" | sed 's/[^a-zA-Z0-9]/_/g' | sed 's/^_//')
    
    # Get MIME type
    mime_type=$(get_mime_type "$file")
    
    # Embed the file
    embed_file "$file" "$var_name" "$rel_path" "$mime_type"
    
    # Register the resource
    echo "EMBED_RESOURCE($var_name, \"$rel_path\", \"$mime_type\");" >> "$OUTPUT_FILE"
done

echo "Resources embedded successfully in $OUTPUT_FILE"
