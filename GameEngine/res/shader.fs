#version 120

varying vec2 texCoord0;
varying vec3 normal0;

uniform sampler2D diffuse;

void main()
{
	vec3 lightDir = vec3(0,0,1);
	// lambert lightning
	gl_FragColor = texture2D(diffuse, texCoord0) * clamp(dot(-lightDir, normal0), 0.0, 1.0);
}