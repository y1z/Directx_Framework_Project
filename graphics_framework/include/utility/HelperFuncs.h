#pragma once
#include "enDefs.h"
#include <string>
// forward Declaration
class cDevice;
class cDeviceContext;
class cSwapChain;
class cWindow;
class cApiComponents;
class cShaderResourceView;
class cShaderBase;
class cDepthStencilView;
/********************************/
struct sDepthStencilDescriptor;
struct sTextureDescriptor;

namespace helper
{
  /*! Create and unite the swap-chain device */
  bool
    CreateDeviceAndSwapchain(cDevice &device, cDeviceContext &deviceContext,
                             cSwapChain &swapChian, cWindow &window,
                             cApiComponents &ApiComponent);

  /*! used to compile a shader */
  bool
    CompileShader(const wchar_t *FileName, const char*shaderModel,
                  const char*entryPoint, cShaderBase &shader);

  float
    radiansToDegrees(float radians);

  float
    degreesToRadians(float degrees);

  sTextureDescriptor
    createDepthStencilDesc(uint32 width, uint32 height);

  std::string
    openFile(cWindow handel);
}

