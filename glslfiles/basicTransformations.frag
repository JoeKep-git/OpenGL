#version 400

in  vec2 ex_TexCoord; //texture coord arriving from the vertex

out vec4 out_Color;   //colour for the pixel

uniform sampler2D DiffuseMap;

void main(void)
{
    out_Color = texture(DiffuseMap, ex_TexCoord); //show texture
}