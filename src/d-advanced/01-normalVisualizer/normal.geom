 // geometry shader 
#version 330  
uniform mat4 osg_ModelViewProjectionMatrix;  
uniform mat3 osg_NormalMatrix;
uniform float normal_length;
uniform bool vetexNormal; // 顶点法线还是面法线
in vec3 local_normal[];
out vec4 color;
layout(triangles) in;
layout(line_strip,max_vertices = 8) out;

void main()  
{  
	//float normal_length = 1.0;
	if(true)
	{
		for(int i = 0; i < gl_in.length(); i++)
		{
			vec4 P0 = gl_in[i].gl_Position;
			gl_Position = osg_ModelViewProjectionMatrix * P0;
			color = vec4(0,1,0,1);
			EmitVertex();

			vec4 N = vec4(local_normal[i],0.0);
			vec4 P1 = gl_in[i].gl_Position + N * normal_length;//normal_length ;
			color = vec4(1,0,0,1);
			gl_Position = osg_ModelViewProjectionMatrix * P1;
			EmitVertex();
			EndPrimitive();
		}
	}
	else
	{
		int n = gl_in.length();
		if(n >2)
		{
			vec4 P0 = gl_in[0].gl_Position;
			vec4 P1 = gl_in[1].gl_Position;
			vec4 P2 = gl_in[2].gl_Position;
			vec4 N0 = (P0 + P1 + P2) / 3.0;
			color = vec4(0,1,0,1);
			gl_Position = osg_ModelViewProjectionMatrix * N0;
			EmitVertex();

			vec3 n = (local_normal[0] + local_normal[1] + local_normal[2])/3.0;
			vec4 delta = vec4(n * normal_length,0);
			vec4 NP0 = N0 + delta;
		 
			color = vec4(1,0,0,1);
			gl_Position = osg_ModelViewProjectionMatrix * NP0;
			EmitVertex();

			EndPrimitive();

		}
		 
			
		 
	}
{
 // face normal
	vec3 P0 = gl_in[0].gl_Position.xyz;
	vec3 P1 = gl_in[1].gl_Position.xyz;
	vec3 P2 = gl_in[2].gl_Position.xyz;
	 // Center of the triangle
	 vec3 P = (P0+P1+P2) / 3.0;

	vec3 V0 = P0 - P1;
	vec3 V1 = P2 - P1;
  
	  vec3 N = cross(V1, V0);
	  N = normalize(N);

	gl_Position = osg_ModelViewProjectionMatrix * vec4(P, 1.0);
	color = vec4(0, 0, 0, 1);
	EmitVertex();
  
	gl_Position = osg_ModelViewProjectionMatrix * vec4(P + N * normal_length, 1.0);
	color = vec4(1, 0, 0, 1);
	EmitVertex();
	EndPrimitive();
  }
  
 
  

	
}  