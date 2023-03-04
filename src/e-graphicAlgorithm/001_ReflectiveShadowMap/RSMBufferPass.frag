#version 330 compatibility

in vec2 v2f_TexCoords;
in vec3 v2f_Normal;
in vec3 v2f_FragPosInViewSpace;


 layout(location = 0) out vec3 Flux_;
 layout(location = 1) out vec3 Normal_;
 layout(location =2) out vec3 Position_;

  uniform sampler2D u_DiffuseTexture;
 uniform vec3 u_LightColor = vec3(1);

void main()
{
	vec3 TexelColor = texture(u_DiffuseTexture, v2f_TexCoords.xy).rgb;
	Flux_ = u_LightColor * TexelColor;
	Normal_ = v2f_Normal;
	Position_ = v2f_FragPosInViewSpace;
}