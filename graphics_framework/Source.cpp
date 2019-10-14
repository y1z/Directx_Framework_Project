//--------------------------------------------------------------------------------------
// File: Tutorial07.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "utility/Grafics_libs.h"
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
#endif // DIRECTX
/*****************************************************/
#if DIRECTX
#include "../include/directx_structs.h"
#endif // DIRECTX
/*****************************************************/
#include "utility/HelperFuncs.h"
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
cConstBuffer my_constNeverChanges;
cConstBuffer my_constChangeOnResize;
cConstBuffer my_constChangesEveryFrame;
cSampler my_sampler;
cViewport my_viewport;
cShaderResourceView my_shaderResourceView;
cSwapChain my_swapChain;
cModel* my_modelPtr = new cModel();
imGuiManager my_gui;
Timer my_timer;
/*****************************************************/
cWindow my_window;
cCamera my_camera;
std::unique_ptr <cActor>my_actor = std::make_unique<cActor>();
std::unique_ptr<cCameraManager>my_cameraManager = std::make_unique<cCameraManager>();
/**********************************************************/
cApiComponents my_apiComponent;
/*****************************************************/
#include <cassert>
#include <iostream>

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
//HINSTANCE                           g_hInst = NULL;
//D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
//D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
/*ID3D11Buffer*                       g_pVertexBuffer = NULL;
ID3D11Buffer*                       g_pIndexBuffer = NULL;
ID3D11Buffer*                       g_pCBNeverChanges = NULL;
ID3D11Buffer*                       g_pCBChangeOnResize = NULL;
ID3D11Buffer*                       g_pCBChangesEveryFrame = NULL;*/

sMatrix4x4 g_World;
sMatrix4x4 g_View;
sMatrix4x4 g_Projection;
bool g_isInit(false);
//! this is the path local to the solution of the program
const std::filesystem::path g_initPath = std::filesystem::current_path();
sColorf g_vMeshColor;

float g_TransfromAmount = 1.0f;
//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------

HRESULT InitDevice();
/*!this function only exits so i can keep track of when i destroy all obj in
 the actor */
void DestroyAllComponentsFromActor(cActor &actor);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
std::string ModelSelectMenu(cWindow &window);
void Render();

