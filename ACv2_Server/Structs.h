/*
*  Version: MPL 1.1
*
*  The contents of this file are subject to the Mozilla Public License Version
*  1.1 (the "License"); you may not use this file except in compliance with
*  the License. You may obtain a copy of the License at
*  http://www.mozilla.org/MPL/
*
*  Software distributed under the License is distributed on an "AS IS" basis,
*  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
*  for the specific language governing rights and limitations under the
*  License.
*
*  The Original Code is the YSI 2.0 SA:MP plugin.
*
*  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
*  Portions created by the Initial Developer are Copyright (C) 2008
*  the Initial Developer. All Rights Reserved.
*
*  Contributor(s):
*
*  Peter Beverloo
*  Marcus Bauer
*  MaVe;
*  Sammy91
*  Incognito
*
*  Special Thanks to:
*
*  SA:MP Team past, present and future
*/

#ifndef YSF_STRUCTS_H
#define YSF_STRUCTS_H

#include "Network/BitStream.h"
#include "PacketPriority.h"
#include "CVector.h"
#include "Types.h"

#include "GDK/sampgdk.h"

#include <map>
#include <cstdio>

class CGangZonePool;
#define PAD(a, b)			char a[b]

// SA-MP defines
//#define	MAX_PLAYERS					500
//#define	MAX_OBJECTS					1000
//#define	MAX_GANG_ZONES				1024
//#define	MAX_PICKUPS					4096
//#define	MAX_TEXT_DRAWS				2048
//#define	MAX_PLAYER_TEXT_DRAWS		256
//#define	MAX_VEHICLES				2000
#define	MAX_ATTACHED_OBJECTS		10
//#define	MAX_PLAYER_NAME				24
//#define	MAX_3DTEXT_GLOBAL			1024
//#define	MAX_3DTEXT_PLAYER			1024
//#define	MAX_MENUS					128
//#define	INVALID_PLAYER_ID			65535
#define	MAX_FILTER_SCRIPTS			16
/*
#define WEAPON_BRASSKNUCKLE				1
#define WEAPON_GOLFCLUB					2
#define WEAPON_NITESTICK				3
#define WEAPON_KNIFE					4
#define WEAPON_BAT						5
#define WEAPON_SHOVEL					6
#define WEAPON_POOLSTICK				7
#define WEAPON_KATANA					8
#define WEAPON_CHAINSAW					9
#define WEAPON_DILDO					10
#define WEAPON_DILDO2					11
#define WEAPON_VIBRATOR					12
#define WEAPON_VIBRATOR2				13
#define WEAPON_FLOWER					14
#define WEAPON_CANE						15
#define WEAPON_GRENADE					16
#define WEAPON_TEARGAS					17
#define WEAPON_MOLTOV					18
#define WEAPON_COLT45					22
#define WEAPON_SILENCED					23
#define WEAPON_DEAGLE					24
#define WEAPON_SHOTGUN					25
#define WEAPON_SAWEDOFF					26
#define WEAPON_SHOTGSPA					27
#define WEAPON_UZI						28
#define WEAPON_MP5						29
#define WEAPON_AK47						30
#define WEAPON_M4						31
#define WEAPON_TEC9						32
#define WEAPON_RIFLE					33
#define WEAPON_SNIPER					34
#define WEAPON_ROCKETLAUNCHER			35
#define WEAPON_HEATSEEKER				36
#define WEAPON_FLAMETHROWER				37
#define WEAPON_MINIGUN					38
#define WEAPON_SATCHEL					39
#define WEAPON_BOMB						40
#define WEAPON_SPRAYCAN					41
#define WEAPON_FIREEXTINGUISHER			42
#define WEAPON_CAMERA					43
#define WEAPON_NIGHTVISION              44
#define WEAPON_INFRARED              	45
#define WEAPON_PARACHUTE				46
#define WEAPON_VEHICLE					49
#define WEAPON_DROWN					53
#define WEAPON_COLLISION				54

#define PLAYER_STATE_NONE						(0)
#define PLAYER_STATE_ONFOOT						(1)
#define PLAYER_STATE_DRIVER						(2)
#define PLAYER_STATE_PASSENGER					(3)
#define PLAYER_STATE_EXIT_VEHICLE				(4) // (used internally)
#define PLAYER_STATE_ENTER_VEHICLE_DRIVER		(5) // (used internally)
#define PLAYER_STATE_ENTER_VEHICLE_PASSENGER	(6) // (used internally)
#define PLAYER_STATE_WASTED						(7)
#define PLAYER_STATE_SPAWNED					(8)
#define PLAYER_STATE_SPECTATING					(9)
*/
#define AFK_ACCURACY					1500

typedef int                 INT;
typedef unsigned int        UINT;

enum CON_VARTYPE { CON_VARTYPE_FLOAT, CON_VARTYPE_INT, CON_VARTYPE_BOOL, CON_VARTYPE_STRING };

#define CON_VARFLAG_DEBUG		1
#define CON_VARFLAG_READONLY	2
#define CON_VARFLAG_RULE		4	// Gets sent with a RULES query responce

typedef void(*VARCHANGEFUNC)();

struct ConsoleVariable_s
{
#pragma pack( 1 )
	CON_VARTYPE VarType;
	DWORD VarFlags;
	void* VarPtr;
	VARCHANGEFUNC VarChangeFunc;
};

#pragma pack(push, 1)
typedef struct Text3DLabels_t  // size 0x21
{
	char* text;                                     // + 0x00
	uint color;                         // + 0x04
	float posX;                                     // + 0x08
	float posY;                                     // + 0x0C
	float posZ;                                     // + 0x10
	float drawDistance;                     // + 0x14
	bool useLineOfSight;            // + 0x18
	int virtualWorld;                  // + 0x19
	ushort attachedToPlayerID;    // + 0x1D
	ushort attachedToVehicleID;   // + 0x1F
} C3DText;
#pragma pack(pop)

#pragma pack(push, 1)
class C3DTextPool
{
public:
	BOOL                    m_bIsCreated[MAX_3DTEXT_GLOBAL];
	C3DText					m_TextLabels[MAX_3DTEXT_GLOBAL];
};
#pragma pack(pop)

