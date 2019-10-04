#pragma once
//#include "enum_headers/enumComponentTypes.h"
////class cDevice;
//class cDeviceContext;
//
//class baseComponent
//{
//public:
//  baseComponent();
//  virtual ~baseComponent() = default;
//public:
//  /*! this function will be used to know if the component is ready to be used */
//  virtual bool isReady()const = 0;
//  /*! use when isReady returns true */
//  virtual void Init() = 0;
//  /*! this is so the component can draw it self */
//  virtual void Draw(cDeviceContext &devContext) = 0;
///*! use this to eliminate resources associated with component*/
//  virtual void Destroy() = 0;
//protected:
//  /*! use this to signal that the component is ready to be used */
//  void setReady(bool Readyness);
//  /*! use this to set the type of the component*/
//  void setComponentType(componentTypes type);
//private:
//  bool m_Ready;
//  componentTypes m_componentType;
//};
//
