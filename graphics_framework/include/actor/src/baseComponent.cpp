#include "../baseComponent.h"

baseComponent::baseComponent()
: m_Ready(false),
m_componentType(componentTypes::NONE)
{}

void 
baseComponent::setReady(bool Readyness)
{
  m_Ready = Readyness;
}

void 
baseComponent::setComponentType(componentTypes type)
{
  m_componentType = type;
}

componentTypes 
baseComponent::getComponentType() const
{
  return this->m_componentType;
}

