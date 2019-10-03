
#include "cCameraBase.h"


cCameraBase::cCameraBase()
{// default values taken from the tutorial 
#if DIRECTX
  m_eye.vector4 = dx::XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
  m_at.vector4 = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  m_up.vector4 = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
#else 
#endif // DIRECTX
}

cCameraBase::~cCameraBase()
{}


sVector4
cCameraBase::getEye()const
{
  return m_eye;
}

sVector4
cCameraBase::getAt()const
{
  return m_at;
}

sVector4
cCameraBase::getUp()const
{
  return m_up;
}


void cCameraBase::setEye(float x, float y, float z, float w)
{
#if DIRECTX
  m_eye.vector4 = dx::XMVectorSet(x, y, z, w);
#else 
#endif // DIRECTX
}

void cCameraBase::setAt(float x, float y, float z, float w)
{
#if DIRECTX
  m_at.vector4 = dx::XMVectorSet(x, y, z, w);
#else 
#endif // DIRECTX
}

void cCameraBase::setUp(float x, float y, float z, float w)
{
#if DIRECTX
  m_up.vector4 = dx::XMVectorSet(x, y, z, w);
#else 
#endif // DIRECTX
}