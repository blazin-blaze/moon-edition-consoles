#pragma once

#include <np.h>
#include <np/np_basic.h>
#include <apputil.h>
#include <np_toolkit.h>
#include <np_toolkit\friends_interface.h>

typedef enum {
	SCE_NP_BASIC_ONLINE_STATUS_UNKNOWN = 0,
	SCE_NP_BASIC_ONLINE_STATUS_OFFLINE = 1,
	SCE_NP_BASIC_ONLINE_STATUS_AFK = 2,
	SCE_NP_BASIC_ONLINE_STATUS_ONLINE = 3
} SceNpBasicOnlineStatus;

typedef void(*SceNpBasicFriendOnlineStatusEventHandler)(
	const SceNpId *friendId,
	SceNpBasicOnlineStatus status,
	void *userdata
	);

typedef enum {
	SCE_NP_BASIC_FRIEND_CONTEXT_STATE_UNKNOWN = 0,
	SCE_NP_BASIC_FRIEND_CONTEXT_STATE_OUT_OF_CONTEXT = 1,
	SCE_NP_BASIC_FRIEND_CONTEXT_STATE_IN_CONTEXT = 2
} SceNpBasicFriendContextState;

typedef void(*SceNpBasicFriendContextEventHandler)(
	const SceNpId *friendId,
	SceNpBasicFriendContextState state,
	void *userdata
	);
typedef struct SceAppUtilNpBasicJoinablePresenceParam {
	SceNpCommunicationId	commId;
	SceNpId					npId;
} SceAppUtilNpBasicJoinablePresenceParam;

int sceNpBasicGetFriendOnlineStatus(const SceNpId *friendId, SceNpBasicOnlineStatus *status);
int sceNpBasicGetFriendContextState(const SceNpId *friendId, SceNpBasicFriendContextState *state);
int sceNpBasicGetGamePresenceOfFriend(const SceNpId *friendId, SceNpBasicGamePresence *presence);
int sceNpBasicGetFriendListEntryCount(SceSize *count);
int sceNpBasicGetFriendListEntries(unsigned int startIndex, SceNpId *entries, SceSize numEntries, SceSize *retrieved);
int sceNpBasicGetFriendRequestEntryCount(SceSize *count);
int sceNpBasicGetFriendRequestEntries(unsigned int startIndex, SceNpId *entries, SceSize numEntries, SceSize *retrieved);
int sceNpBasicGetRequestedFriendRequestEntryCount(SceSize *count);
int sceNpBasicGetRequestedFriendRequestEntries(unsigned int startIndex, SceNpId *entries, SceSize numEntries, SceSize *retrieved);
SceInt32 sceAppUtilAppEventParseNpBasicJoinablePresence(const SceAppUtilAppEventParam *eventParam, SceAppUtilNpBasicJoinablePresenceParam *param);

#define SCE_APPUTIL_APPEVENT_TYPE_NP_BASIC_JOINABLE_PRESENCE	(3)	