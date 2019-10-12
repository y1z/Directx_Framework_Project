#pragma once 
#include "utility/Grafics_libs.h"
#include "utility/enDefs.h"
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
  /*! set a function pointer to a function that 
  opens file explorer 
  \param [in] openFileFunc the function that open file explorer*/
  void 
    setOpenFileFunction(ptr_FileOpenFunc openFileFunc);
/*! calculates and displays the average fps 
\param [in] DeltaTime it's how much time it took to render a frame */
  void 
    FpsCountWindow(float DeltaTime);
  
  void
    MenuForOpenFile(cWindow &window, std::string &PathOfFile, bool &used);

std::string 
    OpenFileFunc(cWindow & window);

  //! for beginning new frames for imGui to draw in
  void 
    beginFrame(const char*windowName);
  /*! create a child in a frame that will count 
  \param [in] childId this is the id imGui uses to identify a child 
  \param [in] itemName this is the name the user give the item 
  \param [in] itemCount how many instances of a certain item exist*/
  void
    beginChildWithItemCount(const char* childId,std::string_view itemName,uint32 itemCount );

  /*! remove all children made by imGuiManager*/
  void
    endAllChildren();
  //! end and renders the frame 
  void
    endFrame();
private:
  /*! calculates the average fps 
  \param [in] deltaTime the amount of time that passed*/
  float
    calculateAverageFPS(float deltaTime);

private:// variables 
  //! keeps track of how many children the frame has 
  uint32 m_childCount;
  //! a pointer to a function that opens a file 
  ptr_FileOpenFunc mptr_FileFunc = nullptr;
};

