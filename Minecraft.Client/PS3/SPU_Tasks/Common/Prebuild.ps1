# str1k3r - This prebuild makes sure that the ObjFiles directory exists so the PS3 SPU tasks can properly build
param(
    [string]$ProjectDir,
    [string]$Configuration
)

$ParentDir = Split-Path -Path $ProjectDir -Parent

$directories = @(
    "ObjFiles\$Configuration"
)

foreach ($dir in $directories) {
    New-Item -ItemType Directory -Path (Join-Path $ParentDir $dir) -Force | Out-Null
}