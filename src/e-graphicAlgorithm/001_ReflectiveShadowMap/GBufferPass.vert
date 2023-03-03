#version 430 
 
layout(location = 0) in vec4 in_Vertex;
layout(location = 2) in vec3 in_Normal;
//layout(location = 3) in vec4 osg_Color;

layout(location = 8) in vec4 tex0;

out vec2 v2f_TexCoords;
out vec3 v2f_Normal;
out vec3 v2f_FragPosInViewSpace;


uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat4 osg_ModelViewMatrix;
uniform mat4 osg_ViewMatrixInverse;
uniform mat4 osg_NormalMatrix;

void main()
{
	gl_Position = osg_ModelViewProjectionMatrix * in_Vertex;
	v2f_Normal = vec3(osg_NormalMatrix * vec4(in_Normal,1));
	v2f_FragPosInViewSpace = vec3(osg_ModelViewMatrix * in_Vertex);
	v2f_TexCoords = tex0.xy;
}