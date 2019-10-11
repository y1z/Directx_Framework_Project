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
#include "../include/cDeviceContext.h" //UNFINISHED 
#include "../include/cTexture2D.h"// UNFINISHED
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
/*****************************************************/
#include "enum_headers/enFormatEnums.h" 
#include "enum_headers/enumTextureAddress.h"
#include "enum_headers/enumFilter.h"
#include "enum_headers/enumComparasion.h"
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
cModel my_model;
imGuiManager my_gui;
Timer my_timer;
/*****************************************************/
cWindow my_window;
cCamera my_camera;
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
const std::filesystem::path g_initPath = std::filesystem::current_path();
sFloat4 g_vMeshColor;

float g_Shearing = 1.0f;
//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------

HRESULT InitDevice();
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
  g_vMeshColor.vector4 = {0.7f, 0.7f, 0.7f, 1.0f};
  // alternate way to get an HINSTANCE  
  /* initialized for loading textures */
#if DIRECTX
  hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
  if (FAILED(hr))
  { return 0; }

#endif // DIRECTX
  my_window.init(WndProc, hInstance);
  /**init a console **/
  if (AllocConsole())
  {
    freopen("CONOUT$", "w", stdout);
    std::cout << "This works" << std::endl;
  }

  my_gui.setOpenFileFunction(helper::openFile);

  my_model.setModelPath(ModelSelectMenu(my_window));

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
#if DIRECTX
  /* */
  CoUninitialize();
#endif // DIRECTX
  return (int) msg.wParam;
}



