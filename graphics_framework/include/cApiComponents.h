#pragma once
#include "utility/Grafics_libs.h"
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

  void 
    setSupportedVersion(int SupportedVersion);

  void 
    setHardwareVersion(int HardwareVersion);  
  //!this is for components of the API's that are everywhere i don't think DirectX has something like this 
  // so i just going a dummy function 
  unsigned int  getGlobalComponent();
  void setGlobalComponent(unsigned int Component);
#else
  
  unsigned int &getGlobalComponent();
#endif // DIRECTX

private:
#if DIRECTX
  //! used to know information about the hardware 
  D3D_DRIVER_TYPE m_driveType;
  //! used to know which version of the api is needed 
  D3D_FEATURE_LEVEL m_version;
#else
  unsigned int m_program{0};
#endif // DIRECTX
};
