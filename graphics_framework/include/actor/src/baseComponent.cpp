#include "../baseComponent.h"

baseComponent::baseComponent()
: m_Ready(false),
m_componentType(enComponentTypes::NONE)
{}

void 
baseComponent::setReady(bool Readyness)
{
  m_Ready = Readyness;
}

void 
baseComponent::setComponentType(enComponentTypes type)
{
  m_componentType = type;
}

enComponentTypes 
baseComponent::getComponentType() const
{
  return this->m_componentType;
}

