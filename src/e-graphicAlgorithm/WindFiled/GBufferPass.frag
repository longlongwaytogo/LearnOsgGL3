#version 330  compatibility 
in vec2 v2f_TexCoords;
in vec3 v2f_Normal;
in vec3 v2f_FragPosInViewSpace;

layout(location = 0) out vec4 Albedo_;
 uniform sampler2D u_DiffuseTexture;

void main()
{
	vec3 Albedo = texture(u_DiffuseTexture,v2f_TexCoords).rgb;
	Albedo = pow(Albedo, vec3(2.2f)); // to linear space 
	
	Albedo_ = vec4(Albedo, 1.0f);
}



