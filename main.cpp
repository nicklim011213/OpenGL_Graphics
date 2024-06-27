#include <ShaderHelper.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Graphics_Engine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 1920, 1080);

	// Used to allow transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader Shader1("VertexShader.txt", 'V');
	if (!Shader1.Compile())
	{
		return - 1;
	}

	Shader Shader2("FragmentShader.txt", 'F');
	if (!Shader2.Compile())
	{
		return -1;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, Shader1.ShaderResult);
	glAttachShader(shaderProgram, Shader2.ShaderResult);
	glLinkProgram(shaderProgram);

	glDeleteShader(Shader1.ShaderResult);
	glDeleteShader(Shader2.ShaderResult);
	//END Shader Section

	float vertices[] = {
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	unsigned int indicies[]
	{
		0, 1, 3,
		1, 2, 3
	};

	//Create Array of Vertex Buffers
	unsigned int VertexArrayObject;
	glGenVertexArrays(1, &VertexArrayObject);

	// Create Indidvidual Vertex Buffer
	unsigned int VertexBufferObject;
	glGenBuffers(1, &VertexBufferObject);

	// Create a Vertex Index Buffer Combo (A.K.A ElementBufferObject)
	unsigned int ElementBufferObject;
	glGenBuffers(1, &ElementBufferObject);

	//Bind the Array
	glBindVertexArray(VertexArrayObject);

	//Bind the Vertex Buffer and pass its data
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind the Element Buffer and pass along its data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	// Read from position 0, Expect 3 floats, Do not normalize, each vertex attribute is 3 * size of a float, no offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(VertexArrayObject);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		glDisableVertexAttribArray(0);
	else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		glEnableVertexAttribArray(0);
}