int APIENTRY
wWinMain(HINSTANCE hInstance,
         [[maybe_unused]] HINSTANCE hPrevInstance,
         [[maybe_unused]] LPWSTR    lpCmdLine,
         int       nCmdShow)
{
  HRESULT hr = S_FALSE;
  g_vMeshColor = {0.7f, 0.7f, 0.7f, 1.0f};
  // alternate way to get an HINSTANCE  
  /* initialized for loading textures */
#if DIRECTX

  /*Initializes the COM library
  this is so I can load images for directX */
  hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
  if (FAILED(hr))
  { return -1; }
#elif OPEN_GL

#endif // DIRECTX

  /*init's the window*/
  if (!my_window.init(WndProc, hInstance))
  { return -1; }

  /**init a console **/
  if (AllocConsole())
  {
    freopen("CONOUT$", "w", stdout);
    std::cout << "This works" << std::endl;
  }

  my_gui.setOpenFileFunction(helper::openFile);

  //  my_modelPtr->setModelPath(ModelSelectMenu(my_window));

  cModel *ptr_model = new cModel();

  /* the actor now owns the model */
  my_actor->AddComponents(ptr_model);

  if (FAILED(InitDevice()))
  {
    return 0;
  }
  // Main message loop
  MSG msg = {0};
  while (WM_QUIT != msg.message)
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

  /****free the console *****/
  FreeConsole();

  /********remove all component from my actor **********/
  DestroyAllComponentsFromActor(*my_actor);
  my_actor.release();
#if DIRECTX
  /*Uninitialized the comm library*/
  CoUninitialize();
#endif // DIRECTX
  return (int) msg.wParam;
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
  //
  RECT rc;
  GetClientRect(my_window.getHandle(), &rc);
  UINT width = rc.right - rc.left;
  UINT height = rc.bottom - rc.top;

  // DXGI_FORMAT_R32G32B32A32_FLOAT = 2;
  // my_renderTarget.setDescription(width, height, Formats::fR16G16B16A16);
  my_swapChain.setRenderTarget(width, height, Formats::fR32G32B32A32);

  my_swapChain.setDepthStencilView(Formats::depthStencil_format);
  isSuccesful = my_swapChain.InitBuffer();
  //  isSuccesful = my_swapChain.GetBuffer(my_renderTarget.getTexture(), 0);
  assert(("Error with swap-chain getting a buffer " &&  isSuccesful == true));

  // Create a render target view
  isSuccesful = my_device.CreateRenderTargetView(my_swapChain.getRenderTarget().getTexture(), my_swapChain.getRenderTargerView());
  assert((isSuccesful == true && "Error with render-target creation"));
  // Create depth stencil texture

  sTextureDescriptor TextureDesc = helper::createDepthStencilDesc(width, height);

  isSuccesful = my_device.CreateTexture2D(TextureDesc, my_swapChain.getDepthStencil());
  assert(isSuccesful == true && "Error with Texture 2d creation ");

  /**********************************************************/
  my_gui.Init(my_device, my_deviceContext, my_window.getHandle());
  /**********************************************************/
  // set update depth-stencil-view
  my_swapChain.setDepthStencilView(Formats::depthStencil_format);

  isSuccesful = my_device.CreateDepthStencilView(my_swapChain.getDepthStencilView());
  assert(isSuccesful == true && "Error with depth-stencil creation");

  my_deviceContext.OMSetRenderTargets(&my_swapChain.getRenderTargerView(),
                                      my_swapChain.getDepthStencilView());

  // Setup the viewport
  my_viewport.setViewport(width, height,
                          0.0f, 1.0f);

  my_deviceContext.RSSetViewports(&my_viewport);
  //g_pImmediateContext->RSSetViewports(1, &vp);

  const wchar_t *selectedShader = L"Tutorial07.fx";

  std::filesystem::path shaderPath(g_initPath);

  shaderPath += L"//";
  shaderPath += selectedShader;
  isSuccesful = helper::CompileShader(shaderPath.generic_wstring().c_str(), "vs_4_0",
                                      "VS", my_vertexShader);

  // Compile the vertex shader
  if (isSuccesful == false)
  {
  #if WIND_OS
    MessageBox(NULL,
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
  //hr = g_pd3dDevice->CreateInputLayout(layout,
  //                                     numElements,
  //                                     my_vertexShader.getInfo()->GetBufferPointer(),
  //                                     my_vertexShader.getInfo()->GetBufferSize(), 
  //                                     &g_pVertexLayout);

  // Set the input layout
  my_deviceContext.IASetInputLayout(my_vertexInputLayout);

  isSuccesful = helper::CompileShader(shaderPath.generic_wstring().c_str(), "ps_4_0",
                                      "PS", my_pixelShader);

  if (isSuccesful == false)
  {
  #if WIND_OS
    MessageBox(NULL,
               L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
    return hr;
  #endif // WIND_OS
  }

  // Create the pixel shader
  isSuccesful = my_device.CreatePixelShader(my_pixelShader);
  assert((isSuccesful == true && "Error creating the pixel shader"));

#ifndef MODEL_LOAD
  // Create vertex buffer
  SimpleVertex vertices[] =
  {
      { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
      { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
      { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

      { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
      { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
      { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

      { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
      { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
      { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

      { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
      { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
      { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

      { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
      { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
      { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

      { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
      { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
      { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
  };


  my_vertexBuffer.setDescription(sizeof(SimpleVertex),
                                 24,
                                 0);
  // setting the sub-resource
  my_vertexBuffer.setData(vertices);

  D3D11_SUBRESOURCE_DATA InitData;
  SecureZeroMemory(&InitData, sizeof(InitData));
  InitData.pSysMem = vertices;

  isSuccesful = my_device.CreateVertexBuffer(my_vertexBuffer);
  assert((isSuccesful == true, "Error creating the vertex buffer "));

  // Set vertex buffer
  UINT stride = sizeof(SimpleVertex);
  UINT offset = 0;
  my_deviceContext.IASetVertexBuffers(&my_vertexBuffer, 1);

  //g_pImmediateContext->IASetVertexBuffers(0,
  //                                        1,
  //                                        my_vertexBuffer.getBufferRef(),
  //                                        &stride,
  //                                        &offset);

  // Create index buffer
  // Create vertex buffer
  WORD indices[] =
  {
      3,1,0,
      2,1,3,

      6,4,5,
      7,4,6,

      11,9,8,
      10,9,11,

      14,12,13,
      15,12,14,

      19,17,16,
      18,17,19,

      22,20,21,
      23,20,22
  };

  my_indexBuffer.setDescription(sizeof(WORD),
                                36,
                                0);

  my_indexBuffer.setData(indices);

  isSuccesful = my_device.CreateIndexBuffer(my_indexBuffer);
  assert((isSuccesful == true, "Error with creating Index buffer"));
  //hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
  //if (FAILED(hr))
  //  return hr;


  // Set index buffer
  my_deviceContext.IASetIndexBuffer(my_indexBuffer, 57/*equivalent to DXGI_FORMAT_R16_UINT*/);
  //g_pImmediateContext->IASetIndexBuffer(my_indexBuffer.getBuffer(),
  //                                      DXGI_FORMAT_R16_UINT,
  //                                      0);
#else
  const char *ModelPath = "resources/media/3d models/obj/drakefire_pistol_low.obj";
  const char *TexPath = "resources/media/3d models/textures/drakefire_tex/base_albedo.jpg";

  cModel* ptr_toModel = helper::findModelComponent(*my_actor);
  assert(ptr_toModel != nullptr && "Error component 'model' does NOT exist in current actor");

  ptr_toModel->setModelPath(ModelSelectMenu(my_window));
  isSuccesful = ptr_toModel->LoadModelFromFile(my_device);
  assert(("Error with loading model file" && isSuccesful == true));

#endif // !MODEL_LOAD

  // Set primitive topology
  my_constNeverChanges.setDescription(sizeof(GlViewMatrix),
                                      1,
                                      0);

  isSuccesful = my_device.CreateConstBuffer(my_constNeverChanges);
  assert(isSuccesful == true && "Error Creating constant buffer");

  //hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBNeverChanges);
  //if (FAILED(hr))
  //  return hr;

  my_constChangeOnResize.setDescription(sizeof(GlProjectionMatrix),
                                        1,
                                        0);

  isSuccesful = my_device.CreateConstBuffer(my_constChangeOnResize);
  assert(isSuccesful == true && "Error Creating constant buffer");

  my_constChangesEveryFrame.setDescription(sizeof(GlChangeEveryFrame),
                                           1,
                                           0);

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

  //hr = dx::CreateDDSTextureFromFile(my_device.getDevice(),
  //                                  L"seafloor.dds",
  //                                  nullptr,
  //                                  my_shaderResourceView.getShaderResourceRef());
  assert((!FAILED(hr) && "Error loading file"));
#endif // DIRECTX

  my_sampler.setDescirption(static_cast<int>(Filter::Anisotropic),
                            static_cast<int>(TextureAddress::Wrap), //equivalent to D3D11_TEXTURE_ADDRESS_WRAP
                            static_cast<int>(TextureAddress::Wrap),
                            static_cast<int>(TextureAddress::Wrap),
                            static_cast<int>(Comparasion::Never),//equivalent to D3D11_COMPARISON_NEVER
                            10);

  isSuccesful = my_device.CreateSamplerState(my_sampler);
  assert(isSuccesful == true && "Error with creating sampler state");
  //hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
  //if (FAILED(hr))
  //  return hr;
  // Initialize the world matrices
  g_World.matrix = glm::identity<glm::mat4>();
  cCamera orthoCamera;
  orthoCamera.calculateAndSetView();
  orthoCamera.calculateAndSetOrthographic(my_window, 100.0f, 0.1f);

  my_camera.calculateAndSetView();
  my_camera.calculateAndSetPerpective(my_window, 70.0f, 100.0f, 0.1f);

  my_cameraManager->pushBackCamera(my_camera);
  my_cameraManager->pushBackCamera(orthoCamera);
  //g_Projection.matrix = dx::XMMatrixPerspectiveFovLH(dx::XM_PIDIV4,
  //                                                   width / (FLOAT) height,
  //                                                   0.01f,
  //                                                   100.0f);

  //g_View.matrix = dx::XMMatrixLookAtLH(Eye, At, Up);
  GlViewMatrix cbNeverChanges;
  cbNeverChanges.matrix = my_cameraManager->getViewMatrix().matrix;

  //cbNeverChanges.matrix = glm::transpose(cbNeverChanges.matrix);

  my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                     &cbNeverChanges);

  // Initialize the projection matrix
  GlProjectionMatrix cbChangesOnResize;
  cbChangesOnResize.matrix = my_cameraManager->getProjectionMatrix().matrix;//dx::XMMatrixTranspose(g_Projection.matrix);

//cbChangesOnResize.matrix = glm::transpose(cbChangesOnResize.matrix);
  my_deviceContext.UpdateSubresource(&my_constChangeOnResize,
                                     &cbChangesOnResize);

  g_isInit = true;

  return S_OK;
}

void
DestroyAllComponentsFromActor(cActor & actor)
{
  actor.DestroyAllComponents();
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
#else
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#endif // DIRECTX
  PAINTSTRUCT ps;
  HDC hdc;
  // getting the center point of my mouse 
  RECT rect;
  GetClientRect(my_window.getHandle(), &rect);
  POINT centerPoint;
  centerPoint.x = (rect.right - rect.left) * 0.5f;
  centerPoint.y = (rect.bottom - rect.top) * 0.5f;

  //SetCapture(my_window.getHandle());
  /*this helps select the axis for the transforms*/
  static uint8 chosenAxis{0};

  if (message == WM_PAINT)
  {
    hdc = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
  }
  if (message == WM_SIZING)
  {
    RECT rc;
    GetClientRect(my_window.getHandle(), &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    // get current position of mouse 
    my_swapChain.Resize(my_device, width, height);

    my_deviceContext.OMSetRenderTargets(&my_swapChain.getRenderTargerView(), my_swapChain.getDepthStencilView());

    GetClientRect(my_window.getHandle(), &rc);

    //my_camera.initProjectionMatrix(my_window, my_camera.getFovDeg(),
    //                               my_camera.getFar(), my_camera.getNear());

    //CBChangeOnResize newProjection;
    //newProjection.mProjection = my_camera.getProjection().matrix;

    //my_deviceContext.UpdateSubresource(&my_constChangeOnResize,
    //                                   &newProjection);

    my_viewport.setViewport(width, height, 0.0f, 1.0f/*std::numeric_limits<float>::max()*/);

    my_deviceContext.RSSetViewports(&my_viewport);
  }
  if (message == WM_KEYDOWN)
  {
    // used to alter the view matrix 
    GlViewMatrix ChangeWithViewMatrix;
    GlProjectionMatrix  ChangeOnProjectionChange;
    // going forwards 
    helper::handelCameraKeyInput(wParam, *my_cameraManager,
                                 my_window, my_deviceContext,
                                 &my_constNeverChanges, &my_constChangeOnResize);

    if (wParam == (WPARAM)'R')
    {
      my_actor->m_transform.resetToIdentity();
    }

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
    else if (wParam == VK_RIGHT)
    {
      switch (chosenAxis)
      {
        case 0:
          my_actor->m_transform.rotateInXAxis(g_TransfromAmount);
          break;

        case 1:
          my_actor->m_transform.rotateInYAxis(g_TransfromAmount);
          break;

        case 2:
          my_actor->m_transform.rotateInZAxis(g_TransfromAmount);
          break;
      }
    }
    else if (wParam == VK_LEFT)
    {
      switch (chosenAxis)
      {
        case 0:
          my_actor->m_transform.rotateInXAxis(-g_TransfromAmount);
          break;
        case 1:
          my_actor->m_transform.rotateInYAxis(-g_TransfromAmount);
          break;
        case 2:
          my_actor->m_transform.rotateInZAxis(-g_TransfromAmount);
          break;
      }
    }
    if (wParam == (WPARAM)'U')
    {
      switch (chosenAxis)
      {
        case 0:
          my_actor->m_transform.shearTransformInXAxis(g_TransfromAmount);
          break;
        case 1:
          my_actor->m_transform.shearTransformInYAxis(g_TransfromAmount);
          break;
        case 2:
          my_actor->m_transform.shearTransformInZAxis(g_TransfromAmount);
          break;
      }
    }

    else if (wParam == (WPARAM)'I')
    {
      switch (chosenAxis)
      {
        case 0:
          my_actor->m_transform.shearTransformInXAxis(-g_TransfromAmount);
          break;
        case 1:
          my_actor->m_transform.shearTransformInYAxis(-g_TransfromAmount);
          break;
        case 2:
          my_actor->m_transform.shearTransformInZAxis(-g_TransfromAmount);
          break;
      }
    }

    else if (wParam == (WPARAM)'O')
    {
      switch (chosenAxis)
      {
        case 0:
          my_actor->m_transform.reflectTransfromInXAxis(g_TransfromAmount);
          break;
        case 1:
          my_actor->m_transform.reflectTransfromInYAxis(g_TransfromAmount);
          break;
        case 2:
          my_actor->m_transform.reflectTransfromInZAxis(g_TransfromAmount);
          break;
      }
    }

    if (wParam == (WPARAM)'P')
    {
      switch (chosenAxis)
      {
        case 0:
          my_actor->m_transform.reflectTransfromInXAxis(-g_TransfromAmount);
          break;
        case 1:
          my_actor->m_transform.reflectTransfromInYAxis(-g_TransfromAmount);
          break;
        case 2:
          my_actor->m_transform.reflectTransfromInZAxis(-g_TransfromAmount);
          break;
      }
    }
  }
  if (message == WM_MOUSEMOVE && g_isInit)
  {

    if (wParam & MK_SHIFT)
    {
      GlViewMatrix ChangeWithViewMatrix;
      POINT newPoint;
      GetCursorPos(&newPoint);

      sVector3 newVector;
      newVector.vector3 = glm::vec3(newPoint.x, newPoint.y, 0.0f);
      // set position to middle of window 
      SetCursorPos(centerPoint.x, centerPoint.y);
      sVector3 centerVector;
      centerVector.vector3 = {centerPoint.x, centerPoint.y,0.0f};

      sVector3 rotation;
      rotation.vector3 = (newVector.vector3 - centerVector.vector3) * 0.016f;
      rotation.vector3.y = -rotation.vector3.y;

      my_cameraManager->rotateCamera(rotation, my_window);
      ChangeWithViewMatrix.matrix = my_cameraManager->getViewMatrix().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
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
  //  ReleaseCapture();
  return 0;
}

std::string
ModelSelectMenu(cWindow & window)
{
  return my_gui.OpenFileFunc(window);
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

  if (my_apiComponent.getHardwareVersion() == D3D_DRIVER_TYPE_REFERENCE)
  {
    t += (float) dx::XM_PI * 0.0125f;
  }
  else
  {
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if (dwTimeStart == 0)
      dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;
  }

  // Rotate cube around the origin
  g_World.matrix = glm::rotate(g_World.matrix, t, glm::vec3(0, 1.0f, 0));

  // Modify the color
  g_vMeshColor.red = (sinf(t * 1.0f) + 1.0f) * 0.5f;
  g_vMeshColor.green = (cosf(t * 3.0f) + 1.0f) * 0.5f;
  g_vMeshColor.blue = (sinf(t * 5.0f) + 1.0f) * 0.5f;
  //
  // Clear the back buffer
  //
  float ClearColor[4] = {0.0f, 0.125f, 0.3f, 1.0f}; // red, green, blue, alpha
  my_deviceContext.ClearRenderTargetView(my_swapChain.getRenderTargerView());
  //g_pImmediateContext->ClearRenderTargetView(my_renderTragetView.getRenderTragetView()

  my_deviceContext.ClearDepthStencilView(my_swapChain.getDepthStencilView());
  //g_pImmediateContext->ClearDepthStencilView(my_depthStencilView.getDepthStencilView()
  //                                           , D3D11_CLEAR_DEPTH, 1.0f
  //                                           , 0);
  glm::vec4 glMoveVector = {2,1,1,1};
  glm::vec4 glScaleVector = {1,1,std::fabs(std::cosf(t)),1};
  glm::vec3 glRotVector = {1,0,0};

#ifndef MODEL_LOAD
  GlChangeEveryFrame Cb;
  Cb.world = glm::rotate(Cb.world, t, glRotVector);
  Cb.color.red = 0.5f;
  Cb.color.green = 0.5f;
  Cb.color.blue = 0.6f;
  Cb.color.alpha = 1.0f;

  cb.vMeshColor = g_vMeshColor;
  my_deviceContext.UpdateSubresource(&my_constChangesEveryFrame, &Cb);

#endif // !MODEL_LOAD
  //
  // Render the cube
  //

  /*setting values for the vertex shader*/
  my_deviceContext.VSSetShader(my_vertexShader);
  my_deviceContext.VSSetConstantBuffers(my_constNeverChanges, 0);
  my_deviceContext.VSSetConstantBuffers(my_constChangeOnResize, 1);
  my_deviceContext.VSSetConstantBuffers(my_constChangesEveryFrame, 2);

  /*setting values for the pixel shader */
  my_deviceContext.PSSetShader(my_pixelShader);
  my_deviceContext.PSSetConstantBuffers(my_constChangesEveryFrame, 2);
  my_deviceContext.PSSetShaderResources(&my_shaderResourceView);
  my_deviceContext.PSSetSamplers(&my_sampler);

#ifndef MODEL_LOAD
  my_deviceContext.DrawIndexed(36, 0);
  cb.mWorld = dx::XMMatrixMultiply(tempMatrixMove, tempMatrixScale);//XMMatrixTranspose(XMMatrixTranslationFromVector(moveVector));

  cb.vMeshColor = g_vMeshColor;
  my_deviceContext.UpdateSubresource(&my_constChangesEveryFrame, &cb);

  my_deviceContext.DrawIndexed(36, 0);

  dx::XMVECTOR Move2 = {-3,1,1,0};
  // CBChangesEveryFrame cb;
  tempMatrixMove = dx::XMMatrixTranspose(dx::XMMatrixTranslationFromVector(Move2));

  cb.mWorld = dx::XMMatrixMultiply(tempMatrixMove, tempMatrixRotate);
  cb.vMeshColor = g_vMeshColor;
  my_deviceContext.UpdateSubresource(&my_constChangesEveryFrame, &cb);

  my_deviceContext.DrawIndexed(36, 0);
#else

  static std::vector<cConstBuffer *> bufferArray =
  {
    &my_constChangesEveryFrame,
    &my_constChangeOnResize,
    &my_constNeverChanges
  };

  glm::mat4 RandomTransform(1.0f);
  glm::mat4 ShearMatrix(1.0f);
  ShearMatrix[1][0] = -g_TransfromAmount;
  glm::vec3 MoveRight(-2.5, 1, 1);
  glm::mat4 result = glm::translate(RandomTransform, MoveRight);

  //ptr_toModel->setTransform(ShearMatrix);
  //my_actor->m_transform.shearTransformInXAxis(g_Shearing);
  //my_actor->m_transform.rotateInYAxis(std::sinf(t));
  my_actor->update(my_deviceContext);

  sColorf color;
  color.red = std::sinf(t);
  color.green = 0.5f;
  color.blue = 1.0f;
  color.alpha = 1.0f;

  //my_modelPtr->DrawMeshes(my_deviceContext, bufferArray, color);
  ptr_toModel->DrawMeshes(my_deviceContext, bufferArray, color);
  //GlChangeEveryFrame  Cb;
  //Cb.world = glm::mat4(1.0f);
  //Cb.color = {0.81f,1.0f,0.8314f,1.0f};

#endif // !MODEL_LOAD
  my_timer.EndTiming();
  float deltaTime = my_timer.GetResultSeconds();
  my_gui.beginFrame("Data");
  my_gui.beginChildWithFpsCount(deltaTime);
  my_gui.addItemCountToChild("Mesh count ", "Mesh", ptr_toModel->getMeshCount());
  my_gui.addItemCountToChild("vertices count ", "vertices", ptr_toModel->getVertexCount());
  my_gui.addSliderFloat("Transform amount", g_TransfromAmount, -5.0f, 5.0f);
  my_gui.addText("\nControls \n"
                 "chose axis with keys 'x' , 'y' , 'z'\n"
                 "do rotation with left and right arrow keys\n"
                 "use the 'u' and 'i' keys to shear the model\n"
                 "use the 'o' and 'p' keys to apply reflection Transform \n");

  my_gui.endAllChildren();
  my_gui.endFrame();
  // Present our back buffer to our front buffer
  my_swapChain.Present(0, 0);
  my_window.update();
}

