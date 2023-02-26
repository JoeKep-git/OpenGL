// Fragment Shader

#version 400

in  vec3 ex_Color;  //colour arriving from the vertex
out vec4 out_Color; //colour for the pixel
uniform vec4 objectColor;

void main(void)
{
	out_Color = mix(objectColor, vec4(ex_Color, 1.0), 0.7);
}