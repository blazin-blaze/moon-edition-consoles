param(
    [string]$OutDir,
    [string]$ProjectDir
)

Write-Host "PSVita Postbuild: script started. Output Directory: $OutDir, Project Directory: $ProjectDir"

$directories = @(
    "PSVITA_GAME\music",
	"PSVITA_GAME\PSVita",
	"PSVITA_GAME\Common",
    "PSVITA_GAME\Common\Media",
    "PSVITA_GAME\Common\res",
    "PSVITA_GAME\PSVita\Sound",
    "PSVITA_GAME\PSVita\Tutorial"
)

foreach ($dir in $directories) {
    New-Item -ItemType Directory -Path (Join-Path $OutDir $dir) -Force | Out-Null
}

$folderCopies = @(
    @{ Source = "music";		     Dest = "PSVITA_GAME\music" },
    @{ Source = "Common\Media";      Dest = "PSVITA_GAME\Common\Media" },
    @{ Source = "Common\res";        Dest = "PSVITA_GAME\Common\res" },
	@{ Source = "PSVitaMedia\Sound"; Dest = "PSVITA_GAME\PSVita\Sound" },
	@{ Source = "PSVitaMedia\DLC";   Dest = "PSVITA_GAME\PSVita\DLC" },
    @{ Source = "PSVita\Tutorial";   Dest = "PSVITA_GAME\PSVita\Tutorial" },
	@{ Source = "$env:SCE_PSP2_SDK_DIR\target\sce_module";         Dest = "PSVITA_GAME\sce_module" },
    @{ Source = Join-Path $PSScriptRoot "Contents\PSVita\sce_sys"; Dest = "PSVITA_GAME\sce_sys" }
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
        Write-Host "PSVita Postbuild: Folder not found, skipping: $src"
    }
}

$fileCopies = @(
    @{ Source = "PSVita\PSVitaProductCodes.bin"; Dest = "PSVITA_GAME\PSVita\PSVitaProductCodes.bin" },
	@{ Source = "PSVita\session_image.png";      Dest = "PSVITA_GAME\PSVita\session_image.png" }
)

foreach ($copy in $fileCopies) {
    $src = Join-Path $ProjectDir $copy.Source
    $dst = Join-Path $OutDir $copy.Dest
    if (Test-Path $src) {
        New-Item -ItemType Directory -Path (Split-Path $dst -Parent) -Force | Out-Null
        Copy-Item -Path $src -Destination $dst -Force
	} else {
        Write-Host "PSVita Postbuild: File not found, skipping: $src"
    }
}

$builtFile = Join-Path $OutDir "Minecraft.Client.self"
$destDir   = Join-Path $OutDir "PSVITA_GAME"
$destPath  = Join-Path $destDir "eboot.bin"
$psp2bin   = Join-Path $env:SCE_PSP2_SDK_DIR "host_tools\build\bin\psp2bin.exe"

if (Test-Path $builtFile) {
    New-Item -ItemType Directory -Path $destDir -Force | Out-Null
    & "$psp2bin" -i "$builtFile" --strip-all -o "$destPath"
}

$deleteDirs = @(
    "PSVITA_GAME\Common\Media\Sound",
    "PSVITA_GAME\Common\Media\Graphics",
	"PSVITA_GAME\Common\Media\de-DE",
	"PSVITA_GAME\Common\Media\es-ES",
	"PSVITA_GAME\Common\Media\fr-FR",
	"PSVITA_GAME\Common\Media\it-IT",
	"PSVITA_GAME\Common\Media\ja-JP",
	"PSVITA_GAME\Common\Media\ko-KR",
	"PSVITA_GAME\Common\Media\pt-BR",
	"PSVITA_GAME\Common\Media\pt-PT",
	"PSVITA_GAME\Common\Media\zh-CHT"
)

foreach ($dir in $deleteDirs) {
    $path = Join-Path $OutDir $dir
    if (Test-Path $path) {
        Remove-Item $path -Recurse -Force -ErrorAction SilentlyContinue
    }
}

$delDirs = @(
    "PSVITA_GAME\Common\Media",
    "PSVITA_GAME\Common\Media\font"
)

foreach ($dir in $delDirs) {
    if ($delDirs -contains $dir) {
        $path = Join-Path $OutDir $dir
        if (Test-Path $path) {
            Get-ChildItem -Path $path -Recurse -Include *.swf, *.txt, *.resx, *.xml, *.loc, *.lang, *.col, *.h, *.xui, *.abc, "Mojang Font_7.ttf", "Mojang Font_11.ttf", MediaDurango.arc, MediaWindows64.arc, MediaPS3.arc, MediaOrbis.arc -File |
                Remove-Item -Force -ErrorAction SilentlyContinue
        }
    }
}

# Make PSVita VPK
$vpkPath  = Join-Path $OutDir "Minecraft.Client.vpk"
$vpkSrc   = Join-Path $OutDir "PSVITA_GAME"
$sevenZip = "C:\Program Files\7-Zip\7z.exe"

if (Test-Path $vpkPath) {
    Remove-Item $vpkPath -Force
}

if (-not (Test-Path $sevenZip)) {
    Write-Host "PSVita Postbuild: 7-Zip not found at '$sevenZip', skipping VPK creation."
} elseif (-not (Test-Path $vpkSrc)) {
    Write-Host "PSVita Postbuild: '$vpkSrc' not found, skipping VPK creation."
} else {
    Push-Location $vpkSrc
    & "$sevenZip" a -tzip -mx0 "$vpkPath" *
    Pop-Location
    Write-Host "PSVita Postbuild: VPK created at '$vpkPath'"
}