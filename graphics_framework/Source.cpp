//--------------------------------------------------------------------------------------
// File: Tutorial07.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "utility/enGraphics.h"
#include "utility/enDefs.h"
/******************************************************/
#include "../include/cDevice.h"// FINISHED
#include "../include/cDeviceContext.h" //FINISHED 
#include "../include/cTexture2D.h"// FINISHED
#include "../include/cRenderTargetView.h"// UNFINISHED
#include "../include/cRenderTarget.h"//UNFINISHED 
#include "../include/cDepthStencilView.h"// UNFINISHED
#include "../include/cVertexShader.h"// UNFINISHED
#include "../include/cInputLayout.h"// UNFINISHED
#include "../include/cPixelShader.h"// UNFINISHED 
#include "../include/cBuffer.h"// FINISHED
#include "../include/cVertexBuffer.h"// FINISHED 
#include "../include/cIndexBuffer.h"// FINISHED 
#include "../include/cConstBuffer.h"// FINISHED 
#include "../include/cSampler.h"// FINISHED 
#include "../include/cViewport.h" // FINISHED
#include "../include/cShaderResourceView.h"//FINISHED
#include "../include/cSwapChain.h"
#include "../include/cModel.h"
#include "../include/imGuiManager.h"
#include "utility/Timer.h"
#include "utility/CustomStructs.h"
#include "cWindow.h"
#include "cApiComponents.h"
#include "cCamera.h"
#include "cCameraManager.h"
#include "actor/cActor.h"
#include "cShaderTarget.h"

/*****************************************************/
#include "cResourceManager.h"
#include "cShaderManager.h"


/*****************************************************/
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
/*****************************************************/
/* FOR LOADING IMAGES FOR directX*/
#if DIRECTX
#include <wrl.h>
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

#else 
#define STB_IMAGE_IMPLEMENTATION
#include "utility/stb_image.h"
#endif // STBI_INCLUDE_STB_IMAGE_H  
/*****************************************************/
#include "utility/HelperFuncs.h"
#include "utility/enHelperTemplates.h" 
// std includes 
#include <memory>
#include <filesystem>

/*****************************************************/
cDepthStencilView my_depthStencilView;
/*****************************************************/

cInputLayout my_vertexInputLayout;
/*****************************************************/

//cVertexBuffer my_vertexBuffer;
//cIndexBuffer my_indexBuffer;

cConstBuffer my_constCameraData;
cConstBuffer my_constProjectionMatrix;
cConstBuffer my_constChangesEveryFrame;
cConstBuffer my_constLightData;
cSampler my_sampler;
cViewport my_viewport;
cShaderResourceView my_shaderResourceView;
std::unique_ptr<cShaderResourceView > my_normalMap = std::make_unique<cShaderResourceView >();
cSwapChain my_swapChain;
imGuiManager my_gui;
Timer my_timer;
/*****************************************************/
cWindow my_window;
std::unique_ptr <cActor> my_actor = std::make_unique<cActor>();
std::unique_ptr<cCameraManager> my_cameraManager = std::make_unique<cCameraManager>();
std::unique_ptr<cShaderTarget> my_shaderTarget = std::make_unique<cShaderTarget>();
/**********************************************************/
cApiComponents my_apiComponent;

std::unique_ptr <cResourceManager> my_resourceManager = std::make_unique<cResourceManager>();
std::unique_ptr <cShaderManager> my_shaderManager = std::make_unique<cShaderManager>();
/*****************************************************/
#include <cassert>
#include <iostream>
//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------

sWindowSize g_windowSizeTracker;

bool g_isInit(false);
bool g_isRunnig(true);

//! this is the path local to the Project directory of the program
const std::filesystem::path g_initPath = std::filesystem::current_path();
sColorf g_vMeshColor;

float g_TransformAmount = 1.0f;
static float g_trackedTime = 0.0f;
static size_t g_selectedShader = 0u;
static bool g_ControlMouse(false);
static bool g_loadNewModel(false);
/********************************************************/
//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------

HRESULT
InitDevice();
/*!this function only exits so i can keep track of when i destroy all obj in
 the actor */
void
DestroyAllComponentsFromActor(cActor &actor);

LRESULT
CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void
Update();

void
Render();

#if OPEN_GL
void
GLkeycallback(GLFWwindow *window, int key, int scancode, int action, int mods);

void
GLMoveMouse(GLFWwindow *window, double xPos, double yPos);

