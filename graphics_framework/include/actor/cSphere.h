#pragma once
#include <glm/glm.hpp>
#include "baseComponent.h"
class cMesh;
class cModel;
/*! this is a sphere collider  */
class cSphere : public baseComponent

{
public:// constructor 
  cSphere();
  ~cSphere() = default;
public:// functions 
/*! this will find the center/middle point of the geometry
of the model */
  void
    init(const cModel &model);

  /*check if 2 spheres are colliding */
  bool
    checkCollsion(const cSphere &otherSphere);
  /***get the center point ***/
  /*!*/
  float
    getCenterX()const;

  float
    getCenterY()const;

  float
    getCenterZ()const;
  /***get the radius ***/
  float
    getRadiusX()const;

  float
    getRadiusY()const;

  float
    getRadiusZ()const;
public://inherited virtual functions  

  bool
    isReady()const override;
private:
  /*! use when isReady returns true */
  void
    Init(cDevice &device, cDeviceContext &deviceContext)override;
  /*! this is so the component can draw it self */
  void
    Draw(cDeviceContext &devContext, std::vector<cConstBuffer*> &constBuffers)override;

public:
  void
    update(cDeviceContext &deviceContext, const sMatrix4x4 &Transform)override;
  /*! use this to eliminate resources associated with component*/
  void
    Destroy();

private:
  glm::vec4 m_centerPoint;
  glm::vec3 m_radius;
};

