#include "CheatsThread.h"
#include <synchapi.h>



VOID UpdateMatrix()
{
	while (true)
	{
		Sleep(1);
		// Update matrix
		if (!ProcessMgr.ReadMemory(gGame.GetMatrixAddress(), gGame.View.Matrix, 64))
			return;
	}
}

VOID LoadLocalEntity()
{
	while (true)
	{
		Sleep(1);
		// LocalEntity
		DWORD64 LocalControllerAddress = 0;
		DWORD64 LocalPawnAddress = 0;

		if (!ProcessMgr.ReadMemory(gGame.GetLocalControllerAddress(), LocalControllerAddress))
			return;
		if (!ProcessMgr.ReadMemory(gGame.GetLocalPawnAddress(), LocalPawnAddress))
			return;

		
		if (!LocalEntityPlayer.UpdateController(LocalControllerAddress))
			return;
		if (!LocalEntityPlayer.UpdatePawn(LocalPawnAddress))
			return;
	}
}
std::vector<CEntity> TempEntityList;
VOID UpdatePlayer(int index) {
	CEntity Entity;
	DWORD64 EntityAddress = 0;
	if (!ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListEntry() + (index + 1) * 0x78, EntityAddress))
		return;
	

	if (!Entity.UpdateController(EntityAddress))
		return;

	if (MenuConfig::TeamCheck && Entity.Controller.TeamID == LocalEntityPlayer.Controller.TeamID)
		return;

	if (!Entity.UpdatePawn(Entity.Pawn.Address))
		return;

	

	if (!Entity.IsAlive())
		return;

	TempEntityList.push_back(Entity);


}
VOID LoadEntity()
{
	while (true)
	{
		Sleep(1);
		std::vector<std::thread> threads;
		TempEntityList.clear();
		for (int i = 0; i < 64; i++)
		{
			threads.push_back(std::thread(UpdatePlayer,i));
		}
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
		EntityList = TempEntityList;
	}
}

VOID UpdateEntityListEntry()
{
	while (true)
	{
		Sleep(1);
		// Update EntityList Entry
		gGame.UpdateEntityListEntry();
	}
}



