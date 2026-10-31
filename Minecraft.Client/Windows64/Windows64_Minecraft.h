#pragma once

#include <d3d11.h>

static bool isF11Pressed = false;
static bool wasF11Pressed = false;

extern BOOL g_bWidescreen;

extern int g_iScreenWidth;
extern int g_iScreenHeight;

extern BOOL g_bResolutionSetByCMD;

extern ID3D11Device* g_pd3dDevice;

void CleanupDevice();

struct Windows64LaunchArgs
{
	bool fullscreen;
	int resolution;
};