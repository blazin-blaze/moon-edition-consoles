#pragma once

#include "Common\Leaderboards\SonyLeaderboardManager.h"
#include "Common\Leaderboards\LeaderboardManager.h"

#include "PS3\Passphrase\ps3__np_conf.h"

class PS3LeaderboardManager : public SonyLeaderboardManager
{
public:
	PS3LeaderboardManager();

protected:

	virtual HRESULT initialiseScoreUtility();

	virtual bool scoreUtilityAlreadyInitialised(HRESULT hr);

	virtual HRESULT createTitleContext(const SceNpId &npId);

	virtual HRESULT destroyTitleContext(int titleContext);

	virtual HRESULT createTransactionContext(int titleContext);

	virtual HRESULT abortTransactionContext(int transactionContext);

	virtual HRESULT destroyTransactionContext(int transactionContext);

	virtual HRESULT getFriendsList(const SceNpId &myNpId, SceNpId* &npIds, uint32_t &len);

	virtual char *	getComment(SceNpScoreComment *comment);
};

