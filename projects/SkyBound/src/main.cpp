#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
	// Initialize GLFW
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialize Glad" << std::endl;
		return 1;
	}

	// Create a new GLFW window
	GLFWwindow* window = glfwCreateWindow(800, 800, "SkyBound", nullptr, nullptr);
	// We want GL commands to be executed for our window, so we make our window's context the current one
	glfwMakeContextCurrent(window);



	// Initialize GLAD
	// Let glad know what function loader we are using (will call gl commands via glfw)
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		std::cout << "Failed to initialize Glad" << std::endl;
		return 2;
	}

	// Display our GPU and OpenGL version
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;



	// Run as long as the window is open
	while (!glfwWindowShouldClose(window)) {
		// Poll for events from windows (clicks, keypressed, closing, all that)
		glfwPollEvents();

		// Clear our screen every frame
					//Red, Green, Blue, Alpha
		glClearColor(0.3f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Present our image to windows
		glfwSwapBuffers(window);
	}



	return 0;
}
