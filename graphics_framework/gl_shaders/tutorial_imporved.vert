#version 430 core 
/*! VERTEX SHADER 
\author Yhaliff Said Barraza
 */

/** STRUCTS */

struct Vertex{
  vec4 position; 
  vec2 texcoords;
};

/** LAYOUT */
layout(location = 0) in vec4 la_position; 
layout(location = 1) in vec2 la_texcoords;
  
/** UNIFORM  VARIABLES */

//represent the view matrix 
uniform mat4 uView;
// represents the projection matrix 
uniform mat4 uProjection;
uniform u_worldAndColor 
{
  mat4 u_world;
  vec4 u_color;
};

/** OUT VARIABLES */ 
out vec2 outTexcoords;
//out vec4 outColor;

/** MAIN */
void main(){
 // mat4 mvp =  uProjection * u_world * uView  ;
  mat4 mvp = u_world * uView * uProjection;
/* needs to be multiplied in this order
vector4 * matrix4x4 
 */
  gl_Position = la_position * mvp;//* Tmvp;// mvp;// mvp;
  outTexcoords = la_texcoords;
 // outColor = vec4(0.0f,0.0f,0.8f,1.0f);
}