#pragma pack(push, 1)
class CPlayerText3DLabels // size 0x9802
{
public:
	C3DText				TextLabels[MAX_3DTEXT_PLAYER];	// + 0x0000
	BOOL				isCreated[MAX_3DTEXT_PLAYER];	// + 0x8400
	BYTE				unknown9800[MAX_3DTEXT_PLAYER];				// + 0x9400
	WORD				ownerID;
};
#pragma pack(pop)

// Big thanks for OrMisicL
#pragma pack(push, 1)
class CSyncData
{
public:
	WORD			wUDAnalog;				// 0x0076 - 0x0078
	WORD			wLRAnalog;				// 0x0078 - 0x007A
	WORD			wKeys;					// 0x007A - 0x007C
	CVector			vecPosition;			// 0x007C - 0x0088
	float			fQuaternionAngle;		// 0x0088 - 0x008C
	CVector			vecQuaternion;			// 0x008C - 0x0098
	BYTE			byteHealth;				// 0x0098 - 0x0099
	BYTE			byteArmour;				// 0x0099 - 0x009A
	BYTE			byteWeapon;				// 0x009A - 0x009B
	BYTE			byteSpecialAction;		// 0x009B - 0x009C
	CVector			vecVelocity;			// 0x009C - 0x00A8
	CVector			vecSurfing;				// 0x00A8 - 0x00B4
	WORD			wSurfingInfo;			// 0x00B4 - 0x00B6
	int				iAnimationId;			// 0x00B6 - 0x00BA
	// Size = 68

};
#pragma pack(pop)

#pragma pack(push, 1)
class CAimSyncData
{
public:
	BYTE			byteCameraMode;			// 0x0000 - 0x0001
	CVector			vecFront;				// 0x0001 - 0x000D
	CVector			vecPosition;			// 0x000D - 0x0019
	float			fZAim;					// 0x0019 - 0x001D
	BYTE			byteWeaponState : 6;	// 0x001D - 0x001E
	BYTE			byteCameraZoom : 2;
	BYTE			unk;					// 0x001E - 0x001F
	// Size = 31
};
#pragma pack(pop)

#pragma pack(push, 1)
class CVehicleSyncData
{
public:
	WORD			wVehicleId;				// 0x001F - 0x0021
	WORD			wUDAnalog;				// 0x0021 - 0x0023
	WORD			wLRAnalog;				// 0x0023 - 0x0025
	WORD			wKeys;					// 0x0025 - 0x0027
	float			fQuaternionAngle;		// 0x0027 - 0x002B
	CVector			vecQuaternion;			// 0x002B - 0x0037
	CVector			vecPosition;			// 0x0037 - 0x0043
	CVector			vecVelocity;			// 0x0043 - 0x004F
	float			fHealth;				// 0x004F - 0x0053
	BYTE			bytePlayerHealth;		// 0x0053 - 0x0054
	BYTE			bytePlayerArmour;		// 0x0054 - 0x0055
	BYTE			bytePlayerWeapon;		// 0x0055 - 0x0056
	BYTE			byteSirenState;			// 0x0056 - 0x0057
	BYTE			byteGearState;			// 0x0057 -	0x0058
	WORD			wTrailerID;				// 0x0058 - 0x005A
	union									// 
	{
		WORD			wHydraReactorAngle[2];
		float           fTrainSpeed;
	};
	// Size = 63

};
#pragma pack(pop)

#pragma pack(push, 1)
class CPassengerSyncData
{
public:
	WORD			wVehicleId;				// 0x005E - 0x0060
	BYTE			byteSeatFlags : 7;
	BYTE			byteDriveBy : 1;
	BYTE			bytePlayerWeapon;		// 0x0061 - 0x0062
	BYTE			bytePlayerHealth;		// 0x0062 - 0x0063
	BYTE			bytePlayerArmour;		// 0x0063 - 0x0064
	WORD			wUDAnalog;				// 0x0064 - 0x0066
	WORD			wLRAnalog;				// 0x0066 - 0x0068
	WORD			wKeys;					// 0x0068 - 0x006A
	CVector			vecPosition;			// 0x006A - 0x0076
	// Size = 24
};
#pragma pack(pop)

#pragma pack(push, 1)
class CAttachedObject // sizeof = 52 - 0x34
{
public:
	int				iModelID;
	int				iBoneiD;
	CVector         vecPos;
	CVector         vecRot;
	CVector         vecScale;
	DWORD			dwMaterialColor1;
	DWORD			dwMaterialColor2;
};
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct CTextdraw
{
	union
	{
		BYTE byteFlags;			// 25
		struct
		{
			BYTE byteBox : 1;
			BYTE byteLeft : 1;
			BYTE byteRight : 1;
			BYTE byteCenter : 1;
			BYTE byteProportional : 1;
			BYTE bytePadding : 3;
		};
	};
	float fLetterWidth;			// 1
	float fLetterHeight;		// 5
	DWORD dwLetterColor;		// 9
	float fLineWidth;			// 13
	float fLineHeight;			// 17
	DWORD dwBoxColor;			// 21
	BYTE byteShadow; // 26
	BYTE byteOutline; // 27
	DWORD dwBackgroundColor; // 31
	BYTE byteStyle; // 32
	BYTE byteSelectable; // 32
	float fX; // 33
	float fY; // 37
	WORD dwModelIndex; // 41
	CVector vecRot;
	float fZoom;
	WORD color1;
	WORD color2;
} _CTextdraw;
#pragma pack(pop)

#pragma pack(push, 1)
class CPlayerTextDraw
{
public:
	BOOL				bSlotState[MAX_PLAYER_TEXT_DRAWS];
	CTextdraw		    *TextDraw[MAX_PLAYER_TEXT_DRAWS];
	char				*szFontText[MAX_PLAYER_TEXT_DRAWS];
	bool				bHasText[MAX_PLAYER_TEXT_DRAWS];
};
#pragma pack(pop)