void
GlCloseWindow(GLFWwindow *window);

void
SetCallBackFunctions(cWindow &window);

void
GlResizeBuffer(GLFWwindow* window, int width, int height);


void
GlWindowResize(GLFWwindow* window, int width, int height);
#endif // OPEN_GL

/**
* ENTRY POINT
*
*/
int APIENTRY
wWinMain(HINSTANCE hInstance,
         [[maybe_unused]] HINSTANCE hPrevInstance,
         [[maybe_unused]] LPWSTR    lpCmdLine,
         int       nCmdShow)
{
  HRESULT hr = S_FALSE;
  g_vMeshColor = { 0.7f, 0.7f, 0.7f, 1.0f };
  // alternate way to get an HINSTANCE  
  /* initialized for loading textures */
#if DIRECTX
  /*Initializes the COM library
  this is so I can load images for directX */
  hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
  if (FAILED(hr))
  { return -1; }

#elif OPEN_GL
  /* Initialize the library */
  if (glfwInit() != GLFW_TRUE)
  {
    OutputDebugStringA("Error initializing glfw");
    return -1;
  }
#endif // DIRECTX

  HMODULE Hmodule;

  GetModuleHandleEx(0x00, NULL, &Hmodule);

  /*init's the window*/
  if (my_window.init(WndProc, &Hmodule) == false)
  {
    return -1;
  }

#if OPEN_GL
  glewExperimental = true;
  if (glewInit() != GLEW_OK)
  {
    assert((true == false) && " failed  to start glew");
  }

  SetCallBackFunctions(my_window);
#endif // OPEN_GL

 /* init a console **/
  FILE *standardOutStream = nullptr;
  FILE *standardErrorStream = nullptr;
  if (AllocConsole())
  {
    freopen_s(&standardOutStream, "CONOUT$", "w", stdout);
    std::cout << "output stream" << std::endl;

    freopen_s(&standardErrorStream, "CONOUT$", "w", stderr);
    std::clog << "error stream" << std::endl;
  }

  my_gui.setOpenFileFunction(helper::openFile);

  /* the actor now owns the model */
  my_actor->AddComponents(new cModel());

  if (FAILED(InitDevice()))
  {
    return -1;
  }

  sWindowSize sizeOfWindow = helper::getWindowSize(my_window);

  std::shared_ptr<cDevice> ptr_device = my_resourceManager->getPtrDevice();

  if (my_shaderTarget->init(sizeOfWindow, *ptr_device))
  {
    std::cout << "worked someHow" << "\n\n" << std::endl;
  }

  // Main message loop
  MSG msg = { 0 };
  while (WM_QUIT != msg.message && g_isRunnig)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      Render();
    }
  }

  /********remove all component from my actor **********/
  DestroyAllComponentsFromActor(*my_actor);
  my_actor.release();
  FreeLibrary(my_window.getInstance());

#if DIRECTX

  /*Uninitialized the comm library*/
  CoUninitialize();
#endif // DIRECTX

  /****free the console *****/
  FreeConsole();
  if (standardOutStream)
  {
    if (fclose(standardOutStream) == EOF)
    {
      return -1;
    }
  }

  if (standardErrorStream)
  {
    if (fclose(standardErrorStream) == EOF)
    {
      return -1;
    }
  }

  return ( int )msg.wParam;
}

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT
InitDevice()
{
  bool isSuccesful = true;

  isSuccesful = my_resourceManager->init(my_swapChain,
                                         my_window,
                                         my_apiComponent);

  if (isSuccesful == false)
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return S_FALSE;
  }

  std::shared_ptr<cDevice> ptr_device = my_resourceManager->getPtrDevice();
  std::shared_ptr<cDeviceContext> ptr_deviceContext = my_resourceManager->getPtrDeviceContext();


  HRESULT hr = S_FALSE;

  sWindowSize windowSize = helper::getWindowSize(my_window);

  my_swapChain.setRenderTarget(windowSize.width, windowSize.height, enFormats::fR32G32B32A32);

  my_swapChain.setDepthStencilView(enFormats::depthStencil_format);
  isSuccesful = my_swapChain.InitBuffer();

  assert(("Error with swap-chain getting a buffer " &&  isSuccesful == true));

  // Create a render target view
  isSuccesful = ptr_device->CreateRenderTargetView(my_swapChain.getRenderTarget().getTexture(), my_swapChain.getRenderTargetView());
  assert((isSuccesful == true && "Error with render-target creation"));

  // Create depth stencil texture
  sTextureDescriptor TextureDesc = helper::createDepthStencilDesc(windowSize.width, windowSize.height);
  isSuccesful = ptr_device->CreateTexture2D(TextureDesc, my_swapChain.getDepthStencil());
  assert(isSuccesful == true && "Error with Texture 2d creation ");

  /**********************************************************/

  isSuccesful = my_gui.Init(*ptr_device, *ptr_deviceContext, my_window);
  assert(isSuccesful == true && "problem with initializing  imGui");

 /**********************************************************/
   // set depth-stencil-view
  my_swapChain.setDepthStencilView(enFormats::depthStencil_format);

  isSuccesful = ptr_device->CreateDepthStencilView(my_swapChain.getDepthStencilView());
  assert(isSuccesful == true && "Error with depth-stencil creation");

  ptr_deviceContext->OMSetRenderTargets(&my_swapChain.getRenderTargetView(),
                                        my_swapChain.getDepthStencilView());

    // Setup the viewport
  my_viewport.setViewport(windowSize.width, windowSize.height,
                          0.0f, 1.0f);

  ptr_deviceContext->RSSetViewports(&my_viewport);

  std::filesystem::path shaderPath(g_initPath.parent_path());

