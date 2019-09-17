//--------------------------------------------------------------------------------------
// File: Tutorial07.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#include "include/utiliy/Grafics_libs.h"

/******************************************************/
#include "include/cDevice.h"// FINISHED
#include "include/cDeviceContext.h" //UNFINISHED 
#include "include/cTexture2D.h"// UNFINISHED
#include "include/cRenderTargetView.h"// UNFINISHED
#include "include/cDepthStencilView.h"// UNFINISHED
#include "include/cVertexShader.h"// UNFINISHED
#include "include/cInputLayout.h"// UNFINISHED
#include "include/cPixelShader.h"// UNFINISHED 
#include "include/cBuffer.h"// FINISHED
#include "include/cVertexBuffer.h"// FINISHED 
#include "include/cIndexBuffer.h"// FINISHED 
#include "include/cConstBuffer.h"// FINISHED 
#include "include/cSampler.h"// FINISHED 
#include "include/cViewport.h" // FINISHED
#include "include/cShaderResourceView.h"//FINISHED
#include "include/cSwapChain.h"
/*****************************************************/
cDevice my_device;
cDeviceContext my_deviceContext;
cTexture2D my_backBuffer;
cTexture2D my_depthStencil;
cDepthStencilView my_depthStencilView;
cRenderTargetView my_renderTragetView;
cVertexShader my_vertexShader;
cPixelShader my_pixelShader;
cInputLayout my_vertexInputLayout;
cVertexBuffer my_vertexBuffer;
cIndexBuffer my_indexBuffer;
cConstBuffer my_constNeverChanges;
cConstBuffer my_constChangeOnResize;
cConstBuffer my_constChangesEveryFrame;
cSampler my_sampler;
cViewport my_viewport;
cShaderResourceView my_shaderResourceView;
cSwapChain my_swapChain;
/*****************************************************/
#include <cassert>
//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
  XMFLOAT3 Pos;
  XMFLOAT2 Tex;
};

struct CBNeverChanges
{
  XMMATRIX mView;
};

struct CBChangeOnResize
{
  XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
  XMMATRIX mWorld;
  XMFLOAT4 vMeshColor;
};


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE                           g_hInst = NULL;
HWND                                g_hWnd = NULL;
D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
//ID3D11Device*                       g_pd3dDevice = NULL;
//ID3D11DeviceContext*                g_pImmediateContext = NULL;
//IDXGISwapChain*                     g_pSwapChain = NULL;
ID3D11RenderTargetView*             g_pRenderTargetView = NULL;
ID3D11Texture2D*                    g_pDepthStencil = NULL;
ID3D11DepthStencilView*             g_pDepthStencilView = NULL;
ID3D11VertexShader*                 g_pVertexShader = NULL;
ID3D11PixelShader*                  g_pPixelShader = NULL;
//ID3D11InputLayout*                  g_pVertexLayout = NULL;
/*ID3D11Buffer*                       g_pVertexBuffer = NULL;
ID3D11Buffer*                       g_pIndexBuffer = NULL;
ID3D11Buffer*                       g_pCBNeverChanges = NULL;
ID3D11Buffer*                       g_pCBChangeOnResize = NULL;
ID3D11Buffer*                       g_pCBChangesEveryFrame = NULL;*/
//ID3D11ShaderResourceView*           g_pTextureRV = NULL;
//ID3D11SamplerState*                 g_pSamplerLinear = NULL;
XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;
XMFLOAT4                            g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void Render();


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  if (FAILED(InitWindow(hInstance, nCmdShow)))
    return 0;

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

  return (int) msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
  // Register class
  WNDCLASSEX wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, (LPCTSTR) IDI_TUTORIAL1);
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = L"TutorialWindowClass";
  wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR) IDI_TUTORIAL1);
  if (!RegisterClassEx(&wcex))
    return E_FAIL;

  // Create window
  g_hInst = hInstance;
  RECT rc = {0, 0, 640, 480};
  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
  g_hWnd = CreateWindow(L"TutorialWindowClass", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                        NULL);
  if (!g_hWnd)
    return E_FAIL;

  ShowWindow(g_hWnd, nCmdShow);

  return S_OK;
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
  HRESULT hr = S_OK;

  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

  ID3DBlob* pErrorBlob;
  hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
                             dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
  if (FAILED(hr))
  {
    if (pErrorBlob != NULL)
      OutputDebugStringA((char*) pErrorBlob->GetBufferPointer());
    if (pErrorBlob) pErrorBlob->Release();
    return hr;
  }
  if (pErrorBlob) pErrorBlob->Release();

  return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
  HRESULT hr = S_OK;
  bool isSuccesful = true;

  RECT rc;
  GetClientRect(g_hWnd, &rc);
  UINT width = rc.right - rc.left;
  UINT height = rc.bottom - rc.top;

  UINT createDeviceFlags = 0;
