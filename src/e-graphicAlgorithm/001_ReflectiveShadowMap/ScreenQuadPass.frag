#version 430 core
in vec2 v2f_TexCoords;
uniform sampler2D u_DiffuseTexture;

layout(location = 0) out vec4 fragColor;
void main()
{
	fragColor = texture(u_DiffuseTexture,v2f_TexCoords);

}