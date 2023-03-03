#version 430
in vec2 v2f_TexCoords;
in vec3 v2f_Normal;
in vec3 v2f_FragPosInViewSpace;

layout(location = 0) out vec4 Albedo_;
layout(location = 1) out vec4 Normal_;
layout(location = 2) out vec4 Positon_;

uniform sampler2D u_DiffuseTexture;

void main()
{
	vec3 Albedo = texture(u_DiffuseTexture,v2f_TexCoords).rgb;
	Albedo = pow(Albedo, vec3(2.2f)); // to linear space 
	
	Albedo_ = vec4(Albedo, 1.0f);
	Normal_ = vec4(v2f_Normal,1.0);
	Positon_ = vec4(v2f_FragPosInViewSpace,1.0);
}



