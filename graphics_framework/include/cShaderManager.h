#pragma once
#include "cVertexShader.h"
#include "cPixelShader.h"
//std includes 
#include <deque>
// forward declarations
class cInputLayout;
class cDevice;
class cDeviceContext;


struct sShadersManagerDesc
{
  std::string vertexShaderPath{ "" };
  std::string vertexEntry{ "" };

  std::string pixelShader{ "" };
  std::string pixelEntry{ "" };

  std::string vertexShaderVersion{ "" };
  std::string pixelShaderVersion{ "" };
};

/*! used to store shader of different types */
struct sShadersPairs
{
  cVertexShader vertShader;
  cPixelShader pixelShader;
};

/**
*@brief helps with keeping track of which shader is selected to be set,
*/
class cShaderManager
{
public:
  cShaderManager();

  ~cShaderManager();
public: // functions 

  /*! creates all the shaders that will be managed by this manager*/
  [[nodiscard]]bool
    init(cDevice &resourceManager,
         sShadersManagerDesc &descriptor);


  //! make sure the selected index is valid then change it to the give index 
  bool
    swichShader(size_t index);

    /*!@returns the total amount of shader that the manager contains */
  size_t
    getShaderCount()const;

    //@returns a reference to a cVertexShader determined by the class variable 'm_selectedShaders'
  cVertexShader&
    getVertexShaderRef();


    //@returns a reference to a cPixelShader determined by the class variable 'm_selectedShaders'
  cPixelShader&
    getPixelShaderRef();

    //@returns a pointer to a cVertexShader determined by the class variable 'm_selectedShaders'
  cVertexShader*
    getVertexShaderPtr();

    //@returns a pointer to a cPixelShader determined by the class variable 'm_selectedShaders'
  cPixelShader*
    getPixelShaderPtr();

  void
    setShader(cDeviceContext &deviceContext);

private:

  /*! create all the version of the shaders with defines*/
  std::vector<std::string>
    addDefinesToShaders(const std::string &originalShader,
                        std::vector<std::string> &defineStaments);

private:// variables 
  std::deque<sShadersPairs> m_shaders;
  std::vector<std::string> m_defineOptions;
  size_t m_selectedShaders{ 0 };
};

