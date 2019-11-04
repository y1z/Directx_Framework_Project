//--------------------------------------------------------------------------------------
// File: Tutorial07.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "utility/enGraphics.h"
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
#include "enum_headers/enFormatEnums.h" 
#include "enum_headers/enumTextureAddress.h"
#include "enum_headers/enumFilter.h"
#include "enum_headers/enumComparasion.h"
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

//#ifndef STBI_INCLUDE_STB_IMAGE_H  &&  !STB_IMAGE_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#include "utility/stb_image.h"

//#endif // STB_IMAGE_IMPLEMENTATION

#endif // STBI_INCLUDE_STB_IMAGE_H  
/*****************************************************/
#if DIRECTX
#include "../include/directx_structs.h"
#endif // DIRECTX
/*****************************************************/
#include "utility/HelperFuncs.h"
#include "utility/enHelperTemplates.h" 
#include <memory>
#include <filesystem>

/*****************************************************/
cDevice my_device;
cDeviceContext my_deviceContext;
/*****************************************************/
cDepthStencilView my_depthStencilView;
/*****************************************************/
cVertexShader my_vertexShader;
cPixelShader my_pixelShader;
cInputLayout my_vertexInputLayout;
/*****************************************************/

cVertexBuffer my_vertexBuffer;
cIndexBuffer my_indexBuffer;
cConstBuffer my_constViewMatrix;
cConstBuffer my_constProjectionMatrix;
cConstBuffer my_constChangesEveryFrame;
cSampler my_sampler;
cViewport my_viewport;
cShaderResourceView my_shaderResourceView;
cSwapChain my_swapChain;
imGuiManager my_gui;
Timer my_timer;
/*****************************************************/
cWindow my_window;
std::unique_ptr <cActor>my_actor = std::make_unique<cActor>();
std::unique_ptr <cActor>my_XArrow = std::make_unique<cActor>();
std::unique_ptr <cActor>my_YArrow = std::make_unique<cActor>();
std::unique_ptr <cActor>my_ZArrow = std::make_unique<cActor>();
std::unique_ptr<cCameraManager>my_cameraManager = std::make_unique<cCameraManager>();
std::unique_ptr <cActor> my_tornado = std::make_unique<cActor>();
std::unique_ptr<cShaderTarget>  my_shaderTarget = std::make_unique<cShaderTarget>();
/**********************************************************/
cApiComponents my_apiComponent;
/*****************************************************/
#include <cassert>
#include <iostream>
//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------

sMatrix4x4 g_World;
sMatrix4x4 g_View;
sMatrix4x4 g_Projection;
sWindowSize g_windowSizeTracker;
bool g_isInit(false);
bool g_isRunnig(true);
//! this is the path local to the solution of the program
const std::filesystem::path g_initPath = std::filesystem::current_path();
sColorf g_vMeshColor;

float g_TransformAmount = 1.0f;
static float g_trackedTime = 0.0f;
static bool g_ControlMouse(false);
static bool g_loadNewModel(false);
/********************************************************/
//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------

HRESULT InitDevice();
/*!this function only exits so i can keep track of when i destroy all obj in
 the actor */
void DestroyAllComponentsFromActor(cActor &actor);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//std::string ModelSelectMenu(cWindow &window);

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

  //my_XArrow->AddComponents(new cModel());
  //my_YArrow->AddComponents(new cModel());
  //my_ZArrow->AddComponents(new cModel());

 /**init a console **/
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

  //static sVertexPosTex Pos0;
  //Pos0.pos = glm::vec4(-1.0, -1.0, 0.0, 1.0);
  //static sVertexPosTex Pos1;
  //Pos1.pos = glm::vec4(1.0, -1.0, 0.0, 1.0);
  //static sVertexPosTex Pos2;
  //Pos2.pos = glm::vec4(-1.0, 1.0, 0.0, 1.0);

  //static uint16 indice[] = { 0,2,1 };

  //static sVertexPosTex Triangle[] =
  //{
  //  {Pos0},
  //  {Pos1},
  //  {Pos2}
  //};
  //glGenBuffers(1, &g_tempVertBuffer);
  //glBindBuffer(GL_ARRAY_BUFFER, g_tempVertBuffer);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(sVertexPosTex) * 3, Triangle, GL_STATIC_DRAW);
  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  //glGenBuffers(1, &g_tempIndexBuffer);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_tempVertBuffer);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * 3, indice, GL_STATIC_DRAW);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if (FAILED(InitDevice()))
  {
    return -1;
  }

  sWindowSize sizeOfWindow = helper::getWindowSize(my_window);

  if (my_shaderTarget->init(sizeOfWindow, my_device))
  {
    std::cout << "worked someHow";
  }


