#pragma once
#include <array>
#include <optional>
#include "utility/enDefs.h"
#include "cCamera.h"
// forward declarations
struct sMatrix4x4;
struct sVector4;
class cWindow;

/*! contains multiple cameras and can switch between them*/
class cCameraManager
{
public://constructor and destructor
  cCameraManager();
  ~cCameraManager() = default;

public://functions 
  //! return the current camera 
  cCamera*
    getCurrentCamera();
  //! switches which camera is bing used 
  void
    switchCamera(uint8 chosenCamera);
  /*! init both the view and projection matrix */
  void
    initCameraMatrixes(cWindow &window, float fov, float Far = 100.0f, float Near = 0.1f);
  /*! move the current camera forward or backwards */
  void
    moveFront(float unit, cWindow &window, float deltaTime = 0.016f);
  /*! move the current camera to the right or left*/
  void
    moveRight(float unit, cWindow &window, float deltaTime = 0.016f);
  /*! move the current camera up or down*/
  void
    moveUp(float unit, cWindow &window, float deltaTime = 0.016f);
  /*! rotates the currently selected camera */
  void
    rotateCamera(sVector3 &Off, cWindow &window);
  /*! returns the current cameras view matrix */
  sMatrix4x4
    getViewMatrix()const;
  /*! returns the current cameras projection matrix */
  sMatrix4x4
    getProjectionMatrix()const;

  sVector4
    getEye()const;

  sVector4
    getAt()const;

  sVector4
    getUp()const;

  /*! set the 'eye' of the current camera */
  void
    setEye(float x, float y, float z, float w = 1.0f);
  /*! set the 'at/focal point' of the current camera */
  void
    setAt(float x, float y, float z, float w = 1.0f);
  /*! set the 'up' direction of the current camera */
  void
    setUp(float x, float y, float z, float w = 1.0f);

  /*! adds a camera to the last index */
  bool
    pushBackCamera(const cCamera& newCamera);
private:
  //! make sure i don't go out of bounds of the array
  bool CheckIsInBound(uint8 cameraIndex) const;
private://variables 
  //! the maximum amount a camera this manager can take
  constexpr static uint8 c_cameraLimit = 8;
  //! where the cameras are being stored
  std::array<cCamera, c_cameraLimit > m_cameras;
  //! controls which camera is being used 
  std::size_t m_currentCamera;
  //! how many cameras does the manager have
  std::size_t m_cameraCount;
};

