#include "cApiComponents.h"

namespace fs = std::filesystem;

#if OPEN_GL
unsigned int cApiComponents::GlShaderProgram = 0;
unsigned int cApiComponents::vertexArrayObject = 0;
#endif//OPEN_GL

cApiComponents::cApiComponents()
{
  m_stratingPath = fs::current_path();
#if OPEN_GL
  m_majorVersion = 0;
  m_minorVersion = 0;
#endif // OPEN_GL
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


#elif OPEN_GL

#endif // DIRECTX
void
cApiComponents::setSupportedVersion(int  majorVersion, int minorVersion)
{
#if DIRECTX
  m_version = static_cast< D3D_FEATURE_LEVEL >(majorVersion);
#elif OPEN_GL

#endif // DIRECTX
}

void
cApiComponents::setHardwareVersion(int HardwareVersion)
{
#if DIRECTX
  m_driveType = static_cast< D3D_DRIVER_TYPE >(HardwareVersion);
#elif OPEN_GL

#endif // DIRECTX
}

#if OPEN_GL
unsigned int*
cApiComponents::getShaderProgram()
{
  return &cApiComponents::GlShaderProgram;
}

unsigned int*
cApiComponents::getvertexArrayObject()
{
  return &cApiComponents::vertexArrayObject; 
}

#endif // OPEN_GL
