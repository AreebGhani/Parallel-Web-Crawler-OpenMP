# Load the recommendations from the extensions.json file
$extensions = (Get-Content extensions.json | ConvertFrom-Json).recommendations

# Install each extension using VS Code's CLI
foreach ($extension in $extensions) {
    code --install-extension $extension
}

Write-Host "All recommended extensions have been installed."
