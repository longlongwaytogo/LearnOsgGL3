#version 430 core
layout(location = 0) vec4	_Position;
layout(location = 2) in vec3 _Normal;
layout(location = 8) in vec2 _TexCoord;

uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat4 osg_ViewMatrix;
uniform mat4 osg_ViewMatrixInverse;
uniform mat4 osg_ProjectionMatrix;
uniform mat4 osg_ModelViewMatrix;
 
uniform mat4 u_LightVPMatrix;
uniform mat4 osg_NormalMatrix;

out vec2 v2f_TexCoords;
out vec3 v2f_Normal;
out vec3 v2f_FragPosInViewSpace;

void main()
{
	vec4 FragPosInWorldSpace = osg_ViewMatrixInverse*osg_ModelViewMatrix * _Position;
	gl_Position = u_LightVPMatrix * FragPosInWorldSpace;
	v2f_TexCoords = _TexCoord;
	v2f_Normal = mat3(osg_NormalMatrix) * _Normal;
	v2f_FragPosInViewSpace = (osg_ModelViewMatrix * _Position).xyz;
}