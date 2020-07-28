#version 330 core

out vec4 FragColor;
in vec2 TexCoord;
in vec2 FaceTexCoord;
uniform vec3 ourColor;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, FaceTexCoord), 0.2) * vec4(ourColor, 1.0);
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
