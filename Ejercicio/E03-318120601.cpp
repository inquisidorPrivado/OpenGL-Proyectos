//práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

// Mis shaders 
static const char* fShaderAzul = "shaders/shader_azul.frag";
static const char* fShaderCafe = "shaders/shader_cafe.frag";
static const char* fShaderMagenta = "shaders/shader_magenta.frag";
static const char* fShaderVerde = "shaders/shader_verde.frag";
static const char* fShaderRojo = "shaders/shader_rojo.frag";
static const char* fShaderAmarillo = "shaders/shader_amarillo.frag";
static const char* fShaderNegro = "shaders/shader_negro.frag";



void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}



//tetraedro rectángulo para hacer perfecta la esquina para el cubo
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 1,
		1, 2, 3
	};
	GLfloat vertices_piramide_triangular[] = {
		// El origen (0,0,0) es la PUNTA EXACTA de la pirámide (la esquina)
		0.0f, 0.0f, 0.0f,  // 0: Punta exterior
		0.5f, 0.0f, 0.0f,  // 1: Pata en X
		0.0f, 0.5f, 0.0f,  // 2: Pata en Y
		0.0f, 0.0f, 0.5f   // 3: Pata en Z
	};
	Mesh* piramidet = new Mesh();
	piramidet->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(piramidet);
}





//Código del profesor
// Pirámide triangular regular
/*
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* piramidet = new Mesh();
	piramidet->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(piramidet);

}
*/




//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,//frontal
		3,2,4,//izquierda
		2,1,4,//trasera
		1,0,4,//derecha
		0,1,2,//abajo1
		0,2,3//abajo2

	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramidec = new Mesh();
	piramidec->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramidec);
}



