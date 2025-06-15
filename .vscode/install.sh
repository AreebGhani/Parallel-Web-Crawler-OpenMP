#!/bin/bash

# Read the recommendations from the extensions.json file
extensions=$(grep -oP '"recommendations":\s*\[\K[^\]]+' extensions.json | tr -d '",[:space:]' | tr ' ' '\n')

# Loop through each extension and install it using VS Code CLI
for extension in $extensions; do
    code --install-extension "$extension"
done

echo "All recommended extensions have been installed."
