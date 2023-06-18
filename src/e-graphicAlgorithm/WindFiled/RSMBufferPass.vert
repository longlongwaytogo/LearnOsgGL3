#version 330 compatibility 

layout(location = 0) in vec4 _Position;
layout(location = 2) in vec3 _Normal;
layout(location = 8) in vec2 _TexCoord;

uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat4 osg_ViewMatrix;
uniform mat4 osg_ViewMatrixInverse;
uniform mat4 osg_ProjectionMatrix;
uniform mat4 osg_ModelViewMatrix;
 
uniform mat4 u_LightVPMatrix;
uniform mat4 u_LightViewMatrixInverse;
uniform mat3 osg_NormalMatrix;

uniform mat4 u_MainViewMatrix;
out vec2 v2f_TexCoords;
out vec3 v2f_Normal;
out vec3 v2f_FragPosInViewSpace;
// osg中： osg_ViewMatrix osg_ViewMatrixInverse 总是主相机的viewmatrix
void main()
{
	mat4 ModelMatrix = u_LightViewMatrixInverse*osg_ModelViewMatrix;
	//vec4 FragPosInWorldSpace = ModelMatrix * _Position;
	gl_Position = osg_ModelViewProjectionMatrix * _Position;//FragPosInWorldSpace;
	v2f_TexCoords = _TexCoord;
	//存储的是在相机空间下的位置以及法线，不是光源空间下的
	mat4 MainModelView = u_MainViewMatrix * ModelMatrix;
	v2f_Normal = normalize(mat3(transpose(inverse(MainModelView))) * _Normal);

	v2f_FragPosInViewSpace = (MainModelView * _Position).xyz;
}