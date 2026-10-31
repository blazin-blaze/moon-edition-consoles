#include "stdafx.h"

#include "PS3LeaderboardManager.h"

#include "PS3\PS3_App.h"
#include "PS3\PS3Extras\ShutdownManager.h"

#include "Common\Consoles_App.h"
#include "Common\Network\Sony\SQRNetworkManager.h"

#include "..\..\..\Minecraft.World\StringHelpers.h"

#include <cstdlib>

#include <np.h>

LeaderboardManager *LeaderboardManager::m_instance = new PS3LeaderboardManager(); //Singleton instance of the LeaderboardManager

PS3LeaderboardManager::PS3LeaderboardManager() : SonyLeaderboardManager() {}

HRESULT PS3LeaderboardManager::initialiseScoreUtility() 
{
	return sceNpScoreInit();
}

bool PS3LeaderboardManager::scoreUtilityAlreadyInitialised(HRESULT hr) 
{ 
	return hr == SCE_NP_COMMUNITY_ERROR_ALREADY_INITIALIZED; 
}

HRESULT PS3LeaderboardManager::createTitleContext(const SceNpId &npId)
{ 
	return sceNpScoreCreateTitleCtx(&s_npCommunicationId, &s_npCommunicationPassphrase, &npId); 
}

HRESULT PS3LeaderboardManager::destroyTitleContext(int titleContext)
{
	return sceNpScoreDestroyTitleCtx(titleContext);
}

HRESULT PS3LeaderboardManager::createTransactionContext(int titleContext)
{
	return sceNpLookupCreateTransactionCtx(titleContext);
}

HRESULT PS3LeaderboardManager::abortTransactionContext(int transactionContext)
{
	return sceNpScoreAbortTransaction(transactionContext);
}

HRESULT PS3LeaderboardManager::destroyTransactionContext(int transactionContext)
{
	return sceNpScoreDestroyTransactionCtx(transactionContext);
}

HRESULT PS3LeaderboardManager::getFriendsList(const SceNpId &myNpId, SceNpId* &npIds, uint32_t &len)
{
	EFilterMode filterMode = m_eFilterMode;
	m_eFilterMode = eFM_Friends;

	HRESULT ret = fillByIdsQuery(myNpId, npIds, len);

	m_eFilterMode = filterMode;

	return ret;
}

char *PS3LeaderboardManager::getComment(SceNpScoreComment *comment)
{
	return comment->data;
}