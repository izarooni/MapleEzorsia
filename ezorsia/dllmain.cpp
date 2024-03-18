// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "NMCO.h"
#include "Client.h"
#include "Discord.h"
#include "Hooks.h"
#include "INIReader.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	{
		INIReader reader("config.ini");
		if (reader.GetBoolean("general", "debug", false)) {
			Logger::CreateConsole();
		}

		if (reader.ParseError() == 0) Logger::Info("[Main] Config successfully parsed");
		else Logger::Warn("[Main] No config found. Skipping");

		Client::m_nGameWidth = reader.GetInteger("general", "width", 1024);
		Client::m_nGameHeight = reader.GetInteger("general", "height", 768);
		Logger::Info("[Main] Using resolution %dx%d", Client::m_nGameWidth, Client::m_nGameHeight);

		NMCO::CreateHook();
		HookCreateWindowExA();
		HookGetModuleFileName();
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
		Logger::Info("[Main] Updated process and current thread priority");

		Client::UpdateResolution();
		Client::ApplyMods();

		if (reader.GetBoolean("general", "discord_presence", false)) {
			Discord::StartThread();
		}
		break;
	}
	default: break;
	case DLL_PROCESS_DETACH:
		ExitProcess(0);
	}
	return TRUE;
}