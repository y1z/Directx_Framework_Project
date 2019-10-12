#include "..\cActor.h"
#include <cassert>

constexpr static std::size_t c_defaultSize = 16u;

cActor::cActor()
{
  m_components.reserve(c_defaultSize); 
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
  if (m_components.size() - 1 > Index)
  {
    return m_components[Index];
  }
  else
  {
    assert("Error out of vector Bounds " && m_components.size() - 1 > Index);
  }
}