#define MAX_PVARS               800
#define MAX_PVAR_NAME			40
/*
// PVar enumeration
#define PLAYER_VARTYPE_NONE		0
#define PLAYER_VARTYPE_INT		1
#define PLAYER_VARTYPE_STRING	2
#define PLAYER_VARTYPE_FLOAT	3
*/
typedef struct PVAR_DATA_t
{
#pragma pack( 1 )
	char			pVarName[MAX_PVAR_NAME + 1];
	BOOL			isReadOnly;
	int				pVarType;
	int				intValue;
	float			floatValue;
	char*			stringValue;
} PVAR_DATA;

class CPlayerVar
{
#pragma pack( 1 )
public:

	PVAR_DATA       pVars[MAX_PVARS];
	BOOL			isPVarActive[MAX_PVARS];
	int				upperIndex;
};

#pragma pack(push, 1)
typedef struct _PLAYER_SPAWN_INFO // size  46
{
	BYTE byteTeam;
	int iSkin;
	BYTE unk;
	CVector vecPos;
	float fRotation;
	int iSpawnWeapons[3];
	int iSpawnWeaponsAmmo[3];
} PLAYER_SPAWN_INFO;
#pragma pack(pop)

typedef struct SPECTATING_SYNC_t		// size 0x12
{
#pragma pack( 1 )
	WORD		wLeftRightKeysOnSpectating;				// + 0x0000
	WORD		wUpDownKeysOnSpectating;				// + 0x0002
	WORD		wKeysOnSpectating;						// + 0x0004
	CVector		vecPosition;							// + 0x0006
} SPECTATING_SYNC;

typedef struct TRAILER_SYNC_t // size 0x32
{
#pragma pack( 1 )

	WORD wTrailerID;				// + 0x0000
	CVector	vecRoll;				// + 0x0002
	CVector vecDirection;			// + 0x000E
	CVector vecPosition;			// + 0x001A
	CVector vecVelocity;			// + 0x0026
	DWORD pad;
} TRAILER_SYNC;

typedef struct UNOCCUPIED_SYNC_t // size 0x43
{
#pragma pack( 1 )

	WORD wVehicleID;				// + 0x0000
	BYTE bytePassengerSlot;			// + 0x0002
	CVector vecRool;				// + 0x0003
	CVector vecDirection;			// + 0x000F
	CVector vecPosition;			// + 0x001B
	CVector vecVelocity;			// + 0x0027
	CVector vecTurnVelocity;		// + 0x0033
	float fHealth;				// + 0x003F
} UNOCCUPIED_SYNC;

//         ++*(_BYTE *)((unsigned __int16)a2 + v4 + 825); b streamed in
//        ++*(_DWORD *)(v4 + 8949); - maybe streamedin cars count

// something [1024] - 3825 BYTE
// bIsPickupStreamedIn[4096] - 4849 BYTE 
// bStreamedInPickups 8961 DWORD
#pragma pack(push, 1)
class CPlayer
{
public:
	CAimSyncData			aimSyncData;			// 0 - 31
	CVehicleSyncData		vehicleSyncData;		// 31 - 94
	CPassengerSyncData		passengerSyncData;		// 94 - 118
	CSyncData				syncData;				// 118 - 186
	UNOCCUPIED_SYNC			unoccupiedSyncData;		// 186 - 253
	SPECTATING_SYNC			spectatingSyncData;		// 253 - 271
	TRAILER_SYNC			trailerSyncData;		// 271 - 325
	BYTE					byteStreamedIn[MAX_PLAYERS];	// 325 - 825
	BYTE					byteVehicleStreamedIn[MAX_VEHICLES]; // 825 - 2825
	BYTE					byteSomething[1000];			// 2825 - 3825
	BYTE					byte3DTextLabelStreamedIn[1024];// 3825  - 4849
	BYTE					bPickupStreamedIn[MAX_PICKUPS]; // 4849 - 8945
	DWORD					dwStreamedInPlayers;
	DWORD					dwStreamedInVehicles;
	DWORD					unk1;
	DWORD					dwStreamedIn3DTextLabels; // 8957 - 8961
	DWORD					dwStreamedInPickups; // 8961 - 8965
	DWORD					bHasSetVehiclePos; // 8965
	DWORD					dwSetVehiclePosTick; // 8969 - 8981
	CVector					vecVehicleNewPos;	// 8981 - 8985
	DWORD					bHasSpawnInfo;			// 8985
	BOOL					bUpdateKeys;		// 0x231D - 0x2321     // 8164
	CVector					vecPosition;		// 0x2321 - 0x232D
	float					fHealth;			// 0x232D - 0x2331
	float					fArmour;			// 0x2331 - 0x2335
	float					fQuaternion[4];		// 0x2335 - 0x2345
	float					fAngle;				// 0x2345 - 0x2349
	CVector					vecVelocity;		// 0x2349 - 0x2355
	WORD					wUDAnalog;			// 9045
	WORD					wLRAnalog;			// 9047
	DWORD					dwKeys;				// 9049
	DWORD					dwOldKeys;			// 9053
	BOOL					bEditObject;		// 9057
	BOOL					bEditAttachedObject;// 9061
	WORD					wDialogID;			// 9065
	CPlayerTextDraw*		pTextdraw;			// 9067
	CPlayerText3DLabels*	p3DText;			// 9071
	WORD					wPlayerId;			// 9075
	int						iUpdateState;		// 9079
	DWORD					dwLastSyncTick;		// 9083
	CAttachedObject			attachedObject[MAX_ATTACHED_OBJECTS]; // 9087
	BOOL					attachedObjectSlot[MAX_ATTACHED_OBJECTS]; // 9607
	BOOL					bHasAimSync;		// 9645
	BOOL					bHasTrailerSync;	// 9649
	BOOL					bHasUnoccupiedSync;	// 9653
	BYTE					byteState;			// 9657
	CVector					vecCPPos;			// 9658	- 9670
	float					fCPSize;			// 9670 - 9674
	BOOL					bIsInCP;			// 9674 - 9678
	CVector					vecRaceCPPos;		// 9678
	CVector					vecRaceCPNextPos;	// 9690
	BYTE					byteRaceCPType;		// 9702
	float					fRaceCPSize;		// 9703
	BOOL					bIsInRaceCP;		// 9707
	BOOL					bIsInModShop;		// 9711
	WORD					wSkillLevel[11];	// 9715
	int						iLastMarkerUpdate;	// 9737 - 9741
	PLAYER_SPAWN_INFO		spawn;				// 9741 - 9787
	BOOL					bReadyToSpawn;		// 9787 - 9791
	BYTE					byteWantedLevel;	// 9791 - 9792
	BYTE					byteFightingStyle;  // 9792 - 9793
	BYTE					byteSeatId;			// 9793 - 9794
	WORD					wVehicleId;			// 9794 - 9796
	DWORD					iNickNameColor;		// 9796 - 9800
	BOOL					bShowCheckpoint;	// 9800 - 9804
	BOOL					bShowRaceCheckpoint;// 9804 - 9808
	int						iInteriorId;		// 9808 - 9812
	WORD					wWeaponAmmo[12];	// 9812 - 9836
	PAD(pad10, 28);								// 9836 - 9864
	BYTE					byteWeaponId[12];	// 9864 - 0x2688 - 0x2694
	BYTE					byteWeaponID_unknown;// 9876 - 9877
	BYTE					byteCurrentWeapon;	// 9877 - 9878
	WORD					wTargetId;			// 9878 - 9880
	DWORD					dwLastShotTick;		// 9880 - 9884
	BYTE					byteLastShotWeapon;	// 9884 - 9885
	CVector					vecBulletStart;		// 9885	- 9897
	CVector					vecHitTarget;		// 9897 - 9909
	CVector					vecCenterOfHit;		// 9909 - 9921
	WORD					wMayebLastShotPLayer;	// 9921	
	BYTE					wMaybeLastShotType;	// 9923
	BYTE					m_byteTime;			// 9924 - 9925
	float					m_fGameTime;		// 9925 - 9929
	BYTE					byteSpectateType;	// 9929 - 9930
	DWORD					wSpectateID;		// 9930 - 9934
	DWORD					dwLastStreaming;	// 9934 - 9938
	DWORD					dwNPCRecordingType;	// 9938 - 9942
	FILE					*pRecordingFile;	// 9942 - 9946
	DWORD					dwFirstNPCWritingTime; // 9946 - 9950 
	PAD(unused, 9);								// 9950 - 9959
	CPlayerVar*				pPlayerVars;		// 9959 - 9963
	// Size = 9963
};
#pragma pack(pop)

