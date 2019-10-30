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

// represents the projection matrix 
uniform mat4 u_projection;
//represent the view matrix 
uniform mat4 u_view;
uniform u_worldAndColor 
{
  mat4x4 u_world;
  vec4 u_color;
};

/** OUT VARIABLES */ 
out vec2 outTexcoords;
out vec4 outColor;
/** IN VARIABLES */

/** MAIN */
void main(){
  mat4 mv = u_view * u_projection ;
  mat4 mvp = mv * u_world  ;
  gl_Position = (u_world * la_position);
//  gl_Position = (gl_Position * u_view);
 // gl_Position = (gl_Position * u_projection);
  outTexcoords = la_texcoords;
  outColor = vec4(0.0f,0.0f,0.8f,1.0f);
}