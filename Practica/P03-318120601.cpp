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



// Vamos agregar las funciones para poder colorear las caras de la pirámide cuadrangular 
void CrearCaraTriangular()
{
	unsigned int indices[] = { 0, 1, 2 };
	GLfloat vertices[] = {
		-0.5f, -0.5f,  0.5f, // Vértice inferior izquierdo
		 0.5f, -0.5f,  0.5f, // Vértice inferior derecho
		 0.0f,  0.5f,  0.0f  // Punta superior
	};
	Mesh* caraTri = new Mesh();
	caraTri->CreateMesh(vertices, indices, 9, 3);
	meshList.push_back(caraTri);
}


void CrearCaraCuadrada()
{
	unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
	GLfloat vertices[] = {
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f
	};
	Mesh* caraCuad = new Mesh();
	caraCuad->CreateMesh(vertices, indices, 12, 6);
	meshList.push_back(caraCuad);
}




int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(20, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CrearCaraTriangular(); 
	CrearCaraCuadrada();
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








		/*
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
		*/




		// Matriz base para que rote todo junto con las teclas E, R, T
		glm::mat4 baseModel = glm::mat4(1.0f);
		baseModel = glm::translate(baseModel, glm::vec3(0.0f, 0.0f, -4.0f));
		baseModel = glm::rotate(baseModel, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		baseModel = glm::rotate(baseModel, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		baseModel = glm::rotate(baseModel, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));


		// Ejericico 1: cohete
		glm::mat4 rocketBase = glm::translate(baseModel, glm::vec3(-3.0f, 0.0f, 0.0f));
		// Cilindro - meshList[2]
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		//
		glm::mat4 modelCuerpo = glm::scale(rocketBase, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCuerpo));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glm::vec3 colorGris = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorGris));
		meshList[2]->RenderMeshGeometry();


		//El cono - meshList[3]
		shaderList[2].useShader(); // Azul
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		uniformView = shaderList[2].getViewLocation();
		// 
		glm::mat4 modelPunta = glm::translate(rocketBase, glm::vec3(0.0f, 3.25f, 0.0f));
		modelPunta = glm::scale(modelPunta, glm::vec3(0.5f, 1.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPunta));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		meshList[3]->RenderMeshGeometry();


		//Las esferas
		shaderList[7].useShader(); // Amarillo
		uniformModel = shaderList[7].getModelLocation();
		uniformProjection = shaderList[7].getProjectLocation();
		uniformView = shaderList[7].getViewLocation();
		// 1
		glm::mat4 modelVentana_01 = glm::translate(rocketBase, glm::vec3(0.0f, 0.5f, 1.0f)); //0.0f, 1.5f, 1.0f
		modelVentana_01 = glm::scale(modelVentana_01, glm::vec3(0.4f, 0.4f, 0.4f)); //0.2f, 0.2f, 0.2f
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelVentana_01));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		sp.render();
		// 2
		/*
		glm::mat4 modelVentana_02 = glm::translate(rocketBase, glm::vec3(0.0f, 1.5f, 1.0f)); //
		modelVentana_02 = glm::scale(modelVentana_02, glm::vec3(0.2f, 0.2f, 0.2f)); //0.4f, 0.4f, 0.4f
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelVentana_02));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		sp.render();
		*/

		//Aletas - 4 Pirámides cuadrangulares
		shaderList[6].useShader(); // Rojo
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		uniformView = shaderList[6].getViewLocation();

		float tamañoPiramide = 1.0f;
		glm::vec3 escalaAleta = glm::vec3(tamañoPiramide, tamañoPiramide, tamañoPiramide);
		float distanciaCentro = 1.0f + (tamañoPiramide / 2.0f);
		float alturaY = -2.5f;

		// Aleta izquierda (-X)
		glm::mat4 modelAletaIzq = glm::translate(rocketBase, glm::vec3(-distanciaCentro, alturaY, 0.0f));
		modelAletaIzq = glm::rotate(modelAletaIzq, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Punta hacia -X
		modelAletaIzq = glm::scale(modelAletaIzq, escalaAleta);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAletaIzq));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		meshList[4]->RenderMesh();
		// Aleta Derecha (+X)
		glm::mat4 modelAletaDer = glm::translate(rocketBase, glm::vec3(distanciaCentro, alturaY, 0.0f));
		modelAletaDer = glm::rotate(modelAletaDer, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Punta hacia +X
		modelAletaDer = glm::scale(modelAletaDer, escalaAleta);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAletaDer));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		meshList[4]->RenderMesh();
		// Aleta Frontal (+Z) - Apuntando directo a la cámara (Forma un cuadrado con una X)
		glm::mat4 modelAletaFrente = glm::translate(rocketBase, glm::vec3(0.0f, alturaY, distanciaCentro));
		modelAletaFrente = glm::rotate(modelAletaFrente, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Punta hacia +Z
		modelAletaFrente = glm::scale(modelAletaFrente, escalaAleta);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAletaFrente));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		meshList[4]->RenderMesh();
		// Aleta Trasera (-Z) - Apuntando hacia atrás
		glm::mat4 modelAletaAtras = glm::translate(rocketBase, glm::vec3(0.0f, alturaY, -distanciaCentro));
		modelAletaAtras = glm::rotate(modelAletaAtras, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Punta hacia -Z
		modelAletaAtras = glm::scale(modelAletaAtras, escalaAleta);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAletaAtras));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		meshList[4]->RenderMesh();


		// Motor - cubo negro 
		shaderList[8].useShader(); // Negro
		uniformModel = shaderList[8].getModelLocation();
		uniformProjection = shaderList[8].getProjectLocation();
		uniformView = shaderList[8].getViewLocation();

		glm::vec3 escalaMotor = glm::vec3(0.6f, 0.6f, 0.6f);
		float posicionMotorY = -2.6f;


		glm::mat4 modelMotor = glm::translate(rocketBase, glm::vec3(0.0f, posicionMotorY, 0.0f));
		modelMotor = glm::scale(modelMotor, escalaMotor);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMotor));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		meshList[0]->RenderMesh();



	

		

		// Ejercicio 2
		glm::mat4 baseFigura = glm::translate(baseModel, glm::vec3(4.0f, 0.0f, 0.0f));

		// --- ROTACIÓN GLOBAL DE LA FIGURA ---
		float angGlobalX = 25.0f;
		float angGlobalY = 45.0f;
		baseFigura = glm::rotate(baseFigura, glm::radians(angGlobalX), glm::vec3(1.0f, 0.0f, 0.0f));
		baseFigura = glm::rotate(baseFigura, glm::radians(angGlobalY), glm::vec3(0.0f, 1.0f, 0.0f));

		// VARIABLES PARA EL PAR SUPERIOR E INFERIOR
		glm::vec3 escalaArribaAbajo = glm::vec3(2.9f, 1.5f, 2.8f);
		float pos_Y_Arriba = 1.5f;
		float pos_Y_Abajo = -1.5f;

		// VARIABLES PARA EL PAR IZQUIERDO Y DERECHO
		glm::vec3 escalaIzqDer = glm::vec3(2.95f, 1.5f, 2.8f);
		float pos_X_Der = 1.5f;
		float pos_X_Izq = -1.5f;

		// Arreglos de posición, escala y rotaciones estructurales
		glm::vec3 posiciones[4] = {
			glm::vec3(0.0f, pos_Y_Arriba, 0.0f),  // 0: Arriba
			glm::vec3(0.0f, pos_Y_Abajo, 0.0f),   // 1: Abajo
			glm::vec3(pos_X_Der, 0.0f, 0.0f),     // 2: Derecha
			glm::vec3(pos_X_Izq, 0.0f, 0.0f)      // 3: Izquierda
		};
		glm::vec3 escalas[4] = { escalaArribaAbajo, escalaArribaAbajo, escalaIzqDer, escalaIzqDer };
		float rotacionZ[4] = { 0.0f, 0.0f, -90.0f, 90.0f };
		float rotacionY[4] = { 45.0f, 45.0f, 0.0f, 0.0f };


		// Rota cada pirámide sobre su propio eje para mezclar los colores.
		// Valores posibles para no deformar: 0.0f, 90.0f, 180.0f, 270.0f
		float giroColor[4][2] = {
			{ 0.0f,   90.0f },  // Arriba:   (Pirámide exterior, Pirámide interior)
			{ 90.0f,  180.0f }, // Abajo:    (Pirámide exterior, Pirámide interior)
			{ 180.0f, 270.0f }, // Derecha:  (Pirámide exterior, Pirámide interior)
			{ 270.0f, 0.0f }    // Izquierda:(Pirámide exterior, Pirámide interior)
		};

		// --- DIBUJAR LOS 4 GRUPOS ---
		for (int grupo = 0; grupo < 4; grupo++) {
			glm::mat4 modelGrupo = glm::translate(baseFigura, posiciones[grupo]);
			modelGrupo = glm::rotate(modelGrupo, glm::radians(rotacionZ[grupo]), glm::vec3(0.0f, 0.0f, 1.0f));
			modelGrupo = glm::rotate(modelGrupo, glm::radians(rotacionY[grupo]), glm::vec3(0.0f, 1.0f, 0.0f));
			modelGrupo = glm::scale(modelGrupo, escalas[grupo]);

			// --- DIBUJAR LAS 2 PIRÁMIDES DE CADA GRUPO ---
			for (int pir = 0; pir < 2; pir++) {
				glm::mat4 modelPiramide;
				if (pir == 0) {
					// Pirámide 1 (Apuntando hacia afuera)
					modelPiramide = glm::translate(modelGrupo, glm::vec3(0.0f, 0.5f, 0.0f));
				}
				else {
					// Pirámide 2 (Apuntando hacia adentro)
					modelPiramide = glm::translate(modelGrupo, glm::vec3(0.0f, -0.5f, 0.0f));
					modelPiramide = glm::rotate(modelPiramide, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				}

				// APLICAMOS EL GIRO DE COLOR AQUÍ
				modelPiramide = glm::rotate(modelPiramide, glm::radians(giroColor[grupo][pir]), glm::vec3(0.0f, 1.0f, 0.0f));



				// 1. Cara Frontal (Roja - Shader 6)
				shaderList[6].useShader();
				glUniformMatrix4fv(shaderList[6].getModelLocation(), 1, GL_FALSE, glm::value_ptr(modelPiramide));
				glUniformMatrix4fv(shaderList[6].getProjectLocation(), 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(shaderList[6].getViewLocation(), 1, GL_FALSE, glm::value_ptr(view));
				meshList[5]->RenderMesh();

				// 2. Cara Derecha (Verde - Shader 5)
				shaderList[5].useShader();
				glm::mat4 mDer = glm::rotate(modelPiramide, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(shaderList[5].getModelLocation(), 1, GL_FALSE, glm::value_ptr(mDer));
				glUniformMatrix4fv(shaderList[5].getProjectLocation(), 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(shaderList[5].getViewLocation(), 1, GL_FALSE, glm::value_ptr(view));
				meshList[5]->RenderMesh();

				// 3. Cara Trasera (Amarilla - Shader 7)
				shaderList[7].useShader();
				glm::mat4 mAtras = glm::rotate(modelPiramide, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(shaderList[7].getModelLocation(), 1, GL_FALSE, glm::value_ptr(mAtras));
				glUniformMatrix4fv(shaderList[7].getProjectLocation(), 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(shaderList[7].getViewLocation(), 1, GL_FALSE, glm::value_ptr(view));
				meshList[5]->RenderMesh();

				// 4. Cara Izquierda (Magenta - Shader 4)
				shaderList[4].useShader();
				glm::mat4 mIzq = glm::rotate(modelPiramide, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(shaderList[4].getModelLocation(), 1, GL_FALSE, glm::value_ptr(mIzq));
				glUniformMatrix4fv(shaderList[4].getProjectLocation(), 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(shaderList[4].getViewLocation(), 1, GL_FALSE, glm::value_ptr(view));
				meshList[5]->RenderMesh();

				// 5. Base Cuadrada (Azul - Shader 2)
				shaderList[2].useShader();
				glUniformMatrix4fv(shaderList[2].getModelLocation(), 1, GL_FALSE, glm::value_ptr(modelPiramide));
				glUniformMatrix4fv(shaderList[2].getProjectLocation(), 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(shaderList[2].getViewLocation(), 1, GL_FALSE, glm::value_ptr(view));
				meshList[6]->RenderMesh();
			}
		}
















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

	
		