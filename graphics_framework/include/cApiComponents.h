#pragma once
#include "utility/Grafics_libs.h"
#include <filesystem>
#include "utility/enDefs.h"

/*! this class is a container for variables that are used as globals inside 
a API */
class cApiComponents
{
public:
  cApiComponents() ;
  ~cApiComponents()= default;
public:
#if DIRECTX
  //! the which version is supported in the api 
  D3D_FEATURE_LEVEL 
    getSupportedVersion() const ;
  //! get the information for  
  D3D_DRIVER_TYPE 
    getHardwareVersion()const ;
#elif OPEN_GL
#endif // DIRECTX

  /*! set the version of the api used
  \param [in] majorVersion keeps track of the bigger version changes of the api
  \param [in] minorVersion */
  void
    setSupportedVersion(int majorVersion, int minorVersion = 0);

  /*! set the hardware being used 
  by the api */
  void 
    setHardwareVersion(int HardwareVersion);  

#if OPEN_GL

  /*! returns a reference to the shader program 
  for open_gl */
  static uint32
    *getShaderProgram();

  /*! returns a reference to the 
  vertex array object */
  static uint32 
    *getvertexArrayObject();

#endif // OPEN_GL
private:
  std::filesystem::path m_stratingPath;
#if DIRECTX
  //! used to know information about the hardware 
  D3D_DRIVER_TYPE m_driveType;
  //! used to know which version of the api is needed 
  D3D_FEATURE_LEVEL m_version;

#elif OPEN_GL
  static uint32 GlShaderProgram ;
  static uint32 vertexArrayObject;
  int m_majorVersion;
  int m_minorVersion;
#endif
};

