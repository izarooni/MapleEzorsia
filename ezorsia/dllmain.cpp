// dllmain.cpp : Defines the entry point for the DLL applications
#include "stdafx.h"
#include "NMCO.h"
#include "INIReader.h"

void CreateConsole() {
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
}

bool HookGetModuleFileName() {
	static decltype(&GetModuleFileNameW) _GetModuleFileNameW = &GetModuleFileNameW;

	const decltype(&GetModuleFileNameW) GetModuleFileNameW_Hook = [](HMODULE hModule, LPWSTR lpFileName, DWORD dwSize) -> DWORD {
		auto len = _GetModuleFileNameW(hModule, lpFileName, dwSize);
		// Check to see if the length is invalid (zero)
		if (!len) {
			// Try again without the provided module for a fixed result
			len = _GetModuleFileNameW(nullptr, lpFileName, dwSize);
		}
		return len;
	};

	return Memory::SetHook(true, reinterpret_cast<void**>(&_GetModuleFileNameW), GetModuleFileNameW_Hook);
}

/// <summary>
/// Creates a detour for the User32.dll CreateWindowExA function applying the following changes:
/// 1. Enable the window minimize box
/// </summary>
inline void HookCreateWindowExA() {
	static auto create_window_ex_a = decltype(&CreateWindowExA)(GetProcAddress(LoadLibraryA("USER32"), "CreateWindowExA"));
	static const decltype(&CreateWindowExA) hook = [](DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) -> HWND {
		dwStyle |= WS_MINIMIZEBOX; // enable minimize button
		return create_window_ex_a(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	};

	Memory::SetHook(true, reinterpret_cast<void**>(&create_window_ex_a), hook);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	{
		//CreateConsole();

		INIReader reader("config.ini");
		if (reader.ParseError() == 0) {
			Client::m_nGameWidth = reader.GetInteger("general", "width", 1024);
			Client::m_nGameHeight = reader.GetInteger("general", "height", 768);
		}

		HookCreateWindowExA();
		HookGetModuleFileName();
		std::cout << "GetModuleFileName hook created" << std::endl;
		NMCO::CreateHook();
		std::cout << "NMCO hook initialized" << std::endl;

		std::cout << "Applying resolution " << Client::m_nGameWidth << "x" << Client::m_nGameHeight << std::endl;
		Client::UpdateResolution();
		break;
	}
	default: break;
	case DLL_PROCESS_DETACH:
		ExitProcess(0);
	}
	return TRUE;
}