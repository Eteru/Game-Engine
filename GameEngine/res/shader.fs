#version 120

varying vec3 v_position;
varying vec2 v_texCoords;
varying vec3 v_normal;
varying vec3 v_toLight;
varying vec3 v_toCamera;

uniform sampler2D u_diffuse;

// parameters of the light and possible values
uniform vec3 u_lightA;
uniform vec3 u_lightD;
uniform vec3 u_lightS;

// parameters of the material and possible values
uniform vec3 u_materialA;
uniform vec3 u_materialD;
uniform vec3 u_materialS;
uniform float u_materialShine;

// returns intensity of reflected ambient lighting
vec3 AmbientLighting()
{
   return u_materialA * u_lightA;
}

// returns intensity of diffuse reflection
vec3 DiffuseLighting(in vec3 N, in vec3 L)
{
   // calculation as for Lambertian reflection
   float diffuseTerm = clamp(dot(N, L), 0, 1) ;
   return u_materialD * u_lightD * diffuseTerm;
}

// returns intensity of specular reflection
vec3 SpecularLighting(in vec3 N, in vec3 L, in vec3 V)
{
   float specularTerm = 0;

   // calculate specular reflection only if
   // the surface is oriented to the light source
   if(dot(N, L) > 0)
   {
      // half vector
      vec3 H = normalize(L + V);
      specularTerm = pow(dot(N, H), u_materialShine);
   }
   return u_materialS * u_lightS * specularTerm;
}

void main()
{
	// normalize vectors after interpolation
	vec3 L = normalize(v_toLight);
	vec3 V = normalize(v_toCamera);
	vec3 N = normalize(v_normal);

	// get Blinn-Phong reflectance components
	vec3 IA = AmbientLighting();
	vec3 ID = DiffuseLighting(N, L);
	vec3 IS = SpecularLighting(N, L, V);

	// diffuse color of the object from texture
	vec4 tex = texture2D(u_diffuse, v_texCoords);
	gl_FragColor = vec4(IA + ID, 1) * tex + vec4(IS, 1);
}