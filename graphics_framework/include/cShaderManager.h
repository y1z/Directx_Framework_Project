#pragma once
#include "cVertexShader.h"
#include "cPixelShader.h"
//std includes 
#include <deque>
// forward declarations
class cInputLayout;
class cDevice;
class cDeviceContext;


/*!
*@brief give the shader manager all the necessary
*/
struct sShadersManagerDesc
{
  std::string vertexShaderPath{ "" };
  std::string vertexEntry{ "" };

  std::string pixelShaderPath{ "" };
  std::string pixelEntry{ "" };

  std::string vertexShaderVersion{ "" };
  std::string pixelShaderVersion{ "" };
};

/*! used to store shader of different types 
*
*/
struct sShadersPairs
{
  cVertexShader vertexShader;
  cPixelShader pixelShader;
   
  std::string name{""};
  size_t id;
};

/**
*@brief holds all the shaders and create every possible version
* using the '#define' statements
*TODO : make this class a modulo
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

  //!@returns a reference to a cVertexShader determined by the class variable 'm_selectedShaders'
  cVertexShader&
    getVertexShaderRef();

  //!@returns a reference to a cPixelShader determined by the class variable 'm_selectedShaders'
  cPixelShader&
    getPixelShaderRef();

  //!@returns a pointer to a cVertexShader determined by the class variable 'm_selectedShaders'
  cVertexShader*
    getVertexShaderPtr();

  //!@returns a pointer to a cPixelShader determined by the class variable 'm_selectedShaders'
  cPixelShader*
    getPixelShaderPtr();

  //!@returns the name of the currently selected shader determined by the class variable 'm_selectedShaders'
  const std::string &
    getShaderNameRef();

  //
  void
    setShader(cDeviceContext &deviceContext);

private:

  /*! adds defines that are only found on one version of the shader*/
  std::vector<std::string>
    addUniqueDefines(const std::string &originalShader);

  /*!doubles the amount of shader, each one with a different version of the same define
  *@param containerOfShaders[in][out] used to contain the shader
  *@param  globalDefines[in] contain all the defines that should be in every shader
  */
  std::vector<std::string>
    createShaderWithGlobalDefines(std::vector<std::string >  &containerOfShaders,
                                  std::vector< std::string> &globalDefines);

  /*! will add "#define " and finish with a '\n' if the current define does not have one or both.*/
  void
    makeDefineValid(std::string &define);

  /**
  *@brief will change the value of a define(if it's 0 then it will become 1 and vice-versa)
  * this function expects a shader and the position of the define
  *@param shader [in][out] the shader that contain the define expected to be changed 
  *@param definePos [in] the position of the define in the shader
  */
  void
    changeValueOfDefine(std::string &shader,
                        size_t definePos);

  /*!
  *@brief gives each shader a name based on it's defines 
  */
  void
    generateNamesForShader();


private:// variables 
  //! the final container of the shader
  std::deque<sShadersPairs> m_shaders;

  //! used to contain all the defines for the shaders
  std::vector<std::string> m_defineOptions;

  //! used to store names to all the shaders 
  std::vector<std::string> m_shaderNames;

  //!used to keep track of the currently selected shaders
  size_t m_selectedShaders{ 0 };
};

