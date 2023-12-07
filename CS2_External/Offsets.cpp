#include "Offsets.h"
DWORD64 SearchOffsets(std::string Signature, DWORD64 ModuleAddress)
{
	std::vector<DWORD64> TempAddressList;
	DWORD64 Address = 0;
	DWORD Offsets = 0;

	TempAddressList = ProcessMgr.SearchMemory(Signature, ModuleAddress, ModuleAddress + 0x4000000);
	
	if (TempAddressList.size() <= 0)
		return 0;

	if (!ProcessMgr.ReadMemory<DWORD>(TempAddressList.at(0) + 3, Offsets))
		return 0;

	Address = TempAddressList.at(0) + Offsets + 7;
	return Address;
}

bool Offset::UpdateOffsets()
{
	
    std::ifstream offsetsFile("offsets.json");
    std::ifstream clientFile("client.dll.json");
	Json::CharReaderBuilder charReaderBuilder;
	Json::Value offsetsRoot;
	Json::Value clientRoot;
	Json::Reader reader;

	Json::parseFromStream(charReaderBuilder, offsetsFile, &offsetsRoot, NULL);
	Json::parseFromStream(charReaderBuilder, clientFile, &clientRoot, NULL);

	Json::Value valueData;
	Json::Value value;
	reader.parse(offsetsRoot["client_dll"].toStyledString(), valueData, false);
	reader.parse(valueData["data"].toStyledString(), valueData, false);

	reader.parse(valueData["dwEntityList"].toStyledString(), value, false);
	Offset::EntityList = stoi(value["value"].asString());

	reader.parse(valueData["dwViewMatrix"].toStyledString(), value, false);
	Offset::Matrix = stoi(value["value"].asString());

	reader.parse(valueData["dwViewAngles"].toStyledString(), value, false);
	Offset::ViewAngle = stoi(value["value"].asString());

	reader.parse(valueData["dwLocalPlayerController"].toStyledString(), value, false);
	Offset::LocalPlayerController = stoi(value["value"].asString());

	reader.parse(valueData["dwLocalPlayerPawn"].toStyledString(), value, false);
	Offset::LocalPlayerPawn = stoi(value["value"].asString());

	reader.parse(valueData["dwForceJump"].toStyledString(), value, false);
	Offset::ForceJump = stoi(value["value"].asString());

	reader.parse(valueData["dwGlobalVars"].toStyledString(), value, false);
	Offset::GlobalVars = stoi(value["value"].asString());


	Json::Value C_BaseEntity;
	Json::Value CCSPlayerController;
	Json::Value CBasePlayerController;
	Json::Value C_BasePlayerPawn;
	Json::Value C_CSPlayerPawnBase;
	Json::Value CPlayer_CameraServices;
	Json::Value C_CSPlayerPawn;
	Json::Value CCSPlayerBase_CameraServices;
	Json::Value EntitySpottedState_t;

	reader.parse(clientRoot["C_BaseEntity"].toStyledString(), C_BaseEntity, false);
	reader.parse(C_BaseEntity["data"].toStyledString(), C_BaseEntity, false);

	reader.parse(clientRoot["CCSPlayerController"].toStyledString(), CCSPlayerController, false);
	reader.parse(CCSPlayerController["data"].toStyledString(), CCSPlayerController, false);

	reader.parse(clientRoot["CBasePlayerController"].toStyledString(), CBasePlayerController, false);
	reader.parse(CBasePlayerController["data"].toStyledString(), CBasePlayerController, false);

	reader.parse(clientRoot["C_BasePlayerPawn"].toStyledString(), C_BasePlayerPawn, false);
	reader.parse(C_BasePlayerPawn["data"].toStyledString(), C_BasePlayerPawn, false);

	reader.parse(clientRoot["C_CSPlayerPawnBase"].toStyledString(), C_CSPlayerPawnBase, false);
	reader.parse(C_CSPlayerPawnBase["data"].toStyledString(), C_CSPlayerPawnBase, false);

	reader.parse(clientRoot["CPlayer_CameraServices"].toStyledString(), CPlayer_CameraServices, false);
	reader.parse(CPlayer_CameraServices["data"].toStyledString(), CPlayer_CameraServices, false);

	reader.parse(clientRoot["C_CSPlayerPawn"].toStyledString(), C_CSPlayerPawn, false);
	reader.parse(C_CSPlayerPawn["data"].toStyledString(), C_CSPlayerPawn, false);

	reader.parse(clientRoot["CCSPlayerBase_CameraServices"].toStyledString(), CCSPlayerBase_CameraServices, false);
	reader.parse(CCSPlayerBase_CameraServices["data"].toStyledString(), CCSPlayerBase_CameraServices, false);

	reader.parse(clientRoot["EntitySpottedState_t"].toStyledString(), EntitySpottedState_t, false);
	reader.parse(EntitySpottedState_t["data"].toStyledString(), EntitySpottedState_t, false);


	reader.parse(C_BaseEntity["m_iHealth"].toStyledString(), valueData, false);
	
	Offset::Health = stoi(valueData["value"].asString());

	reader.parse(C_BaseEntity["m_iTeamNum"].toStyledString(), valueData, false);
	Offset::TeamID = stoi(valueData["value"].asString());
	reader.parse(CCSPlayerController["m_bPawnIsAlive"].toStyledString(), valueData, false);
	Offset::IsAlive = stoi(valueData["value"].asString());

	reader.parse(CCSPlayerController["m_hPlayerPawn"].toStyledString(), valueData, false);
	Offset::PlayerPawn = stoi(valueData["value"].asString());

	reader.parse(CBasePlayerController["m_iszPlayerName"].toStyledString(), valueData, false);
	Offset::iszPlayerName = stoi(valueData["value"].asString());

	reader.parse(C_BasePlayerPawn["m_vOldOrigin"].toStyledString(), valueData, false);
	Offset::Pos = stoi(valueData["value"].asString());

	reader.parse(C_BaseEntity["m_iMaxHealth"].toStyledString(), valueData, false);
	Offset::MaxHealth = stoi(valueData["value"].asString());

	reader.parse(C_BaseEntity["m_iHealth"].toStyledString(), valueData, false);
	Offset::CurrentHealth = stoi(valueData["value"].asString());
	reader.parse(C_BaseEntity["m_pGameSceneNode"].toStyledString(), valueData, false);
	Offset::GameSceneNode = stoi(valueData["value"].asString());

	reader.parse(CPlayer_CameraServices["m_fOverrideFogEnd"].toStyledString(), valueData, false);
	Offset::BoneArray = stoi(valueData["value"].asString());

	reader.parse(C_CSPlayerPawnBase["m_angEyeAngles"].toStyledString(), valueData, false);
	Offset::angEyeAngles = stoi(valueData["value"].asString());

	reader.parse(C_CSPlayerPawnBase["m_vecLastClipCameraPos"].toStyledString(), valueData, false);
	Offset::vecLastClipCameraPos = stoi(valueData["value"].asString());

	reader.parse(C_CSPlayerPawnBase["m_pClippingWeapon"].toStyledString(), valueData, false);
	Offset::pClippingWeapon = stoi(valueData["value"].asString());

	reader.parse(C_CSPlayerPawnBase["m_iShotsFired"].toStyledString(), valueData, false);
	Offset::iShotsFired = stoi(valueData["value"].asString());

	reader.parse(C_CSPlayerPawnBase["m_flFlashDuration"].toStyledString(), valueData, false);
	Offset::flFlashDuration = stoi(valueData["value"].asString());

	reader.parse(C_CSPlayerPawn["m_aimPunchAngle"].toStyledString(), valueData, false);
	Offset::aimPunchAngle = stoi(valueData["value"].asString());

	reader.parse(C_CSPlayerPawn["m_aimPunchCache"].toStyledString(), valueData, false);
	Offset::aimPunchCache = stoi(valueData["value"].asString());

	reader.parse(C_CSPlayerPawnBase["m_iIDEntIndex"].toStyledString(), valueData, false);
	Offset::iIDEntIndex = stoi(valueData["value"].asString());

	reader.parse(C_BaseEntity["m_iTeamNum"].toStyledString(), valueData, false);
	Offset::iTeamNum = stoi(valueData["value"].asString());

	reader.parse(C_BasePlayerPawn["m_pCameraServices"].toStyledString(), valueData, false);
	Offset::CameraServices = stoi(valueData["value"].asString());

	reader.parse(CCSPlayerBase_CameraServices["m_iFOVStart"].toStyledString(), valueData, false);
	Offset::iFovStart = stoi(valueData["value"].asString());

	reader.parse(C_BaseEntity["m_fFlags"].toStyledString(), valueData, false);
	Offset::fFlags = stoi(valueData["value"].asString());

	reader.parse(C_CSPlayerPawnBase["m_entitySpottedState"].toStyledString(), valueData, false);
	int m_entitySpottedState = stoi(valueData["value"].asString());

	reader.parse(EntitySpottedState_t["m_bSpottedByMask"].toStyledString(), valueData, false);
	int m_bSpottedByMask = stoi(valueData["value"].asString());

	Offset::bSpottedByMask = m_entitySpottedState + m_bSpottedByMask;
	offsetsFile.close();
	clientFile.close();
	return true;
}