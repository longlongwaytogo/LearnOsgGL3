﻿#version 430 
in vec2 v2f_TexCoords;
layout(binding = 0) uniform sampler2D u_DiffuseTexture0;
 //uniform sampler2D u_DiffuseTexture0;
//layout(binding = 1) uniform sampler2D u_DiffuseTexture1;

layout(location = 0) out vec4 fragColor;
void main()
{
	fragColor = texture(u_DiffuseTexture0,v2f_TexCoords);
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0f/2.2f));
}