#if DIRECTX
  shaderPath += L"\\DxShaders\\";
#elif  OPEN_GL
  shaderPath += L"\\GlShaders\\";
#endif // DIRECTX

#if DIRECTX
  const wchar_t *selectedVertexShader = L"lighting.hlsl";
#elif OPEN_GL
  const wchar_t *selectedVertexShader = L"lighting.vert";
#else 
  const wchar_t *selectedVertexShader = L"tutorial_imporved.vert";
#endif // DIRECTX

  shaderPath += selectedVertexShader;

  std::cout << "path to the vertex shader [" << shaderPath << "]\n";
 // isSuccesful = my_vertexShader.compileShader(shaderPath.generic_string(), "VS", "vs_4_0");

  // Compile the vertex shader
  if (isSuccesful == false)
  {
  #if WIND_OS
    MessageBoxW(NULL,
                EN_CHAR("The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file."),
                EN_CHAR("Error"),
                MB_OK);
    return hr;
  #endif // WIND_OS
  }

  /*********************/

  sShadersManagerDesc managerResource;
  managerResource.vertexShaderVersion = "vs_4_0";
  managerResource.pixelShaderVersion = "ps_4_0";

  managerResource.vertexEntry = "VS";
  managerResource.pixelEntry = "PS";

  managerResource.vertexShaderPath = shaderPath.generic_string();

  shaderPath = g_initPath.parent_path();
#if DIRECTX
  const wchar_t *selectedPixelhader = L"lighting.hlsl";
  shaderPath += L"//DxShaders//";
#elif OPEN_GL
  const wchar_t *selectedPixelhader = L"lighting.frag";
  shaderPath += L"\\GlShaders\\";
#else 
  const wchar_t * selectedPixelhader = L"no shader";
#endif // DIRECTX

  shaderPath += selectedPixelhader;

  managerResource.pixelShaderPath = shaderPath.generic_string();

  std::cout << "path to the pixel shader["<< managerResource.pixelShaderPath << '\n';

  isSuccesful = my_shaderManager->init(*ptr_device,
                                       managerResource);

  assert(isSuccesful == true && "Error with initializing the cShaderManager");

  cPixelShader* ptr_pixelShader = my_shaderManager->getPixelShaderPtr();
  cVertexShader* ptr_vertexShader = my_shaderManager->getVertexShaderPtr();

  ptr_deviceContext->SetShaders(*ptr_vertexShader, *ptr_pixelShader);

  isSuccesful = my_vertexInputLayout.ReadShaderData(*ptr_vertexShader);
  assert(isSuccesful == true && "Error reading the vertex-shader data");

  // Create the input layout
  isSuccesful = ptr_device->CreateInputLayout(my_vertexInputLayout,
                                              *ptr_vertexShader);
  assert(isSuccesful == true && "Error creating Input layout ");

  // Set the input layout
  ptr_deviceContext->IASetInputLayout(my_vertexInputLayout);

  if (isSuccesful == false)
  {
  #if WIND_OS
    MessageBoxW(NULL,
                L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.",
                L"Error",
                MB_OK);

    hr = S_FALSE;
    return hr;
  #endif // WIND_OS
  }

  cModel* ptr_toModel = helper::findModelComponent(*my_actor);
  assert(ptr_toModel != nullptr && "Error component 'model' does NOT exist in current actor");

  ptr_toModel->setModelPath(helper::openFile(my_window));

  isSuccesful = ptr_toModel->LoadModelFromFile(*ptr_device);

  assert(("Error with loading model file" && isSuccesful == true));

  my_constCameraData.init(sizeof(CameraData),
                          1,
                          0);

  my_constCameraData.setIndex(0);


  isSuccesful = ptr_device->CreateConstBuffer(my_constCameraData);
  assert(isSuccesful == true && "Error Creating constant buffer");

  my_constProjectionMatrix.init(sizeof(ProjectionMatrix),
                                1,
                                0);

  my_constProjectionMatrix.setIndex(1);

  isSuccesful = ptr_device->CreateConstBuffer(my_constProjectionMatrix);
  assert(isSuccesful == true && "Error Creating constant buffer");

  my_constChangesEveryFrame.init(sizeof(GlChangeEveryFrame),
                                 1,
                                 0);

  my_constChangesEveryFrame.setIndex(2);


  isSuccesful = ptr_device->CreateConstBuffer(my_constChangesEveryFrame);
  assert(isSuccesful == true && "Error Creating constant buffer");