#pragma pack(push, 1)
class CPlayerPool // sizeof = 99520
{
public:
	DWORD				dwVirtualWorld[MAX_PLAYERS];			// 0 - 2000
	DWORD				dwPlayersCount;							// 2000 - 2004
	DWORD				dwlastMarkerUpdate;						// 2004 - 2008
	float				fUpdatePlayerGameTimers;				// 2008 - 2012
	DWORD				dwScore[MAX_PLAYERS];					// 2012 - 4012
	DWORD				dwMoney[MAX_PLAYERS];					// 4012 - 6012
	DWORD				dwDrunkLevel[MAX_PLAYERS];				// 6012 - 8012
	DWORD				dwLastScoreUpdate[MAX_PLAYERS];			// 8012 - 10012
	char				szSerial[MAX_PLAYERS][0x65];			// 10012 - 60512								
	char				szVersion[MAX_PLAYERS][29];				// 60512 - 75012
	BOOL				bIsPlayerConnected[MAX_PLAYERS];		// 75012 - 77012
	CPlayer				*pPlayer[MAX_PLAYERS];					// 77012 - 79012
	char				szName[MAX_PLAYERS][25];				// 79012 - 91512
	BOOL				bIsAnAdmin[MAX_PLAYERS];				// 91512 - 93512
	BOOL				bIsNPC[MAX_PLAYERS];					// 93512 - 95512
};
#pragma pack(pop)

#pragma pack(push, 1)
class CGameMode
{
public:
	AMX m_amx;
	bool m_bInitialised;
	bool m_bSleeping;
	float m_fSleepTime;
};
#pragma pack(pop)

#pragma pack(push, 1)
class CFilterScripts
{
public:
	void* m_pFilterScripts[MAX_FILTER_SCRIPTS];
	char m_szFilterScriptName[MAX_FILTER_SCRIPTS][255];
	int m_iFilterScriptCount;
};
#pragma pack(pop)

#pragma pack(push, 1) // sizeof = 64
typedef struct _MATRIX4X4
{
	CVector right;
	DWORD  flags;
	CVector up;
	float  pad_u;
	CVector at;
	float  pad_a;
	CVector pos;
	float  pad_p;
} MATRIX4X4, *PMATRIX4X4;
#pragma pack(pop)

typedef struct VEHSPAWNS_t // size 36
{
#pragma pack( 1 )
	int			iModelID;
	CVector		vecPos;
	float		fRot;
	int			iColor1;
	int			iColor2;
	int			iRespawnTime;
	int			iInterior;
} tVEHSPAWNS;

typedef struct VEHMOD_INFO // sizeof = 26
{
#pragma pack( 1 )
	BYTE byteModSlots[14];                // + 0x0000
	BYTE bytePaintJob;                    // + 0x000E
	int iColor1;                             // + 0x000F
	int iColor2;                             // + 0x0010
} VEHMOD_INFO;

typedef struct VEHPARAM_EX // sizeof = 7
{
#pragma pack ( 1 )

	BYTE engine;
	BYTE lights;
	BYTE alarm;
	BYTE doors;
	BYTE bonnet;
	BYTE boot;
	BYTE objective;
} VEHPARAM_EX;

