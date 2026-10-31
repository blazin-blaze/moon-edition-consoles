param(
    [string]$ProjectDir,
    [string]$Platform,
    [string]$Configuration
)

if ($ProjectDir -match "Minecraft.World") {
    $BaseDir = "$ProjectDir..\Minecraft.Client\"
} elseif ($ProjectDir -match "Minecraft.Client") {
    $BaseDir = $ProjectDir
} else {
    Write-Host "4JLibs: Unknown project directory: '$ProjectDir'"
    exit 1
}

switch ($Platform) {
    "x64"       { $LibsDir = Join-Path $BaseDir "Windows64\4JLibs\"; $ExpectedCount = 4; $PlatformName = "Windows64" }
    "Durango"   { $LibsDir = Join-Path $BaseDir "Durango\4JLibs\";   $ExpectedCount = 4; $PlatformName = "Durango" }
    "Xbox 360"  { $LibsDir = Join-Path $BaseDir "Xbox\4JLibs\";      $ExpectedCount = 5; $PlatformName = "Xbox" }
    "PS3"       { $LibsDir = Join-Path $BaseDir "PS3\4JLibs\";       $ExpectedCount = 4; $PlatformName = "PS3" }
    "Orbis"     { $LibsDir = Join-Path $BaseDir "Orbis\4JLibs\";     $ExpectedCount = 4; $PlatformName = "Orbis" }
    "PSVita"    { $LibsDir = Join-Path $BaseDir "PSVita\4JLibs\";    $ExpectedCount = 4; $PlatformName = "PSVita" }
    default     { Write-Host "4JLibs: Unknown platform: '$Platform'";   exit 1 }
}

if ($Configuration -match "ContentPackage" -or $Configuration -match "Release") {
    $BuildConfig = "Release"
} elseif ($Configuration -match "Debug") {
    $BuildConfig = "Debug"
} else {
    Write-Host "4JLibs: Configuration did not match any known patterns, defaulting to Debug"
    $BuildConfig = "Debug"
}

if (-not (Test-Path $LibsDir)) {
    Write-Host "4JLibs: $PlatformName 4JLibs directory not found"
    exit 1
} elseif (-not (Test-Path "$LibsDir\Headers\$PlatformName")) {
    Write-Host "4JLibs: $PlatformName 4JLibs Headers directory not found"
    exit 1
} elseif (-not (Test-Path "$LibsDir\Headers")) {
    Write-Host "4JLibs: Headers directory not found"
    exit 1
}

$incPath  = Join-Path $LibsDir "inc"
$libsPath = Join-Path $LibsDir "libs"

$incCount  = (Get-ChildItem -Path $incPath -Filter "*.h" -ErrorAction SilentlyContinue).Count

if ($BuildConfig -eq "Release") {
    $libCount = (Get-ChildItem -Path $libsPath -Filter "*_r.lib" -ErrorAction SilentlyContinue).Count
} else {
    $libCount = (Get-ChildItem -Path $libsPath -Filter "*_d.lib" -ErrorAction SilentlyContinue).Count
}

if ($incCount -lt $ExpectedCount) {
    Write-Host "4JLibs: Copying header files for $PlatformName"
    xcopy "$LibsDir\Headers\$PlatformName\*" "$incPath" /E /I /Y
} else {
    Write-Host "4JLibs: inc files already present, skipping copy."
}

if ($libCount -lt $ExpectedCount) {
    Write-Host "4JLibs: Building 4JLibs for $PlatformName in $BuildConfig"

    & "C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe" `
        (Join-Path $LibsDir "4JLibs.sln") `
        /p:Configuration=$BuildConfig `
        /p:Platform=$PlatformName `
        /v:minimal `
        /m
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "4JLibs: Build failed with exit code: $LASTEXITCODE"
        exit $LASTEXITCODE
    }
} else {
    Write-Host "4JLibs: libs already present, skipping build."
}