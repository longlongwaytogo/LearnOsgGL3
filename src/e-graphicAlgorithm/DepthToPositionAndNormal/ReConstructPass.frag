#version 430 
in vec2 v2f_TexCoords;

layout(binding = 0) uniform sampler2D u_AlbedoTexture;
layout(binding = 1) uniform sampler2D u_NormalTexture;
layout(binding = 2) uniform sampler2D u_PositionTexture;
layout(binding = 3) uniform sampler2D u_DepthTexture;

uniform float u_Fov = 45.0f;
uniform float u_WindowWidth;
uniform float u_WindowHeight;
uniform float u_Near = 0.1;
uniform float u_Far = 100.0;
//https://blog.csdn.net/qq_35312463/article/details/119694441

layout(location = 0) out vec4 fragColor;

// 从深度缓存中读取深度值depth，此时深度值范围为(0,1),将深度值depth 变化到NDC坐标系(-1,1) 
#define ZeroToNOne(a) (a*2.0-1.0)   
float ViewSpaceZFromDepth(float d)
{
	d = ZeroToNOne(d); // 从0，1 --> -1,1
	//视线坐标系看向的z轴负方向，因此要求视觉空间的z值应该要把线性深度变成负值
	 return (2.0 * u_Near * u_Far) / (u_Far + u_Near - d * (u_Far - u_Near)); 
}

vec3 UVToViewSpace(vec2 uv, float z)
{
	uv = ZeroToNOne(uv);
	float halfFov = tan(u_Fov * 0.5);
	uv.x = uv.x * halfFov * u_WindowWidth/u_WindowHeight * z;
	uv.y = uv.y * halfFov * z;
	return vec3(uv,-z);
}

vec3 GetViewPos(vec2 uv)
{
	float z = ViewSpaceZFromDepth(texture(u_DepthTexture,uv).r);
	return UVToViewSpace(uv,z);
}

float Length2(vec3 V)
{
	return dot(V,V);
}

vec3 MinDiff(vec3 P, vec3 Pr, vec3 Pl)
{
    vec3 V1 = Pr - P;
    vec3 V2 = P - Pl;
    return (Length2(V1) < Length2(V2)) ? V1 : V2;
}

void main()
{

	float xOffset = 1.0 / u_WindowWidth;
	float yOffset = 1.0 / u_WindowHeight;

	// 计算法线
 	vec3 P = GetViewPos(v2f_TexCoords);
 	vec3 PL = GetViewPos(v2f_TexCoords+vec2(-xOffset,0));
 	vec3 PR = GetViewPos(v2f_TexCoords+vec2( xOffset,0));
 	vec3 PU = GetViewPos(v2f_TexCoords+vec2(0, yOffset));
 	vec3 PD = GetViewPos(v2f_TexCoords+vec2(0,-yOffset));
 	vec3 rightDir = MinDiff(P,PR,PL); //求出最小的变换量
 	vec3 upDir = MinDiff(P,PU,PD);
 	vec3 normal = normalize(cross(rightDir,upDir));
 
 	fragColor.rgb =  normalize(normal);

	//fragColor = vec4(GetViewPos(v2f_TexCoords), 1.0f);
//	fragColor = texture(u_AlbedoTexture,v2f_TexCoords);
//	fragColor.rgb = pow(fragColor.rgb, vec3(1.0f/2.2f));

	 



}