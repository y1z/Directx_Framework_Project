#include "cCameraManager.h"
#include <cassert>

cCameraManager::cCameraManager()
  :m_currentCamera(0),
  m_cameraCount(0)
{}


cCamera *
cCameraManager::getCurrentCamera()
{
  return &m_cameras[m_currentCamera];
}

sMatrix4x4
cCameraManager::getViewMatrix() const
{
  return this->m_cameras[m_currentCamera].getView();
}

sMatrix4x4
cCameraManager::getProjectionMatrix() const
{
  return this->m_cameras[m_currentCamera].getProjection();
}

void cCameraManager::switchCamera(uint8 chosenCamera)
{
  if (CheckIsInBound(chosenCamera))
  {
    m_currentCamera = chosenCamera;
  }
  else
  {
    assert("going out of bounds" && CheckIsInBound(chosenCamera));
  }
}

void cCameraManager::initCameraMatrixes(cWindow & window, float fov, float Far, float Near)
{
  m_cameras[m_currentCamera].calculateAndSetView();

  if (m_cameras[m_currentCamera].getIsOrtho())
  {
    m_cameras[m_currentCamera].calculateAndSetOrthographic(window, Far, Near);
  }
  else
  {
    m_cameras[m_currentCamera].calculateAndSetPerpective(window, fov, Far, Near);
  }

}

void cCameraManager::moveFront(float unit, cWindow & window, float deltaTime)
{
  m_cameras[m_currentCamera].moveFront(unit, window, deltaTime);
}

void cCameraManager::moveRight(float unit, cWindow & window, float deltaTime)
{
  m_cameras[m_currentCamera].moveRight(unit, window, deltaTime);
}

void cCameraManager::moveUp(float unit, cWindow & window, float deltaTime)
{
  m_cameras[m_currentCamera].moveUp(unit, window, deltaTime);
}


void cCameraManager::setEye(float x, float y, float z,float w)
{
  //m_cameras[m_currentCamera].setEye(x, y, z);
  m_cameras[m_currentCamera].setEye(x, y, z,w);
}

void cCameraManager::setAt(float x, float y, float z,float w)
{
  //m_cameras[m_currentCamera].setAt(x, y, z);
  m_cameras[m_currentCamera].setAt(x, y, z,w);
}

void cCameraManager::setUp(float x, float y, float z,float w)
{
  //m_cameras[m_currentCamera].setUp(x, y, z);
  m_cameras[m_currentCamera].setUp(x, y, z,w);
}

bool cCameraManager::pushBackCamera(const cCamera & newCamera)
{
  if (m_cameraCount < c_cameraLimit)
  {
    m_cameras[m_cameraCount++] = newCamera;
    return true;
  }
  return false;
}

bool cCameraManager::CheckIsInBound(uint8 cameraIndex) const
{
  if (cameraIndex <= c_cameraLimit)
  {
    return true;
  }
  return false;
}
