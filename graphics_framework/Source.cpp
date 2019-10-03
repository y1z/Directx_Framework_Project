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
#include "../include/cRenderTarget.h"
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
/*****************************************************/
#include "enum_headers/enFormatEnums.h" 
#include "enum_headers/enumTextureAddress.h"
#include "enum_headers/enumFilter.h"
#include "enum_headers/enumComparasion.h"

/*****************************************************/
/* FOR LOADING IMAGES FOR WINDOW */
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

/*****************************************************/
cDevice my_device;
cDeviceContext my_deviceContext;
/*****************************************************/
cDepthStencilView my_depthStencilView;
/*****************************************************/
cRenderTargetView my_renderTargetView;
cRenderTarget my_renderTarget;
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
cApiComponents my_apiComponent;
/*****************************************************/
#include <cassert>

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
//HINSTANCE                           g_hInst = NULL;
D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
/*ID3D11Buffer*                       g_pVertexBuffer = NULL;
ID3D11Buffer*                       g_pIndexBuffer = NULL;
ID3D11Buffer*                       g_pCBNeverChanges = NULL;
ID3D11Buffer*                       g_pCBChangeOnResize = NULL;
ID3D11Buffer*                       g_pCBChangesEveryFrame = NULL;*/
sMatrix4x4 g_World;
sMatrix4x4 g_View;
sMatrix4x4 g_Projection;
//dx::XMMATRIX                            g_World;
//dx::XMMATRIX                            g_View;
//dx::XMMATRIX                            g_Projection;
// dx::XMFLOAT4                            g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);
sFloat4 g_vMeshColor;
//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void Render();

