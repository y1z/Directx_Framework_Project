#pragma once 
#include "utility/Grafics_libs.h"
#include <string>

class cDevice;
class cDeviceContext;
class cWindow;

class imGuiManager
{
public:
	imGuiManager();
	~imGuiManager();
private:// typedefs 
  using ptr_FileOpenFunc = std::string(*)(cWindow);
public:
	/*! init the imgui library*/
	bool 
    Init(cDevice &Device, cDeviceContext &DeviceContext, HWND Handle);
	/*! init the imgui library*/
	//bool Init(CWindow &Window);
	//! has nothing just the simplest window you can make 
  void 
    setOpenFileFunction(ptr_FileOpenFunc openFileFunc);

	void 
    MakeBasicWindow(const char* WindowName);
	//! here is a window that contains very specific information 
	void 
    MakeWindowFpsAndVertexCount(const char* WindowName, float DeltaTime, int VertexCount);

  void 
    FpsCountWindow(const char* windowName, float DeltaTime);
  /*! calculates the average fps */
  float
    calculateAverageFPS(float deltaTime);
  
  void
    MenuForOpenFile(cWindow &window, std::string &PathOfFile, bool &used);

std::string 
    OpenFileFunc(cWindow & window);

private:
  //! for beginning new frames  
  void 
    beginFrame(const char*windowName);

  //void
  //  beginChild(const char* Name, float xSize, float ySize);

  //void endChild();
  //! end and renders the frame 
  void
    endFrame();

private:// variables 
  ptr_FileOpenFunc mptr_FileFunc = nullptr;
};

