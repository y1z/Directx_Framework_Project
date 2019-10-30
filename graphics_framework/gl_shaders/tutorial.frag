#version 430 core
/*! FRAGMENT/PIXEL SHADER 
\author Yhaliff Said Barraza
 */

/** IN VARIABLES */
in vec2 outTexcoords;
in vec4 outColor;

/** OUT VARIABLES */ 
out vec4 resultColor;
out vec2 resultTexcoords;

/** MAIN*/
void main(){
 resultTexcoords = sin(outTexcoords.xy);
 resultColor = outColor + vec4( resultTexcoords.xy,0.0f,0.0f) ;
}