int APIENTRY
wWinMain(HINSTANCE hInstance,
         [[maybe_unused]] HINSTANCE hPrevInstance,
         [[maybe_unused]] LPWSTR    lpCmdLine,
         int       nCmdShow)
{
  HRESULT hr = S_FALSE;

  // alternate way to get an HINSTANCE  
  //GetModuleHandle(NULL)
  /* initialized for loading textures */
#if DIRECTX
  hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
  if (FAILED(hr))
  { return 0; }
#endif // DIRECTX
  my_window.init(WndProc, hInstance);

  g_vMeshColor.vector4 = {0.7f, 0.7f, 0.7f, 1.0f};
  if (FAILED(InitDevice()))
  {
    CleanupDevice();
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

  CleanupDevice();
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
  // Create a render target view
  isSuccesful = my_swapChain.InitBuffer();
  //  isSuccesful = my_swapChain.GetBuffer(my_renderTarget.getTexture(), 0);
  assert(("Error with swap-chain getting a buffer " &&  isSuccesful == true));

  //hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
  isSuccesful = my_device.CreateRenderTargetView(my_swapChain.getRenderTarget().getTexture(), my_renderTargetView);
  assert((isSuccesful == true && "Error with render-target creation"));
  // Create depth stencil texture

  sTextureDescriptor TextureDesc = helper::createDepthStencilDesc(width, height);
  //memset(&TextureDesc, 0, sizeof(TextureDesc));
  //TextureDesc.texHeight = height;
  //TextureDesc.texWidth = width;
  //TextureDesc.texFormat = Formats::D24_uniform_S8_UINT;// equivalent to DXGI_FORMAT_D24_UNORM_S8_UINT
  //TextureDesc.BindFlags = 0x40L;// equivalent to DD3D11_BIND_DEPTH_STENCIL
  //TextureDesc.Usage = 0;// equivalent to D3D11_USAGE_DEFAULT
  //TextureDesc.CpuAccess = 0;
  //TextureDesc.arraySize = 1;

  isSuccesful = my_device.CreateTexture2D(TextureDesc, my_swapChain.getDepthStencil());
  assert(isSuccesful == true && "Error with Texture 2d creation ");

  /**********************************************************/
  my_gui.Init(my_device, my_deviceContext, my_window.getHandle());
  /**********************************************************/
  // my own descriptor 
  //sDepthStencilDescriptor depthDesc;
  //depthDesc.Format = TextureDesc.texFormat;
  //depthDesc.Dimension = 3;// equivalent to D3D11_DSV_DIMENSION_TEXTURE2D 
  //depthDesc.Mip = 0;

  //  my_depthStencilView

  isSuccesful = my_device.CreateDepthStencilView(my_swapChain.getDepthStencilView());
  assert(isSuccesful == true && "Error with depth-stencil creation");

  my_deviceContext.OMSetRenderTargets(&my_renderTargetView,
                                      my_depthStencilView);

  //g_pImmediateContext->OMSetRenderTargets(1
  //                                        , my_renderTragetView.getRenderTragetViewRef(),
  //                                        my_depthStencillView.getDepthStencilView());
  // Setup the viewport
  my_viewport.setViewport(width, height,
                          0.0f, 1.0f);

  my_deviceContext.RSSetViewports(&my_viewport);
  //g_pImmediateContext->RSSetViewports(1, &vp);

  isSuccesful = helper::CompileShader(L"Tutorial07.fx", "vs_4_0",
                                      "VS", my_vertexShader);

  // Compile the vertex shader
  //hr = CompileShaderFromFile(L"Tutorial07.fx",
  //                           "VS",
  //                           "vs_4_0",
  //                           my_vertexShader.getInfoRef());

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

  isSuccesful = helper::CompileShader(L"Tutorial07.fx", "ps_4_0",
                                      "PS", my_pixelShader);
  // Compile the pixel shader
  //hr = CompileShaderFromFile(L"Tutorial07.fx"
  //                           , "PS"
  //                           , "ps_4_0"
  //                           , my_pixelShader.getInfoRef());

  if (isSuccesful == false)
  {
    MessageBox(NULL,
               L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
    return hr;
  }

  // Create the pixel shader
  isSuccesful = my_device.CreatePixelShader(my_pixelShader);
  assert((isSuccesful == true && "Error creating the pixel shader"));
  //hr = g_pd3dDevice->CreatePixelShader(
  //  pPSBlob->GetBufferPointer()
  //  , pPSBlob->GetBufferSize()
  //  , NULL
  //  , &g_pPixelShader);
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


  //D3D11_BUFFER_DESC bd;
  //SecureZeroMemory(&bd, sizeof(bd));
  //bd.Usage = D3D11_USAGE_DEFAULT;
  //bd.ByteWidth = sizeof(SimpleVertex) * 24;
  //bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  //bd.CPUAccessFlags = 0;

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

  isSuccesful = my_model.LoadModelFromFile("resources/media/3d models/obj/drakefire_pistol_low.obj",
                                           my_device, TexPath);
  assert(("Error with loading model file" && isSuccesful == true));
#endif // !MODEL_LOAD

  // Set primitive topology
  //my_deviceContext.IASetPrimitiveTopology(static_cast<int>(Topology::PointList));//equivalent to D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
  // Create the constant buffers
  my_constNeverChanges.setDescription(sizeof(CBNeverChanges),
                                      1,
                                      0);

  isSuccesful = my_device.CreateConstBuffer(my_constNeverChanges);
  assert(isSuccesful == true, "Error Creating constant buffer");

  //hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBNeverChanges);
  //if (FAILED(hr))
  //  return hr;

  my_constChangeOnResize.setDescription(sizeof(CBChangeOnResize),
                                        1,
                                        0);

  isSuccesful = my_device.CreateConstBuffer(my_constChangeOnResize);
  assert(isSuccesful == true, "Error Creating constant buffer");

  //bd.ByteWidth = sizeof(CBChangeOnResize);
  //hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBChangeOnResize);
  //if (FAILED(hr))
  //return hr;

  my_constChangesEveryFrame.setDescription(sizeof(CBChangesEveryFrame),
                                           1,
                                           0);

  isSuccesful = my_device.CreateConstBuffer(my_constChangesEveryFrame);
  assert(isSuccesful == true, "Error Creating constant buffer");

  //bd.ByteWidth = sizeof(CBChangesEveryFrame);
  //hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pCBChangesEveryFrame);
  //if (FAILED(hr))
  //  return hr;

  // Load the Texture
#if DIRECTX
  hr = dx::CreateWICTextureFromFile(my_device.getDevice(),
                                    L"base_albedo.jpg",
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
  assert(isSuccesful == true, "Error with creating sampler state");
  //hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
  //if (FAILED(hr))
  //  return hr;
  // Initialize the world matrices

#if DIRECTX
  g_World.matrix = dx::XMMatrixIdentity();
#endif // DIRECTX

  my_camera.initViewMatrix();
  my_camera.initProjectionMatrix(my_window, 70.0f, 100.0f, 0.1f);
  //g_Projection.matrix = dx::XMMatrixPerspectiveFovLH(dx::XM_PIDIV4,
  //                                                   width / (FLOAT) height,
  //                                                   0.01f,
  //                                                   100.0f);

  //g_View.matrix = dx::XMMatrixLookAtLH(Eye, At, Up);
#if DIRECTX
  CBNeverChanges cbNeverChanges;
  cbNeverChanges.mView = my_camera.getView().matrix;
  my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                     &cbNeverChanges);

  // Initialize the projection matrix
  CBChangeOnResize cbChangesOnResize;
  cbChangesOnResize.mProjection = my_camera.getProjection().matrix;//dx::XMMatrixTranspose(g_Projection.matrix);
  my_deviceContext.UpdateSubresource(&my_constChangeOnResize,
                                     &cbChangesOnResize);
#endif // DIRECTX

  return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{}


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
   
    my_swapChain.Resize(my_device, my_renderTargetView,
                        my_window.getHandle(), width, height);

    my_deviceContext.OMSetRenderTargets(&my_renderTargetView, my_swapChain.getDepthStencilView());

    GetClientRect(my_window.getHandle(), &rc);

    my_viewport.setViewport(width, height, 0.0f, 1.0f/*std::numeric_limits<float>::max()*/);

    my_deviceContext.RSSetViewports(&my_viewport);

    my_camera.initProjectionMatrix(my_window, my_camera.getFovDeg(),
                                   my_camera.getFar(), my_camera.getNear());

    CBChangeOnResize newProjection;
    newProjection.mProjection = my_camera.getProjection().matrix;

    my_deviceContext.UpdateSubresource(&my_constChangeOnResize,
                                       &newProjection);
  }
  else if (message == WM_KEYDOWN)
  {
    // used to alter the view matrix 
    CBNeverChanges cbNeverChanges;
    // going forwards 
    if (wParam == (WPARAM)'W')
    {
      my_camera.moveFront(1.0f, my_window);
      cbNeverChanges.mView = my_camera.getView().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &cbNeverChanges);
    }//going backwards
    else if (wParam == (WPARAM)'S')
    {
      my_camera.moveFront(-1.0f, my_window);
      cbNeverChanges.mView = my_camera.getView().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &cbNeverChanges);
    }//going right
    else if (wParam == (WPARAM)'D')
    {
      my_camera.moveRight(1.0f, my_window);
      cbNeverChanges.mView = my_camera.getView().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &cbNeverChanges);
    }//going left 
    else if (wParam == (WPARAM)'A')
    {
      my_camera.moveRight(-1.0f, my_window);
      cbNeverChanges.mView = my_camera.getView().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &cbNeverChanges);
    }
    //going up 
    else if (wParam == (WPARAM)'E')
    {
      my_camera.moveUp(1.0f, my_window);
      cbNeverChanges.mView = my_camera.getView().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &cbNeverChanges);
    }//going down 
    else if (wParam == (WPARAM)'Q')
    {
      my_camera.moveUp(-1.0f, my_window);
      cbNeverChanges.mView = my_camera.getView().matrix;
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &cbNeverChanges);
    }
    else if (wParam == VK_SHIFT)
    {
      // set position to middle of window 
      SetCursorPos(centerPoint.y, centerPoint.y);
      // get the new position 
      POINT newPoint;
      GetCursorPos(&newPoint);

      sVector4 rot{newPoint.x - centerPoint.x, newPoint.y - centerPoint.y,0,0};
      sVector4  ScalarVector{0.016f,0.016f,0.016f,0.016f};
      rot.vector4 = dx::XMVectorMultiply(rot.vector4, ScalarVector.vector4);
      // rot.vector4 *=  rot.vector4 + vector4 ;
      my_camera.AddToAt(rot, my_window);
      my_deviceContext.UpdateSubresource(&my_constNeverChanges,
                                         &cbNeverChanges);
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
  g_World.matrix = dx::XMMatrixRotationY(t);

  // Modify the color
  g_vMeshColor.vector4.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
  g_vMeshColor.vector4.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
  g_vMeshColor.vector4.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;
  //
  // Clear the back buffer
  //
  float ClearColor[4] = {0.0f, 0.125f, 0.3f, 1.0f}; // red, green, blue, alpha
  my_deviceContext.ClearRenderTargetView(my_renderTargetView);
  //g_pImmediateContext->ClearRenderTargetView(my_renderTragetView.getRenderTragetView()

  my_deviceContext.ClearDepthStencilView(my_swapChain.getDepthStencilView());
  //g_pImmediateContext->ClearDepthStencilView(my_depthStencilView.getDepthStencilView()
  //                                           , D3D11_CLEAR_DEPTH, 1.0f
  //                                           , 0);

  /* setup for manipulating 3d models */
  dx::XMVECTOR moveVector = {2,1,1,1};
  dx::XMVECTOR scaleVector = {1,1,std::fabs(std::cosf(t)),1};
  dx::XMVECTOR rotVector = {-t,0, t,1};
  // CBChangesEveryFrame cb;
  dx::XMMATRIX tempMatrixMove = dx::XMMatrixTranspose(dx::XMMatrixTranslationFromVector(moveVector));
  dx::XMMATRIX tempMatrixScale = dx::XMMatrixScalingFromVector(scaleVector);
  dx::XMMATRIX tempMatrixRotate = dx::XMMatrixRotationRollPitchYawFromVector(rotVector);//XMMatrixRotationZ(-t);

  dx::XMVECTOR moveVector2 = {0,1,-1,1};
  dx::XMVECTOR scaleVector2 = {1,1,std::fabs(std::cosf(t)) * 2,1};
  dx::XMVECTOR rotVector2 = {0,0, -t,1};

  dx::XMMATRIX tempMatrixMove2 = dx::XMMatrixTranspose(dx::XMMatrixTranslationFromVector(moveVector2));
  dx::XMMATRIX tempMatrixScale2 = dx::XMMatrixScalingFromVector(scaleVector2);
  dx::XMMATRIX tempMatrixRotate2 = dx::XMMatrixRotationRollPitchYawFromVector(rotVector2);//XMMatrixRotationZ(-t);

  CBChangesEveryFrame cb;
#ifndef MODEL_LOAD
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

  my_model.DrawMeshes(my_deviceContext, bufferArray);
  dx::XMMATRIX MoveGunLeft = dx::XMMatrixTranspose(dx::XMMatrixTranslationFromVector({-2,1,1,1}));
  cb.mWorld = XMMatrixMultiply(MoveGunLeft, dx::XMMatrixRotationY(t));// XMMatrixTranslationFromVector(moveVector)
  cb.vMeshColor = {0.81f,0.17f,0.8314f,1.0f};

  my_deviceContext.UpdateSubresource(&my_constChangesEveryFrame, &cb);

#endif // !MODEL_LOAD
  my_timer.EndTiming();
  float deltaTime = my_timer.GetResultSeconds();
  my_gui.FpsCountWindow("Data", deltaTime);
  // Present our back buffer to our front buffer

  // returns a boolean value 
  my_swapChain.Present(0, 0);
  my_window.update();
}

