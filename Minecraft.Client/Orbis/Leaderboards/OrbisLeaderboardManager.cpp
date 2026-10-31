#include "stdafx.h"

#include "OrbisLeaderboardManager.h"

#include "..\Orbis_App.h"
#include "Orbis\OrbisExtras\ShutdownManager.h"

#include "..\..\Common\Consoles_App.h"
#include "Common\Network\Sony\SQRNetworkManager.h"

#include "..\..\..\Minecraft.World\StringHelpers.h"

#include <cstdlib>

#include <np.h>

LeaderboardManager *LeaderboardManager::m_instance = new OrbisLeaderboardManager(); //Singleton instance of the LeaderboardManager

OrbisLeaderboardManager::OrbisLeaderboardManager() : SonyLeaderboardManager() {}

HRESULT OrbisLeaderboardManager::initialiseScoreUtility() 
{
	return SCE_OK;
}

bool OrbisLeaderboardManager::scoreUtilityAlreadyInitialised(HRESULT hr) 
{ 
	return hr == SCE_NP_COMMUNITY_ERROR_ALREADY_INITIALIZED; 
}

HRESULT OrbisLeaderboardManager::createTitleContext(const SceNpId &npId)
{
	return sceNpScoreCreateNpTitleCtx(0, &npId);
}

HRESULT OrbisLeaderboardManager::destroyTitleContext(int titleContext)
{
	return sceNpScoreDestroyTitleCtx(titleContext);
}

HRESULT OrbisLeaderboardManager::createTransactionContext(int titleContext)
{
	return sceNpScoreCreateTransactionCtx(titleContext);
}

HRESULT OrbisLeaderboardManager::abortTransactionContext(int transactionContext)
{
	return sceNpScoreAbortTransaction(transactionContext);
}

HRESULT OrbisLeaderboardManager::destroyTransactionContext(int transactionContext)
{
	return sceNpScoreDestroyTransactionCtx(transactionContext);
}

HRESULT OrbisLeaderboardManager::getFriendsList(sce::Toolkit::NP::Utilities::Future<sce::Toolkit::NP::FriendsList> &friendsList)
{
	return sce::Toolkit::NP::Friends::Interface::getFriendslist(&friendsList, false);
}

char *OrbisLeaderboardManager::getComment(SceNpScoreComment *comment)
{
	return comment->utf8Comment;
}