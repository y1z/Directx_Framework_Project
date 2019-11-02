#version 430 core
/*! FRAGMENT/PIXEL SHADER 
\author Yhaliff Said Barraza
 */

 /** UNIFORMS */
uniform sampler2D uTextureSampler;

/** IN VARIABLES */
in vec2 outTexcoords;
//in vec4 outColor;

/** OUT VARIABLES */ 
out vec4 resultColor;
//out vec2 resultTexcoords;

/** MAIN*/
void main()
{
 resultColor = texture(uTextureSampler,outTexcoords); 
}