#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_DRIVER_TYPE driverTypes[] =
  {
      D3D_DRIVER_TYPE_HARDWARE,
      D3D_DRIVER_TYPE_WARP,
      D3D_DRIVER_TYPE_REFERENCE,
  };
  UINT numDriverTypes = ARRAYSIZE(driverTypes);

  D3D_FEATURE_LEVEL featureLevels[] =
  {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
  };
  UINT numFeatureLevels = ARRAYSIZE(featureLevels);

  DXGI_SWAP_CHAIN_DESC sd;
  SecureZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 1;
  sd.BufferDesc.Width = width;
  sd.BufferDesc.Height = height;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = g_hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  my_swapChain.setDescription(width, height, 28,//equivalent to DXGI_FORMAT_R8G8B8A8_UNORM
                              32,/*equivalent to DXGI_USAGE_RENDER_TARGET_OUTPUT*/ g_hWnd);


  for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
  {
    g_driverType = driverTypes[driverTypeIndex];
    hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType,
                                       NULL, createDeviceFlags,
                                       featureLevels, numFeatureLevels,
                                       D3D11_SDK_VERSION, &my_swapChain.getSwapChainDesc(),
                                       my_swapChain.getSwapChainRef(), my_device.getDeviceRef(),
                                       &g_featureLevel, my_deviceContext.getDeviceContextRef());
    if (SUCCEEDED(hr))
      break;
  }
  if (FAILED(hr))
    return hr;

  // Create a render target view
  isSuccesful = my_swapChain.GetBuffer(my_backBuffer, 0);
  assert(("Error with swap-chain getting a buffer ", isSuccesful == true));
  //hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) my_backBuffer.getTextureRef());
  //if (FAILED(hr))
  //  return hr;
  /// OLD CODE
  //hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
  isSuccesful = my_device.CreateRenderTargetView(my_backBuffer, my_renderTragetView);
  assert(isSuccesful == true, "Error with render-target creation");
  // Create depth stencil texture
  D3D11_TEXTURE2D_DESC descDepth;
  ZeroMemory(&descDepth, sizeof(descDepth));
  descDepth.Width = width;
  descDepth.Height = height;
  descDepth.MipLevels = 1;
  descDepth.ArraySize = 1;
  descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//DXGI_FORMAT_D24_UNORM_S8_UINT
  descDepth.SampleDesc.Count = 1;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = D3D11_USAGE_DEFAULT;
  descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  descDepth.CPUAccessFlags = 0;
  descDepth.MiscFlags = 0;

  sTextureDescriptor TextureDesc;
  memset(&TextureDesc, 0, sizeof(TextureDesc));
  TextureDesc.texHeight = height;
  TextureDesc.texWidth = width;
  TextureDesc.texFormat = 45;// equivalent to DXGI_FORMAT_D24_UNORM_S8_UINT
  TextureDesc.BindFlags = 0x40L;// equivalent to DD3D11_BIND_DEPTH_STENCIL
  TextureDesc.Usage = 0;// equivalent to D3D11_USAGE_DEFAULT
  TextureDesc.CpuAccess = 0;
  TextureDesc.arraySize = 1;

  isSuccesful = my_device.CreateTexture2D(TextureDesc, my_depthStencil);
  assert(isSuccesful == true, "Error with Texture 2d creation ");

  // Create the depth stencil view
  //D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  //SecureZeroMemory(&descDSV, sizeof(descDSV));
  //descDSV.Format = static_cast<DXGI_FORMAT>(TextureDesc.texFormat);
  //descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  //descDSV.Texture2D.MipSlice = 0;

  // my own descriptor 
  sDepthStencilDescriptor depthDesc;
  depthDesc.Format = TextureDesc.texFormat;
  depthDesc.Dimension = 3;// equivalent to D3D11_DSV_DIMENSION_TEXTURE2D 
  depthDesc.Mip = 0;

  isSuccesful = my_device.CreateDepthStencilView(my_depthStencil, depthDesc, my_depthStencilView);
  assert(isSuccesful == true, "Error with depth-stencil creation");

  my_deviceContext.OMSetRenderTargets(&my_renderTragetView,
                                      my_depthStencilView);

  //g_pImmediateContext->OMSetRenderTargets(1
  //                                        , my_renderTragetView.getRenderTragetViewRef(),
  //                                        my_depthStencillView.getDepthStencilView());
  // Setup the viewport
  my_viewport.setViewport(width, height,
                          0.0f, 1.0f);

  my_deviceContext.RSSetViewports(&my_viewport);
  //g_pImmediateContext->RSSetViewports(1, &vp);

  // Compile the vertex shader

  hr = CompileShaderFromFile(L"Tutorial07.fx",
                             "VS",
                             "vs_4_0",
                             my_vertexShader.getInfoRef());
  if (FAILED(hr))
  {
    MessageBox(NULL,
               L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
    return hr;
  }

  // Create the vertex shader
  isSuccesful = my_device.CreateVertexShader(my_vertexShader);
  assert(isSuccesful == true, "Error creating vertex shader");
  // Define the input layout
  //D3D11_INPUT_ELEMENT_DESC layout[] =
  //{
  //    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  //    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  //};

  sInputDescriptor my_layout[] =
  {
    {"POSITION",
    0
    ,6/*equivalent to DXGI_FORMAT_R32G32B32_FLOAT*/
    ,0
    ,0
    ,0/*equivalent to D3D11_INPUT_PER_VERTEX_DATA*/
    ,0},

    {"TEXCOORD",
    0,
    16,
    0,
    12,
    0,
    0}
  };

  UINT numElements = ARRAYSIZE(my_layout);
  // Create the input layout
  isSuccesful = my_device.CreateInputLayout(my_vertexInputLayout,
                                            numElements,
                                            my_layout,
                                            my_vertexShader);

  assert(isSuccesful == true, "Error creating Input layout ");
  //hr = g_pd3dDevice->CreateInputLayout(layout,
  //                                     numElements,
  //                                     my_vertexShader.getInfo()->GetBufferPointer(),
  //                                     my_vertexShader.getInfo()->GetBufferSize(), 
  //                                     &g_pVertexLayout);

  // Set the input layout
  my_deviceContext.IASetInputLayout(my_vertexInputLayout);

  // Compile the pixel shader
  hr = CompileShaderFromFile(L"Tutorial07.fx"
                             , "PS"
                             , "ps_4_0"
                             , my_pixelShader.getInfoRef());

  if (FAILED(hr))
  {
    MessageBox(NULL,
               L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
    return hr;
  }

  // Create the pixel shader
  isSuccesful = my_device.CreatePixelShader(my_pixelShader);
  assert((isSuccesful == true, "Error creating the pixel shader"));
  //hr = g_pd3dDevice->CreatePixelShader(
  //  pPSBlob->GetBufferPointer()
  //  , pPSBlob->GetBufferSize()
  //  , NULL
  //  , &g_pPixelShader);

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

  // Set primitive topology
  my_deviceContext.IASetPrimitiveTopology(4);//equivalent to D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
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
//    return hr;

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
  hr = D3DX11CreateShaderResourceViewFromFile(my_device.getDevice(),
                                              L"seafloor.dds"
                                              , NULL
                                              , NULL
                                              , my_shaderResourceView.getShaderResourceRef()
                                              , NULL);

  if (FAILED(hr))
    return hr;

  // Create the sample state
  D3D11_SAMPLER_DESC sampDesc;
  SecureZeroMemory(&sampDesc, sizeof(sampDesc));
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

  my_sampler.setDescirption(0x15,// equivalent to D3D11_FILTER_MIN_MAG_MIP_LINEAR
                            1, //equivalent to D3D11_TEXTURE_ADDRESS_WRAP
                            1,
                            1,
                            1);//equivalent to D3D11_COMPARISON_NEVER

  isSuccesful = my_device.CreateSamplerState(my_sampler);
  assert(isSuccesful == true, "Error with creating sampler state");
  //hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
  //if (FAILED(hr))
  //  return hr;

  // Initialize the world matrices
  g_World = XMMatrixIdentity();

  // Initialize the view matrix
  XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
  XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  g_View = XMMatrixLookAtLH(Eye, At, Up);

  CBNeverChanges cbNeverChanges;
  cbNeverChanges.mView = XMMatrixTranspose(g_View);
  my_deviceContext.UpdateSubresource(&my_constNeverChanges, &cbNeverChanges);
  //g_pImmediateContext->UpdateSubresource(my_constNeverChanges.getBuffer(),
  //                                       0,
  //                                       NULL,
  //                                       &cbNeverChanges,
  //                                       0,
  //                                       0);

  // Initialize the projection matrix
  g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4,
                                          width / (FLOAT) height,
                                          0.01f,
                                          100.0f);

  CBChangeOnResize cbChangesOnResize;
  cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
  my_deviceContext.UpdateSubresource(&my_constChangeOnResize, &cbChangesOnResize);
  //g_pImmediateContext->UpdateSubresource(my_constChangeOnResize.getBuffer(),
  //                                       0,
  //                                       NULL,
  //                                       &cbChangesOnResize,
  //                                       0,
  //                                       0);

  return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
  //if (g_pImmediateContext) g_pImmediateContext->ClearState();

  // if (g_pSamplerLinear) g_pSamplerLinear->Release();
  //if (g_pTextureRV) g_pTextureRV->Release(); //
  //if (g_pCBNeverChanges) g_pCBNeverChanges->Release();
  //if (g_pCBChangeOnResize) g_pCBChangeOnResize->Release();
  //if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();
  //if (g_pVertexBuffer) g_pVertexBuffer->Release();
  //if (g_pIndexBuffer) g_pIndexBuffer->Release();
//  if (g_pVertexLayout) g_pVertexLayout->Release();
  if (g_pVertexShader) g_pVertexShader->Release();
  if (g_pPixelShader) g_pPixelShader->Release();
  if (g_pDepthStencil) g_pDepthStencil->Release();
  if (g_pDepthStencilView) g_pDepthStencilView->Release();
  if (g_pRenderTargetView) g_pRenderTargetView->Release();
  //  if (g_pSwapChain) g_pSwapChain->Release();
    // if (g_pImmediateContext) g_pImmediateContext->Release();
     //  if (g_pd3dDevice) g_pd3dDevice->Release();
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message)
  {
    case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      EndPaint(hWnd, &ps);
      break;

    case WM_DESTROY:
      PostQuitMessage(0);
      break;

    default:
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
  if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
  {
    t += (float) XM_PI * 0.0125f;
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
  g_World = XMMatrixRotationY(t);

  // Modify the color
  g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
  g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
  g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;
  //
  // Clear the back buffer
  //
  float ClearColor[4] = {0.0f, 0.125f, 0.3f, 1.0f}; // red, green, blue, alpha
  my_deviceContext.ClearRenderTargetView(my_renderTragetView);
  //g_pImmediateContext->ClearRenderTargetView(my_renderTragetView.getRenderTragetView()
  //                                           , ClearColor);


  //
  // Clear the depth buffer to 1.0 (max depth)
  //

  my_deviceContext.ClearDepthStencilView(my_depthStencilView);
  //g_pImmediateContext->ClearDepthStencilView(my_depthStencilView.getDepthStencilView()
  //                                           , D3D11_CLEAR_DEPTH, 1.0f
  //                                           , 0);

  /* setup for manipulating 3d models */
  XMVECTOR moveVector = {2,1,1,1};
  XMVECTOR scaleVector = {1,1,std::fabs(std::cosf(t)),1};
  XMVECTOR rotVector = {-t,0, t,1};
  // CBChangesEveryFrame cb;
  XMMATRIX tempMatrixMove = XMMatrixTranspose(XMMatrixTranslationFromVector(moveVector));
  XMMATRIX tempMatrixScale = XMMatrixScalingFromVector(scaleVector);
  XMMATRIX tempMatrixRotate = XMMatrixRotationRollPitchYawFromVector(rotVector);//XMMatrixRotationZ(-t);

  XMVECTOR moveVector2 = {0,1,-1,1};
  XMVECTOR scaleVector2 = {1,1,std::fabs(std::cosf(t)) * 2,1};
  XMVECTOR rotVector2 = { 0,0, -t,1};

  XMMATRIX tempMatrixMove2 = XMMatrixTranspose(XMMatrixTranslationFromVector(moveVector2));
  XMMATRIX tempMatrixScale2 = XMMatrixScalingFromVector(scaleVector2);
  XMMATRIX tempMatrixRotate2 = XMMatrixRotationRollPitchYawFromVector(rotVector2);//XMMatrixRotationZ(-t);
  CBChangesEveryFrame cb;
  cb.mWorld = XMMatrixMultiply(XMMatrixMultiply(tempMatrixMove2, tempMatrixRotate2),tempMatrixScale2) ;
  cb.vMeshColor = g_vMeshColor;
  my_deviceContext.UpdateSubresource(&my_constChangesEveryFrame, &cb);
  //g_pImmediateContext->UpdateSubresource(my_constChangesEveryFrame.getBuffer(),
  //                                       0, NULL,
  //                                       &cb, 0,
  //                                       0);


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
  my_deviceContext.DrawIndexed(36, 0);
  /// PRIVIOUS FUNCTIONS
  //g_pImmediateContext->VSSetShader(my_vertexShader.getVertexShader(), NULL, 0);// DONE

  //g_pImmediateContext->VSSetConstantBuffers(0, 1, my_constNeverChanges.getBufferRef());      //DONE
  //g_pImmediateContext->VSSetConstantBuffers(1, 1, my_constChangeOnResize.getBufferRef());//DONE
  //g_pImmediateContext->VSSetConstantBuffers(2, 1, my_constChangesEveryFrame.getBufferRef());//DONE

  //g_pImmediateContext->PSSetShader(my_pixelShader.getPixelShader(), NULL, 0);
  //g_pImmediateContext->PSSetConstantBuffers(2, 1, my_constChangesEveryFrame.getBufferRef());
  //g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
  //g_pImmediateContext->PSSetSamplers(0, 1, my_sampler.getSamplerRef());
  //g_pImmediateContext->DrawIndexed(36, 0, 0);



  cb.mWorld = XMMatrixMultiply(tempMatrixMove, tempMatrixScale);//XMMatrixTranspose(XMMatrixTranslationFromVector(moveVector));

  cb.vMeshColor = g_vMeshColor;
  my_deviceContext.UpdateSubresource(&my_constChangesEveryFrame, &cb);

  my_deviceContext.DrawIndexed(36, 0);

  XMVECTOR Move2 = {-3,1,1,0};
  // CBChangesEveryFrame cb;
  tempMatrixMove = XMMatrixTranspose(XMMatrixTranslationFromVector(Move2));

  cb.mWorld = XMMatrixMultiply(tempMatrixMove, tempMatrixRotate);
  cb.vMeshColor = g_vMeshColor;
  my_deviceContext.UpdateSubresource(&my_constChangesEveryFrame, &cb);

  my_deviceContext.DrawIndexed(36, 0);
  //
  // Present our back buffer to our front buffer
  //

  // returns a boolean value 
  my_swapChain.Present(0, 0);
}