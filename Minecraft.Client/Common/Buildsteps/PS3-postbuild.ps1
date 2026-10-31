param(
    [string]$OutDir,
    [string]$ProjectDir,
	[string]$Configuration
)

Write-Host "PS3 Postbuild: script started. Output Directory: $OutDir, Project Directory: $ProjectDir"

$directories = @(
    "PS3_GAME\USRDIR\music",
    "PS3_GAME\USRDIR\DLC",
    "PS3_GAME\USRDIR\Common\Media",
	"PS3_GAME\USRDIR\Common\res",
    "PS3_GAME\USRDIR\PS3\Tutorial",
	"PS3_GAME\USRDIR\PS3\Sound"
)

foreach ($dir in $directories) {
    New-Item -ItemType Directory -Path (Join-Path $OutDir $dir) -Force | Out-Null
}

$folderCopies = @(
    @{ Source = "music";            Dest = "PS3_GAME\USRDIR\music" },
    @{ Source = "Common\Media";     Dest = "PS3_GAME\USRDIR\Common\Media" },
    @{ Source = "Common\res";       Dest = "PS3_GAME\USRDIR\Common\res" },
	@{ Source = "PS3Media\DLC";		Dest = "PS3_GAME\USRDIR\DLC" },
    @{ Source = "PS3Media\Sound";   Dest = "PS3_GAME\USRDIR\PS3\Sound" },
	#@{ Source = "PSVita\Tutorial";  Dest = "PS3_GAME\USRDIR\PS3\Tutorial" }, # str1k3r - softlocks game
	@{ Source = Join-Path $PSScriptRoot "Contents\PS3\PS3_GAME\LICDIR";  Dest = "PS3_GAME\LICDIR" },
	@{ Source = Join-Path $PSScriptRoot "Contents\PS3\PS3_GAME\TROPDIR"; Dest = "PS3_GAME\TROPDIR" }
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
        Write-Host "PS3 Postbuild: Folder not found, skipping: $src"
    }
}

$fileCopies = @(
    @{ Source = "PS3\PS3ProductCodes.bin"; Dest = "PS3_GAME\USRDIR\PS3\PS3ProductCodes.bin" },
	@{ Source = Join-Path $PSScriptRoot "Contents\PS3\PS3_DISC.SFB";         Dest = "PS3_DISC.SFB" },
	@{ Source = Join-Path $PSScriptRoot "Contents\PS3\PS3_GAME\ICON0.PNG";   Dest = "PS3_GAME\ICON0.PNG" },
	@{ Source = Join-Path $PSScriptRoot "Contents\PS3\PS3_GAME\PARAM.SFO";   Dest = "PS3_GAME\PARAM.SFO" },
	@{ Source = Join-Path $PSScriptRoot "Contents\PS3\PS3_GAME\PIC1.PNG";    Dest = "PS3_GAME\PIC1.PNG" },
	@{ Source = Join-Path $PSScriptRoot "Contents\PS3\PS3_GAME\PS3GAME.BG0"; Dest = "PS3_GAME\PS3GAME.BG0" },
	@{ Source = Join-Path $PSScriptRoot "Contents\PS3\PS3_GAME\PS3GAME.BG1"; Dest = "PS3_GAME\PS3GAME.BG1" },
	@{ Source = Join-Path $PSScriptRoot "Contents\PS3\PS3_GAME\PS3GAME.VR0"; Dest = "PS3_GAME\PS3GAME.VR0" },
	@{ Source = Join-Path $PSScriptRoot "Contents\PS3\PS3_GAME\PS3LOGO.DAT"; Dest = "PS3_GAME\PS3GAME.DAT" },
	@{ Source = Join-Path $PSScriptRoot "Contents\PS3\PS3_GAME\SND0.AT3";    Dest = "PS3_GAME\SND0.AT3" }
)

foreach ($copy in $fileCopies) {
    $src = Join-Path $ProjectDir $copy.Source
    $dst = Join-Path $OutDir $copy.Dest
    if (Test-Path $src) {
        Copy-Item -Path $src -Destination $dst -Force
	} else {
        Write-Host "PS3 Postbuild: File not found, skipping: $src"
    }
}

if ($Configuration -match "ContentPackage") {
    $builtFile = Join-Path $OutDir "Minecraft.Client.elf"
} else {
    $builtFile = Join-Path $OutDir "Minecraft.Client.self"
}

$newName   = "EBOOT.BIN"
$destDir   = Join-Path $OutDir "PS3_GAME\USRDIR"
$destPath  = Join-Path $destDir $newName

if (Test-Path $builtFile) {
    New-Item -ItemType Directory -Path $destDir -Force | Out-Null
    Copy-Item -Path $builtFile -Destination $destPath -Force
}

$deleteDirs = @(
    "PS3_GAME\USRDIR\Common\Media\Sound",
    "PS3_GAME\USRDIR\Common\Media\Graphics",
	"PS3_GAME\USRDIR\Common\Media\de-DE",
	"PS3_GAME\USRDIR\Common\Media\es-ES",
	"PS3_GAME\USRDIR\Common\Media\fr-FR",
	"PS3_GAME\USRDIR\Common\Media\it-IT",
	"PS3_GAME\USRDIR\Common\Media\ja-JP",
	"PS3_GAME\USRDIR\Common\Media\ko-KR",
	"PS3_GAME\USRDIR\Common\Media\pt-BR",
	"PS3_GAME\USRDIR\Common\Media\pt-PT",
	"PS3_GAME\USRDIR\Common\Media\zh-CHT"
)

foreach ($dir in $deleteDirs) {
    $path = Join-Path $OutDir $dir
    if (Test-Path $path) {
        Remove-Item $path -Recurse -Force -ErrorAction SilentlyContinue
    }
}

$delDirs = @(
    "PS3_GAME\USRDIR\Common\Media",
    "PS3_GAME\USRDIR\Common\Media\font"
)

foreach ($dir in $delDirs) {
    if ($delDirs -contains $dir) {
        $path = Join-Path $OutDir $dir
        if (Test-Path $path) {
            Get-ChildItem -Path $path -Recurse -Include *.swf, *.txt, *.resx, *.xml, *.loc, *.lang, *.col, *.xui, *.abc, *.h, "Mojang Font_7.ttf", "Mojang Font_11.ttf", MediaDurango.arc, MediaWindows64.arc, MediaOrbis.arc, MediaPSVita.arc -File |
                Remove-Item -Force -ErrorAction SilentlyContinue
        }
    }
}