#pragma pack(push, 1)
class CVehicle
{
public:
	CVector			vecPosition;		// 0 - 12
	MATRIX4X4		vehMatrix;			// 12 - 76
	CVector			vecVelocity;		// 76 - 88
	CVector			vecTurnSpeed;		// 88 - 100
	WORD			wVehicleID;			// 100 - 102
	WORD			wTrailerID;			// 102 - 104
	WORD			wCabID;				// 104 - 106
	WORD			wLastDriverID;		// 106 - 108
	WORD			vehPassengers[7];	// 108 - 122
	DWORD			vehActive;			// 122 - 126
	DWORD			vehWasted;			// 126 - 130	
	tVEHSPAWNS		customSpawn;		// 130 - 166
	float			fHealth;			// 166 - 170
	uint32_t		vehDoorStatus;		// 170 - 174
	uint32_t		vehPanelStatus;		// 174 - 178
	BYTE			vehLightStatus;		// 178 - 179
	BYTE			vehTireStatus;		// 179 - 180
	bool			bDead;				// 180 - 181
	WORD			wKillerID;			// 181 - 183
	VEHMOD_INFO     vehModInfo;			// 183 - 206
	char			szNumberplate[32 + 1]; // 206
	VEHPARAM_EX		vehParamEx;			// 239 - 246
	BYTE			bDeathNotification; // 246 - 247
	BYTE			bOccupied;			// 247 - 248
	DWORD			vehOccupiedTick;	// 248 - 252
	DWORD			vehRespawnTick;		// 252 - 256
};
#pragma pack(pop)

#pragma pack(push, 1)
class CVehiclePool
{
public:
	BOOL				bVehicleSlotState[MAX_VEHICLES];	// 0 - 8000
	BYTE				modelsUsed[212];					// 8000 - 8212
	int					byteVirtualWorld[MAX_VEHICLES];		// 8212 - 0x3F54
	CVehicle			*pVehicle[MAX_VEHICLES];			// 0x3F54 - 0x5E94
};
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _VECTOR {
	float X, Y, Z;
} VECTOR, *PVECTOR;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _VECTOR2D {
	float X, Y;
} VECTOR2D, *PVECTOR2D;
#pragma pack(pop)

#pragma pack(push, 1)
class CObjectMaterial // sizeof = 212
{
public:
	BYTE byteUsed; // 196
	BYTE byteSlot; // 197
	WORD wModelID; // 198
	DWORD dwMaterialColor; // 200
	char szMaterialTXD[64 + 1]; // 204
	char szMaterialTexture[64];	 // 269 - 333
	BYTE unk;
	BYTE byteMaterialSize;
	char szFont[64]; // 335 - 399
	BYTE vminulla; // 399 - 400
	BYTE byteFontSize; // 400 - 401
	BYTE byteBold; // 401 - 402
	DWORD dwFontColor; // 402 - 406
	DWORD dwBackgroundColor; // 406 - 410
	BYTE byteAlignment;
};
#pragma pack(pop)

#pragma pack(push, 1)
class CObject // sizeof = 3700
{
public:
	WORD				wObjectID;		// 0 - 2
	int					iModel;			// 2 - 6
	BOOL				bActive;		// 6 - 10
	MATRIX4X4			matWorld;		// 10 - 74 - pos - Object position
	CVector				vecRot; 		// 74 - 86 - Object rotation
	MATRIX4X4			matTarget;		// 86 - 150	- 
	BYTE				bIsMoving;		// 150 - 151
	float				fMoveSpeed;		// 151 - 155
	DWORD				unk_4;		// 155 -159
	float				fDrawDistance;	// 159 - 163
	ushort				wAttachedVehicleID;	// 163 - 165
	ushort				wAttachedObjectID;	// 165 - 167
	CVector				vecAttachedOffset;	// 167 - 179
	CVector				vecAttachedRotation;// 179 - 191
	BYTE				byteSomething; // 191 - 192
	DWORD				dwMaterialCount; // 192 - 196
	CObjectMaterial		Material[16]; // 196 - 3636
	char				*szMaterialText[16]; // 3636 - 3652
};
#pragma pack(pop)

#pragma pack(push, 1)
class CObjectPool
{
public:

	BOOL m_bPlayerObjectSlotState[MAX_PLAYERS][MAX_OBJECTS];	// 0 
	BOOL m_bPlayersObject[MAX_OBJECTS];							// 2.000.000
	CObject *m_pPlayerObjects[MAX_PLAYERS][MAX_OBJECTS];		// 2.004.000
	BOOL m_bObjectSlotState[MAX_OBJECTS];
	CObject *m_pObjects[MAX_OBJECTS];
};
#pragma pack(pop)

#pragma pack(push, 1)
class CTextDrawPool
{
public:
	BOOL				bSlotState[MAX_TEXT_DRAWS];
	CTextdraw		    *TextDraw[MAX_TEXT_DRAWS];
	char				*szFontText[MAX_TEXT_DRAWS];
	bool				bHasText[MAX_TEXT_DRAWS][MAX_PLAYERS];
};
#pragma pack(pop)

#pragma pack(push, 1)
class CSAMPGangZonePool
{
public:
	float			fGangZone[MAX_GANG_ZONES][4];
	BOOL			bSlotState[MAX_GANG_ZONES];
};
#pragma pack(pop)

#define MAX_MENU_TEXT_SIZE	32
#define MAX_ITEMS			12
#define MAX_COLUMNS			2

struct t_MenuInteraction
{
	BOOL	Menu;
	BOOL	Row[MAX_ITEMS];
	uchar	unknown[12];
};

class CMenu	// size 0xB84
{
#pragma pack( 1 )
public:

	uchar	menuID;														// + 0x0000
	char	title[MAX_MENU_TEXT_SIZE];								// + 0x0001
	char	items[MAX_ITEMS][MAX_COLUMNS][MAX_MENU_TEXT_SIZE];	// + 0x0021
	char	headers[MAX_COLUMNS][MAX_MENU_TEXT_SIZE];					// + 0x0321
	BOOL	isInitiedForPlayer[MAX_PLAYERS];							// + 0x0361
	t_MenuInteraction interaction;										// + 0x0B31
	float	posX;														// + 0x0B71
	float	posY;														// + 0x0B75
	float	column1Width;												// + 0x0B79
	float	column2Width;												// + 0x0B7D
	uchar	columnsNumber;												// + 0x0B81
	uchar	itemsCount[MAX_COLUMNS];									// + 0x0B82

};

class CMenuPool
{
#pragma pack( 1 )
public:

	CMenu*		menu[MAX_MENUS];			//	+ 0x0000
	BOOL		isCreated[MAX_MENUS];		//	+ 0x0200
	BOOL		playerMenu[MAX_PLAYERS];	//	+ 0x0400
private:
};

