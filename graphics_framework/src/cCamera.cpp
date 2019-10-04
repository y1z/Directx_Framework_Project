#include "cCamera.h"
#include "utility/HelperFuncs.h"
#include "cWindow.h"

cCamera::cCamera()
  :cCameraBase(),
  m_fov(3.14159f / 4.0f)
{
#if DIRECTX
  m_trasfrom.matrix = dx::XMMatrixIdentity();
  this->updateCoords();
#else
#endif // DIRECTX
}

void
cCamera::moveRight(float unit, cWindow &window, float deltaTime)
{
#if DIRECTX
  dx::XMVECTOR UnitVector = {unit,unit,unit,unit};
  m_eye.vector4 = dx::XMVectorAdd(m_eye.vector4, dx::XMVectorMultiply(m_right.vector4, UnitVector));
  m_at.vector4 = dx::XMVectorAdd(m_at.vector4, dx::XMVectorMultiply(m_right.vector4, UnitVector));

  this->updateCoords();
  this->updateCamera(window);
#else
#endif // DIRECTX
}

void
cCamera::moveUp(float unit, cWindow &window, float deltaTime)
{
#if DIRECTX
  dx::XMVECTOR UnitVector = {unit,unit,unit,unit};
  m_eye.vector4 = dx::XMVectorAdd(m_eye.vector4, dx::XMVectorMultiply(m_up.vector4, UnitVector));
  m_at.vector4 = dx::XMVectorAdd(m_at.vector4, dx::XMVectorMultiply(m_up.vector4, UnitVector));

  this->updateCoords();
  this->updateCamera(window);
#else
#endif // DIRECTX
}

void
cCamera::moveFront(float unit, cWindow &window, float deltaTime)
{
#if DIRECTX
  dx::XMVECTOR UnitVector = {unit,unit,unit,unit};
  m_eye.vector4 = dx::XMVectorAdd(m_eye.vector4, dx::XMVectorMultiply(m_front.vector4, UnitVector));
  m_at.vector4 = dx::XMVectorAdd(m_at.vector4, dx::XMVectorMultiply(m_front.vector4, UnitVector));

  this->updateCoords();
  this->updateCamera(window);
#else
#endif // DIRECTX
}

void cCamera::rotateCamera(sVector4 & Off, cWindow &window)
{
#if DIRECTX
  dx::XMVECTOR TEST = dx::XMVectorSet(1, 0, 0, 0);
  m_at.vector4 = dx::XMVectorAdd(m_at.vector4, dx::XMVectorMultiply(m_at.vector4, TEST));
  this->updateCoords();
  this->updateCamera(window);
#endif // DIRECTX

}

void
cCamera::calculateAndSetView()
{
#if DIRECTX
  m_view.matrix = dx::XMMatrixLookAtLH(this->m_eye.vector4,
                                       this->m_at.vector4,
                                       this->m_up.vector4);

  m_view.matrix = dx::XMMatrixTranspose(m_view.matrix);
#else
#endif // DIRECTX
}

void
cCamera::calculateAndSetPerpective(cWindow &window,
                                   float AngleInDegrees,
                                   float Far, float Near)
{
  m_fov = helper::degreesToRadians(AngleInDegrees);
  m_far = Far;
  m_near = Near;
#if DIRECTX
  m_projection.matrix = dx::XMMatrixPerspectiveFovLH(m_fov,
                                                     window.getWidth() / (float) window.getHeight(),
                                                     m_near, m_far);

  m_projection.matrix = dx::XMMatrixTranspose(m_projection.matrix);

  isOrtho = false;
  updateCoords();
#else
#endif // DIRECTX
}

void
cCamera::calculateAndSetOrthographic(cWindow & window, float Far, float Near)
{
  m_far = Far;
  m_near = Near;
#if DIRECTX
  m_projection.matrix = dx::XMMatrixOrthographicLH(window.getWidth(), window.getHeight(), Near, Far);

  m_projection.matrix = dx::XMMatrixTranspose(m_projection.matrix);
  this->isOrtho = true;
#else
#endif // DIRECTX
}

void
cCamera::moveTransform(sVector4 & Vector)
{
#if DIRECTX
  m_trasfrom.matrix = dx::XMMatrixTranslationFromVector(Vector.vector4);
#else
#endif // DIRECTX
}

void
cCamera::copyMatrixTransform(sMatrix4x4 & newTransform)
{
#if DIRECTX
  m_trasfrom = newTransform;
#else
#endif // DIRECTX
}

void
cCamera::updateCamera(cWindow & window)
{
#if DIRECTX
  m_view.matrix = dx::XMMatrixLookAtLH(this->m_eye.vector4,
                                       this->m_at.vector4,
                                       this->m_up.vector4);

  m_view.matrix = dx::XMMatrixTranspose(m_view.matrix);
  if (this->isOrtho == false)
  {
    m_projection.matrix = dx::XMMatrixPerspectiveFovLH(m_fov,
                                                       window.getWidth() / (float) window.getHeight(),
                                                       m_near, m_far);

    m_projection.matrix = dx::XMMatrixTranspose(m_projection.matrix);
  }
  else
  {
    m_projection.matrix = dx::XMMatrixOrthographicLH(window.getWidth(), window.getHeight(), m_near, m_far);

    m_projection.matrix = dx::XMMatrixTranspose(m_projection.matrix);
  }
#else 
#endif // DIRECTX
  updateCoords();
}

void
cCamera::updateCoords()
{
#if DIRECTX
  m_front.vector4 = dx::XMVector4Normalize(dx::XMVectorSubtract(m_at.vector4, m_eye.vector4));
  m_right.vector4 = dx::XMVector4Normalize(dx::XMVector3Cross(m_up.vector4, m_front.vector4));
  m_up.vector4 = dx::XMVector4Normalize(dx::XMVector3Cross(m_front.vector4, m_right.vector4));
#else
#endif // DIRECTX
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

float cCamera::getFovRad() const
{
  return m_fov;
}

float cCamera::getFovDeg() const
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

bool cCamera::getIsOrtho() const
{
  return isOrtho;
}


