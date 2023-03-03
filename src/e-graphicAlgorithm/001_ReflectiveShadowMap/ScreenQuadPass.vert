#version 430 core
layout(location = 0) in vec4 osg_Vertex;
layout(location = 2) in vec3 osg_Normal;
layout(location = 8) in vec4 osg_MultTexCoord0;

out vec2 v2f_TexCoords;
uniform mat4 osg_ModelViewProjectionMatrix;

void main()
{
	gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;
	v2f_TexCoords = osg_MultTexCoord0.xy;
}