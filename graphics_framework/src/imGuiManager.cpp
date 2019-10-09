#include "..\include\imGuiManager.h"
#include "cDevice.h"
#include "cDeviceContext.h"
#include "cWindow.h"
/*******************************/
#include"imgui/imgui.h"
#ifdef DIRECTX
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#elif USING_OPEN_GL
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
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

bool imGuiManager::Init(cDevice & Device, cDeviceContext & DeviceContext, HWND Handle)
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
//

//bool imGuiManager::Init(CWindow & Window)
//{
//	bool isSuccesful = false;
//	isSuccesful = IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& GuiIo = ImGui::GetIO(); 
//	// check for possible error 
//#ifdef USING_OPEN_GL
//	isSuccesful = ImGui_ImplGlfw_InitForOpenGL(Window.GetHandler(), true);
//	isSuccesful = ImGui_ImplOpenGL3_Init(GlslVersion);
//#endif // USING_DIRECTX
//	ImGui::StyleColorsDark();
//
//	return isSuccesful;
//}

void
imGuiManager::setOpenFileFunction(ptr_FileOpenFunc openFileFunc)
{
  mptr_FileFunc = openFileFunc;
}

void 
imGuiManager::MakeBasicWindow(const char * WindowName)
{
  // start a imGui frame
#ifdef DIRECTX
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
#elif USING_OPEN_GL
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
#endif // USING_DIRECTX
  ImGui::NewFrame();
  // creates Imgui Test window 
  ImGui::Begin(WindowName);
  ImGui::End();

  // assemble together Draw Data
  ImGui::Render();
  //this is what actually draws the window ONLY work on DirectX 
#ifdef DIRECTX
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#elif USING_OPEN_GL
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // USING_DIRECTX
}

void
imGuiManager::MakeWindowFpsAndVertexCount(const char * WindowName, float DeltaTime, int VertexCount)
{
#ifdef DIRECTX
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
#elif USING_OPEN_GL
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
#endif // USING_DIRECTX

  // Formating for the FPS counter 
  std::string fpsMessage("FPS ");
  fpsMessage += std::to_string(1 / DeltaTime);

  std::string VertexMessage("Vertex Count # ");
  VertexMessage += std::to_string(VertexCount);

  ImGui::NewFrame();
  // creates Imgui Test window 
  ImGui::Begin(WindowName);

  ImGui::BeginChild("VertexCount", ImVec2(200, 200));
  ImGui::Text(VertexMessage.c_str());
  ImGui::EndChild();

  ImGui::BeginChild("FPS", ImVec2(200, 200));
  ImGui::Text(fpsMessage.c_str());
  ImGui::EndChild();

  ImGui::End();
  // assemble together Draw Data
  ImGui::Render();
  //this is what actually draws the window ONLY work on DirectX 
#ifdef DIRECTX
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#elif USING_OPEN_GL
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // USING_DIRECTX

}

void
imGuiManager::FpsCountWindow(const char * windowName, float DeltaTime)
{
#if DIRECTX
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
#else 
#endif// DIRECTX

  float averageFps = this->calculateAverageFPS(DeltaTime);

  std::string fpsMassage("FPS : ");
  fpsMassage += std::to_string(averageFps);

  this->beginFrame(windowName);

  ImGui::BeginChild("FPS", ImVec2(200, 200));
  ImGui::Text(fpsMassage.c_str());
  ImGui::EndChild();

  this->endFrame();

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

void imGuiManager::MenuForOpenFile(cWindow & window, std::string & PathOfFile, bool & used)
{
  this->beginFrame("Menu");
  if (ImGui::Button("Load model from file "))
  {
    PathOfFile = mptr_FileFunc(window);
    used = true;
  }

  this->endFrame();
}

std::string
imGuiManager::OpenFileFunc(cWindow & window) 
{
 return this->mptr_FileFunc(window);
}

void
imGuiManager::beginFrame(const char * windowName)
{
  ImGui::NewFrame();
  ImGui::Begin(windowName);
}

void
imGuiManager::endFrame()
{
  ImGui::End();
  ImGui::Render();

#ifdef DIRECTX
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#elif USING_OPEN_GL
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // USING_DIRECTX
}