/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);


	Shader* shaderAzul = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shaderAzul->CreateFromFiles(vShader, fShaderAzul);
	shaderList.push_back(*shaderAzul); // Índice 2

	Shader* shaderCafe = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shaderCafe->CreateFromFiles(vShader, fShaderCafe);
	shaderList.push_back(*shaderCafe); // Índice 3

	Shader* shaderMagenta = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shaderMagenta->CreateFromFiles(vShader, fShaderMagenta);
	shaderList.push_back(*shaderMagenta); // Índice 4

	Shader* shaderVerde = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shaderVerde->CreateFromFiles(vShader, fShaderVerde);
	shaderList.push_back(*shaderVerde); // Índice 5

	Shader* shaderRojo = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shaderRojo->CreateFromFiles(vShader, fShaderRojo);
	shaderList.push_back(*shaderRojo); // Índice 6

	Shader* shaderAmarillo = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shaderAmarillo->CreateFromFiles(vShader, fShaderAmarillo);
	shaderList.push_back(*shaderAmarillo); // Índice 7

	Shader* shaderNegro = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shaderNegro->CreateFromFiles(vShader, fShaderNegro);
	shaderList.push_back(*shaderNegro); // Índice 8
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(5, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();
	
	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 0.9f, 0.8f, 1.0f);//naranja ultraduper claro
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		//shaderList[0].useShader();
		// Calcular matriz de vista de la cámara una sola vez
		glm::mat4 view = camera.calculateViewMatrix();









		// Piso negro
		shaderList[8].useShader();
		uniformModel = shaderList[8].getModelLocation();
		uniformProjection = shaderList[8].getProjectLocation();
		uniformView = shaderList[8].getViewLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, -7.0f));
		model = glm::scale(model, glm::vec3(10.0f, 0.5f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		// Dibujamos usando la primitiva geométrica del cubo
		meshList[0]->RenderMesh();



		

		// Matriz base para que rote todo junto con las teclas E, R, T
		glm::mat4 baseModel = glm::mat4(1.0f);
		baseModel = glm::translate(baseModel, glm::vec3(0.0f, 0.0f, -4.0f));
		baseModel = glm::rotate(baseModel, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		baseModel = glm::rotate(baseModel, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		baseModel = glm::rotate(baseModel, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

		// El cubo azul como base (Se forma el fondo de los rombos automaticamente)
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		uniformView = shaderList[2].getViewLocation();
		model = glm::scale(baseModel, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		meshList[0]->RenderMesh();

		//Los 6 cuadrados cafés
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		uniformView = shaderList[3].getViewLocation();
		float rSize = 0.2f; // Tamaño del rombo café
		float rOff = 0.405f; // Desplazamiento mínimo para pegarlo a la cara
		//En frente y atrás (Rotan en Z)
		model = glm::translate(baseModel, glm::vec3(0.0f, 0.0f, rOff)); model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
		model = glm::scale(model, glm::vec3(rSize, rSize, 0.05f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)); meshList[0]->RenderMesh();
		model = glm::translate(baseModel, glm::vec3(0.0f, 0.0f, -rOff)); model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
		model = glm::scale(model, glm::vec3(rSize, rSize, 0.05f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)); 
		meshList[0]->RenderMesh();
		// Izquierda y derecha (Rotan en X)
		model = glm::translate(baseModel, glm::vec3(-rOff, 0.0f, 0.0f)); 
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.05f, rSize, rSize)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)); 
		meshList[0]->RenderMesh();
		model = glm::translate(baseModel, glm::vec3(rOff, 0.0f, 0.0f)); 
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.05f, rSize, rSize)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		meshList[0]->RenderMesh();
		// Arriba y abajo (Rotan en Y)
		model = glm::translate(baseModel, glm::vec3(0.0f, rOff, 0.0f)); 
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(rSize, 0.05f, rSize)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)); 
		meshList[0]->RenderMesh();
		model = glm::translate(baseModel, glm::vec3(0.0f, -rOff, 0.0f)); 
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(rSize, 0.05f, rSize)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)); 
		meshList[0]->RenderMesh();

		//PIRÁMIDES EN LAS 8 ESQUINAS
		float c = 0.4001f; // Distancia exacta a las esquinas del cubo azul
		float p = 0.8f; // Este valor hace que la pirámide tape exactamente hasta la mitad de las aristas
		// 4 ESQUINAS SUPERIORES (+Y)
		shaderList[7].useShader(); 
		uniformModel = shaderList[7].getModelLocation(); 
		uniformProjection = shaderList[7].getProjectLocation(); 
		uniformView = shaderList[7].getViewLocation();
		model = glm::translate(baseModel, glm::vec3(-c, c, c)); 
		model = glm::scale(model, glm::vec3(p, -p, -p)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)); 
		meshList[1]->RenderMesh();
		shaderList[6].useShader(); 
		uniformModel = shaderList[6].getModelLocation(); 
		uniformProjection = shaderList[6].getProjectLocation(); 
		uniformView = shaderList[6].getViewLocation();
		model = glm::translate(baseModel, glm::vec3(c, c, c)); 
		model = glm::scale(model, glm::vec3(-p, -p, -p)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)); 
		meshList[1]->RenderMesh();
		shaderList[4].useShader(); 
		uniformModel = shaderList[4].getModelLocation(); 
		uniformProjection = shaderList[4].getProjectLocation(); 
		uniformView = shaderList[4].getViewLocation();
		model = glm::translate(baseModel, glm::vec3(-c, c, -c)); 
		model = glm::scale(model, glm::vec3(p, -p, p)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)); 
		meshList[1]->RenderMesh();
		shaderList[5].useShader(); 
		uniformModel = shaderList[5].getModelLocation(); 
		uniformProjection = shaderList[5].getProjectLocation(); 
		uniformView = shaderList[5].getViewLocation();
		model = glm::translate(baseModel, glm::vec3(c, c, -c)); 
		model = glm::scale(model, glm::vec3(-p, -p, p)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)); 
		meshList[1]->RenderMesh();
		// 4 ESQUINAS INFERIORES (-Y)
		shaderList[5].useShader(); 
		uniformModel = shaderList[7].getModelLocation(); 
		uniformProjection = shaderList[7].getProjectLocation(); 
		uniformView = shaderList[7].getViewLocation();
		model = glm::translate(baseModel, glm::vec3(-c, -c, c)); 
		model = glm::scale(model, glm::vec3(p, p, -p)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)); 
		meshList[1]->RenderMesh();
		shaderList[4].useShader(); 
		uniformModel = shaderList[6].getModelLocation(); 
		uniformProjection = shaderList[6].getProjectLocation(); 
		uniformView = shaderList[6].getViewLocation();
		model = glm::translate(baseModel, glm::vec3(c, -c, c)); 
		model = glm::scale(model, glm::vec3(-p, p, -p)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)); 
		meshList[1]->RenderMesh();
		shaderList[6].useShader(); 
		uniformModel = shaderList[4].getModelLocation(); 
		uniformProjection = shaderList[4].getProjectLocation(); 
		uniformView = shaderList[4].getViewLocation();
		model = glm::translate(baseModel, glm::vec3(-c, -c, -c)); 
		model = glm::scale(model, glm::vec3(p, p, p)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)); 
		meshList[1]->RenderMesh();
		shaderList[7].useShader(); 
		uniformModel = shaderList[5].getModelLocation(); 
		uniformProjection = shaderList[5].getProjectLocation(); 
		uniformView = shaderList[5].getViewLocation();
		model = glm::translate(baseModel, glm::vec3(c, -c, -c)); 
		model = glm::scale(model, glm::vec3(-p, p, p)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)); 
		meshList[1]->RenderMesh();





		/*
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		model = glm::mat4(1.0);
		//Traslación inicial para posicionar en -Z a los objetos
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//otras transformaciones para el objeto
		//model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo, pirámide triangular y pirámide base cuadrangular
		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono
		//sp.render(); //dibuja esfera
		
		
		/*
		//ejercicio: Instanciar primitivas geométricas para recrear las figuras 2 y 3 de la práctica pasada en 3D,
		//se requiere que exista piso
		*/
		

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		