// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the AUDIUSEXT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// AUDIUSEXT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef AUDIUSEXT_EXPORTS
#define AUDIUSEXT_API __declspec(dllexport)
#else
#define AUDIUSEXT_API __declspec(dllimport)
#endif

AUDIUSEXT_API bool HookMediaKeys(HWND hWnd);
AUDIUSEXT_API void UnhookMediaKeys();

//AUDIUSEXT_API LRESULT CALLBACK KbdProc(int nCode, WPARAM wParam, LPARAM lParam);
