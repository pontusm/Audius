// AudiusExt.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "AudiusExt.h"

// Globally shared data segment
#pragma data_seg(".HOOKDATA")
HHOOK g_hook = NULL;
HWND g_hWnd = NULL;
#pragma data_seg()

#pragma comment(linker, "/SECTION:.HOOKDATA,RWS")

HINSTANCE g_hInstance = NULL;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hInstance = hModule;
		break;
		//case DLL_THREAD_ATTACH:
		//case DLL_THREAD_DETACH:
		//case DLL_PROCESS_DETACH:
		//	break;
	}

	return TRUE;
}


LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HSHELL_APPCOMMAND)
	{
		if(g_hWnd != NULL)
		{
			short cmd = GET_APPCOMMAND_LPARAM(lParam);
			switch(cmd)
			{
			case APPCOMMAND_MEDIA_NEXTTRACK:
			case APPCOMMAND_MEDIA_PLAY_PAUSE:
			case APPCOMMAND_MEDIA_PREVIOUSTRACK:
			case APPCOMMAND_MEDIA_STOP:
				SetActiveWindow(g_hWnd);
				//PostMessage(g_hWnd, WM_APPCOMMAND, wParam, lParam);
				SendMessage(g_hWnd, WM_APPCOMMAND, wParam, lParam);
				return TRUE;
			}
		}
	}
	return ( CallNextHookEx(g_hook, nCode, wParam, lParam) );
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION && wParam == WM_KEYDOWN)
	{
		KBDLLHOOKSTRUCT* kbdinfo = (KBDLLHOOKSTRUCT*)lParam;
		if(kbdinfo->vkCode == VK_MEDIA_PLAY_PAUSE)
		{
			SendMessage(g_hWnd, wParam, kbdinfo->vkCode, kbdinfo->scanCode << 4);
			return TRUE;
		}
	}
	return ( CallNextHookEx(g_hook, nCode, wParam, lParam) );
}

// Install keyhook to listen for media key presses
AUDIUSEXT_API bool HookMediaKeys(HWND hWnd)
{
	g_hWnd = hWnd;
	//g_hook = SetWindowsHookEx( WH_SHELL, ShellProc, g_hInstance, NULL);
	g_hook = SetWindowsHookEx( WH_KEYBOARD_LL, LowLevelKeyboardProc, g_hInstance, NULL);
	if(!g_hook)
		return false;

	return true;
}

AUDIUSEXT_API void UnhookMediaKeys()
{
	if(g_hook)
	{
		UnhookWindowsHookEx(g_hook);
		g_hook = NULL;
	}
}
