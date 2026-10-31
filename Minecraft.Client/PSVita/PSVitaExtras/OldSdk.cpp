#include "stdafx.h"
#include "OldSdk.h"

int sceNpBasicGetFriendOnlineStatus(const SceNpId *friendId, SceNpBasicOnlineStatus *status) {
	if (status) *status = SCE_NP_BASIC_ONLINE_STATUS_UNKNOWN;
	return -1;
}

int sceNpBasicGetFriendContextState(const SceNpId *friendId, SceNpBasicFriendContextState *state) {
	if (state) *state = SCE_NP_BASIC_FRIEND_CONTEXT_STATE_UNKNOWN;
	return -1;
}

int sceNpBasicGetGamePresenceOfFriend(const SceNpId *friendId, SceNpBasicGamePresence *presence) {
	if (presence) memset(presence, 0, sizeof(SceNpBasicGamePresence));
	return -1;
}

int sceNpBasicGetFriendListEntryCount(SceSize *count) {
	if (count) *count = 0;
	return 0;
}

int sceNpBasicGetFriendListEntries(unsigned int startIndex, SceNpId *entries, SceSize numEntries, SceSize *retrieved) {
	if (retrieved) *retrieved = 0;
	return 0;
}

int sceNpBasicGetFriendRequestEntryCount(SceSize *count) {
	if (count) *count = 0;
	return 0;
}

int sceNpBasicGetFriendRequestEntries(unsigned int startIndex, SceNpId *entries, SceSize numEntries, SceSize *retrieved) {
	if (retrieved) *retrieved = 0;
	return 0;
}

int sceNpBasicGetRequestedFriendRequestEntryCount(SceSize *count) {
	if (count) *count = 0;
	return 0;
}

int sceNpBasicGetRequestedFriendRequestEntries(unsigned int startIndex, SceNpId *entries, SceSize numEntries, SceSize *retrieved) {
	if (retrieved) *retrieved = 0;
	return 0;
}

SceInt32 sceAppUtilAppEventParseNpBasicJoinablePresence(const SceAppUtilAppEventParam *eventParam, SceAppUtilNpBasicJoinablePresenceParam *param) {
	return -1;
}