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
  /*! TODO: rotates the camera by moving the focal point.*/
  void rotateCamera(sVector3 &Off,cWindow &window);
  /*! */
  void
    calculateAndSetView();
  /*! */
  void
    calculateAndSetPerpective(cWindow &Window,
                         float AngleInDegrees,
                         float Far, float Near);
  /*!*/
  void
    calculateAndSetOrthographic(cWindow &Window,
                                float Far, float Near);
/*! used to move the transform matrix with a vector */
  void 
    moveTransform(sVector4 &Vector);

  void 
    copyMatrixTransform( const sMatrix4x4 &newTransform);
/*! update the current view and projection matrices*/
  void
    updateCamera(cWindow &window) override;
/*! update the current eye,at,up vales */
  void
    updateCoords();
public:
  //! returns the current view matrix 
  sMatrix4x4 
    getView() const;
  //! returns the current projection matrix 
  sMatrix4x4 
    getProjection() const;
  //! returns the current transform matrix 
  sMatrix4x4 
    getTransfrom() const;
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
  /*! to know if the camera is perspective or orthographic*/
  bool
    getIsOrtho()const;
private:
  sMatrix4x4 m_view;
  sMatrix4x4 m_projection;
  sMatrix4x4 m_trasfrom;
/***********************/
  //! the direction where camera is point to  
  sVector3 m_front;
  //! the right relative to  camera 
  sVector3 m_right;
/***********************/
  float m_fov;
  float m_far;
  float m_near;
/***********************/
  //! use to know if we use the perspective or orthographic matrix 
  bool isOrtho = false;
};