#pragma pack(push, 1)
struct ScriptTimer_s // sizeof = 0x11B (283)
{
	char szScriptFunc[255];
	int iTotalTime;
	int iRemainingTime;
	BOOL bRepeating;
	BOOL bKilled;
	AMX* pAMX;
	int iParamCount;
	void* cellParams;
};

typedef std::map<DWORD, ScriptTimer_s*> DwordTimerMap;
#pragma pack(pop)

#pragma pack(push, 1)
class CScriptTimers
{
public:
	DwordTimerMap m_Timers;
	DWORD m_dwTimerCount;
};
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct Pickup_t // size 0x14
{
	int	iModel;
	int	iType;
	CVector		vecPos;
} tPickup;
#pragma pack(pop)

#pragma pack(push, 1)
class CPickupPool_
{
public:
	tPickup			m_Pickup[MAX_PICKUPS];			// + 0x0000
	BOOL			m_bActive[MAX_PICKUPS];			// + 0xA000
	int		m_iWorld[MAX_PICKUPS];		// + 0xC000
	int		m_iPickupCount;
};
#pragma pack(pop)

#define GAMESTATE_STOPPED	 0
#define GAMESTATE_RUNNING	 1
#define GAMESTATE_RESTARTING 2

#pragma pack(push, 1)
typedef struct _BULLET_SYNC_DATA
{
	BYTE byteHitType;
	WORD wHitID;
	CVector vecHitOrigin;
	CVector vecHitTarget;
	CVector vecCenterOfHit;
} BULLET_SYNC_DATA; // by 0x688
#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PlayerID
{
	unsigned int binaryAddress;
	unsigned short port;
};

const PlayerID UNASSIGNED_PLAYER_ID =
{
	0xFFFFFFFF, 0xFFFF
};

#pragma pack(push, 1)
/// All RPC functions have the same parameter list - this structure.
struct RPCParameters
{
	/// The data from the remote system
	unsigned char *input;

	/// How many bits long \a input is
	unsigned int numberOfBitsOfData;

	/// Which system called this RPC
	PlayerID sender;

	/// Which instance of RakPeer (or a derived RakServer or RakClient) got this call
	void *recipient;

	/// You can return values from RPC calls by writing them to this BitStream.
	/// This is only sent back if the RPC call originally passed a BitStream to receive the reply.
	/// If you do so and your send is reliable, it will block until you get a reply or you get disconnected from the system you are sending to, whichever is first.
	/// If your send is not reliable, it will block for triple the ping time, or until you are disconnected, or you get a reply, whichever is first.
	RakNet::BitStream *replyToSender;
};
#pragma pack(pop)

/// \sa NetworkIDGenerator.h
typedef unsigned char UniqueIDType;
typedef unsigned short PlayerIndex;
typedef unsigned char RPCIndex;
const int MAX_RPC_MAP_SIZE = ((RPCIndex)-1) - 1;
const int UNDEFINED_RPC_INDEX = ((RPCIndex)-1);

/// First byte of a network message
typedef unsigned char MessageID;

typedef unsigned int RakNetTime;
typedef long long RakNetTimeNS;

struct RakNetStatisticsStruct
{
	///  Number of Messages in the send Buffer (high, medium, low priority)
	unsigned messageSendBuffer[NUMBER_OF_PRIORITIES];
	///  Number of messages sent (high, medium, low priority)
	unsigned messagesSent[NUMBER_OF_PRIORITIES];
	///  Number of data bits used for user messages
	unsigned messageDataBitsSent[NUMBER_OF_PRIORITIES];
	///  Number of total bits used for user messages, including headers
	unsigned messageTotalBitsSent[NUMBER_OF_PRIORITIES];

	///  Number of packets sent containing only acknowledgements
	unsigned packetsContainingOnlyAcknowlegements;
	///  Number of acknowledgements sent
	unsigned acknowlegementsSent;
	///  Number of acknowledgements waiting to be sent
	unsigned acknowlegementsPending;
	///  Number of acknowledgements bits sent
	unsigned acknowlegementBitsSent;

	///  Number of packets containing only acknowledgements and resends
	unsigned packetsContainingOnlyAcknowlegementsAndResends;

	///  Number of messages resent
	unsigned messageResends;
	///  Number of bits resent of actual data
	unsigned messageDataBitsResent;
	///  Total number of bits resent, including headers
	unsigned messagesTotalBitsResent;
	///  Number of messages waiting for ack (// TODO - rename this)
	unsigned messagesOnResendQueue;

	///  Number of messages not split for sending
	unsigned numberOfUnsplitMessages;
	///  Number of messages split for sending
	unsigned numberOfSplitMessages;
	///  Total number of splits done for sending
	unsigned totalSplits;

	///  Total packets sent
	unsigned packetsSent;

	///  Number of bits added by encryption
	unsigned encryptionBitsSent;
	///  total bits sent
	unsigned totalBitsSent;

	///  Number of sequenced messages arrived out of order
	unsigned sequencedMessagesOutOfOrder;
	///  Number of sequenced messages arrived in order
	unsigned sequencedMessagesInOrder;

	///  Number of ordered messages arrived out of order
	unsigned orderedMessagesOutOfOrder;
	///  Number of ordered messages arrived in order
	unsigned orderedMessagesInOrder;

	///  Packets with a good CRC received
	unsigned packetsReceived;
	///  Packets with a bad CRC received
	unsigned packetsWithBadCRCReceived;
	///  Bits with a good CRC received
	unsigned bitsReceived;
	///  Bits with a bad CRC received
	unsigned bitsWithBadCRCReceived;
	///  Number of acknowledgement messages received for packets we are resending
	unsigned acknowlegementsReceived;
	///  Number of acknowledgement messages received for packets we are not resending
	unsigned duplicateAcknowlegementsReceived;
	///  Number of data messages (anything other than an ack) received that are valid and not duplicate
	unsigned messagesReceived;
	///  Number of data messages (anything other than an ack) received that are invalid
	unsigned invalidMessagesReceived;
	///  Number of data messages (anything other than an ack) received that are duplicate
	unsigned duplicateMessagesReceived;
	///  Number of messages waiting for reassembly
	unsigned messagesWaitingForReassembly;
	///  Number of messages in reliability output queue
	unsigned internalOutputQueueSize;
	///  Current bits per second
	double bitsPerSecond;
	///  connection start time
	RakNetTime connectionStartTime;
};

