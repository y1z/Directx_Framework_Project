#pragma once
#include "../include/cCameraBase.h"
#include "../include/utiliy/CustomStructs.h"
/*! left-handed based camera */
class cCamera :
  public cCameraBase
{
public:
  cCamera();

  ~cCamera() = default;

public:
  //! moves the camera to it's relative right side (assuming the unit is positive)
  void 
    moveRight(float unit, float deltaTime) override;
  //! moves the camera to it's relative Top side(assuming the unit is positive)
  void 
    moveUp(float unit, float deltaTime)override;
  //! moves the camera to it's relative Front side(assuming the unit is positive)
  void 
    moveFront(float unit, float deltaTime)override;
//! view matrix initialized with default values from 'cCameraBase'
  void 
    initViewMatrix();
/*! TODO */
  void
    updateCamera()override;

private:

  sMatrix4x4 m_view;
  sMatrix4x4 m_projection;
  sMatrix4x4 m_trasfrom;
};