//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
  helper::CreateDeviceAndSwapchain(my_device, my_deviceContext,
                                   my_swapChain, my_window,
                                   my_apiComponent);

  HRESULT hr = S_OK;
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
    MessageBox(NULL,
               L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
    return hr;
  }

  // Create the vertex shader
  isSuccesful = my_device.CreateVertexShader(my_vertexShader);
  assert(isSuccesful == true && "Error creating vertex shader");

  isSuccesful = my_vertexInputLayout.ReadShaderData(my_vertexShader);

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
    MessageBox(NULL,
               L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
    return hr;
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

  isSuccesful = my_model.LoadModelFromFile(my_device);
  assert(("Error with loading model file" && isSuccesful == true));
#endif // !MODEL_LOAD

  // Set primitive topology
  //my_deviceContext.IASetPrimitiveTopology(static_cast<int>(Topology::PointList));//equivalent to D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
  // Create the constant buffers
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

  //bd.ByteWidth = sizeof(CBChangeOnResize);
  //hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBChangeOnResize);
  //if (FAILED(hr))
  //return hr;

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

  return S_OK;
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

  if (message == WM_PAINT)
  {
    hdc = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
  }
  else if (message == WM_SIZING)
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
  else if (message == WM_KEYDOWN)
  {
    // used to alter the view matrix 
    GlViewMatrix ChangeWithViewMatrix;
    GlProjectionMatrix  ChangeOnProjectionChange;
    // going forwards 
    if (wParam == (WPARAM)'W')
    {
      my_cameraManager->moveFront(1.0f, my_window);
      ChangeWithViewMatrix.matrix = my_cameraManager->getViewMatrix().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &ChangeWithViewMatrix);
    }//going backwards
    else if (wParam == (WPARAM)'S')
    {
      my_cameraManager->moveFront(-1.0f, my_window);
      ChangeWithViewMatrix.matrix = my_cameraManager->getViewMatrix().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &ChangeWithViewMatrix);
    }//going right
    else if (wParam == (WPARAM)'D')
    {
      my_cameraManager->moveRight(1.0f, my_window);
      ChangeWithViewMatrix.matrix = my_cameraManager->getViewMatrix().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &ChangeWithViewMatrix);
    }//going left 
    else if (wParam == (WPARAM)'A')
    {
      my_cameraManager->moveRight(-1.0f, my_window);
      ChangeWithViewMatrix.matrix = my_cameraManager->getViewMatrix().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &ChangeWithViewMatrix);
    }
    //going up 
    else if (wParam == (WPARAM)'E')
    {
      my_cameraManager->moveUp(1.0f, my_window);
      ChangeWithViewMatrix.matrix = my_cameraManager->getViewMatrix().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &ChangeWithViewMatrix);
    }//going down 
    else if (wParam == (WPARAM)'Q')
    {
      my_cameraManager->moveUp(-1.0f, my_window);
      ChangeWithViewMatrix.matrix = my_cameraManager->getViewMatrix().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &ChangeWithViewMatrix);
    }
    else if (wParam == (WPARAM)'1')
    {
      my_cameraManager->switchCamera(0);
      ChangeWithViewMatrix.matrix = my_cameraManager->getViewMatrix().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &ChangeWithViewMatrix);
      ChangeOnProjectionChange.matrix = my_cameraManager->getProjectionMatrix().matrix;
      my_deviceContext.UpdateSubresource(&my_constChangeOnResize,
                                         &ChangeOnProjectionChange);
    }
    else if (wParam == (WPARAM)'2')
    {
      my_cameraManager->switchCamera(1);
      ChangeWithViewMatrix.matrix = my_cameraManager->getViewMatrix().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &ChangeWithViewMatrix);
      //for changing the projection
      ChangeOnProjectionChange.matrix = my_cameraManager->getProjectionMatrix().matrix;
      my_deviceContext.UpdateSubresource(&my_constChangeOnResize,
                                         &ChangeOnProjectionChange);
    }
    else if (wParam == (WPARAM)'U')
    {

      g_Shearing--;

    }
    else if (wParam == (WPARAM)'I')
    {
      g_Shearing++;
    }
    else if (wParam == VK_SHIFT)
    {
      // set position to middle of window 
      SetCursorPos(centerPoint.y, centerPoint.y);
      // get the new position 
      POINT newPoint;
      GetCursorPos(&newPoint);

      //sVector4 rot{newPoint.x - centerPoint.x, newPoint.y - centerPoint.y,0,0};
      //sVector4  ScalarVector{0.016f,0.016f,0.016f,0.016f};
      //rot.vector4 = dx::XMVectorMultiply(rot.vector4, ScalarVector.vector4);
      //// rot.vector4 *=  rot.vector4 + vector4 ;
      //my_camera.rotateCamera(rot, my_window);
      //my_deviceContext.UpdateSubresource(&my_constNeverChanges,
      //                                   &cbNeverChanges);

    }

  }
  else if (message == WM_DESTROY)
  {
    PostQuitMessage(0);
  }
  else
  {
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

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
  g_vMeshColor.vector4.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
  g_vMeshColor.vector4.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
  g_vMeshColor.vector4.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;
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
  glm::vec4 glRotVector = {-t,0, t,1};
  /* setup for manipulating 3d models */
  //dx::XMVECTOR moveVector = {2,1,1,1};
  //dx::XMVECTOR scaleVector = {1,1,std::fabs(std::cosf(t)),1};
  //dx::XMVECTOR rotVector = {-t,0, t,1};

  // CBChangesEveryFrame cb;
  //dx::XMMATRIX tempMatrixMove = dx::XMMatrixTranspose(dx::XMMatrixTranslationFromVector(moveVector));
  //dx::XMMATRIX tempMatrixScale = dx::XMMatrixScalingFromVector(scaleVector);
  //dx::XMMATRIX tempMatrixRotate = dx::XMMatrixRotationRollPitchYawFromVector(rotVector);//XMMatrixRotationZ(-t);

  //dx::XMVECTOR moveVector2 = {0,1,-1,1};
  //dx::XMVECTOR scaleVector2 = {1,1,std::fabs(std::cosf(t)) * 2,1};
  //dx::XMVECTOR rotVector2 = {0,0, -t,1};

  //dx::XMMATRIX tempMatrixMove2 = dx::XMMatrixTranspose(dx::XMMatrixTranslationFromVector(moveVector2));
  //dx::XMMATRIX tempMatrixScale2 = dx::XMMatrixScalingFromVector(scaleVector2);
  //dx::XMMATRIX tempMatrixRotate2 = dx::XMMatrixRotationRollPitchYawFromVector(rotVector2);//XMMatrixRotationZ(-t);



#ifndef MODEL_LOAD
  CBChangesEveryFrame cb;
  cb.mWorld = XMMatrixMultiply(XMMatrixMultiply(tempMatrixMove2, tempMatrixRotate2), tempMatrixScale2);
  cb.vMeshColor = g_vMeshColor;
  my_deviceContext.UpdateSubresource(&my_constChangesEveryFrame, &cb);

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
  glm::mat4 reflec(1.0f);
  reflec[1][0] = -g_Shearing;
  glm::vec3 MoveRight(-2.5, 1, 1);
  //glm::reflect(RandomTransform, glm::vec3(0, -1, 0));
  glm::mat4 result = glm::translate(RandomTransform, MoveRight);
  my_model.setTransform(reflec);

  my_model.DrawMeshes(my_deviceContext, bufferArray);
  GlChangeEveryFrame  Cb;
  Cb.world = glm::mat4(1.0f);
  Cb.color = {0.81f,1.0f,0.8314f,1.0f};
  ////cb.mWorld = XMMatrixMultiply(MoveGunLeft, dx::XMMatrixRotationY(t));// XMMatrixTranslationFromVector(moveVector)
  ////cb.vMeshColor = {0.81f,1.0f,0.8314f,1.0f};

//  my_deviceContext.UpdateSubresource(&my_constChangesEveryFrame, &Cb);

#endif // !MODEL_LOAD
  my_timer.EndTiming();
  float deltaTime = my_timer.GetResultSeconds();
  my_gui.FpsCountWindow("Data", deltaTime);
  // Present our back buffer to our front buffer

  // returns a boolean value 
  my_swapChain.Present(0, 0);
  my_window.update();
}

