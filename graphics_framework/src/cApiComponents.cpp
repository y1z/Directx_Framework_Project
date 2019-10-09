#include "cApiComponents.h"

namespace fs = std::filesystem;

cApiComponents::cApiComponents()
{
  m_stratingPath = fs::current_path();
}

#if DIRECTX

D3D_FEATURE_LEVEL
cApiComponents::getSupportedVersion() const
{
  return this->m_version;
}

D3D_DRIVER_TYPE 
cApiComponents::getHardwareVersion() const
{
  return this->m_driveType;
}

void  
cApiComponents::setSupportedVersion(int SupportedVersion)
{
  m_version = static_cast<D3D_FEATURE_LEVEL>(SupportedVersion);
}

void 
cApiComponents::setHardwareVersion(int HardwareVersion)
{ m_driveType = static_cast<D3D_DRIVER_TYPE>(HardwareVersion); }
// foo
unsigned int
cApiComponents::getGlobalComponent()
{
  return 0;
}
// foo 
void 
cApiComponents::setGlobalComponent(unsigned int Component)
{}

#endif // DIRECTX
