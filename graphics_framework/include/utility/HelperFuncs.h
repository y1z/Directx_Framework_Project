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
class cModel;
class cActor;
class cCameraManager;
class cConstBuffer;
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
  /*!converts radians to degrees*/
  float
    radiansToDegrees(float radians);
  /*!converts degrees to radians*/
  float
    degreesToRadians(float degrees);

  sTextureDescriptor
    createDepthStencilDesc(uint32 width, uint32 height);

  [[nodiscard]]std::string
    openFile(cWindow handel);
  /*! return a pointer to a model if the actor happens to have one */
  [[nodiscard]] cModel* 
    findModelComponent(cActor &actor);
/*! take care of operation that uses a key press and the camera */
  void
    handelCameraKeyInput(const uint8 pressedKey, cCameraManager &currentCamera, 
                      cWindow &window,cDeviceContext &deviceContext,
                         cConstBuffer *neverChange, cConstBuffer *resizeChange);
}

