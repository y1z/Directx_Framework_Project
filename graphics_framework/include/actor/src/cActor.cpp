#include "..\cActor.h"
#include <iostream>
#include "utility/enDefs.h"
#include <cassert>

constexpr static std::size_t c_defaultSize = 16u;

cActor::cActor()
{
  m_components.reserve(c_defaultSize);

  for (baseComponent *component : m_components)
  {
    component = nullptr;
  }
}

cActor::~cActor()
{
  for (baseComponent* Componet : m_components)
  {
    if (Componet != nullptr)
    {
      delete Componet;
      Componet = nullptr;
    }
  }
}

void
cActor::DrawComponent(std::size_t Index, cDeviceContext &deviceContext, std::vector<cConstBuffer*> &buffers)
{
  if (m_components.size() - 1 > Index)
  {
    m_components[Index]->Draw(deviceContext, buffers);
  }
  else
  {
    assert("Error out of vector Bounds " && m_components.size() - 1 > Index);
  }
}

void 
cActor::DrawAllComponents(cDeviceContext & deviceContext, std::vector<cConstBuffer*>& buffers)
{
  for (auto & component : m_components)
  {
    component->Draw(deviceContext, buffers);
  }

}

void
cActor::InitComponent(std::size_t Index, cDeviceContext & deviceContext, cDevice & device)
{
  if (m_components.size() - 1 > Index)
  {
    if (m_components[Index]->isReady())
    {
      m_components[Index]->Init(device, deviceContext);
    }
  }
  else
  {
    assert("Error out of vector Bounds " && m_components.size() - 1 > Index);
  }
}

void
cActor::DestroyComponent(std::size_t Index)
{
  if (m_components.size() - 1 > Index)
  {
    m_components[Index]->Destroy();
  }
  else
  {
    assert("Error out of vector Bounds " && m_components.size() - 1 > Index);
  }
}

void cActor::DestroyAllComponents()
{
  for (auto component : m_components)
  {
    if (component != nullptr)
    {
      component->Destroy();
      delete component;
      component = nullptr;
    }
  }

}

void
cActor::AddComponents(baseComponent * component)
{
  m_components.emplace_back(component);
}

std::size_t
cActor::getComponentCount() const
{
  return m_components.size();
}

baseComponent *
cActor::getComponent(std::size_t Index)
{
  if (m_components.size() > Index && !m_components.empty())
  {
    return m_components[Index];
  }
  else
  {
    assert("Error out of vector Bounds " && m_components.size() - 1 > Index);

  }
  return nullptr;
}


std::vector<baseComponent*>::iterator
cActor::getIteratorBegin()
{
  return  m_components.begin();
}

std::vector<baseComponent*>::const_iterator
cActor::getIteratorEnd()
{
  return  m_components.end();
}


bool 
cActor::update(cDeviceContext &deviceContext)
{
  for (baseComponent *component : m_components)
  {
    if (!component->isReady())
    {
      EN_LOG_ERROR
      return false;
    }
    else{
      component->update(deviceContext,m_transform.getMatrix());
    }

  }
  return true;
}