#pragma pack(push, 1)
/// This represents a user message from another system.
struct Packet
{
	/// Server only - this is the index into the player array that this playerId maps to
	PlayerIndex playerIndex;

	/// The system that send this packet.
	PlayerID playerId;

	/// The length of the data in bytes
	/// \deprecated You should use bitSize.
	unsigned int length;

	/// The length of the data in bits
	unsigned int bitSize;

	/// The data from the sender
	unsigned char* data;

	/// @internal
	/// Indicates whether to delete the data, or to simply delete the packet.
	bool deleteData;
};
#pragma pack(pop)

#ifdef _WIN32
class CCRakServer
{
public:
	~CCRakServer();
	virtual void _0();
	virtual bool Start(unsigned short AllowedPlayers, unsigned int depreciated, int threadSleepTimer, unsigned short port, const char *forceHostAddress = 0) = 0; // 4
	virtual void DisableSecurity(void); // 8
	virtual void SetPassword(const char *_password);	// 12
	virtual bool HasPassword(void);	// 16
	virtual void Disconnect(unsigned int blockDuration, unsigned char orderingChannel = 0);	// 20
	virtual bool Send_ASD(const char *data, const int length, int priority, int reliability, char orderingChannel, PlayerID playerId, bool broadcast); // 24
	virtual bool Send(RakNet::BitStream* parameters, int priority, int reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);	// 28
	virtual void _20(); // 
	virtual void _24();
	virtual Packet* Receive(void);
	virtual void CloseConection(const PlayerID playerId); // 0x2C
	virtual void DeallocatePacket(Packet* asd);
	virtual void SetAllowedPlayers(unsigned short numberAllowed); // 0x34
	virtual unsigned short GetAllowedPlayers(void) const;
	virtual unsigned short GetConnectedPlayers(void);
	virtual void GetPlayerIPFromID(const PlayerID playerId, char returnValue[22], unsigned short *port);
	virtual void PingPlayer(const PlayerID playerId);
	virtual int GetAveragePing(const PlayerID playerId);
	virtual int GetLastPing(const PlayerID playerId); // 4C
	virtual int GetLowestPing(const PlayerID playerId);
	virtual void _54();
	virtual void _58();
	virtual void _5C();
	virtual void _60();
	virtual void _64();
	virtual void _68();
	virtual void _6C();
	virtual void _70();
	virtual void RegisterAsRemoteProcedureCall(int* uniqueID, void(*functionPointer) (RPCParameters *rpcParms));	// 74
	virtual void RegisterClassMemberRPC(int* uniqueID, void *functionPointer);	// 78
	virtual void UnregisterAsRemoteProcedureCall(int* uniqueID);	// 7C
	virtual bool RPC(int* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp); // 80
	virtual void _84();
	virtual void _88();
	virtual void _8C();
	virtual void _90();
	virtual void _94();
	virtual void _98();
	virtual void _9C();
	virtual void _A0();
	virtual void _A4();
	virtual void _A8();
	virtual void _AC();
	virtual void _B0();
	virtual void _B4();
	virtual void _B8();
	virtual void _BC();
	virtual void _C0();
	virtual void _C4();
	virtual void _C8();
	virtual void _CC();
	virtual const char* GetLocalIP(unsigned int index);
	virtual PlayerID GetInternalID(void) const;
	virtual void PushBackPacket(Packet *packet, bool pushAtHead);
	virtual void SetRouterInterface(void *routerInterface);
	virtual void RemoveRouterInterface(void *routerInterface);
	virtual int GetIndexFromPlayerID(PlayerID playerId); // E4
	virtual PlayerID GetPlayerIDFromIndex(int index); // E8
	virtual void AddToBanList(const char *IP, unsigned int milliseconds = 0);
	virtual void RemoveFromBanList(const char *IP);
	virtual void ClearBanList(void);
	virtual bool IsBanned(const char *IP);
	virtual bool IsActivePlayerID(const PlayerID playerId);
	virtual void SetTimeoutTime(RakNetTime timeMS, const PlayerID target);
	virtual bool SetMTUSize(int size);
	virtual int GetMTUSize(void) const;
	virtual void AdvertiseSystem(const char *host, unsigned short remotePort, const char *data, int dataLength);
	virtual RakNetStatisticsStruct * const GetStatistics(const PlayerID playerId);
	virtual void ApplyNetworkSimulator(double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance);
};

#else

