#version 120

attribute vec3 i_position;

uniform mat4 u_mvp;

void main()
{
	gl_Position =  u_mvp * vec4(i_position,1);
}