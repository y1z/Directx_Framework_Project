#pragma once
#include "enum_headers/enumComponentTypes.h"
#include <vector>

//Forward declarations
class cDeviceContext;
class cDevice;
class cConstBuffer;

/*! this is a interface to component objects 
*/
class baseComponent
{
public://constructor and destructor  
  baseComponent();
  virtual ~baseComponent() = default;
public:// functions 
  /*! this function will be used to know if the component is ready to be used */
  virtual bool 
    isReady()const = 0;
  /*! use when isReady returns true */
  virtual void 
    Init(cDevice &device, cDeviceContext &deviceContext) = 0;
  /*! this is so the component can draw it self */
  virtual void 
    Draw(cDeviceContext &devContext,std::vector<cConstBuffer*> &constBuffers) = 0;
  /*! use this to eliminate resources associated with component*/
  virtual void 
    Destroy() = 0;
protected:
  /*! use this to signal that the component is ready to be used */
  void 
    setReady(bool Readyness);
  /*!this set the component type  */
  void 
    setComponentType(componentTypes type);
  /*! returns the component type */
  componentTypes 
    getComponentType()const;
protected:
  //! use to know if the component is ready to use 
  bool m_Ready;
  //! help's distinguish the drive classes
  componentTypes m_componentType;
};
