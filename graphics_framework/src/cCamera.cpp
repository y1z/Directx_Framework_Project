#include "cCamera.h"
#include "utility/HelperFuncs.h"
#include "cWindow.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/matrix.hpp"

cCamera::cCamera()
  :cCameraBase(),
  m_fov(3.14159f / 4.0f),
  m_speed(1200.0f)
{
#if DIRECTX
  m_trasfrom.matrix = glm::identity<glm::mat4x4>();
  m_apiSpeedMultiplyer = 1.0f;
  this->updateCoords();
#elif OPEN_GL
  m_apiSpeedMultiplyer = 0.1f;
#endif // DIRECTX
}

void
cCamera::moveRight(float unit, cWindow &window, float deltaTime)
{
  glm::vec3 tempRight = m_right.vector3 * (m_speed * m_apiSpeedMultiplyer) * unit;

  m_eye.vector4 += glm::vec4((tempRight * deltaTime), 0.0f);
  m_at.vector4 += glm::vec4((tempRight * deltaTime), 0.0f);

  //this->updateCoords();
  this->updateCamera(window);
}

void
cCamera::moveUp(float unit, cWindow &window, float deltaTime)
{
  glm::vec4 tempUP = m_up.vector4 * (m_speed * m_apiSpeedMultiplyer) * unit;

  m_eye.vector4 += tempUP * deltaTime;
  m_at.vector4 += tempUP * deltaTime;

  // this->updateCoords();
  this->updateCamera(window);
}

void
cCamera::moveFront(float unit, cWindow &window, float deltaTime)
{
  glm::vec3 tempFront = m_front.vector3 * (m_speed * m_apiSpeedMultiplyer) * unit;

  m_eye.vector4 += glm::vec4((tempFront *  deltaTime), 0.0f);
  m_at.vector4 += glm::vec4((tempFront * deltaTime), 0.0f);

  //this->updateCoords();
  this->updateCamera(window);
}

void cCamera::rotateCamera(sVector3 & OffSet, cWindow &window)
{
  this->m_at.vector4 = m_at.vector4 + glm::vec4(OffSet.vector3, 0.0f);

  //this->updateCoords();
  this->updateCamera(window);
}

void
cCamera::calculateAndSetView()
{
  m_view.matrix = glm::lookAtLH(static_cast< glm::vec3 >(m_eye.vector4),
                                static_cast< glm::vec3 >(m_at.vector4),
                                static_cast< glm::vec3 >(m_up.vector4));

  helper::arrangeForApi(m_view);
}

void
cCamera::calculateAndSetPerpective(cWindow &window,
                                   float AngleInDegrees,
                                   float Far, float Near)
{
  m_fov = helper::degreesToRadians(AngleInDegrees);
  m_far = Far;
  m_near = Near;
  m_projection.matrix = glm::perspectiveFovLH<float>(m_fov, window.getWidth(),
                                                     window.getHeight(), m_near, m_far);

  //m_projection.matrix = glm::transpose(m_projection.matrix);
  helper::arrangeForApi(m_projection);
  isOrtho = false;
  updateCoords();
}

void
cCamera::calculateAndSetOrthographic(cWindow & window, float Far, float Near)
{
  m_far = Far;
  m_near = Near;
  //m_projection.matrix = //glm::orthoLH(window.getWidth(),window.getHeight(),)

  sWindowSize currentWindowSize = helper::getWindowSize(window);

  //m_projection.matrix = glm::orthoLH<float>(WindowCoords.left, WindowCoords.right,
  //                                          WindowCoords.bottom, WindowCoords.top,
  //                                          m_near, m_far);

  //m_projection.matrix = glm::ortho<float>(WindowCoords.left, -WindowCoords.left,
  //                                        WindowCoords.bottom, -WindowCoords.bottom,
  //                                        m_near, m_far);

  //m_projection.matrix = glm::orthoLH<float>(WindowCoords.left, WindowCoords.right,
  //                                          WindowCoords.bottom, WindowCoords.top,
  //                                          -1.0f, 1.0f);

  m_projection.matrix = glm::orthoLH<float>(0.0f, currentWindowSize.width,
                                            0, currentWindowSize.height,
                                            -1.0f, 1.0f);
  helper::arrangeForApi(m_projection);
  //m_projection.matrix = glm::transpose(m_projection.matrix);

  this->isOrtho = true;
}

void
cCamera::moveTransform(sVector4 & Vector)
{
  m_trasfrom.matrix = glm::translate(m_trasfrom.matrix, static_cast< glm::vec3 >(Vector.vector4));
}

void
cCamera::copyMatrixTransform(const sMatrix4x4 & newTransform)
{
  m_trasfrom = newTransform;
}

void
cCamera::updateCamera(cWindow & window)
{


  if (this->isOrtho == false)
  {
    m_projection.matrix = glm::perspectiveFovLH<float>(m_fov, window.getWidth(),
                                                       window.getHeight(), m_near, m_far);

    //m_projection.matrix = glm::transpose(m_projection.matrix);
    helper::arrangeForApi(m_projection);
    //    m_projection.matrix = dx::XMMatrixTranspose(m_projection.matrix);
  }
  else
  {
    sWindowSize currentWindowSize = helper::getWindowSize(window);

    m_projection.matrix = glm::orthoLH<float>(0.0f, currentWindowSize.width,
                                              0, currentWindowSize.height,
                                              -1.0f, 1.0f);

    helper::arrangeForApi(m_projection);
    //m_projection.matrix = glm::transpose(m_projection.matrix);
    this->isOrtho = true;
  }
  updateCoords();
}

void
cCamera::updateCoords()
{
  m_view.matrix = glm::lookAtLH(static_cast< glm::vec3 >(m_eye.vector4),
                                static_cast< glm::vec3 >(m_at.vector4),
                                static_cast< glm::vec3 >(m_up.vector4));

  helper::arrangeForApi(m_view);
  //m_view.matrix = glm::transpose(m_view.matrix);

  m_front.vector3 = glm::normalize(static_cast< glm::vec3 >(m_at.vector4 - m_eye.vector4));
  m_right.vector3 = glm::normalize(glm::cross(static_cast< glm::vec3 >(m_up.vector4),
                                   static_cast< glm::vec3 >(m_front.vector3)));

  m_up.vector4 = glm::vec4(glm::normalize(glm::cross(m_front.vector3, m_right.vector3)), 0.0f);
}

sMatrix4x4
cCamera::getView() const
{
  return this->m_view;
}

sMatrix4x4
cCamera::getProjection() const
{
  return this->m_projection;
}

sMatrix4x4
cCamera::getTransfrom() const
{
  return this->m_trasfrom;
}

float
cCamera::getFovRad() const
{
  return m_fov;
}

float
cCamera::getFovDeg() const
{
  return this->m_fov * (180.0f / 3.14159f);
}

float
cCamera::getNear() const
{
  return  this->m_near;
}

float
cCamera::getFar() const
{
  return this->m_far;
}

float cCamera::getSpeed() const
{
  return m_speed;
}

bool
cCamera::getIsOrtho() const
{
  return isOrtho;
}

void
cCamera::setSpeed(float newSpeed)
{
  m_speed = newSpeed;
}


