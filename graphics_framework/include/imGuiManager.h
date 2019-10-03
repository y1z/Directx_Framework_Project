#pragma once

#include "utility/Grafics_libs.h"
#include "cDevice.h"
#include "cDeviceContext.h"

class imGuiManager
{
public:
	imGuiManager();
	~imGuiManager();

	/*! init the imgui library*/
	bool Init(cDevice &Device, cDeviceContext &DeviceContext, HWND Handle);
	/*! init the imgui library*/
	//bool Init(CWindow &Window);
	//! has nothing just the simplest window you can make 
	void MakeBasicWindow(const char* WindowName);
	//! here is a window that contains very specific information 
	void MakeWindowFpsAndVertexCount(const char* WindowName, float DeltaTime, int VertexCount);

  void FpsCountWindow(const char* windowName, float DeltaTime);
private:
  //! for beginning new frames  
  void 
    beginFrame(const char*windowName);
//! end and renders the frame 
  void
    endFrame();

  void
    darwFrame();
};

