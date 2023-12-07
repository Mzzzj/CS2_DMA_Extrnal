
#include "Cheats.h"
#include "Utils/Format.hpp"
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <cstdlib>
#include <KnownFolders.h>
#include <ShlObj.h>
namespace fs = std::filesystem;

int main()
{

	Offset::UpdateOffsets();
	auto ProcessStatus = ProcessMgr.Attach("cs2.exe");
	char documentsPath[MAX_PATH];
	if (SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, documentsPath) != S_OK) {
		std::cerr << "Failed to get the Documents folder path." << std::endl;
		goto END;
	}
	MenuConfig::path = documentsPath;
	MenuConfig::path += "/CS2_External";

	if (ProcessStatus != StatusCode::SUCCEED)
	{
		std::cout << "[ERROR] Failed to attach process, StatusCode:" << ProcessStatus << std::endl;
		goto END;
	}



	if (!gGame.InitAddress())
	{
		std::cout << "[ERROR] Failed to call InitAddress()." << std::endl;
		goto END;
	}

	if (fs::exists(MenuConfig::path))
		std::cout << "Config folder connected: " << MenuConfig::path << std::endl;
	else
	{
		if (fs::create_directory(MenuConfig::path))
			std::cout << "Config folder created: " << MenuConfig::path << std::endl;
		else
		{
			std::cerr << "Failed to create the config directory." << std::endl;
			goto END;
		}
	}

	std::cout << "Runing..." << std::endl;

	try
	{
		char moduleIndox[2];
		std::cout << "请选择模式: " << std::endl;
		std::cout << "	[1]单机模式" << std::endl;
		std::cout << "	[2]双击(Moonlight)模式" << std::endl;
		std::cout << "	[3]双击自定义附加窗口模式" << std::endl;
		std::cout << "请输入序号: ";
		std::cin.getline(moduleIndox, 2);
		std::cout << std::atoi(moduleIndox) << std::endl;
		if (std::atoi(moduleIndox) == 1) {
			std::cout << "单机模式 " << std::endl;
			Gui.AttachAnotherWindow("Counter-Strike 2", "SDL_app", Cheats::Run);
		}
		if (std::atoi(moduleIndox) == 2) {
			std::cout << "双击(Moonlight)模式" << std::endl;
			Gui.AttachAnotherWindow("Moonlight", "SDL_app", Cheats::Run);
		}
		if (std::atoi(moduleIndox) == 3) {
			char windowName[256];
			std::cout << "请输入需要附加的窗口名:";
			std::cin.getline(windowName, 256);
			std::cout << "自定义模式-窗口名:" << windowName << std::endl;
			Gui.AttachAnotherWindow(windowName, "SDL_app", Cheats::Run);
		}

	}
	catch (OSImGui::OSException& e)
	{
		std::cout << e.what() << std::endl;
	}


END:
	system("pause");
	return 0;
}