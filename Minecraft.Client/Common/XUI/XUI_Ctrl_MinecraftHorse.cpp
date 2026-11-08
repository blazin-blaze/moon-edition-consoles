#include "stdafx.h"
#include "..\..\Minecraft.h"
#include "..\..\ScreenSizeCalculator.h"
#include "..\..\EntityRenderDispatcher.h"
#include "..\..\HorseRenderer.h"
#include "..\..\Lighting.h"
#include "..\..\ModelHorse.h"
#include "XUI_Ctrl_MinecraftHorse.h"
#include "XUI_Scene_AbstractContainer.h"
#include "XUI_Scene_HorseInventory.h"
#include "..\..\Options.h"

//str1k3r - based off of XUI_Ctrl_MinecraftPlayer.cpp

CXuiCtrlMinecraftHorse::CXuiCtrlMinecraftHorse() :
	m_bDirty(FALSE),
	m_fScale(1.0f),
	m_fAlpha(1.0f)
{
	Minecraft *pMinecraft = Minecraft::GetInstance();

	ScreenSizeCalculator ssc(pMinecraft->options, pMinecraft->width_phys, pMinecraft->height_phys);
	m_fScreenWidth = (float)pMinecraft->width_phys;
	m_fRawWidth = (float)ssc.rawWidth;
	m_fScreenHeight = (float)pMinecraft->height_phys;
	m_fRawHeight = (float)ssc.rawHeight;
}

HRESULT CXuiCtrlMinecraftHorse::OnInit(XUIMessageInit* pInitData, BOOL& rfHandled)
{
	HRESULT hr = S_OK;

	HXUIOBJ parent = m_hObj;
	HXUICLASS hcHorseInventoryClass = XuiFindClass(L"CXuiSceneHorseInventory");
	HXUICLASS currentClass;

	do
	{
		XuiElementGetParent(parent, &parent);
		currentClass = XuiGetObjectClass(parent);
	} while (parent != NULL && !XuiClassDerivesFrom(currentClass, hcHorseInventoryClass));

	assert(parent != NULL);

	VOID *pObj;
	XuiObjectFromHandle(parent, &pObj);
	m_containerScene = (CXuiSceneHorseInventory *)pObj;

	m_iPad = m_containerScene->getPad();

	return hr;
}

HRESULT CXuiCtrlMinecraftHorse::OnRender(XUIMessageRender *pRenderData, BOOL &bHandled)
{
#ifdef _XBOX
	HXUIDC hDC = pRenderData->hDC;

	// build and render with the game call

	RenderManager.Set_matrixDirty();

	Minecraft *pMinecraft = Minecraft::GetInstance();

	glColor4f(1, 1, 1, 1);
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m_fRawWidth, m_fRawHeight, 0, 1000, 3000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -2000);


	D3DXMATRIX matrix;
	CXuiControl xuiControl(m_hObj);
	xuiControl.GetFullXForm(&matrix);
	float bwidth, bheight;
	xuiControl.GetBounds(&bwidth, &bheight);

	float xo = ((matrix._41 + ((bwidth*matrix._11) / 2)) / m_fScreenWidth) * m_fRawWidth;
	float yo = ((matrix._42 + (bheight*matrix._22)) / m_fScreenHeight) * m_fRawHeight;

	glEnable(GL_RESCALE_NORMAL);
	glEnable(GL_COLOR_MATERIAL);

	glPushMatrix();
	//str1k3r - 3.5f also is wrong but idk what 4J made it
	glTranslatef(xo, yo - 3.5f, 50.0f);

	shared_ptr<LivingEntity> entityHorse = m_containerScene->GetHorse();

	float ss;
	
	//str1k3r - these are completely wrong but i cant get it to scale properly with the right numbers which are commented out
	if (!RenderManager.IsHiDef() && !RenderManager.IsWidescreen())
	{
		ss = 24; //40;
	}
	else
	{
		if (app.GetLocalPlayerCount()>1)
		{
			ss = 13; //28;
		}
		else
		{
			ss = 26; //56;
		}
	}

	glScalef(-ss, ss, ss);
	glRotatef(180, 0, 0, 1);

	float oybr = entityHorse->yBodyRot;
	float oyr = entityHorse->yRot;
	float oxr = entityHorse->xRot;
	float oyhr = entityHorse->yHeadRot;

	D3DXVECTOR3 pointerPosition = m_containerScene->GetCursorScreenPosition();
	//printf("Pointer screen position is x:%f, y:%f, z:%f\n",pointerPosition.x, pointerPosition.y, pointerPosition.z);

	float xd = (matrix._41 + ((bwidth*matrix._11) / 2)) - pointerPosition.x;

	// Need to base Y on head position, not centre of mass
	float yd = (matrix._42 + ((bheight*matrix._22) / 2) - 40) - pointerPosition.y;

	glRotatef(45 + 90, 0, 1, 0);
	Lighting::turnOn();
	glRotatef(-45 - 90, 0, 1, 0);

	glRotatef(-(float)atan(yd / 40.0f) * 20, 1, 0, 0);

	entityHorse->yBodyRot = (float)atan(xd / 40.0f) * 20;
	entityHorse->yRot = (float)atan(xd / 40.0f) * 40;
	entityHorse->xRot = -(float)atan(yd / 40.0f) * 20;
	entityHorse->yHeadRot = entityHorse->yRot;
	//entityHorse->glow = 1;
	glTranslatef(0, entityHorse->heightOffset, 0);
	EntityRenderDispatcher::instance->playerRotY = 180;

	// 4J Stu - Turning on hideGui while we do this stops the name rendering in split-screen
	bool wasHidingGui = pMinecraft->options->hideGui;
	pMinecraft->options->hideGui = true;
	EntityRenderDispatcher::instance->render(entityHorse, 0, 0, 0, 0, 1, false, false);
	pMinecraft->options->hideGui = wasHidingGui;
	//entityHorse->glow = 0;

	entityHorse->yBodyRot = oybr;
	entityHorse->yRot = oyr;
	entityHorse->xRot = oxr;
	entityHorse->yHeadRot = oyhr;
	glPopMatrix();
	Lighting::turnOff();
	glDisable(GL_RESCALE_NORMAL);

	XuiRenderRestoreState(hDC);

	bHandled = TRUE;
#endif
	return S_OK;

}