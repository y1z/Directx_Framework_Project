#include "cResourceManager.h"
#include "cSwapChain.h"
#include "cWindow.h"
#include "cApiComponents.h"
#include "utility/enGraphics.h"
#include "utility/HelperFuncs.h"
// std includes 
#include <cstdint>

cResourceManager::cResourceManager()
{
  mptr_device = std::make_shared<cDevice>();
  mptr_deviceContext = std::make_shared<cDeviceContext>();
}

cResourceManager::~cResourceManager()
{}

bool
cResourceManager::init(cSwapChain & swapChain,
                       cWindow & window,
                       cApiComponents &apiComponent)
{
#if DIRECTX
  HRESULT hr = S_OK;

  RECT rc;
  GetClientRect(window.getHandle(), &rc);
  uint32_t  width = rc.right - rc.left;
  uint32_t  height = rc.bottom - rc.top;

  uint32_t createDeviceFlags = 0;
#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_DRIVER_TYPE driverTypes[] =
  {
      D3D_DRIVER_TYPE_HARDWARE,
      D3D_DRIVER_TYPE_WARP,
      D3D_DRIVER_TYPE_REFERENCE,
  };
  uint32_t numDriverTypes = ARRAYSIZE(driverTypes);

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
  uint32_t numFeatureLevels = ARRAYSIZE(featureLevels);

  swapChain.setSwapChain(width, height, enFormats::R8G8B8A8_uniform_norm,//equivalent to DXGI_FORMAT_R8G8B8A8_UNORM
                         static_cast< int >(enBufferUse::renderTragetOut),/*equivalent to DXGI_USAGE_RENDER_TARGET_OUTPUT*/ window);

  D3D_DRIVER_TYPE driverType;
  D3D_FEATURE_LEVEL featureLevel;

  for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
  {
    driverType = driverTypes[driverTypeIndex];
    hr = D3D11CreateDeviceAndSwapChain(NULL, driverType,
                                       NULL, createDeviceFlags,
                                       featureLevels, numFeatureLevels,
                                       D3D11_SDK_VERSION, &swapChain.getSwapChainDesc(),
                                       swapChain.getSwapChainRef(), mptr_device->getDeviceRef(),
                                       &featureLevel, mptr_deviceContext->getDeviceContextRef());

    if (SUCCEEDED(hr))
    {
      apiComponent.setSupportedVersion(static_cast< int >(featureLevel));
      apiComponent.setHardwareVersion(static_cast< int > (driverType));
      return true;
    }
  }
#elif OPEN_GL
  GlRemoveAllErrors();

  swapChain.setGlWindow(window.getHandle());

  int32 majorVersion{ 0 };
  int32 minorVersion{ 0 };

  glfwGetVersion(&majorVersion, &minorVersion, NULL);

  const unsigned char * OpenglVersion = glGetString(GL_VERSION);
  const unsigned char *GlslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
  const unsigned char *OpenglRenderer = glGetString(GL_RENDERER);

  std::cout << "GLFW version : " << "Major [" << majorVersion << "] Minor [" << minorVersion << "]\n"
    << "Open_gl version : " << OpenglVersion << '\n'
    << "Glsl  Shader version : " << GlslVersion << '\n'
    << "Open_gl renderer : " << OpenglRenderer << '\n' <<
    "alignment of sLightData : " << alignof(sLightData) << '\n' <<
    " size of sLightData : " << sizeof(sLightData) << std::endl;

  apiComponent.setSupportedVersion(majorVersion, minorVersion);

  uint32* ptr_vertexArrayObject = cApiComponents::getvertexArrayObject();

  glGenVertexArrays(1, ptr_vertexArrayObject);
  glBindVertexArray(*ptr_vertexArrayObject);

  glfwSwapInterval(1);

  uint32* ptr_ShaderProgram = cApiComponents::getShaderProgram();

  *(ptr_ShaderProgram) = glCreateProgram();

  if (GlCheckForError())
  {
    EN_LOG_ERROR
      return false;
  }

  return true;
#endif // DIRECTX
  return false;
}

std::shared_ptr<cDevice>
cResourceManager::getPtrDevice()
{
  return mptr_device;
}

std::shared_ptr <cDeviceContext>
cResourceManager::getPtrDeviceContext()
{
  return mptr_deviceContext;
}








