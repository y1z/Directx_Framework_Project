#pragma once
#include "actor/baseComponent.h"
#include <utility/CustomStructs.h>
#include <vector>


class cDevice;
class cDeviceContext;
class cConstBuffer;
class baseComponent;

class cTransform : public baseComponent
{
public:
  cTransform();
  ~cTransform() = default;
public:
  /*! this will move the current transform matrix in a direction */
  void
    moveTransform(float x, float y, float z, float w = 1.0f);

  /*! will rotate the transform in the X axis */
  void
    rotateInXAxis(float AngleInDegrees);

  /*! will rotate the transform in the Y axis */
  void
    rotateInYAxis(float AngleInDegrees);

  /*! will rotate the transform in the Z axis */
  void
    rotateInZAxis(float AngleInDegrees);
  
  /*!make the current transform equal to the identity matrix*/
  void
    resetToIdentity();

/*! deforms the matrix in the X Axis  */
  void
    shearTransformInXAxis(float shearingAmount);

/*! deforms the matrix in the X Axis  */
  void
    shearTransformInYAxis(float shearingAmount);

/*! deforms the matrix in the X Axis  */
  void
    shearTransformInZAxis(float shearingAmount);

  /*! does a transform that make the model look
  like it's own reflection in the mirror 
  https://www.varsitytutors.com/hotmath/hotmath_help/topics/transformation-of-graphs-using-matrices-reflection
  */
  void
    reflectTransfromInXAxis(float Amount);

  /*! does a transform that make the model look
  like it's own reflection in the mirror 
  https://www.varsitytutors.com/hotmath/hotmath_help/topics/transformation-of-graphs-using-matrices-reflection
  */
  void
    reflectTransfromInYAxis(float Amount);

  /*! does a transform that make the model look
  like it's own reflection in the mirror 
  https://www.varsitytutors.com/hotmath/hotmath_help/topics/transformation-of-graphs-using-matrices-reflection
  */
  void
    reflectTransfromInZAxis(float Amount);
public: // get and set functions 

  /*!returns the matrix used to keep track of transform */
  sMatrix4x4 
    getMatrix()const;


public: /*inherited virtual functions */


  /*! this function will be used to know if the component is ready to be used */
  bool
    isReady()const override;

  /*! use when isReady returns true */
  void
    Init(cDevice &device, cDeviceContext &deviceContext)override;

  /*! this is so the component can draw it self */
  void
    Draw(cDeviceContext &devContext, std::vector<cConstBuffer*> &constBuffers)override;

  /*! this is so */
  void
    update(cDeviceContext &deviceContext, const sMatrix4x4 &Transform) override;

  /*! use this to eliminate resources associated with component*/
  void
    Destroy()override;
private:
  //!this is the variable that contains all transform related operation
  sMatrix4x4 m_transformMatrix;
};