//***
  my_constLightData.init(sizeof(sLightData), 1, 0);

  my_constLightData.setIndex(3);

  isSuccesful = ptr_device->CreateConstBuffer(my_constLightData);
  assert(isSuccesful == true && "Error Creating constant buffer");

  // Load the Texture
  const wchar_t *SelectedTextureFile = L"\\base_albedo.jpg";
  std::filesystem::path PathToResoure(g_initPath);
  PathToResoure += SelectedTextureFile;

  isSuccesful = my_shaderResourceView.createShaderResourceFromFile(PathToResoure.generic_string(),
                                                                   *ptr_device,
                                                                   *ptr_deviceContext);



  assert(isSuccesful == true && "error with resource creation");

  const wchar_t *NormalMapPath = L"\\base_normal.jpg";
  PathToResoure = g_initPath;
  PathToResoure += NormalMapPath;
  isSuccesful = my_normalMap->createShaderResourceFromFile(PathToResoure.generic_string(),
                                                           *ptr_device,
                                                           *ptr_deviceContext);

  assert(isSuccesful == true && "error with resource creation");

  my_sampler.setDescirption(static_cast< int >(enFilter::Anisotropic),
                            static_cast< int >(enTextureAddress::Wrap), //equivalent to D3D11_TEXTURE_ADDRESS_WRAP
                            static_cast< int >(enTextureAddress::Wrap),
                            static_cast< int >(enTextureAddress::Wrap),
                            static_cast< int >(enComparison::Never),//equivalent to D3D11_COMPARISON_NEVER
                            10);

  isSuccesful = ptr_device->CreateSamplerState(my_sampler);
  assert(isSuccesful == true && "Error with creating sampler state");

  cCamera CameraSecurity;
  CameraSecurity.setEye(0.0f, 10.0f, 20.0f);
  CameraSecurity.setAt(0.0f, 0.0f, 0.0f);
  CameraSecurity.calculateAndSetView();
  CameraSecurity.calculateAndSetPerpective(my_window, 100.0f, 300.1f, 0.01f);

  cCamera perspectiveCamera;
  perspectiveCamera.calculateAndSetView();
  perspectiveCamera.calculateAndSetPerpective(my_window, 70.0f, 500.0f, 0.1f);

  my_cameraManager->pushBackCamera(perspectiveCamera);
  my_cameraManager->pushBackCamera(CameraSecurity);

  CameraData cameraDataBuffer;
  cameraDataBuffer.matrix = my_cameraManager->getViewMatrix().matrix;
  cameraDataBuffer.cameraPos = my_cameraManager->getEye().vector4;
  cameraDataBuffer.viewDir = my_cameraManager->getAt().vector4;


  ptr_deviceContext->UpdateSubresource(&my_constCameraData,
                                       &cameraDataBuffer);

    // Initialize the projection matrix
  ProjectionMatrix cbChangesOnResize;
  cbChangesOnResize.matrix = my_cameraManager->getProjectionMatrix().matrix;

  ptr_deviceContext->UpdateSubresource(&my_constProjectionMatrix,
                                       &cbChangesOnResize);

  GlChangeEveryFrame cbEveryFrame;
  cbEveryFrame.world = glm::mat4(1.0f);
  cbEveryFrame.color = sColorf{ 0.0f,0.f,0.5f,1.0f };

  ptr_deviceContext->UpdateSubresource(&my_constChangesEveryFrame,
                                       &cbEveryFrame);
  //***
  sLightData LightData;
  //std::memset(&LightData, 0, sizeof(LightData));
  LightData.pos.vector4 = { 0.f,0.f,0.f,1.0f };
  LightData.diffuseDir.vector3 = { 1.0f,0.f,0.f };

  LightData.ambientColor = { 0.7f,0.0f,0.0f };

  //LightData.lightIntensity = 0.5f;
  //LightData.ambientIntensity = 0.5f;

  ptr_deviceContext->UpdateSubresource(&my_constLightData,
                                       &LightData);

  g_isInit = true;

  return S_OK;
}

