#pragma once
#include "../include/cCameraBase.h"
#include "utility/CustomStructs.h"
#include "utility/enDefs.h"
class cWindow;
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
    moveRight(float unit,cWindow &window, float deltaTime = 0.01666f) override;
  //! moves the camera to it's relative Top side(assuming the unit is positive)
  void
    moveUp(float unit,cWindow &window, float deltaTime = 0.01666f)override;
  //! moves the camera to it's relative Front side(assuming the unit is positive)
  void
    moveFront(float unit,cWindow &window, float deltaTime = 0.01666f)override;


  void AddToAt(sVector4 &Off,cWindow &window);
  //! view matrix initialized with default values from 'cCameraBase'
  void
    initViewMatrix();

  void
    initProjectionMatrix(cWindow &Window,
                         float AngleInDegrees,
                         float Far, float Near);
/*! used to move the transform matrix with a vector */
  void 
    moveTransform(sVector4 &Vector);

  void 
    copyMatrixTransform(sMatrix4x4 &newTransform);

  void
    updateCamera(cWindow &window) override;

  void
    updateCoords();
public:
  sMatrix4x4 
    getView() const;

  sMatrix4x4 
    getProjection() const;

  sMatrix4x4 
    getTrasfrom() const;
/*! retuns the cameras current field of view */
  float//radions
    getFovRad() const;
/*! returns the field of view in degrees*/
  float
    getFovDeg() const;
/*! returns the how close an object can get before the camera ignore it*/
  float
    getNear()const;
/*! returns the how far an object can be before the camera ignore it*/
  float
    getFar()const;
private:
  sMatrix4x4 m_view;
  sMatrix4x4 m_projection;
  sMatrix4x4 m_trasfrom;
/***********************/
  sVector4 m_front;
  sVector4 m_right;
/***********************/
  float m_fov;
  float m_far;
  float m_near;
};





