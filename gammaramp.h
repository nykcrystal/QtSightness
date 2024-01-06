#ifndef GAMMARAMP_H_
#define GAMMARAMP_H_
#include<Windows.h>
class gammaramp
{
protected:
	HMODULE hGDI32;
	HDC hScreenDC;
	typedef BOOL(WINAPI* Type_SetDeviceGammaRamp)(HDC hDC, LPVOID lpRamp);

	Type_SetDeviceGammaRamp pGetDeviceGammaRamp;
	Type_SetDeviceGammaRamp pSetDeviceGammaRamp;

public:

	gammaramp();
	~gammaramp();
	BOOL LoadLibrary();
	void FreeLibrary();
	BOOL LoadLibraryIfNeeded();
	BOOL SetDeviceGammaRamp(HDC hDC, LPVOID lpRamp);
	BOOL GetDeviceGammaRamp(HDC hDC, LPVOID lpRamp);
	BOOL SetBrightness(HDC hDC, WORD wBrightness);
};
#endif
