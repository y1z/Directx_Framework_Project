#pragma once 
#include "utility/Grafics_libs.h"
#include "utility/enDefs.h"
#include "utility/CustomStructs.h"
#include <string>

class cDevice;
class cDeviceContext;
class cWindow;
class cShaderResourceView;

struct sColorf;


class imGuiManager
{
public:
  imGuiManager();
  ~imGuiManager();

private:// typedefs 
  using ptr_FileOpenFunc = std::string(*)(cWindow &);
public:
  /*! init the imgui library*/
  bool
    Init(cDevice &Device, cDeviceContext &DeviceContext, cWindow &window);

  /*! set a function pointer to a function that
  opens file explorer
  \param [in] openFileFunc the function that open file explorer*/
  void
    setOpenFileFunction(ptr_FileOpenFunc openFileFunc);

/*! calculates and displays the average fps
\param [in] DeltaTime it's how much time it took to render a frame */
  void
    beginChildWithFpsCount(float DeltaTime);

  void
    MenuForOpenFile(cWindow &window, std::string &PathOfFile, bool &used);

  /*! calls the open file function pointer */
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
    addItemCountToChild(const char* childId, std::string_view itemName, uint32 itemCount);

  void
    addButton(std::string_view buttonUse, bool &buttonCondtion);
  

  /*! just add a text to the current frame/child */
  void
    addText(std::string_view message, sColorf TextColor = { 1.0f,1.0f,1.0f,1.0f });

  void
    addImage(cShaderResourceView &Resoure, uint32 SizeInX = 100, uint32 SizeInY = 100);

  /*! add a slider that uses a float to control it
  \param Value [in] [out] this is value that the value represent
  \param lowerRange [in] */
  void
    addSliderFloat(std::string_view NameOfValue, float &Value, float lowerRange = 0.0f, float upperRange = 1.0f);
  /*! just creates a child for imGui, other methods add functionality*/
  void
    beginChild(const char* childID);


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

  bool is_initialized{ false };
};

