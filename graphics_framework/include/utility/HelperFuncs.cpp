#include <d3d11.h>
#include <d3dcompiler.h>
#include "HelperFuncs.h"
#include "cDevice.h"
#include "cDeviceContext.h"
#include "cSwapChain.h"
#include "cWindow.h"
#include "cApiComponents.h"
#include "enum_headers/enFormatEnums.h"
#include  "cShaderBase.h"

namespace helper
{

  bool
    CreateDeviceAndSwapchain(cDevice & device, cDeviceContext & deviceContext,
                             cSwapChain &swapChain, cWindow & window,
                             cApiComponents &apiComponent)
  {
  #if DIRECTX
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(window.getHandle(), &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
  #ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
  #endif
    D3D_DRIVER_TYPE  driverType;

    D3D_FEATURE_LEVEL  featureLevel;

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
      D3D_FEATURE_LEVEL_12_0	,
      D3D_FEATURE_LEVEL_11_1,
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
      D3D_FEATURE_LEVEL_9_3	,
      D3D_FEATURE_LEVEL_9_2,
      D3D_FEATURE_LEVEL_9_1	,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    swapChain.setSwapChain(width, height, Formats::R8G8B8A8_uniform_norm,//equivalent to DXGI_FORMAT_R8G8B8A8_UNORM
                           32,/*equivalent to DXGI_USAGE_RENDER_TARGET_OUTPUT*/ window.getHandle());


    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
      driverType = driverTypes[driverTypeIndex];
      hr = D3D11CreateDeviceAndSwapChain(NULL, driverType,
                                         NULL, createDeviceFlags,
                                         featureLevels, numFeatureLevels,
                                         D3D11_SDK_VERSION, &swapChain.getSwapChainDesc(),
                                         swapChain.getSwapChainRef(), device.getDeviceRef(),
                                         &featureLevel, deviceContext.getDeviceContextRef());
      if (SUCCEEDED(hr))
      {
        apiComponent.setSupportedVersion(static_cast<int>(featureLevel));
        apiComponent.setHardwareVersion(static_cast<int> (driverType));
        return true;
      }
    }
    return false;
  #else
  #endif // DIRECTX
  } // end function


  bool
    CompileShader(const wchar_t * FileName, const char * shaderModel,
                  const char * entryPoint, cShaderBase & shader)
  {
  #ifdef DIRECTX
    HRESULT hr = S_FALSE;
    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
  #if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
  #endif

    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompileFromFile(FileName, NULL,
                            NULL, entryPoint,
                            shaderModel, dwShaderFlags,
                            0, shader.getInfoRef(), &pErrorBlob);

    if (FAILED(hr))
    {
      if (pErrorBlob != NULL)
        OutputDebugStringA((char*) pErrorBlob->GetBufferPointer());
      if (pErrorBlob) pErrorBlob->Release();
      return  false;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return true;
  #else
  #endif // DIRECTX
    return false;
  }

  float
    radiansToDegrees(float radians)
  {
    return radians *= (180.0f / 3.14159f);
  }

  float
    degreesToRadians(float degrees)
  {
    return degrees *= (3.14159f / 180.0f);
  }

  sTextureDescriptor createDepthStencilDesc(uint32 width, uint32 height)
  {
  #if DIRECTX
    sTextureDescriptor TextureDesc;
    memset(&TextureDesc, 0, sizeof(TextureDesc));
    TextureDesc.texHeight = height;
    TextureDesc.texWidth = width;
    TextureDesc.texFormat = Formats::depthStencil_format;// equivalent to DXGI_FORMAT_D24_UNORM_S8_UINT
    TextureDesc.BindFlags = 0x40L;// equivalent to DD3D11_BIND_DEPTH_STENCIL
    TextureDesc.Usage = 0;// equivalent to D3D11_USAGE_DEFAULT
    TextureDesc.CpuAccess = 0;
    TextureDesc.arraySize = 1;
    return TextureDesc;
  #else
  #endif // DIRECTX
    return sTextureDescriptor();
  }

}