//  my_tornado->AddComponents(helper::createHelicoid(0.1f, 5.0f, 0.0f, 7.1f, 20, my_device));

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
  helper::CreateDeviceAndSwapchain(my_device, my_deviceContext,
                                   my_swapChain, my_window,
                                   my_apiComponent);

  HRESULT hr = S_FALSE;
  bool isSuccesful = true;

  sWindowSize windowSize = helper::getWindowSize(my_window);

  my_swapChain.setRenderTarget(windowSize.width, windowSize.height, enFormats::fR32G32B32A32);

  my_swapChain.setDepthStencilView(enFormats::depthStencil_format);
  isSuccesful = my_swapChain.InitBuffer();
  //  isSuccesful = my_swapChain.GetBuffer(my_renderTarget.getTexture(), 0);
  assert(("Error with swap-chain getting a buffer " &&  isSuccesful == true));

  // Create a render target view
  isSuccesful = my_device.CreateRenderTargetView(my_swapChain.getRenderTarget().getTexture(), my_swapChain.getRenderTargetView());
  assert((isSuccesful == true && "Error with render-target creation"));
  // Create depth stencil texture

  sTextureDescriptor TextureDesc = helper::createDepthStencilDesc(windowSize.width, windowSize.height);
  isSuccesful = my_device.CreateTexture2D(TextureDesc, my_swapChain.getDepthStencil());
  assert(isSuccesful == true && "Error with Texture 2d creation ");

  /**********************************************************/

  isSuccesful = my_gui.Init(my_device, my_deviceContext, my_window);
  assert(isSuccesful == true && "problem with initializing  imGui");

 /**********************************************************/
   // set depth-stencil-view
  my_swapChain.setDepthStencilView(enFormats::depthStencil_format);

  isSuccesful = my_device.CreateDepthStencilView(my_swapChain.getDepthStencilView());
  assert(isSuccesful == true && "Error with depth-stencil creation");

  my_deviceContext.OMSetRenderTargets(&my_swapChain.getRenderTargetView(),
                                      my_swapChain.getDepthStencilView());

  // Setup the viewport
  my_viewport.setViewport(windowSize.width, windowSize.height,
                          0.0f, 1.0f);

  my_deviceContext.RSSetViewports(&my_viewport);

  std::filesystem::path shaderPath(g_initPath);

#if DIRECTX
  shaderPath += L"//dx_shaders//";
#elif  OPEN_GL
  shaderPath += L"//gl_shaders//";
#endif // DIRECTX

#if DIRECTX
  const wchar_t *selectedVertexShader = L"Tutorial07.fx";
#elif OPEN_GL
  const wchar_t *selectedVertexShader = L"tutorial_imporved.vert";
#else 
  const wchar_t *selectedVertexShader = L"tutorial_imporved.vert";
