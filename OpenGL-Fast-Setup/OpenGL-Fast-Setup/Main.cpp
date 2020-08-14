#include "imports.h"

// ------------------------Globals----------------------------
const float toRadians = 3.14159265f / 180.0f;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader Path
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader Path
static const char* fShader = "Shaders/shader.frag";


static std::vector<Mesh*> meshList;
static std::vector<Shader> shaderList;

//------------------------------------------------------------


// ----------------------Declarations ------------------------

void DeltaTime(GLfloat& deltaTime, GLfloat& lastTime);
void CreateObjects();
void CreateShaders();

//------------------------------------------------------------



// MAIN======================================================>>>

int main()
{
	Window mainWindow = Window();
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), 
							glm::vec3(0.0f, 1.0f, 0.0f), 
							-90.0f, 0.0f, 5.0f, 0.04f);

	// Projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
											(GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 
											0.1f, 100.0f);



	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Get + Handle user input events
		glfwPollEvents();

		DeltaTime(deltaTime, lastTime);
		camera.getAllCameraControls(mainWindow.getsKeys(), deltaTime, mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shaderList[0].UseShader();

		// Model Indentity Matrix
		glm::mat4 model(1.0f);

		// Transformations
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		// Handling Uniforms
		// model
		glUniformMatrix4fv(shaderList[0].GetUniformLocation("model"), 
							1, GL_FALSE, 
							glm::value_ptr(model));

		// projection
		glUniformMatrix4fv(shaderList[0].GetUniformLocation("projection"), 
							1, GL_FALSE, 
							glm::value_ptr(projection));

		//view
		glUniformMatrix4fv(shaderList[0].GetUniformLocation("view"), 
							1, GL_FALSE, 
							glm::value_ptr(camera.calculateViewMatrix()));

		// Rendring Mesh
		meshList[0]->RenderMesh();



		glUseProgram(0);

		mainWindow.swapBuffers();
	}






	return 0;
}

// ===============================================================================>>>


// Function for adding Mesh
void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

// Funciton for adding shader
void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

// Function for handling simple deltaTime
void DeltaTime(GLfloat& deltaTime, GLfloat& lastTime) {
	GLfloat now = glfwGetTime(); 
	deltaTime = now - lastTime;
	lastTime = now;
}

