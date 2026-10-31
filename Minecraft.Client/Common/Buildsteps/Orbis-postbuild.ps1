param(
    [string]$OutDir,
    [string]$ProjectDir
)

Write-Host "Orbis Postbuild: script started. Output Directory: $OutDir, Project Directory: $ProjectDir"

$directories = @(
    "PS4_GAME\music",
	"PS4_GAME\Orbis",
	"PS4_GAME\Common",
    "PS4_GAME\Common\Media",
	"PS4_GAME\Orbis\Tutorial",
    "PS4_GAME\Common\res",
    "PS4_GAME\Orbis\Sound",
    "PS4_GAME\Orbis\DLCImages",
    "PS4_GAME\Orbis\DLC"
)

foreach ($dir in $directories) {
    New-Item -ItemType Directory -Path (Join-Path $OutDir $dir) -Force | Out-Null
}

$folderCopies = @(
    @{ Source = "music";		    Dest = "PS4_GAME\music" },
    @{ Source = "Common\Media";     Dest = "PS4_GAME\Common\Media" },
    @{ Source = "Common\res";       Dest = "PS4_GAME\Common\res" },
	@{ Source = "OrbisMedia\Sound"; Dest = "PS4_GAME\Orbis\Sound" },
    @{ Source = "OrbisMedia\DLC";   Dest = "PS4_GAME\Orbis\DLC" },
    @{ Source = "Orbis\DLCImages";  Dest = "PS4_GAME\Orbis\DLCImages" },
    @{ Source = "OrbisMedia\Tutorial";  Dest = "PS4_GAME\Orbis\Tutorial" },
    @{ Source = "$env:SCE_ORBIS_SDK_DIR\target\sce_module";       Dest = "PS4_GAME\sce_module" },
    @{ Source = Join-Path $PSScriptRoot "Contents\Orbis\sce_gls"; Dest = "PS4_GAME\sce_gls" },
    @{ Source = Join-Path $PSScriptRoot "Contents\Orbis\sce_sys"; Dest = "PS4_GAME\sce_sys" }
)

foreach ($copy in $folderCopies) {
    if ([System.IO.Path]::IsPathRooted($copy.Source)) {
        $src = $copy.Source
    } else {
        $src = Join-Path $ProjectDir $copy.Source
    }
    $dst = Join-Path $OutDir $copy.Dest

    if (Test-Path $src) {
        xcopy /q /y /i /s /e /d "$src" "$dst" 2>$null
	} else {
        Write-Host "Orbis Postbuild: Folder not found, skipping: $src"
    }
}

$fileCopies = @(
    @{ Source = "Orbis\PS4ProductCodes.bin"; Dest = "PS4_GAME\Orbis\PS4ProductCodes.bin" },
	@{ Source = "Orbis\session_image.jpg"; Dest = "PS4_GAME\Orbis\session_image.jpg" }
)

foreach ($copy in $fileCopies) {
    $src = Join-Path $ProjectDir $copy.Source
    $dst = Join-Path $OutDir $copy.Dest
    if (Test-Path $src) {
        Copy-Item -Path $src -Destination $dst -Force
	} else {
        Write-Host "Orbis Postbuild: File not found, skipping: $src"
    }
}

$builtFile = Join-Path $OutDir "Minecraft.Client.elf"
$newName   = "eboot.bin"
$destDir   = Join-Path $OutDir "PS4_GAME"
$destPath  = Join-Path $destDir $newName

if (Test-Path $builtFile) {
    New-Item -ItemType Directory -Path $destDir -Force | Out-Null
    Copy-Item -Path $builtFile -Destination $destPath -Force
}

