#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.);\n"
"}\0";
const char *fragmentShaderSourceRed = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.f, 0.f, 0.f, 1.f);\n"
"}";
const char *fragmentShaderSourceGreen = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.f, 1.f, 0.f, 1.f);\n"
"}";
const char *fragmentShaderSourceBlue = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.f, 0.f, 1.f, 1.f);\n"
"}";

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

	unsigned int fragmentShaderRed;
	fragmentShaderRed = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderRed, 1, &fragmentShaderSourceRed, NULL);
	glCompileShader(fragmentShaderRed);
	glGetShaderiv(fragmentShaderRed, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderRed, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShaderGreen;
	fragmentShaderGreen = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderGreen, 1, &fragmentShaderSourceGreen, NULL);
	glCompileShader(fragmentShaderGreen);
	glGetShaderiv(fragmentShaderGreen, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderGreen, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShaderBlue;
	fragmentShaderBlue = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderBlue, 1, &fragmentShaderSourceBlue, NULL);
	glCompileShader(fragmentShaderBlue);
	glGetShaderiv(fragmentShaderBlue, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderBlue, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgramRed;
	shaderProgramRed = glCreateProgram();
	glAttachShader(shaderProgramRed, vertexShader);
	glAttachShader(shaderProgramRed, fragmentShaderRed);
	glLinkProgram(shaderProgramRed);
	glGetProgramiv(shaderProgramRed, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramRed, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgramGreen;
	shaderProgramGreen = glCreateProgram();
	glAttachShader(shaderProgramGreen, vertexShader);
	glAttachShader(shaderProgramGreen, fragmentShaderGreen);
	glLinkProgram(shaderProgramGreen);
	glGetProgramiv(shaderProgramGreen, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramGreen, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgramBlue;
	shaderProgramBlue = glCreateProgram();
	glAttachShader(shaderProgramBlue, vertexShader);
	glAttachShader(shaderProgramBlue, fragmentShaderBlue);
	glLinkProgram(shaderProgramBlue);
	glGetProgramiv(shaderProgramBlue, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramBlue, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}


	glDeleteShader(vertexShader);
	glDeleteShader(shaderProgramRed);
	glDeleteShader(shaderProgramGreen);
	glDeleteShader(shaderProgramBlue);

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

	//unsigned int indices[] = {
	//	0, 1, 2,	//first (left) triangle
	//	1, 3, 4,    //second (right) triangle
	//	2, 4, 5		//third (top) triangle
	//};

	unsigned int VAORed, VBORed, EBORed;
	unsigned int VAOGreen, VBOGreen, EBOGreen;
	unsigned int VAOBlue, VBOBlue, EBOBlue;
	glGenVertexArrays(1, &VAORed);
	glGenVertexArrays(1, &VAOGreen);
	glGenVertexArrays(1, &VAOBlue);
	glGenBuffers(1, &VBORed);
	glGenBuffers(1, &VBOGreen);
	glGenBuffers(1, &VBOBlue);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAORed);
	glBindBuffer(GL_ARRAY_BUFFER, VBORed);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRed), verticesRed, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOGreen);
	glBindBuffer(GL_ARRAY_BUFFER, VBOGreen);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesGreen), verticesGreen, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOBlue);
	glBindBuffer(GL_ARRAY_BUFFER, VBOBlue);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBlue), verticesBlue, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	////optionnal
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	//// draw in wireframe polygons
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//start chrono
	start = std::chrono::high_resolution_clock::now();

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw red triangle
		glUseProgram(shaderProgramRed);
		glBindVertexArray(VAORed);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		//draw green triangle
		glUseProgram(shaderProgramGreen);
		glBindVertexArray(VAOGreen);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		//draw blue triangle
		glUseProgram(shaderProgramBlue);
		glBindVertexArray(VAOBlue);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// fps counter
		fpsCounter();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAORed);
	glDeleteVertexArrays(1, &VAOGreen);
	glDeleteVertexArrays(1, &VAOBlue);
	glDeleteBuffers(1, &VBORed);
	glDeleteBuffers(1, &VBOGreen);
	glDeleteBuffers(1, &VBOBlue);
	//glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgramRed);
	glDeleteProgram(shaderProgramGreen);
	glDeleteProgram(shaderProgramBlue);

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
