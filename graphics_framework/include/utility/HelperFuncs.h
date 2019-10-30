#pragma once
#include "enDefs.h"
#include "CustomStructs.h"
#include "enErrorHandling.h"
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
class cVertexShader;
class cPixelShader;
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
    openFile(cWindow &handel);

  /*! return a pointer to a model if the actor happens to have one */
  [[nodiscard]] cModel*
    findModelComponent(cActor &actor);

  /*! load a file then convert-it to a string */
  std::string
    loadFileToString(std::string_view filePath);

  /*! load a file then convert-it to a string */
  std::string
    loadFileToString(std::wstring_view filePath);

  /*! take care of operation that uses a key press and the camera */
  void
    handelCameraKeyInput(const uint16 pressedKey, cCameraManager &currentCamera,
                         cWindow &window, cDeviceContext &deviceContext,
                         cConstBuffer *neverChange, cConstBuffer *resizeChange
                         , float deltaTime = 0.01666f);

  /*! take care of all transform related with the actor
 rotation ,shearing, reflection etc..  */
  void
    handelActorTransforms(cActor &actor, const uint8 chosenAxis,
                          const uint16 pressedKey, float TransformAmount);

  /*!\brief returns the size of the window in form of a struct
    'sWindowSize'
    \param window [in] the window to get the size of.
  */
  sWindowSize
    getWindowSize(cWindow &window);

  /*! take a std::wstring/wchar_t and converts it
  to a std::string */
  std::string
    convertWStringToString(std::wstring_view wideString);

  /*! this arranges the matrices to be column/row depending on the api
  */
  void
    arrangeForApi(sMatrix4x4 &Original);

  /*@brief takes a value from the keyboard input and convert to a value
  * that the framework understands
  */
  int32
    convertKeyValue(const int32 originalValue);


  /**
  *@param [in] lowerMultiplier will control the lowest value to multiple the result's from the 
  * sin and cos functions 
  *@param [in] upperMultiplier will control the largest value to multiple the result's from the 
  * sin and cos functions 
  */
  [[nodiscard]] cModel *
    createHelicoid( float lowerMultiplier, float upperMultiplier,
                     float lowerAngle, float upperAngle, 
                   uint32_t division, cDevice &device);

}