$deleteDirs = @(
    "PS4_GAME\Common\Media\Sound",
    "PS4_GAME\Common\Media\Graphics",
	"PS4_GAME\Common\Media\de-DE",
	"PS4_GAME\Common\Media\es-ES",
	"PS4_GAME\Common\Media\fr-FR",
	"PS4_GAME\Common\Media\it-IT",
	"PS4_GAME\Common\Media\ja-JP",
	"PS4_GAME\Common\Media\ko-KR",
	"PS4_GAME\Common\Media\pt-BR",
	"PS4_GAME\Common\Media\pt-PT",
	"PS4_GAME\Common\Media\zh-CHT"
)

foreach ($dir in $deleteDirs) {
    $path = Join-Path $OutDir $dir
    if (Test-Path $path) {
        Remove-Item $path -Recurse -Force -ErrorAction SilentlyContinue
    }
}

$delDirs = @(
    "PS4_GAME\Common\Media",
    "PS4_GAME\Common\Media\font"
)

foreach ($dir in $delDirs) {
    if ($delDirs -contains $dir) {
        $path = Join-Path $OutDir $dir
        if (Test-Path $path) {
            Get-ChildItem -Path $path -Recurse -Include *.swf, *.txt, *.resx, *.xml, *.loc, *.lang, *.col, *.xui, *.abc, *.h, "Mojang Font_7.ttf", "Mojang Font_11.ttf", MediaDurango.arc, MediaWindows64.arc, MediaPS3.arc, MediaPSVita.arc -File |
                Remove-Item -Force -ErrorAction SilentlyContinue
        }
    }
}

# Make Orbis PKG
$tools = Join-Path $env:SCE_ROOT_DIR "\ORBIS\Tools\Publishing Tools\bin"
$pkgSrc = Join-Path $OutDir "PS4_GAME"

$genGp4 = Join-Path $tools "gengp4_app.exe"
$pubCmd = Join-Path $tools "orbis-pub-cmd.exe"

$pkgPath  = Join-Path $OutDir "Minecraft.Client.pkg"
$gp4Name = $pkgSrc.TrimEnd('\','/')
$gp4File = "$gp4Name.gp4"

if (Test-Path $pkgPath) {
    Remove-Item $pkgPath -Force
}

if (-not (Test-Path $genGp4)) {
    Write-Host "Orbis Postbuild: gengp4_app.exe not found, skipping PKG creation."
} elseif (-not (Test-Path $pkgSrc)) {
    Write-Host "Orbis Postbuild: '$pkgSrc' not found, skipping PKG creation."
} else {
    Write-Host "Orbis Postbuild: Generating GP4..."
    
    Copy-Item -Path (Join-Path $pkgSrc "\sce_sys\param.sfo") -Destination (Join-Path $OutDir "param.sfo.bak") -Force
    $proc = Start-Process -FilePath $genGp4 -ArgumentList $pkgSrc `
        -WorkingDirectory (Split-Path $pkgSrc -Parent) `
        -NoNewWindow -Wait -PassThru

    if ($proc.ExitCode -ne 0) {
        Write-Host "Orbis Postbuild ERROR: gengp4_app.exe failed with exit code $($proc.ExitCode)"
        return
    }

    if (-not (Test-Path $gp4File)) {
        Write-Host "Orbis Postbuild ERROR: $gp4File was not created"
        return
    }

    Write-Host "Orbis Postbuild: Patching GP4 default_id..."
    (Get-Content $gp4File) -replace 'default_id="1"', 'default_id="0"' | Set-Content $gp4File

    Write-Host "Orbis Postbuild: Building PKG..."
    & $pubCmd img_create $gp4File $pkgPath

    (Get-Content $gp4File) -replace 'default_id="1"', 'default_id="0"' | Set-Content $gp4File # str1k3r - ran again so we can open the GP4 file without it throwing a error.
    Copy-Item -Path (Join-Path $OutDir "param.sfo.bak") -Destination (Join-Path $pkgSrc "\sce_sys\param.sfo") -Force # str1k3r - forces the .bak to replace the .sfo so we dont run into "Failed to get TITLE_ID".

    Write-Host "Orbis Postbuild: PKG created at $pkgPath"
}