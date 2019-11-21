#pragma once
#include "cDevice.h"
#include "cDeviceContext.h"
//std includes 
#include <memory>

class cSwapChain;
class cWindow;
class cApiComponents;

/**
* @brief takes care of creating and setting resources
*/
class cResourceManager
{
public:
  cResourceManager();

  ~cResourceManager();
public:

  /**
  *@brief starts all necessary classes for creating and setting
  *resources.
  */
  bool
    init(cSwapChain &swapChain,
         cWindow &window,
         cApiComponents &apiComponents);

  std::shared_ptr<cDevice>
    getPtrDevice();

  std::shared_ptr <cDeviceContext>
    getPtrDeviceContext();

private:
  /** takes care of creating resources*/
  std::shared_ptr<cDevice> mptr_device;

  /** takes care of setting resources*/
  std::shared_ptr< cDeviceContext > mptr_deviceContext;
};

