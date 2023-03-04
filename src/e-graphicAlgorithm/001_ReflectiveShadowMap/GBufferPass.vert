#version 330 compatibility
 
layout(location = 0) in vec4 _Position;
layout(location = 2) in vec3 _Normal;
layout(location = 8) in vec4 _TexCoord;

out vec2 v2f_TexCoords;
out vec3 v2f_Normal;
out vec3 v2f_FragPosInViewSpace;

uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat4 osg_ModelViewMatrix;
uniform mat4 osg_ViewMatrixInverse;
uniform mat4 osg_NormalMatrix;

void main()
{
	gl_Position = osg_ModelViewProjectionMatrix * _Position;
	v2f_TexCoords = _TexCoord.xy;
	v2f_Normal = vec3(osg_NormalMatrix * vec4(_Normal,1));
	v2f_FragPosInViewSpace = vec3(osg_ModelViewMatrix * _Position);
}