param(
    [string]$OutDir,
    [string]$ProjectDir
)

Write-Host "Xbox Postbuild: script started. Output Directory: $OutDir, Project Directory: $ProjectDir"

$directories = @(
    "res",
    "Tutorial"
)

foreach ($dir in $directories) {
    New-Item -ItemType Directory -Path (Join-Path $OutDir $dir) -Force | Out-Null
}

$copies = @(
    @{ Source = "Common\res"; Dest = "res" }
)

foreach ($copy in $copies) {
    $src = Join-Path $ProjectDir $copy.Source
    $dst = Join-Path $OutDir $copy.Dest

    if (Test-Path $src) {
        xcopy /q /y /i /s /e /d "$src" "$dst" 2>$null
    } else {
        Write-Host "Xbox Postbuild: Folder not found, skipping: $src"
    }
}

$fileCopies = @(
    @{ Source = "Xbox\kinect\speech\nuisp1031"; Dest = "nuisp1031" },
	@{ Source = "Xbox\kinect\speech\nuisp1033"; Dest = "nuisp1033" },
	@{ Source = "Xbox\kinect\speech\nuisp1036"; Dest = "nuisp1036" },
	@{ Source = "Xbox\kinect\speech\nuisp1041"; Dest = "nuisp1040" },
	@{ Source = "Xbox\kinect\speech\nuisp1041"; Dest = "nuisp1041" },
	@{ Source = "Xbox\kinect\speech\nuisp2057"; Dest = "nuisp2057" },
	@{ Source = "Xbox\kinect\speech\nuisp2058"; Dest = "nuisp2058" },
	@{ Source = "Xbox\kinect\speech\nuisp3081"; Dest = "nuisp3081" },
	@{ Source = "Xbox\kinect\speech\nuisp3082"; Dest = "nuisp3082" },
	@{ Source = "Xbox\kinect\speech\nuisp3084"; Dest = "nuisp3084" },
	@{ Source = "XboxMedia\AvatarAwards";       Dest = "AvatarAwards" },
	@{ Source = "XboxMedia\XZP\TMSFiles.xzp";   Dest = "TMSFiles.xzp" },
	@{ Source = "Common\Tutorial\Tutorial";     Dest = "Tutorial\Tutorial" }
)

foreach ($copy in $fileCopies) {
    $src = Join-Path $ProjectDir $copy.Source
    $dst = Join-Path $OutDir $copy.Dest
    if (Test-Path $src) {
        Copy-Item -Path $src -Destination $dst -Force
	} else {
        Write-Host "Xbox Postbuild: File not found, skipping: $src"
    }
}