void
DestroyAllComponentsFromActor(cActor & actor)
{
  actor.DestroyAllComponents();
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
  // Update our time
  static float t = 0.0f;
  my_timer.StartTiming();

  cModel* ptr_toModel = helper::findModelComponent(*my_actor);
  std::shared_ptr<cDeviceContext> ptr_deviceContext = my_resourceManager->getPtrDeviceContext();

#if DIRECTX
  if (my_apiComponent.getHardwareVersion() == D3D_DRIVER_TYPE_REFERENCE)
  {
    t += ( float )dx::XM_PI * 0.0125f;
  }
  else
  #endif // DIRECTX
  {
    static DWORD64 dwTimeStart = 0;
    DWORD64 dwTimeCur = GetTickCount64();

    if (dwTimeStart == 0)
      dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;
  }

  static std::vector<cShaderResourceView*> shaderResources =
  {
    &my_shaderResourceView,
    my_normalMap.get(),
    my_shaderTarget->getShaderResourceViewPtr(),
  };

/******** SET TARGET **********/
  ptr_deviceContext->OMSetRenderTargets(&my_shaderTarget->getRenderTargetView(), my_swapChain.getDepthStencilView());

/******** Clear depth**********/
  sColorf color_Mangnolia;
  color_Mangnolia.red = 0.98f;
  color_Mangnolia.green = 0.90f;
  color_Mangnolia.blue = 1.0f;
  color_Mangnolia.alpha = 1.0f;

  ptr_deviceContext->ClearRenderTargetView(my_shaderTarget->getRenderTargetView(), &color_Mangnolia);
  ptr_deviceContext->ClearDepthStencilView(my_swapChain.getDepthStencilView());


    /******** Change camera **********/
  my_cameraManager->switchCamera(1);
  CameraData cameraData;
  cameraData.matrix = my_cameraManager->getViewMatrix().matrix;
  cameraData.viewDir = my_cameraManager->getAt().vector4;
  cameraData.cameraPos = my_cameraManager->getEye().vector4;
  ptr_deviceContext->UpdateSubresource(reinterpret_cast< cBuffer* >(&my_constCameraData),
                                       &cameraData.matrix);

  ProjectionMatrix Proj;
  Proj.matrix = my_cameraManager->getProjectionMatrix().matrix;;
  ptr_deviceContext->UpdateSubresource(reinterpret_cast< cBuffer* >(&my_constProjectionMatrix),
                                       &Proj);

     /************************************************************************************************************************/

    /*setting values for the vertex shader*/
  ptr_deviceContext->VSSetConstantBuffers(my_constCameraData, my_constCameraData.getIndex());
  ptr_deviceContext->VSSetConstantBuffers(my_constProjectionMatrix, my_constProjectionMatrix.getIndex());
  ptr_deviceContext->VSSetConstantBuffers(my_constChangesEveryFrame, my_constChangesEveryFrame.getIndex());
  ptr_deviceContext->VSSetConstantBuffers(my_constLightData, my_constLightData.getIndex());

  /*setting values for the pixel shader */
  ptr_deviceContext->PSSetConstantBuffers(my_constChangesEveryFrame, my_constChangesEveryFrame.getIndex());
  ptr_deviceContext->PSSetConstantBuffers(my_constLightData, my_constLightData.getIndex());
  ptr_deviceContext->PSSetShaderResources(shaderResources);
  ptr_deviceContext->PSSetSamplers(&my_sampler);

  // Modify the color
  g_vMeshColor.red = (sinf(t * 1.0f) + 1.0f) * 0.5f;
  g_vMeshColor.green = (cosf(t * 3.0f) + 1.0f) * 0.5f;
  g_vMeshColor.blue = (sinf(t * 5.0f) + 1.0f) * 0.5f;

  float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha

  /**************************************************************************************************************/

  static std::vector<cConstBuffer *> bufferArray =
  {
    &my_constChangesEveryFrame,
    &my_constProjectionMatrix,
    &my_constCameraData
  };

  /************************************************************************************************************/
  // DRAW ONE 
  /************************************************************************************************************/
  my_actor->DrawAllComponents(*ptr_deviceContext, bufferArray);

  my_actor->update(*ptr_deviceContext);

  ptr_deviceContext->OMSetRenderTargets(&my_swapChain.getRenderTargetView(), my_swapChain.getDepthStencilView());

  ptr_deviceContext->ClearRenderTargetView(my_swapChain.getRenderTargetView());

  ptr_deviceContext->ClearDepthStencilView(my_swapChain.getDepthStencilView());

  /*setting values for the vertex shader*/
  ptr_deviceContext->VSSetConstantBuffers(my_constCameraData, my_constCameraData.getIndex());
  ptr_deviceContext->VSSetConstantBuffers(my_constProjectionMatrix, my_constProjectionMatrix.getIndex());
  ptr_deviceContext->VSSetConstantBuffers(my_constChangesEveryFrame, my_constChangesEveryFrame.getIndex());
  ptr_deviceContext->VSSetConstantBuffers(my_constLightData, my_constLightData.getIndex());

  /*setting values for the pixel shader */
  ptr_deviceContext->PSSetConstantBuffers(my_constChangesEveryFrame, my_constChangesEveryFrame.getIndex());
  ptr_deviceContext->PSSetConstantBuffers(my_constLightData, my_constLightData.getIndex());

  ptr_deviceContext->PSSetShaderResources(shaderResources);
  ptr_deviceContext->PSSetSamplers(&my_sampler);

  my_cameraManager->switchCamera(0);

  cameraData.matrix = my_cameraManager->getViewMatrix().matrix;
  cameraData.viewDir = my_cameraManager->getAt().vector4;
  cameraData.cameraPos = my_cameraManager->getEye().vector4;
  ptr_deviceContext->UpdateSubresource(reinterpret_cast< cBuffer* >(&my_constCameraData),
                                       &cameraData.matrix);


  Proj.matrix = my_cameraManager->getProjectionMatrix().matrix;
  ptr_deviceContext->UpdateSubresource(reinterpret_cast< cBuffer* >(&my_constProjectionMatrix),
                                       &Proj.matrix);
  //***
  sLightData LightData;
  LightData.diffuseDir.vector3 = { 1.0f,cosf(t) ,0.0f };
  LightData.spotDir.vector3 = { 1.0f,cosf(-t) ,0.0f };

  LightData.diffuseColor = { 1.0f , 1.0f,1.0f/* fabsf(sinf(t))*/, 1.0f };
  LightData.specularColor = { 0.0f,0.0f, fabsf(cosf(t)),1.0f };
  LightData.specularIntensity = fabsf(cosf(t));
  // https://rgbcolorcode.com/color/550080
  LightData.ambientColor = { 0.0f,0.0f,0.3f,1.0f };
  LightData.pointColor = { 1.0f,1.0f,0.0f,1.0f };
  //LightData.pointRadius = //sinf(t) *1000.0f;
  //LightData.lightIntensity = 0.5f;
  LightData.ambientIntensity = 0.2f;
  LightData.pos = my_cameraManager->getEye();

  LightData.pos.vector4 = { 0.f,0.f,0.0f,1.0f };
  LightData.specularPower = fabsf(sin(t));
  LightData.spotAlpha = fabsf(sin(t)) * 10.0f;
  LightData.spotBeta = fabsf(sin(t)) * 1.0f;

  ptr_deviceContext->UpdateSubresource(&my_constLightData,
                                       &LightData);

  /************************************************************************************************************/
    //DARW TWO
  /************************************************************************************************************/

  my_actor->update(*ptr_deviceContext);
  my_actor->DrawAllComponents(*ptr_deviceContext, bufferArray);

  my_shaderManager->swichShader(g_selectedShader);
  my_shaderManager->setShader(*ptr_deviceContext);

  my_timer.EndTiming();
  float deltaTime = my_timer.GetResultSeconds();
  g_trackedTime = deltaTime;

  my_gui.beginFrame("Data");
  my_gui.addImage(my_shaderTarget->getShaderResourceView());
  my_gui.addButton("load new model", g_loadNewModel);
  my_gui.beginChildWithFpsCount(deltaTime);
  my_gui.addItemCount("Mesh", ptr_toModel->getMeshCount());
  my_gui.addItemCount("vertices", ptr_toModel->getVertexCount());
  my_gui.addSliderFloat("Transform amount", g_TransformAmount, -7.0f, 7.0f);
  my_gui.addText("\nControls \n"
                 "chose axis with keys 'x' , 'y' , 'z'\n"
                 "do rotation with left and right arrow keys\n"
                 "use the 'u' and 'i' keys to shear the model\n"
                 "use the 'o' and 'p' keys to apply reflection Transform \n"
                 "use the 't' ,'g' , 'h' ,'f' ,'v' and 'n' \n"
                 "keys to apply a translation Transform \n");

  my_gui.endAllChildren();

  my_gui.beginExtraWindow("Shader Switcher");
  my_gui.addCounter(g_selectedShader, "Shader Index");
  my_gui.addText(my_shaderManager->getShaderNameRef());

  my_gui.addItemCount("Shaders ", my_shaderManager->getShaderCount());

  if (!my_shaderManager->swichShader(g_selectedShader))
  {
    sColorf red = { 0.7f,0.1f,0.1f,1.0f };
    my_gui.addText("Error out of range", red);
  }

  my_gui.endAllExtraWindows();
  my_gui.endFrame();
  // Present our back buffer to our front buffer
  my_swapChain.Present(0, 0);
  Update();
}



