#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;
out vec2 TexCoord;
out vec2 FaceTexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	TexCoord = aTexCoord;
	FaceTexCoord = vec2(-aTexCoord.x, aTexCoord.y);
}
