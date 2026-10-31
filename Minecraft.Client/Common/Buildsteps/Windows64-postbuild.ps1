param(
    [string]$OutDir,
    [string]$ProjectDir
)

Write-Host "Windows64 Postbuild: script started. Output Directory: $OutDir, Project Directory: $ProjectDir"

$directories = @(
    "music",
    "Windows64\GameHDD",
    "Common\Media",
    "Common\res",
    "Windows64Media",
	"redist64"
)

foreach ($dir in $directories) {
    New-Item -ItemType Directory -Path (Join-Path $OutDir $dir) -Force | Out-Null
}

$folderCopies = @(
    @{ Source = "music";                Dest = "music" },
    @{ Source = "Common\res";      		Dest = "Common\res" },
    @{ Source = "Common\Media";    		Dest = "Common\Media" },
    @{ Source = "DurangoMedia";    		Dest = "Windows64Media" },
    @{ Source = "Windows64Media";  		Dest = "Windows64Media" },
	@{ Source = "Windows64Media\Sound";	Dest = "Windows64Media\Sound" },
	#@{ Source = "PSVita\Tutorial"; 	Dest = "Windows64Media\Tutorial" }, # str1k3r - doesnt even work bru
	@{ Source = "Windows64\Miles\lib\redist64"; Dest = "Windows64\redist64" }
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
        Write-Host "Windows64 Postbuild: Folder not found, skipping: $src"
    }
}

$fileCopies = @(
    @{ Source = Join-Path $ProjectDir   "Windows64\4JLibs\4J_Input\Windows64\vendor\SDL\lib\x64\SDL3.dll"; Dest = "SDL3.dll" },
	@{ Source = Join-Path $PSScriptRoot "Contents\Windows64\mss64.dll"; Dest = "mss64.dll" },
	@{ Source = "Windows64\Iggy\lib\redist64\iggy_w64.dll"; Dest = "iggy_w64.dll" }
)

foreach ($copy in $fileCopies) {
    $src = $copy.Source
    $dst = Join-Path $OutDir $copy.Dest
    if (Test-Path $src) {
        Copy-Item -Path $src -Destination $dst -Force
	} else {
        Write-Host "Windows64 Postbuild: File not found, skipping: $src"
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
            Get-ChildItem -Path $path -Recurse -Include *.swf, *.txt, *.resx, *.xml, *.loc, *.lang, *.col, *.xui, *.abc, *.h, "Mojang Font_7.ttf", "Mojang Font_11.ttf", MediaDurango.arc, MediaOrbis.arc, MediaPS3.arc, MediaPSVita.arc -File |
                Remove-Item -Force -ErrorAction SilentlyContinue
        }
    }
}