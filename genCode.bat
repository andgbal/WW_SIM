$outputFile = "code.md"
# Ensure the file is created/cleared at the start
New-Item -ItemType File -Path $outputFile -Force

Get-ChildItem -Path "src" -Recurse -File | Where-Object { $_.FullName -notmatch 'node_modules|bin|obj|.git' } | ForEach-Object {
    $relativePath = $_.FullName.Replace($(Get-Location).Path + "\", "")
    
    Add-Content -Path $outputFile -Value "## File: $relativePath" -Encoding UTF8
    Add-Content -Path $outputFile -Value '```' -Encoding UTF8
    
    try {
        # Force UTF8 when reading the source file
        $content = Get-Content $_.FullName -Raw -Encoding UTF8
        Add-Content -Path $outputFile -Value $content -Encoding UTF8
    } catch {
        Add-Content -Path $outputFile -Value "[Error reading file]" -Encoding UTF8
    }
    
    Add-Content -Path $outputFile -Value '```' -Encoding UTF8
    Add-Content -Path $outputFile -Value "" -Encoding UTF8
}

Write-Host "Success! code.md generated with UTF8 encoding." -ForegroundColor Green