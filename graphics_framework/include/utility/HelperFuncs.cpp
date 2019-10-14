#include "utility/Grafics_libs.h"
#include "HelperFuncs.h"
#include "cDevice.h"
#include "cDeviceContext.h"
#include "cSwapChain.h"
#include "cWindow.h"
#include "cApiComponents.h"
#include "enum_headers/enFormatEnums.h"
#include  "cShaderBase.h"
#include "actor/cActor.h"
#include "../include/cModel.h"
#include "../include/cCameraManager.h"
#include <commdlg.h> // for the open file function 

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
  #elif OPEN_GL
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
  #elif OPEN_GL
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
  #elif OPEN_GL
  #endif // DIRECTX
    return sTextureDescriptor();
  }

  std::string
    openFile(cWindow window)
  {
  #if WIND_OS
    OPENFILENAMEA File;
    std::memset(&File, 0, sizeof(File));
    char FileName[4096];

    std::memset(&FileName, 0, sizeof(FileName));
    /* First \0 describes the name of the options
    that the user will be presented with, the second \0
    will discribe the type of files to look for */
    //   static constexpr const char* FileTypes = 
    File.lStructSize = sizeof(OPENFILENAMEA);
    File.hwndOwner = window.getHandle();
    File.lpstrFile = FileName;
    File.lpstrFile[0] = '\0';
    File.nMaxFile = 4096;
    File.lpstrFilter = "All files\0*.*\0obj files\0*.obj\0fbx files\0*.fbx\0";
    //FileTypes;
    File.nFilterIndex = 1;

    GetOpenFileNameA(&File);
  #endif // WIND_OS

    return std::string(FileName);
  }


  cModel *
    findModelComponent(cActor & actor)
  {
    for (auto iter = actor.getIteratorBegin(); iter != actor.getIteratorEnd(); ++iter)
    {
      baseComponent *ptr_possibleModel = *iter;
      cModel * model = dynamic_cast<cModel*>(ptr_possibleModel);
      if (model == nullptr) { continue; }
      else { return model; }
    }
    return nullptr;
  }
  /*************/



  void handelCameraKeyInput(const uint8 pressedKey, cCameraManager & currentCamera,
                            cWindow & window, cDeviceContext &deviceContext,
                            cConstBuffer *neverChange, cConstBuffer *resizeChange)
  {
  #if DIRECTX

    // used to alter the view matrix 
    GlViewMatrix ChangeWithViewMatrix;
    GlProjectionMatrix  ChangeOnProjectionChange;
    // going forwards 
    if (pressedKey == (WPARAM)'W')
    {
      currentCamera.moveFront(1.0f, window);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast<cBuffer*>(neverChange),
                                      &ChangeWithViewMatrix);
    }//going backwards
    else if (pressedKey == (WPARAM)'S')
    {
      currentCamera.moveFront(-1.0f, window);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast<cBuffer*>(neverChange),
                                      &ChangeWithViewMatrix);
    }//going right
    else if (pressedKey == (WPARAM)'D')
    {
      currentCamera.moveRight(1.0f, window);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast<cBuffer*>(neverChange),
                                      &ChangeWithViewMatrix);
    }//going left 
    else if (pressedKey == (WPARAM)'A')
    {
      currentCamera.moveRight(-1.0f, window);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast<cBuffer*>(neverChange),
                                      &ChangeWithViewMatrix);
    }
    //going up 
    else if (pressedKey == (WPARAM)'E')
    {
      currentCamera.moveUp(1.0f, window);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast<cBuffer*>(neverChange),
                                      &ChangeWithViewMatrix);
    }//going down 
    else if (pressedKey == (WPARAM)'Q')
    {
      currentCamera.moveUp(-1.0f, window);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast<cBuffer*>(neverChange),
                                      &ChangeWithViewMatrix);
    }
    else if (pressedKey == (WPARAM)'1')
    {
      currentCamera.switchCamera(0);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast<cBuffer*>(neverChange),
                                      &ChangeWithViewMatrix);
      ChangeOnProjectionChange.matrix = currentCamera.getProjectionMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast<cBuffer*>(resizeChange),
                                      &ChangeOnProjectionChange);
    }
    else if (pressedKey == (WPARAM)'2')
    {
      currentCamera.switchCamera(1);
      ChangeWithViewMatrix.matrix = currentCamera.getViewMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast<cBuffer*>(neverChange),
                                      &ChangeWithViewMatrix);
      //for changing the projection
      ChangeOnProjectionChange.matrix = currentCamera.getProjectionMatrix().matrix;
      deviceContext.UpdateSubresource(reinterpret_cast<cBuffer*>(resizeChange),
                                      &ChangeOnProjectionChange);
    }
  #elif OPEN_GL 
  #endif // DIRECTX

  }

  /*****************/

  void handelActorTransforms(cActor & actor, const uint8 chosenAxis, const uint8 pressedKey, float TransformAmount)
  {
    if (pressedKey == (WPARAM)'R')
    {
      actor.m_transform.resetToIdentity();
    }

    else if (pressedKey == VK_RIGHT)
    {
      switch (chosenAxis)
      {
        case 0:
          actor.m_transform.rotateInXAxis(TransformAmount);
          break;

        case 1:
          actor.m_transform.rotateInYAxis(TransformAmount);
          break;

        case 2:
          actor.m_transform.rotateInZAxis(TransformAmount);
          break;
      }
    }

    else if (pressedKey == VK_LEFT)
    {
      switch (chosenAxis)
      {
        case 0:
          actor.m_transform.rotateInXAxis(-TransformAmount);
          break;
        case 1:
          actor.m_transform.rotateInYAxis(-TransformAmount);
          break;
        case 2:
          actor.m_transform.rotateInZAxis(-TransformAmount);
          break;
      }
    }

    if (pressedKey == (WPARAM)'U')
    {
      switch (chosenAxis)
      {
        case 0:
          actor.m_transform.shearTransformInXAxis(TransformAmount);
          break;
        case 1:
          actor.m_transform.shearTransformInYAxis(TransformAmount);
          break;
        case 2:
          actor.m_transform.shearTransformInZAxis(TransformAmount);
          break;
      }
    }

    else if (pressedKey == (WPARAM)'I')
    {
      switch (chosenAxis)
      {
        case 0:
          actor.m_transform.shearTransformInXAxis(-TransformAmount);
          break;
        case 1:
          actor.m_transform.shearTransformInYAxis(-TransformAmount);
          break;
        case 2:
          actor.m_transform.shearTransformInZAxis(-TransformAmount);
          break;
      }
    }

    else if (pressedKey == (WPARAM)'O')
    {
      switch (chosenAxis)
      {
        case 0:
          actor.m_transform.reflectTransfromInXAxis(TransformAmount);
          break;
        case 1:
          actor.m_transform.reflectTransfromInYAxis(TransformAmount);
          break;
        case 2:
          actor.m_transform.reflectTransfromInZAxis(TransformAmount);
          break;
      }
    }

    if (pressedKey == (WPARAM)'P')
    {
      switch (chosenAxis)
      {
        case 0:
          actor.m_transform.reflectTransfromInXAxis(-TransformAmount);
          break;
        case 1:
          actor.m_transform.reflectTransfromInYAxis(-TransformAmount);
          break;
        case 2:
          actor.m_transform.reflectTransfromInZAxis(-TransformAmount);
          break;
      }
    }

    //move forwards 
    if (pressedKey == (WPARAM)'T')
    {
      actor.m_transform.moveTransform(0.0f, 0.0f, TransformAmount, 0.0f);
    }
    //move backwards 
    else if (pressedKey == (WPARAM)'G')
    {
      actor.m_transform.moveTransform(0.0f, 0.0f, -TransformAmount, 0.0f);
    }
    //move left 
    else if (pressedKey == (WPARAM)'F')
    {
      actor.m_transform.moveTransform(-TransformAmount, 0.0f, 0.0f, 0.0f);
    }
    // move right 
    else if (pressedKey == (WPARAM)'H')
    {
      actor.m_transform.moveTransform(TransformAmount, 0.0f, 0.0f, 0.0f);
    }

    else if (pressedKey == (WPARAM)'V')
    {
      actor.m_transform.moveTransform(0.f, TransformAmount, 0.0f, 0.f);
    }

    else if (pressedKey == (WPARAM)'N')
    {
      actor.m_transform.moveTransform(0.f, -TransformAmount, 0.f, 0.f);
    }
  }

}
/*****************/


