#pragma once
#include "baseComponent.h"
#include "../include/actor/cTransform.h"
#include <vector>

class cConstBuffer;

class cActor
{
public:
  cActor();
   ~cActor();

public:
  /*!draws a component if possible*/
  void 
    DrawComponent(std::size_t Index, cDeviceContext &deviceContext, std::vector<cConstBuffer*> &buffers);
  /*! initializes the component*/
  void 
    InitComponent(std::size_t Index, cDeviceContext &deviceContext, cDevice &device);
  /*! release all resource related with the object*/
  void
    DestroyComponent(std::size_t Index);
  //! add components to the actor 
  void 
    AddComponents(baseComponent *component);
  //! find out how many component the actor has
  std::size_t 
    getComponentCount()const;

  baseComponent* 
    getComponent(std::size_t Index );

  cTransform  m_transform;
private:
  std::vector<baseComponent*> m_components;
  // TODO : this is not used at the moment 
  cActor *mptr_child = nullptr;
};
