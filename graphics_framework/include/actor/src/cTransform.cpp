#include "actor/cTransform.h"
#include  "../include/cDevice.h"
#include  "../include/cDeviceContext.h"
#include "../include/cConstBuffer.h"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

cTransform::cTransform()
  :m_transformMatrix(1.0f)
{
  setReady(true);
  setComponentType(componentTypes::Transform);
}

void 
cTransform::moveTransform(float x, float y, float z, float w)
{
  glm::vec3 moveVector(x, y, z);
  m_transformMatrix = glm::translate(m_transformMatrix, moveVector);
}

void
cTransform::rotateInXAxis(float AngleInDegrees)
{
  m_transformMatrix = glm::rotate(m_transformMatrix, AngleInDegrees, glm::vec3(AngleInDegrees, 0.0f, 0.0f));
}

void
cTransform::rotateInYAxis(float AngleInDegrees)
{
  m_transformMatrix = glm::rotate(m_transformMatrix, AngleInDegrees, glm::vec3(0.0f, AngleInDegrees, 0.0f));
}

void
cTransform::rotateInZAxis(float AngleInDgrees)
{
  m_transformMatrix = glm::rotate(m_transformMatrix, AngleInDgrees, glm::vec3(0.0f, 0.0f, AngleInDgrees));
}

void 
cTransform::resetToIdentity()
{
  m_transformMatrix = glm::mat4(1.0f);
}

void 
cTransform::shearTransformInXAxis(float shearingAmount)
{
  glm::mat4 reflec(1.0f);
  reflec[1][0] = shearingAmount;
  m_transformMatrix = reflec;
}

//! checks if the component is ready
bool 
cTransform::isReady() const
{
  return this->m_Ready;
}

void 
cTransform::Init(cDevice & device, cDeviceContext & deviceContext)
{
  //foo 
}

void cTransform::Draw(cDeviceContext & devContext, std::vector<cConstBuffer*>& constBuffers)
{}

void cTransform::update(cDeviceContext & deviceContext)
{

}



void cTransform::Destroy()
{}