#endif // DIRECTX

  shaderPath += selectedVertexShader;
  isSuccesful = helper::CompileShader(shaderPath.generic_wstring().c_str(), "vs_4_0",
                                      "VS", my_vertexShader);

  // Compile the vertex shader
  if (isSuccesful == false)
  {
  #if WIND_OS
    MessageBoxW(NULL,
                L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
    return hr;
  #endif // WIND_OS
  }

  // Create the vertex shader
  isSuccesful = my_device.CreateVertexShader(my_vertexShader);
  assert(isSuccesful == true && "Error creating vertex shader");

  isSuccesful = my_vertexInputLayout.ReadShaderData(my_vertexShader);
  assert(isSuccesful == true && "Error reading the vertex-shader data");

  // Create the input layout
  isSuccesful = my_device.CreateInputLayout(my_vertexInputLayout,
                                            my_vertexShader);
  assert(isSuccesful == true && "Error creating Input layout ");

  // Set the input layout
  my_deviceContext.IASetInputLayout(my_vertexInputLayout);


  shaderPath = g_initPath;
#if DIRECTX
  const wchar_t *selectedPixelhader = L"Tutorial07.fx";
  shaderPath += L"//dx_shaders//";
#elif OPEN_GL
  const wchar_t *selectedPixelhader = L"tutorial.frag";
  shaderPath += L"//gl_shaders//";
#else 

  const wchar_t * selectedPixelhader = L"no shader";
#endif // DIRECTX

  shaderPath += selectedPixelhader;

  isSuccesful = helper::CompileShader(shaderPath.generic_wstring().c_str(), "ps_4_0",
                                      "PS", my_pixelShader);

  if (isSuccesful == false)
  {
  #if WIND_OS
    MessageBoxW(NULL,
                L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
    return hr;
  #endif // WIND_OS
  }

  // Create the pixel shader
  isSuccesful = my_device.CreatePixelShader(my_pixelShader);
  assert((isSuccesful == true && "Error creating the pixel shader"));

  const char *ModelPath = "resources/media/3d models/obj/drakefire_pistol_low.obj";
  const char *TexPath = "resources/media/3d models/textures/drakefire_tex/base_albedo.jpg";

  cModel* ptr_toModel = helper::findModelComponent(*my_actor);
  assert(ptr_toModel != nullptr && "Error component 'model' does NOT exist in current actor");

  ptr_toModel->setModelPath(helper::openFile(my_window));

  isSuccesful = ptr_toModel->LoadModelFromFile(my_device);

  assert(("Error with loading model file" && isSuccesful == true));


  my_deviceContext.SetShaders(my_vertexShader, my_pixelShader);
  // Set primitive topology
  my_constViewMatrix.init(sizeof(ViewMatrix),
                          1,
                          0);

  my_constViewMatrix.setIndex(0);


  isSuccesful = my_device.CreateConstBuffer(my_constViewMatrix);
  assert(isSuccesful == true && "Error Creating constant buffer");

  //hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBNeverChanges);
  //if (FAILED(hr))
  //  return hr;

  my_constProjectionMatrix.init(sizeof(ProjectionMatrix),
                                1,
                                0);

  my_constProjectionMatrix.setIndex(1);

  isSuccesful = my_device.CreateConstBuffer(my_constProjectionMatrix);
  assert(isSuccesful == true && "Error Creating constant buffer");

  my_constChangesEveryFrame.init(sizeof(GlChangeEveryFrame),
                                 1,
                                 0);

  my_constChangesEveryFrame.setIndex(2);


  isSuccesful = my_device.CreateConstBuffer(my_constChangesEveryFrame);
  assert(isSuccesful == true && "Error Creating constant buffer");

  //bd.ByteWidth = sizeof(CBChangesEveryFrame);
  //hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBChangesEveryFrame);
  //if (FAILED(hr))
  //  return hr;

  // Load the Texture
#if DIRECTX
  const wchar_t *SelectedTextureFile = L"//base_albedo.jpg";
  std::filesystem::path PathToResoure(g_initPath);
  PathToResoure += SelectedTextureFile;

  hr = dx::CreateWICTextureFromFile(my_device.getDevice(),
                                    PathToResoure.generic_wstring().c_str(),
                                    nullptr,
                                    my_shaderResourceView.getShaderResourceRef());

  assert((!FAILED(hr) && "Error loading file"));
#else

  std::filesystem::path resourcePath(g_initPath);
  resourcePath += "//base_albedo.jpg";// "//base_albedo.jpg";
  my_shaderResourceView.createShaderResourceFromFile(resourcePath.generic_string(), my_device, my_deviceContext);

#endif // DIRECTX

  my_sampler.setDescirption(static_cast< int >(enFilter::Anisotropic),
                            static_cast< int >(enTextureAddress::Wrap), //equivalent to D3D11_TEXTURE_ADDRESS_WRAP
                            static_cast< int >(enTextureAddress::Wrap),
                            static_cast< int >(enTextureAddress::Wrap),
                            static_cast< int >(enComparison::Never),//equivalent to D3D11_COMPARISON_NEVER
                            10);

  isSuccesful = my_device.CreateSamplerState(my_sampler);
  assert(isSuccesful == true && "Error with creating sampler state");

  // Initialize the world matrices
  g_World.matrix = glm::identity<glm::mat4>();

  cCamera CameraSecurity;
  CameraSecurity.setEye(0.0f, 30.0f,  20.0f);
  CameraSecurity.setAt(0.0f, 0.0f, 0.0f);
  CameraSecurity.calculateAndSetView();
  CameraSecurity.calculateAndSetPerpective(my_window, 100.0f, 1000.1f, 0.01f);

  cCamera perspectiveCamera;
  perspectiveCamera.calculateAndSetView();
  perspectiveCamera.calculateAndSetPerpective(my_window, 70.0f, 100.0f, 0.1f);

  my_cameraManager->pushBackCamera(perspectiveCamera);
  my_cameraManager->pushBackCamera(CameraSecurity);

  ViewMatrix cbNeverChanges;
  cbNeverChanges.matrix = my_cameraManager->getViewMatrix().matrix;

  my_deviceContext.UpdateSubresource(&my_constViewMatrix,
                                     &cbNeverChanges);

  // Initialize the projection matrix
  ProjectionMatrix cbChangesOnResize;
  cbChangesOnResize.matrix = my_cameraManager->getProjectionMatrix().matrix;

  my_deviceContext.UpdateSubresource(&my_constProjectionMatrix,
                                     &cbChangesOnResize);

  std::filesystem::path arrowModelPath(g_initPath);

  arrowModelPath += "\\resources\\media\\3d models\\fbx\\Arrow.fbx";

  cModel *ptr_YArrow = helper::findComponent<cModel>(*my_YArrow);
  cModel *ptr_ZArrow = helper::findComponent<cModel>(*my_ZArrow);

  if (ptr_YArrow != nullptr)
  {
    ptr_YArrow->setModelPath(arrowModelPath.generic_string());
    ptr_YArrow->LoadModelFromFile(my_device);
  }
  if (ptr_ZArrow)
  {
    ptr_ZArrow->setModelPath(arrowModelPath.generic_string());
    ptr_ZArrow->LoadModelFromFile(my_device);
  }

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
  //cModel *ptr_toTornado = helper::findComponent<cModel>(*my_tornado);
  //cModel* ptr_toXArrow = helper::findModelComponent(*my_XArrow);
  //cModel* ptr_toYArrow = helper::findModelComponent(*my_YArrow);
  //cModel* ptr_toZArrow = helper::findModelComponent(*my_ZArrow);

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

  static cShaderResourceView* shaderResources[2] =
  {
    &my_shaderResourceView,
    &my_shaderTarget->getShaderResourceView(),
  };

/******** SET TARGET **********/
  my_deviceContext.OMSetRenderTargets(&my_shaderTarget->getRenderTargetView(), my_swapChain.getDepthStencilView());

/******** Clear depth**********/
  sColorf color_Mangnolia;
  color_Mangnolia.red = 0.98f;
  color_Mangnolia.green = 0.90f;
  color_Mangnolia.blue = 1.0f;
  color_Mangnolia.alpha = 1.0f;

  my_deviceContext.ClearRenderTargetView(my_shaderTarget->getRenderTargetView(), &color_Mangnolia);
  my_deviceContext.ClearDepthStencilView(my_swapChain.getDepthStencilView());


/******** Change camera **********/
  my_cameraManager->switchCamera(1);
  ViewMatrix neverChange = my_cameraManager->getViewMatrix();
  my_deviceContext.UpdateSubresource(reinterpret_cast< cBuffer* >(&my_constViewMatrix),
                                     &neverChange.matrix);

  ProjectionMatrix Proj;
  Proj.matrix = my_cameraManager->getProjectionMatrix().matrix;;
  my_deviceContext.UpdateSubresource(reinterpret_cast< cBuffer* >(&my_constProjectionMatrix),
                                     &Proj);

   /************************************************************************************************************************/

  /*setting values for the vertex shader*/
  my_deviceContext.VSSetConstantBuffers(my_constViewMatrix, my_constViewMatrix.getIndex());
  my_deviceContext.VSSetConstantBuffers(my_constProjectionMatrix, my_constProjectionMatrix.getIndex());
  my_deviceContext.VSSetConstantBuffers(my_constChangesEveryFrame, my_constChangesEveryFrame.getIndex());

  /*setting values for the pixel shader */
  my_deviceContext.PSSetConstantBuffers(my_constChangesEveryFrame, my_constChangesEveryFrame.getIndex());
  my_deviceContext.PSSetShaderResources(*shaderResources, 2);
  my_deviceContext.PSSetSamplers(&my_sampler);


   // Rotate cube around the origin
  g_World.matrix = glm::rotate(g_World.matrix, t, glm::vec3(0, 1.0f, 0));

  // Modify the color
  g_vMeshColor.red = (sinf(t * 1.0f) + 1.0f) * 0.5f;
  g_vMeshColor.green = (cosf(t * 3.0f) + 1.0f) * 0.5f;
  g_vMeshColor.blue = (sinf(t * 5.0f) + 1.0f) * 0.5f;

  float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha

  //g_pImmediateContext->ClearRenderTargetView(my_renderTragetView.getRenderTragetView()
/**************************************************************************************************************/

  //my_deviceContext.ClearDepthStencilView(my_swapChain.getDepthStencilView());

  static std::vector<cConstBuffer *> bufferArray =
  {
    &my_constChangesEveryFrame,
    &my_constProjectionMatrix,
    &my_constViewMatrix
  };

/************************************************************************************************************/
  // DRAW ONE 
/************************************************************************************************************/
  my_actor->DrawAllComponents(my_deviceContext, bufferArray);


  //my_actor->m_transform.rotateInYAxis(-0.3f);
  my_actor->update(my_deviceContext);

  my_deviceContext.OMSetRenderTargets(&my_swapChain.getRenderTargetView(), my_swapChain.getDepthStencilView());

  my_deviceContext.ClearRenderTargetView(my_swapChain.getRenderTargetView());

  my_deviceContext.ClearDepthStencilView(my_swapChain.getDepthStencilView());

  /*setting values for the vertex shader*/
  my_deviceContext.VSSetConstantBuffers(my_constViewMatrix, my_constViewMatrix.getIndex());
  my_deviceContext.VSSetConstantBuffers(my_constProjectionMatrix, my_constProjectionMatrix.getIndex());
  my_deviceContext.VSSetConstantBuffers(my_constChangesEveryFrame, my_constChangesEveryFrame.getIndex());

  /*setting values for the pixel shader */
  my_deviceContext.PSSetConstantBuffers(my_constChangesEveryFrame, my_constChangesEveryFrame.getIndex());
  //my_deviceContext.PSSetShaderResources(&my_shaderTarget->getShaderResourceView());
  my_deviceContext.PSSetShaderResources(*shaderResources, 2);
  my_deviceContext.PSSetSamplers(&my_sampler);

  ///my_deviceContext.ClearRenderTargetView(my_shaderTarget->getRenderTargetView());


  my_cameraManager->switchCamera(0);
  neverChange = my_cameraManager->getViewMatrix();
  my_deviceContext.UpdateSubresource(reinterpret_cast< cBuffer* >(&my_constViewMatrix),
                                     &neverChange.matrix);

  Proj.matrix = my_cameraManager->getProjectionMatrix().matrix;;
  my_deviceContext.UpdateSubresource(reinterpret_cast< cBuffer* >(&my_constProjectionMatrix),
                                     &Proj);

/************************************************************************************************************/
  //DARW TWO
/************************************************************************************************************/
  my_actor->DrawAllComponents(my_deviceContext, bufferArray);

  my_timer.EndTiming();
  float deltaTime = my_timer.GetResultSeconds();
  g_trackedTime = deltaTime;

  my_gui.beginFrame("Data");
  my_gui.addImage(my_shaderTarget->getShaderResourceView());
  my_gui.addButton("load new model", g_loadNewModel);
  my_gui.beginChildWithFpsCount(deltaTime);
  my_gui.addItemCountToChild("Mesh count ", "Mesh", ptr_toModel->getMeshCount());
  my_gui.addItemCountToChild("vertices count ", "vertices", ptr_toModel->getVertexCount());
  my_gui.addSliderFloat("Transform amount", g_TransformAmount, -5.0f, 5.0f);
  my_gui.addText("\nControls \n"
                 "chose axis with keys 'x' , 'y' , 'z'\n"
                 "do rotation with left and right arrow keys\n"
                 "use the 'u' and 'i' keys to shear the model\n"
                 "use the 'o' and 'p' keys to apply reflection Transform \n"
                 "use the 't' ,'g' , 'h' ,'f' ,'v' and 'n' \n"
                 "keys to apply a translation Transform \n");

  my_gui.endAllChildren();
  my_gui.endFrame();


  // Present our back buffer to our front buffer
  my_swapChain.Present(0, 0);

  Update();
}

void Update()
{
  g_windowSizeTracker.height = my_window.getHeight();
  g_windowSizeTracker.width = my_window.getWidth();

  my_window.update();

  if (g_windowSizeTracker.height != my_window.getHeight()
      || g_windowSizeTracker.width != my_window.getWidth())
  {
    // get current position of mouse 
    my_swapChain.Resize(my_device, my_window.getWidth(), my_window.getHeight());

    my_deviceContext.OMSetRenderTargets(&my_swapChain.getRenderTargetView(), my_swapChain.getDepthStencilView());

    my_cameraManager->getCurrentCamera()->calculateAndSetPerpective(my_window, my_cameraManager->getCurrentCamera()->getFovDeg(),
                                                                    my_cameraManager->getCurrentCamera()->getFar(),
                                                                    my_cameraManager->getCurrentCamera()->getNear());

    ProjectionMatrix newProjection;
    //CBChangeOnResize newProjection;
    newProjection.matrix = my_cameraManager->getProjectionMatrix().matrix;

    my_deviceContext.UpdateSubresource(&my_constProjectionMatrix,
                                       &newProjection);

    my_viewport.setViewport(my_window.getWidth(), my_window.getHeight(), 0.0f, 1.0f/*std::numeric_limits<float>::max()*/);

    my_deviceContext.RSSetViewports(&my_viewport);
  }

  if (g_loadNewModel == true)
  {
    //successful
    bool isSuccessful = helper::loadNewActorModelFromFile(*my_actor, my_window, my_device);
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
                                 my_window, my_deviceContext,
                                 &my_constViewMatrix, &my_constProjectionMatrix
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
      ViewMatrix ChangeWithViewMatrix;
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
      my_deviceContext.UpdateSubresource(&my_constViewMatrix,
                                         &ChangeWithViewMatrix);
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


#ifdef OPEN_GL

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
                               my_window, my_deviceContext,
                               &my_constViewMatrix, &my_constProjectionMatrix
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

    sVector3 result = { (currentPos - centerPos) * 0.016f };

    result.vector3.y = -result.vector3.y;

    my_cameraManager->rotateCamera(result, my_window);

    ViewMatrix ChangeWithViewMatrix;
    ChangeWithViewMatrix.matrix = my_cameraManager->getViewMatrix().matrix;
    my_deviceContext.UpdateSubresource(&my_constViewMatrix,
                                       &ChangeWithViewMatrix);

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

