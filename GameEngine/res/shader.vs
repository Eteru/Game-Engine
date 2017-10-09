#version 120

attribute vec3 i_position;
attribute vec2 i_texCoords;
attribute vec3 i_normal;

varying vec3 v_position;
varying vec2 v_texCoords;
varying vec3 v_normal;
varying vec3 v_toLight;
varying vec3 v_toCamera;

uniform mat4 u_model;
uniform mat4 u_viewProjection;
uniform mat3 u_normalMatrix;
//uniform mat4 u_transform;
uniform vec3 u_lightPosition;
uniform vec3 u_cameraPosition;

void main()
{
	// position in world space
	vec4 worldPosition = u_model * vec4(i_position, 1);

	// normal in world space
	v_normal = normalize(u_normalMatrix * i_normal);

	// direction to light
	v_toLight = normalize(u_lightPosition - worldPosition.xyz);

	// direction to camera
	v_toCamera = normalize(u_cameraPosition - worldPosition.xyz);

	// texture coordinates to fragment shader
	v_texCoords = i_texCoords;

	// screen space coordinates of the vertex
	gl_Position = u_viewProjection * worldPosition;

	// this is the rest
	// gl_Position = u_transform * vec4(i_position, 1.0);

	// v_texCoords = i_texCoords;
	// last argument does translation
	// it is not needed for normals
	// v_normal = (u_transform * vec4(i_normal, 0.0)).xyz;
}