void
Update()
{
  g_windowSizeTracker.height = my_window.getHeight();
  g_windowSizeTracker.width = my_window.getWidth();

  my_window.update();

  std::shared_ptr<cDevice> ptr_device = my_resourceManager->getPtrDevice();

  if (g_windowSizeTracker.height != my_window.getHeight()
      || g_windowSizeTracker.width != my_window.getWidth())
  {
    std::shared_ptr<cDeviceContext> ptr_deviceContext = my_resourceManager->getPtrDeviceContext();
    // get current position of mouse 
    my_swapChain.Resize(*ptr_device, my_window.getWidth(), my_window.getHeight());

    ptr_deviceContext->OMSetRenderTargets(&my_swapChain.getRenderTargetView(), my_swapChain.getDepthStencilView());

    my_cameraManager->getCurrentCamera()->calculateAndSetPerpective(my_window,
                                                                    my_cameraManager->getCurrentCamera()->getFovDeg(),
                                                                    my_cameraManager->getCurrentCamera()->getFar(),
                                                                    my_cameraManager->getCurrentCamera()->getNear());

    ProjectionMatrix newProjection;
    //CBChangeOnResize newProjection;
    newProjection.matrix = my_cameraManager->getProjectionMatrix().matrix;

    ptr_deviceContext->UpdateSubresource(&my_constProjectionMatrix,
                                         &newProjection);

    my_viewport.setViewport(my_window.getWidth(),
                            my_window.getHeight(),
                            0.0f,
                            1.0f);

    ptr_deviceContext->RSSetViewports(&my_viewport);
  }

  if (g_loadNewModel == true)
  {
    //successful
    bool isSuccessful = helper::loadNewActorModelFromFile(*my_actor, my_window, *ptr_device);
    if (!isSuccessful)
    {
      EN_LOG_ERROR_WITH_CODE(enErrorCode::ActorComponentError);
    }

    g_loadNewModel = false;
  }
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
#if DIRECTX
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    return true;

  PAINTSTRUCT ps;
  HDC hdc;

  sWindowSize currentWindowSize = helper::getWindowSize(my_window);

  POINT centerPoint;
  centerPoint.x = (currentWindowSize.width) * 0.5f;
  centerPoint.y = (currentWindowSize.height) * 0.5f;

  /*this helps select the axis for the transforms
  x = 0
  y = 1
  z = 2*/
  static uint8 chosenAxis{ 0 };

  if (message == WM_PAINT)
  {
    hdc = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
  }
  if (message == WM_KEYDOWN)
  {
    // going forwards 
    helper::handelCameraKeyInput(wParam, *my_cameraManager,
                                 my_window, *my_resourceManager->getPtrDeviceContext(),
                                 &my_constCameraData, &my_constProjectionMatrix
                                 , g_trackedTime);

    helper::handelActorTransforms(*my_actor, chosenAxis, wParam, g_TransformAmount);


    if (wParam == (WPARAM)'X')
    {
      chosenAxis = 0;
    }
    if (wParam == (WPARAM)'Y')
    {
      chosenAxis = 1;
    }
    if (wParam == (WPARAM)'Z')
    {
      chosenAxis = 2;
    }
  }
  if (message == WM_MOUSEMOVE && g_isInit)
  {

    if (wParam & MK_SHIFT)
    {
      CameraData ChangeWithViewMatrix;
      POINT newPoint;
      GetCursorPos(&newPoint);

      sVector3 newVector;
      newVector.vector3 = glm::vec3(newPoint.x, newPoint.y, 0.0f);
      // set position to middle of window 
      SetCursorPos(centerPoint.x, centerPoint.y);
      sVector3 centerVector;
      centerVector.vector3 = { centerPoint.x, centerPoint.y,0.0f };

      sVector3 rotation;
      rotation.vector3 = (newVector.vector3 - centerVector.vector3) * 0.016f;
      rotation.vector3.y = -rotation.vector3.y;

      my_cameraManager->rotateCamera(rotation, my_window);
      ChangeWithViewMatrix.matrix = my_cameraManager->getViewMatrix().matrix;
      //my_deviceContext.UpdateSubresource(
      my_resourceManager->getPtrDeviceContext()->UpdateSubresource(&my_constCameraData, &ChangeWithViewMatrix);
    }
  }
  if (message == WM_DESTROY)
  {
    PostQuitMessage(0);
  }
  else
  {
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
}
#else 
LRESULT CALLBACK
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  return 0;
}

