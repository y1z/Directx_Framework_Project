#include "..\include\imGuiManager.h"
#include "cDevice.h"

#include "cDeviceContext.h"
#include "cWindow.h"
#include <string_view>
using namespace std::string_literals;
/*******************************/
#include"imgui/imgui.h"
#ifdef DIRECTX
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#elif OPEN_GL
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h";
#endif // USING_DIRECTX
//#include "CWindow.h"
/*****************************/

#include <cstdint>

namespace ig = ImGui;

static constexpr uint32_t c_fpsSamplesCount = 60;
//! this variable is for ImGui_ImplOpenGL3_Init
static constexpr const char *GlslVersion = "#version 430 core";
//----------------------------------------
// OpenGL    GLSL      GLSL
// version   version   string
//----------------------------------------
//  2.0       110       "#version 110"
//  2.1       120       "#version 120"
//  3.0       130       "#version 130"
//  3.1       140       "#version 140"
//  3.2       150       "#version 150"
//  3.3       330       "#version 330 core"
//  4.0       400       "#version 400 core"
//  4.1       410       "#version 410 core"
//  4.2       420       "#version 410 core"
//  4.3       430       "#version 430 core"
//  ES 2.0    100       "#version 100"      = WebGL 1.0
//  ES 3.0    300       "#version 300 es"   = WebGL 2.0
//----------------------------------------

imGuiManager::imGuiManager()
  :m_childCount(0),
  mptr_FileFunc(nullptr)
{}

/*!  Highly  Important to free the memory*/
imGuiManager::~imGuiManager()
{

#ifdef DIRECTX
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
#elif USING_OPEN_GL
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
#endif // USING_DIRECTX

  ImGui::DestroyContext();
}

bool
imGuiManager::Init(cDevice & Device, cDeviceContext & DeviceContext, HWND Handle)
{
  bool isSuccesful = false;
  isSuccesful = IMGUI_CHECKVERSION();

  ImGui::CreateContext();
  //this can be used to enable or disable opciones
  ImGuiIO& GuiIo = ImGui::GetIO();
  // check for possible error 
#ifdef DIRECTX
  isSuccesful = ImGui_ImplWin32_Init(Handle);
  isSuccesful = ImGui_ImplDX11_Init(Device.getDevice(), DeviceContext.getDeviceContext());
#endif // USING_DIRECTX
  ImGui::StyleColorsDark();

  return isSuccesful;
}

void
imGuiManager::setOpenFileFunction(ptr_FileOpenFunc openFileFunc)
{
  mptr_FileFunc = openFileFunc;
}


void
imGuiManager::beginChildWithFpsCount(float DeltaTime)
{
  m_childCount++;
  float averageFps = this->calculateAverageFPS(DeltaTime);
  std::string fpsMassage("Average FPS :%f ");
  //fpsMassage += std::to_string(averageFps);

  ImGui::BeginChild("FPS", ImVec2(420, 130));
  ImGui::Text(fpsMassage.c_str(), averageFps);

}

float
imGuiManager::calculateAverageFPS(float deltaTime)
{
  // gets the fps 
  float fps = 1 / deltaTime;
  // contains the results for later to average them out 
  static float fpsTimes[c_fpsSamplesCount] = {0};
  static uint32_t  currenFpsIndex = 0;

  if (currenFpsIndex > c_fpsSamplesCount - 1)
  { currenFpsIndex = 0; }

  fpsTimes[currenFpsIndex] = fps;
  currenFpsIndex++;

  float averageFps = 0.0f;
  for (uint8_t i = 0; i < c_fpsSamplesCount; ++i)
  {
    averageFps += fpsTimes[i];
  }
  return averageFps = averageFps / c_fpsSamplesCount;
}

void 
imGuiManager::MenuForOpenFile(cWindow & window, std::string & PathOfFile, bool & used)
{
  if (ImGui::Button("Load model from file "))
  {
    PathOfFile = mptr_FileFunc(window);
    used = true;
  }

}

std::string
imGuiManager::OpenFileFunc(cWindow & window)
{
  return this->mptr_FileFunc(window);
}

void
imGuiManager::beginFrame(const char * windowName)
{
#if DIRECTX
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
#elif OPEN_GL 
#endif// DIRECTX
  ImGui::NewFrame();
  ImGui::Begin(windowName);
}

void 
imGuiManager::addItemCountToChild(const char* childId, std::string_view itemName, uint32 itemCount) {

  static std::string messageStr = "Item Name : ";
 // get the data in the string view 
  messageStr += itemName.data();
  ig::Text(messageStr.c_str());

  messageStr = "Item Count =  %d";

  ig::TextColored(ImVec4(0.98, 0.88, 0.20, 1.0f), messageStr.c_str() , itemCount);
  // reset the static string 
  messageStr = "Item Name : ";

}

void 
imGuiManager::addText(std::string_view message,sColorf TextColor)
{
  ig::TextColored(ImVec4(TextColor.red, TextColor.green, TextColor.blue, TextColor.alpha),message.data());

}

void imGuiManager::addSliderFloat(std::string_view NameOfValue,float & Value, float lowerRange, float upperRange)
{
  ig::SliderFloat(NameOfValue.data(), &Value, lowerRange, upperRange);
}

void
imGuiManager::beginChild(const char * childID)
{
  m_childCount++;
  ig::BeginChild(childID);
}

void 
imGuiManager::endAllChildren()
{
  for (uint32 i = 0; i < m_childCount; ++i)
  {
    ig::EndChild();
  }

  m_childCount = 0;
}

void
imGuiManager::endFrame()
{
  ImGui::End();
  ImGui::Render();

#ifdef DIRECTX
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#elif OPEN_GL
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // USING_DIRECTX
}

