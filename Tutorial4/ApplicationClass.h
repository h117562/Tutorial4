#ifndef _APPLICATION_CLASS_H_
#define _APPLICATION_CLASS_H_

#include <Windows.h>
#include "D3DClass.h"
#include "ColorShaderClass.h"
#include "CameraClass.h"
#include "ModelClass.h"

class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, bool V_SYNC, bool FullScreen, float ScreenDepth, float ScreenNear, float ScreenWidth, float ScreenHeight);
	void Shutdown();
	bool Frame();

private:
	D3DClass* m_Direct3D;
	ColorShaderClass* m_ColorShader;
	CameraClass* m_Camera;
	ModelClass* m_Model;
};
#endif