#endif // DIRECTX


#if OPEN_GL

void
SetCallBackFunctions(cWindow &window)
{
  glfwSetInputMode(window.getHandle(), GLFW_STICKY_KEYS, GLFW_TRUE);
  glfwSetCursorPosCallback(window.getHandle(), GLMoveMouse);
  glfwSetWindowSizeCallback(window.getHandle(), GlWindowResize);
  // void (* GLFWwindowclosefun)(GLFWwindow*);
  glfwSetWindowCloseCallback(window.getHandle(), GlCloseWindow);
  glfwSetKeyCallback(window.getHandle(), GLkeycallback);
  glfwSetFramebufferSizeCallback(window.getHandle(), GlResizeBuffer);
}

void
GLkeycallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  static uint8 chosenAxis{ 0 };

  key = helper::convertKeyValue(key);

  helper::handelCameraKeyInput(key, *my_cameraManager,
                               my_window, *my_resourceManager->getPtrDeviceContext(),
                               &my_constCameraData, &my_constProjectionMatrix
                               , g_trackedTime);

  helper::handelActorTransforms(*my_actor, chosenAxis, key, g_TransformAmount);

  if (key == GLFW_KEY_LEFT_SHIFT)
  {
    if (!g_ControlMouse) { g_ControlMouse = true; }
    else if (g_ControlMouse) { g_ControlMouse = false; }
  }

  if (key == 'X')
  {
    chosenAxis = 0;
  }
  if (key == 'Y')
  {
    chosenAxis = 1;
  }
  if (key == 'Z')
  {
    chosenAxis = 2;
  }
}

void
GLMoveMouse(GLFWwindow * window, double xPos, double yPos)
{
  if (g_ControlMouse)
  {

    glm::vec3 currentPos(( float )xPos, ( float )yPos, 0.0f);
    int32 windowSizeX;
    int32 windowSizeY;

    glfwGetWindowSize(window, &windowSizeX, &windowSizeY);

    glm::vec3 centerPos(windowSizeX * 0.5f, windowSizeY * 0.5f, 0.0f);

    glfwSetCursorPos(window, centerPos.x, centerPos.y);

    sVector3 result(currentPos - centerPos);

    result.vector3 *= 0.016f;

    result.vector3.y = -result.vector3.y;

    my_cameraManager->rotateCamera(result, my_window);

    CameraData ChangeWithViewMatrix;
    ChangeWithViewMatrix.matrix = my_cameraManager->getViewMatrix().matrix;

    my_resourceManager->getPtrDeviceContext()->UpdateSubresource(&my_constCameraData, &ChangeWithViewMatrix);
  }
}

void
GlCloseWindow(GLFWwindow * window)
{
  g_isRunnig = false;

  glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void
GlResizeBuffer(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void
GlWindowResize(GLFWwindow* window, int width, int height)
{}

#endif

