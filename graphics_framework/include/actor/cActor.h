#pragma once
#include "baseComponent.h"
#include "../include/actor/cTransform.h"
#include <vector>

class cDeviceContext;
class cConstBuffer;
/*!
\brief basically it's the controller for components, what are components ? THAT depends an
the component it self here are some examples , Axis Aligned Bounding Box(collision) , Transform(Positioning) ,
Model(a 3d model ) 
*/
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

  /*! remove all components from the actor and
  all resources that actor had */
  void
    DestroyAllComponents();

  //! add components to the actor 
  void
    AddComponents(baseComponent *component);

  //! find out how many component the actor has
  std::size_t
    getComponentCount()const;

  baseComponent*
    getComponent(std::size_t Index);

/*! returns a iterator to the beginning of the container of 
components*/
  std::vector<baseComponent*>::iterator
    getIteratorBegin();

/*! returns a iterator to the end of the container of 
components*/
  std::vector<baseComponent*>::const_iterator
    getIteratorEnd();

/*! update the components transform */
  bool 
    update(cDeviceContext &deviceContext);

  /*! dictates all operation related with moving, deforming ,rotating
  of the components of the actor */
  cTransform  m_transform;
private:
  /*! contains all the component of the actor */
  std::vector<baseComponent*> m_components;
  // TODO : this is not used at the moment 
  cActor *mptr_child = nullptr;
};