class CCRakServer
{
public:
	~CCRakServer();
	virtual bool _0();
	virtual void _4();
	virtual bool Start(unsigned short AllowedPlayers, unsigned int depreciated, int threadSleepTimer, unsigned short port, const char *forceHostAddress = 0) = 0;
	virtual void _C();
	virtual void _10();
	virtual void _14();
	virtual void _18();
	virtual void _1C();
	virtual void _20();
	virtual bool Send(RakNet::BitStream* parameters, int priority, int reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);
	virtual void _28();
	virtual void _2C();
	virtual void _30();
	virtual void _34();
	virtual void SetAllowedPlayers(unsigned short numberAllowed);
	virtual void _3C();
	virtual void _40();
	virtual void _44();
	virtual void _48();
	virtual void _4C();
	virtual int GetLastPing(const PlayerID playerId); // 0x50
	virtual void _54();
	virtual void _58();
	virtual void _5C();
	virtual const char* GetLocalIP(unsigned int index);
	virtual PlayerID GetInternalID(void) const;
	virtual void PushBackPacket(Packet *packet, bool pushAtHead);
	virtual void SetRouterInterface(void *routerInterface);
	virtual void RemoveRouterInterface(void *routerInterface);
	virtual void UnregisterAsRemoteProcedureCall_BAD(int* uniqueID);
	virtual void RegisterAsRemoteProcedureCall(int* uniqueID, void(*functionPointer) (RPCParameters *rpcParms)); // 120
	virtual void RegisterClassMemberRPC(int* uniqueID, void *functionPointer); // 124
	virtual void UnregisterAsRemoteProcedureCall(int* uniqueID); // 128
	virtual void UnregisterAsRemoteProcedureCall_4(int* uniqueID);
	virtual void UnregisterAsRemoteProcedureCall_5(int* uniqueID);
	virtual bool RPC(int* uniqueID, RakNet::BitStream* parameters, int priority, int reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp);
	virtual void _90();
	virtual void _94();
	virtual void _98();
	virtual void _9C();
	virtual void _A0();
	virtual void _A4();
	virtual void _A8();
	virtual void _AC();
	virtual void _B0();
	virtual void _B4();
	virtual void _B8();
	virtual void _BC();
	virtual void _C0();
	virtual void _C4();
	virtual void _C8();
	virtual void _CC();
	virtual void _D0();
	virtual void _D4();
	virtual void _D8();
	virtual void _DC();
	virtual void _E0();
	virtual void _E8();
	virtual int GetIndexFromPlayerID(PlayerID playerId); // EC
	virtual PlayerID GetPlayerIDFromIndex(int index); // F0
	virtual void AddToBanList(const char *IP, unsigned int milliseconds = 0);
	virtual void RemoveFromBanList(const char *IP);
	virtual void ClearBanList(void);
	virtual bool IsBanned(const char *IP);
	virtual bool IsActivePlayerID(const PlayerID playerId);
	virtual void SetTimeoutTime(RakNetTime timeMS, const PlayerID target);
	virtual bool SetMTUSize(int size);
	virtual int GetMTUSize(void) const;
	virtual void AdvertiseSystem(const char *host, unsigned short remotePort, const char *data, int dataLength);
	virtual RakNetStatisticsStruct * const GetStatistics(const PlayerID playerId);
	virtual void ApplyNetworkSimulator(double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance);
};
#endif

#pragma pack(push, 1)
class CNetGame
{
public:
	CGameMode				*pGameModePool;			// 0x0000 - 0x0004
	CFilterScripts			*pFilterScriptPool;		// 0x0004 - 0x0008
	CPlayerPool				*pPlayerPool;			// 0x0008 - 0x000C
	CVehiclePool			*pVehiclePool;			// 0x000C - 0x0010
#ifndef NEW_PICKUP_SYSTEM
	CPickupPool_			*pPickupPool;
#else
	CPickupPool				*pPickupPool;
#endif
	CObjectPool				*pObjectPool;			// 0x0010 - 0x0014
	CMenuPool				*pMenuPool;				// 24
	CTextDrawPool			*pTextDrawPool;			// 28
	C3DTextPool				*p3DTextPool;			// 32
	CGangZonePool			*pGangZonePool;			// 36 - 40
	int						iCurrentGameModeIndex;	// 40 - 44
	int						iCurrentGameModeRepeat;	// 44 - 48
	BOOL					bFirstGameModeLoaded;	// 48 - 52
	BOOL					unkasdasd;				// 52 - 56
	CScriptTimers			*pScriptTimers;			// 56 - 60
	CCRakServer				*pRak;					// 60 - 64
	DWORD					dwSomethingTick;
	DWORD					dwUnk;
	DWORD					dwUnk1;
	BOOL					bLanMode;				// 76
	BOOL					bShowPlayerMarkers;		// 80
	BYTE					byteShowNameTags;		// 84
	BYTE					bTirePopping;			// 85
	BYTE					byteAllowWeapons;		// 86
	BYTE					byteStuntBonus;			// 87
	BYTE					byteWeather;			// 88
	int						iGameState;				// 89
	float					fGravity;				// 93
	int						iDeathDropMoney;		// 97
	BYTE					unklofasz;				// 98
	BYTE					byteMode;				// 102
	BYTE					bLimitGlobalChatRadius;	// 103
	BYTE					bUseCJWalk;				// 104
	float					fGlobalChatRadius;		// 105
	float					fNameTagDrawDistance;	// 109
	BYTE					byteDisableEnterExits;	// 113
	BYTE					byteNameTagLOS;			// 114
	BYTE					bManulVehicleEngineAndLights; // 115
	BYTE					bLimitPlayerMarkers;	// 116
	float					fPlayerMarkesLimit;		// 117
	BOOL					bVehicleFriendlyFire;	// 121
#ifndef _WIN32
	double					dElapsedTime;			// size = 8
#endif
	int						iSpawnsAvailable;		// 125 - 129
	PLAYER_SPAWN_INFO		AvailableSpawns[300];	// 129 - 13929
};
#pragma pack(pop)

// 80 - m_bShowPlayerMarkers
// 84 - m_bShowNameTags
// 86 - m_bAllowWeapons
// 87 - m_bStuntBonus
// 88 - byteWeather
// 89 - m_iGameState
// 93 - fGravity
// 97 - m_iDeathDropMoney
// 102 - byteMode
// 103 - m_bLimitGlobalChatRadius; // limit global player chat to other players within a certain radius
// 104 - m_bUseCJWalk
// 105 - m_fGlobalChatRadius
// 109 - m_fNameTagDrawDistance
// 113 - m_bDisableEnterExits
// 114 - m_byteNameTagLOS
// 115 - m_bManulVehicleEngineAndLights
// 121 - dwVehicleFriendlyFire

enum PacketEnumeration : unsigned char
{
	ID_PLAYER_QUIT = 33,
	ID_PLAYER_SYNC = 212,
	ID_MARKERS_SYNC = 213,
	ID_UNOCCUPIED_SYNC = 214,
	ID_TRAILER_SYNC = 215,
	ID_PASSENGER_SYNC = 216,
	ID_SPECTATOR_SYNC = 217,
	ID_AIM_SYNC = 218,
	ID_VEHICLE_SYNC = 219,
	ID_RCON_COMMAND = 220,
	ID_RCON_RESPONSE = 221,
	ID_WEAPONS_UPDATE = 222,
	ID_STATS_UPDATE = 223,
	ID_BULLET_SYNC = 224,
};

#endif