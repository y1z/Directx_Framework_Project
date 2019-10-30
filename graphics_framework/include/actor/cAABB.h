#pragma once
#include "glm/vec3.hpp"
#include "utility/CustomStructs.h"
#include "baseComponent.h"

class cMesh;
class cModel;

class cAABB : public baseComponent
{
public:
  cAABB();
  ~cAABB() = default ;
public: // functions 
/*! requires an model to calculate the min and max */
  void 
    init(const cModel &model);

  bool
    checkCollision(const cAABB &otherAABB);

  void
    moveAABB(const glm::mat4 &newTransform);

  //glm::vec3
  //  getMiddlePoint() const ;

  //glm::vec3
  //  getRadius()const;

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
  //glm::vec3 m_middlePoint;
  //glm::vec3 m_radius;
  glm::vec3 m_min;
  glm::vec3 m_max;
};

