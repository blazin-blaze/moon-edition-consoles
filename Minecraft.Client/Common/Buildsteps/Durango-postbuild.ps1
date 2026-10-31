param(
    [string]$OutDir,
    [string]$ProjectDir
)

Write-Host "Durango Postbuild: script started. Output Directory: $OutDir, Project Directory: $ProjectDir"

$directories = @(
	"Durango\StoreImages",
    "Common\Media",
    "Common\res",
	"DLCImages",
	"Tutorial",
	"Durango",
	"music",
	"Sound",
	"Xbox",
	"DLC",
	"CU"
)

foreach ($dir in $directories) {
    New-Item -ItemType Directory -Path (Join-Path $OutDir $dir) -Force | Out-Null
}

$folderCopies = @(
    @{ Source = "Common\Media";    		 Dest = "Common\Media" },
    @{ Source = "Common\res";      		 Dest = "Common\res" },
	@{ Source = "DurangoMedia\Sound";    Dest = "Sound" },
	@{ Source = "DurangoMedia\DLC";      Dest = "DLC" },
	@{ Source = "DurangoMedia\Tutorial"; Dest = "Tutorial" },
	@{ Source = "music";      			 Dest = "music" },
	@{ Source = "Durango\DLCImages";	 Dest = "DLCImages" },
	@{ Source = "Durango\CU";	         Dest = "CU" }
)

foreach ($copy in $folderCopies) {
    $src = Join-Path $ProjectDir $copy.Source
    $dst = Join-Path $OutDir $copy.Dest

    if (Test-Path $src) {
        xcopy /q /y /i /s /e /d "$src" "$dst" 2>$null
    } else {
        Write-Host "Durango Postbuild: Folder not found, skipping: $src"
    }
}

$fileCopies = @(
    @{ Source = "Durango\DurangoExtras\xcompress.dll"; Dest = "xcompress.dll" },
	@{ Source = "Durango\DLCXbox1.cmp"; Dest = "DLCXbox1.cmp" }
)

foreach ($copy in $fileCopies) {
    $src = Join-Path $ProjectDir $copy.Source
    $dst = Join-Path $OutDir $copy.Dest
    if (Test-Path $src) {
        Copy-Item -Path $src -Destination $dst -Force
	} else {
        Write-Host "Durango Postbuild: File not found, skipping: $src"
    }
}

$deleteDirs = @(
    "Common\Media\Sound",
    "Common\Media\Graphics",
	"Common\Media\de-DE",
	"Common\Media\es-ES",
	"Common\Media\fr-FR",
	"Common\Media\it-IT",
	"Common\Media\ja-JP",
	"Common\Media\ko-KR",
	"Common\Media\pt-BR",
	"Common\Media\pt-PT",
	"Common\Media\zh-CHT"
)

foreach ($dir in $deleteDirs) {
    $path = Join-Path $OutDir $dir
    if (Test-Path $path) {
        Remove-Item $path -Recurse -Force -ErrorAction SilentlyContinue
    }
}

$delDirs = @(
    "Common\Media",
    "Common\Media\font"
)

foreach ($dir in $delDirs) {
    if ($delDirs -contains $dir) {
        $path = Join-Path $OutDir $dir
        if (Test-Path $path) {
            Get-ChildItem -Path $path -Recurse -Include *.swf, *.txt, *.resx, *.xml, *.loc, *.lang, *.col, *.xui, *.abc, *.h, "Mojang Font_7.ttf", "Mojang Font_11.ttf", MediaWindows64.arc, MediaOrbis.arc, MediaPS3.arc, MediaPSVita.arc -File |
                Remove-Item -Force -ErrorAction SilentlyContinue
        }
    }
}