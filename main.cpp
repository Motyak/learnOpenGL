#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.f);\n"
"}\0";
const char* fragmentShaderSource[3] = {
	"#version 330 core\nout vec4 FragColor;\nvoid main(){FragColor = vec4(1.f, 0.f, 0.f, 1.f);}",
	"#version 330 core\nout vec4 FragColor;\nvoid main(){FragColor = vec4(0.f, 1.f, 0.f, 1.f);}",
	"#version 330 core\nout vec4 FragColor;\nvoid main(){FragColor = vec4(0.f, 0.f, 1.f, 1.f);}"
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void fpsCounter();

int nbOfFrames = 0;
auto start = std::chrono::high_resolution_clock::now();	//useless first value

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Triforce", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader[3], shaderProgram[3];

	for (int i = 0; i <= 2; ++i)
	{
		fragmentShader[i] = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader[i], 1, &fragmentShaderSource[i], NULL);
		glCompileShader(fragmentShader[i]);
		glGetShaderiv(fragmentShader[i], GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader[i], 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		shaderProgram[i] = glCreateProgram();
		glAttachShader(shaderProgram[i], vertexShader);
		glAttachShader(shaderProgram[i], fragmentShader[i]);
		glLinkProgram(shaderProgram[i]);
		glGetProgramiv(shaderProgram[i], GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram[i], 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		}
	}

	glDeleteShader(vertexShader);
	for (int i = 0; i <= 2; ++i)
	{
		glDeleteShader(fragmentShader[i]);
		glDeleteShader(shaderProgram[i]);
	}

	//left triangle
	float verticesRed[] = {
		-1.f, -1.f, 0.f,	//bottom left
		0.f, -1.f, 0.f,		//bottom right
		-0.5f, 0.f, 0.f,	//top
	};
	//right triangle
	float verticesGreen[] = {
		0.f, -1.f, 0.f,		//bottom left
		1.f, -1.f, 0.f,		//bottom right
		0.5f, 0.f, 0.f,		//top
	};
	//top triangle
	float verticesBlue[] = {
		-0.5f, 0.f, 0.f,	//bottom left
		0.5f, 0.f, 0.f,		//bottom right
		0.f, 1.f, 0.f		//top
	};

	unsigned int VAO[3], VBO[3];
	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRed), verticesRed, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesGreen), verticesGreen, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBlue), verticesBlue, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//// draw in wireframe polygons
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//start chrono for fps count
	start = std::chrono::high_resolution_clock::now();

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// draw triangles
		for (int i = 0; i <= 2; ++i)
		{
			glUseProgram(shaderProgram[i]);
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
		}

		// fps counter
		fpsCounter();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);
	for (int i = 0; i <= 2; ++i)
		glDeleteProgram(shaderProgram[i]);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void fpsCounter()
{
	++nbOfFrames;
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	if (elapsed_seconds.count() >= 1)
	{
		std::cout << "FPS: " << nbOfFrames << std::endl;
		start = std::chrono::high_resolution_clock::now();
		nbOfFrames = 0;
	}
}
