#include "actor/cTransform.h"
#include  "../include/cDevice.h"
#include  "../include/cDeviceContext.h"
#include "../include/cConstBuffer.h"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"

cTransform::cTransform()
{
  setReady(true);
  setComponentType(componentTypes::Transform);
}

void
cTransform::moveTransform(float x, float y, float z, float w)
{
  glm::vec3 moveVector(x, y, z);
  m_transformMatrix.matrix = glm::translate(m_transformMatrix.matrix, moveVector);
}

void
cTransform::rotateInXAxis(float AngleInDegrees)
{
  m_transformMatrix.matrix = glm::rotate(m_transformMatrix.matrix, glm::radians(AngleInDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
}

void
cTransform::rotateInYAxis(float AngleInDegrees)
{
  m_transformMatrix.matrix = glm::rotate(m_transformMatrix.matrix, glm::radians(AngleInDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
}

void
cTransform::rotateInZAxis(float AngleInDegrees)
{
  m_transformMatrix.matrix = glm::rotate(m_transformMatrix.matrix, glm::radians(AngleInDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
}

void
cTransform::resetToIdentity()
{
  m_transformMatrix.matrix = glm::mat4(1.0f);
}

void
cTransform::shearTransformInXAxis(float shearingAmount)
{
  glm::mat4 shearMatrix(1.0f);
  glm::vec4 ShearingVec(1.0f, shearingAmount, 0.0f, 0.0f);
  shearMatrix = glm::column(shearMatrix, 0, ShearingVec);
  m_transformMatrix.matrix *= shearMatrix;
}

void cTransform::shearTransformInYAxis(float shearingAmount)
{
  glm::mat4 shearMatrix(1.0f);
  glm::vec4 ShearingVec(0.0f, 1.0f, shearingAmount, 0.0f);
  shearMatrix = glm::column(shearMatrix, 1, ShearingVec);
  m_transformMatrix.matrix *= shearMatrix;
}

void cTransform::shearTransformInZAxis(float shearingAmount)
{
  glm::mat4 shearMatrix(1.0f);
  glm::vec4 ShearingVec(0.0f, shearingAmount, 1.0f, 0.0f);
  shearMatrix = glm::column(shearMatrix, 2, ShearingVec);
  m_transformMatrix.matrix *= shearMatrix;
}

void cTransform::reflectTransfromInXAxis(float Amount)
{
  glm::vec4 reflectionColum(-Amount, 0.0f, 0.0f, 0.0f);
  m_transformMatrix.matrix = glm::column(m_transformMatrix.matrix, 0, reflectionColum);
}

void cTransform::reflectTransfromInYAxis(float Amount)
{
  glm::vec4 reflectionColum(0.0f, -Amount, 0.0f, 0.0f);
  m_transformMatrix.matrix = glm::column(m_transformMatrix.matrix, 1, reflectionColum);
}

void cTransform::reflectTransfromInZAxis(float Amount)
{
  glm::vec4 reflectionColum(0.0f, 0.0f, -Amount, 0.0f);
  m_transformMatrix.matrix = glm::column(m_transformMatrix.matrix, 2, reflectionColum);
}

sMatrix4x4
cTransform::getMatrix() const
{
  return  m_transformMatrix;
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

void cTransform::update(cDeviceContext & deviceContext, const sMatrix4x4 &Transform)
{

}



void cTransform::Destroy()
{}


