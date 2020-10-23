#include <Logging.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <json.hpp>
#include <fstream>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArrayObject.h"
#include "Shader.h"
#include "Camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "InputHelpers.h"
#include "MeshBuilder.h"
#include "MeshFactory.h"
//Includes the obj loader header
#include "ObjLoader.h"
//Includes the not-obj header
#include "VertexTypes.h"

GLFWwindow* window;
Camera::sptr camera = nullptr;

bool initGLFW() {
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to Initialize GLFW" << std::endl;
		return false;
	}

	//Create a new GLFW window
	window = glfwCreateWindow(1024, 768, "Skybound", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	return true;
}

bool initGLAD() {
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		std::cout << "Failed to initialize Glad" << std::endl;
		return false;
	}
}


GLfloat moveY = 0.0f;
GLfloat moveX = 0.0f;

GLfloat rotY = 0.0f;
GLfloat rotX = 0.0f;





int main() {

	//Initialize GLFW
	if (!initGLFW())
		return 1;

	//Initialize GLAD
	if (!initGLAD())
		return 1;


	//Testobj.txt Load player
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	VertexArrayObject::sptr theVAO = nullptr;
	bool loader = ObjLoader::LoadFromFile("SkyBoundCharacter.obj", positions, uvs, normals);

	theVAO = VertexArrayObject::Create();
	VertexBuffer::sptr vertices = VertexBuffer::Create();
	vertices->LoadData(positions.data(), positions.size());

	VertexBuffer::sptr _normals = VertexBuffer::Create();
	_normals->LoadData(normals.data(), normals.size());

	theVAO = VertexArrayObject::Create();

	theVAO->AddVertexBuffer(vertices, {
		BufferAttribute(0, 3, GL_FLOAT, false, 0, NULL)
		});
	theVAO->AddVertexBuffer(_normals, {
		BufferAttribute(2, 3, GL_FLOAT, false, 0, NULL)
		});

	// Load Island 1
	std::vector<glm::vec3> positions1;
	std::vector<glm::vec3> normals1;
	std::vector<glm::vec2> uvs1;


	VertexArrayObject::sptr island1VAO = nullptr;
	bool loader1 = ObjLoader::LoadFromFile("TextObject1.obj", positions1, uvs1, normals1);

	island1VAO = VertexArrayObject::Create();
	VertexBuffer::sptr vertices1 = VertexBuffer::Create();
	vertices1->LoadData(positions1.data(), positions1.size());

	VertexBuffer::sptr _normals1 = VertexBuffer::Create();
	_normals1->LoadData(normals1.data(), normals1.size());

	island1VAO = VertexArrayObject::Create();

	island1VAO->AddVertexBuffer(vertices1, {
		BufferAttribute(0, 3, GL_FLOAT, false, 0, NULL)
		});
	island1VAO->AddVertexBuffer(_normals1, {
		BufferAttribute(2, 3, GL_FLOAT, false, 0, NULL)
		});


	//Shader setup
	Shader::sptr shader = Shader::Create();
	shader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	shader->LoadShaderPartFromFile("shaders/frag_blinn_phong.glsl", GL_FRAGMENT_SHADER);
	shader->Link();

	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 lightCol = glm::vec3(50.0f, 20.0f, 10.0f);
	float     lightAmbientPow = 0.1f;
	float     lightSpecularPow = 10.0f;
	glm::vec3 ambientCol = glm::vec3(20.0f);
	float     ambientPow = 1.0f;
	float     shininess = 1.2f;

	shader->SetUniform("u_LightPos", lightPos);
	shader->SetUniform("inColor", glm::vec3(1.0f));
	shader->SetUniform("u_LightCol", lightCol);
	shader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
	shader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
	shader->SetUniform("u_AmbientCol", ambientCol);
	shader->SetUniform("u_AmbientStrength", ambientPow);
	shader->SetUniform("u_Shininess", shininess);

	//Transforms
	glm::mat4 transform = glm::mat4(0.8f);
	glm::mat4 transform2 = glm::mat4(1.0f);
	glm::mat4 transform3 = glm::mat4(1.0f);
	glm::mat4 transform4 = glm::mat4(1.2f);

	//Camera Setup
	camera = Camera::Create();
	camera->SetPosition(glm::vec3(0, -6.5f, 10)); // Set initial position
	camera->SetUp(glm::vec3(0, -5, -5)); // Use a z-up coordinate system
	camera->LookAt(glm::vec3(0.0f, 4.0f, 0.0f)); // Look at center of the screen
	camera->SetFovDegrees(75.0f); // Set an initial FOV

	double lastFrame = glfwGetTime();

	//Model Matrix
	//glm::mat4 transform = glm::mat4(1.0f); //Identity Matrix - Resetting the matrix

	transform = glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f));

	transform2 = glm::translate(transform, glm::vec3(0.0f, -10.0f, 0.0f));
	transform2 = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));

	

	// Run as long as the window is open
	while (!glfwWindowShouldClose(window)) {
		// Poll for events from windows (clicks, keypressed, closing, all that)
		glfwPollEvents();
		double thisFrame = glfwGetTime();
		float dt = static_cast<float>(thisFrame - lastFrame);


		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			transform = glm::translate(transform, glm::vec3(-3.0f, 0.0f, 0.0f) * dt);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			transform = glm::translate(transform, glm::vec3(3.0f, 0.0f, 0.0f) * dt);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			moveY = 6;
			//transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			rotY = -0.45;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			moveY = -6;
			//transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 3.0f) * dt);
			//rotX = 0.25;
			rotY = 0.45;
		}

		transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -moveY) * dt);
		transform = glm::rotate(transform, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, moveY) * dt);
		rotY = 0;
		moveY = 0;
		moveX = 0;

		//transform4 =  glm::translate(glm::mat4(1.0f), glm::vec3(3, 0.0f, glm::sin(static_cast<float>(thisFrame))));
		//transform4 = glm::rotate_slow(glm::mat4(1.0f), static_cast<float>(thisFrame), glm::vec3(0, -1, 0));

		//transform4 = transform4 * glm::translate(glm::mat4(1.0f), glm::vec3(3, 0.0f, glm::sin(static_cast<float>(thisFrame))));
		// Clear our screen every frame
					//Red, Green, Blue, Alpha
		glClearColor(0.3f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->Bind();
		shader->SetUniform("u_CamPos", camera->GetPosition());

		shader->Bind();
		shader->SetUniformMatrix("u_ModelViewProjection", camera->GetViewProjection() * transform);
		shader->SetUniformMatrix("u_Model", transform);
		shader->SetUniformMatrix("u_ModelRotation", glm::mat3(transform));
		theVAO->Render();

		shader->Bind();
		shader->SetUniformMatrix("u_ModelViewProjection", camera->GetViewProjection() * transform2);
		shader->SetUniformMatrix("u_Model", transform2);
		shader->SetUniformMatrix("u_ModelRotation", glm::mat3(transform2));
		island1VAO->Render();

		
		shader->UnBind();



		//RenderImGui();
		// Present our image to windows
		glfwSwapBuffers(window);
		lastFrame = thisFrame;
	}

	// Display our GPU and OpenGL version
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;


	return 0;
}
