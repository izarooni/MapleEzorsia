#pragma once
#include "stdafx.h"
#include <Unknwn.h>

static void GetMonitorDimensions(int& width, int& height) {
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	width = desktop.right;
	height = desktop.bottom;
}

static void HookGetModuleFileName() {
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

	Memory::SetHook(true, reinterpret_cast<void**>(&_GetModuleFileNameW), GetModuleFileNameW_Hook);
	Logger::Info("[Main] GetModuleFileName hook created");
}

/// <summary>
/// Creates a detour for the User32.dll CreateWindowExA function applying the following changes:
/// 1. Enable the window minimize box
/// </summary>
static void HookCreateWindowExA() {
	static auto create_window_ex_a = decltype(&CreateWindowExA)(GetProcAddress(LoadLibraryA("USER32"), "CreateWindowExA"));
	static const decltype(&CreateWindowExA) hook = [](DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) -> HWND {

		lpWindowName = "MapleStory";
		dwStyle |= WS_MINIMIZEBOX; // enable minimize button

		// center the window on the screen
		int screenWidth, screenHeight;
		GetMonitorDimensions(screenWidth, screenHeight);
		x = screenWidth / 2 - nWidth / 2;
		y = screenHeight / 2 - nHeight / 2;

		return create_window_ex_a(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
		};

	Memory::SetHook(true, reinterpret_cast<void**>(&create_window_ex_a), hook);
	Logger::Info("[Main] CreateWindowExA hook created");
}