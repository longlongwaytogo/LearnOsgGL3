#version 430 
in vec2 v2f_TexCoords;
layout(binding = 0) uniform sampler2D Albedo_;
layout(binding = 1) uniform sampler2D Normal_;

 
 //uniform sampler2D u_DiffuseTexture0;
//layout(binding = 1) uniform sampler2D u_DiffuseTexture1;

layout(location = 0) out vec4 fragColor;

vec3 OilFilter()
{
	// Oil Paint Effect
	// kDistortionScale 0.01, kBrighten 2.0
	// kNormalScales (1.5, 2.5, 1.6)
	// Get the normal map and bring normals into [-1,1] range
	vec4 pNormalMap = texture ( Normal_ ,v2f_TexCoords);
	vec3 nMapNormal = pNormalMap.rgb;//2 * pNormalMap .rgb - vec3( 1, 1, 1 );

	//return nMapNormal.rgb;

	// Distort the UVs using normals ( Dependent Texture Read!)
	float kDistortionScale = 0.01; 
	vec2 uv = clamp(v2f_TexCoords - nMapNormal .xy * kDistortionScale,vec2(0.0),vec2(1.0));
	vec4 pIn = texture (Albedo_ ,uv);


	// Generate the image space lit scene
	float kNormalScales = 1.0;
	vec3 fakeTangN = nMapNormal .rbg * kNormalScales;
	fakeTangN = normalize (fakeTangN );

	// Do this for 3 lights and sum , choose different directions
	// and colors for the lights
	vec3 kLightDir = vec3(-1.00000000, 0.00000000, -0.707099974);
	float NDotL = clamp (dot (kLightDir , fakeTangN ),0.0,1.0);
	vec3 kLightColor = vec3(1,1,1);
	vec3 normalMappingComponent = NDotL * kLightColor ;
	float kBrighten = 1.0f;
	// Combine distorted scene with lit scene
	return  pIn .rgb;// * normalMappingComponent * kBrighten;
	
}
void main()
{
	//fragColor = texture(u_DiffuseTexture0,v2f_TexCoords);
	fragColor.rgb = OilFilter();
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0f/2.2f));
}