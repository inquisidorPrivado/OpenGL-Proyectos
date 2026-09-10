//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
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
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";


//shaders nuevos se crearían acá
static const char* fShaderAzul = "shaders/shader_azul.frag";
static const char* fShaderCafe = "shaders/shader_cafe.frag";
static const char* fShaderMagenta = "shaders/shader_magenta.frag";
static const char* fShaderVerde = "shaders/shader_verde.frag";
static const char* fShaderRojo = "shaders/shader_rojo.frag";
static const char* fShaderAmarillo = "shaders/shader_amarillo.frag";
static const char* fShaderNegro = "shaders/shader_negro.frag";




float angulo = 0.0f;

//color café/marrón en RGB : 0.478, 0.255, 0.067

using std::vector;

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *piramidetriangular = new Mesh();
	piramidetriangular->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(piramidetriangular);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

//Vértices de un cubo
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
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{	
	//indice 0
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,36);
	meshColorList.push_back(letras);

	//indice 1
	GLfloat vertices_triangulomagenta[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	1.0f,
		
	};

	MeshColor* triangulomagenta = new MeshColor();
	triangulomagenta->CreateMeshColor(vertices_triangulomagenta, 18);
	meshColorList.push_back(triangulomagenta);

	//indice 2
	GLfloat vertices_cuadradoazul[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,

	};

	MeshColor* cuadradoazul = new MeshColor();
	cuadradoazul->CreateMeshColor(vertices_cuadradoazul, 36);
	meshColorList.push_back(cuadradoazul);

	//indice 3
	GLfloat vertices_trianguloamarillo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	1.0f,	0.0f,

	};

	MeshColor* trianguloamarillo = new MeshColor();
	trianguloamarillo->CreateMeshColor(vertices_trianguloamarillo, 18);
	meshColorList.push_back(trianguloamarillo);

	//indice 4
	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);


	//indice 5
	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	//indice 6
	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,

	};
	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

	//indice 7
	GLfloat vertices_cuadradonegro[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	0.0f,


	};
	MeshColor* cuadradonegro = new MeshColor();
	cuadradonegro->CreateMeshColor(vertices_cuadradonegro, 36);
	meshColorList.push_back(cuadradonegro);


	//indice 8
	GLfloat vertices_iniciales[] = {
		// D
		// Bloque 1
		-0.8f,  0.4f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.7f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.8f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.7f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.7f,  0.4f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.8f,  0.4f, 0.0f,  1.0f, 0.0f, 0.0f,
		// Bloque 2
		-0.7f,  0.4f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.4f,  0.3f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.7f,  0.3f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.4f,  0.3f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.4f,  0.4f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.7f,  0.4f, 0.0f,  1.0f, 0.0f, 0.0f,
		// Bloque 3
		-0.4f,  0.3f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.3f, -0.4f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.4f, -0.4f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.3f, -0.4f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.3f,  0.3f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.4f,  0.3f, 0.0f,  1.0f, 0.0f, 0.0f,
		// Bloque 4
		-0.7f, -0.4f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.4f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.7f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.4f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.4f, -0.4f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.7f, -0.4f, 0.0f,  1.0f, 0.0f, 0.0f,

		// G
		// Bloque 1
		-0.1f,  0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.2f,  0.3f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.1f,  0.3f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.2f,  0.3f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.2f,  0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.1f,  0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
		// Bloque 2
		-0.2f,  0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.1f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.2f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.1f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.1f,  0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.2f,  0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
		// Bloque 3
		-0.1f, -0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.2f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.1f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.2f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.2f, -0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.1f, -0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
		// Bloque 4
		0.1f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.2f, -0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.1f, -0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.2f, -0.4f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.2f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.1f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		// Bloque 5
		0.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.1f, -0.1f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, -0.1f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.1f, -0.1f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.1f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,

		// S
		// Bloque 1
		0.3f,  0.4f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.7f,  0.3f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.3f,  0.3f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.7f,  0.3f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.7f,  0.4f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.3f,  0.4f, 0.0f,  0.0f, 0.6f, 1.0f,
		// Bloque 2
		0.3f,  0.3f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.4f,  0.0f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.3f,  0.0f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.4f,  0.0f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.4f,  0.3f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.3f,  0.3f, 0.0f,  0.0f, 0.6f, 1.0f,
		// Bloque 3
		0.3f,  0.0f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.7f, -0.1f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.3f, -0.1f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.7f, -0.1f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.7f,  0.0f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.3f,  0.0f, 0.0f,  0.0f, 0.6f, 1.0f,
		// Bloque 4
		0.6f, -0.1f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.7f, -0.4f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.6f, -0.4f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.7f, -0.4f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.7f, -0.1f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.6f, -0.1f, 0.0f,  0.0f, 0.6f, 1.0f,
		// Bloque 5
		0.3f, -0.4f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.7f, -0.5f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.3f, -0.5f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.7f, -0.5f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.7f, -0.4f, 0.0f,  0.0f, 0.6f, 1.0f,
		0.3f, -0.4f, 0.0f,  0.0f, 0.6f, 1.0f
	};

	MeshColor* iniciales = new MeshColor();
	iniciales->CreateMeshColor(vertices_iniciales, 504); // 84 vértices * 6 atributos
	meshColorList.push_back(iniciales);

}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1); // Índice 0

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2); // Índice 1

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
	//Agregamos el 800*800
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearPiramideCuadrangular(); //índice 2 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f); //cuántas unidades voy a lograr a ver, eso es la variable final de la función


	// 
	//la proyeccion ortoganal no toma en cuenta la profundidad
	

	// (filoview se comporta como un ojo humano - angulo del ojo dilatado,aspect ratio, z mid?, z fag?)
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		// Fondo gris claro
		//glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClearColor(1.0f, 0.9f, 0.8f, 1.0f);//naranja ultraduper claro

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		//Para el cubo y las pirámides se usa el primer set de shaders con índice 0 en ShaderList
		// Activamos el shader que lee el color
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		
		angulo += 0.01;
		




		//PISO NEGRO 
		Shader& shaderNegro = shaderList[8];
		shaderNegro.useShader();
		uniformModel = shaderNegro.getModelLocation();
		uniformProjection = shaderNegro.getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, -3.0f));
		model = glm::scale(model, glm::vec3(2.0f, 0.1f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); // Cubo





		// TORRE DE LANZAMIENTO
		Shader& shaderCafe = shaderList[3];
		Shader& shaderAmarillo = shaderList[7];
		Shader& shaderRojo = shaderList[6];
		Shader& shaderVerde = shaderList[5];
		// Pilar Izquierdo (Cubo Café)
		shaderCafe.useShader();
		uniformModel = shaderCafe.getModelLocation();
		uniformProjection = shaderCafe.getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.82f, -0.35f, -3.0f));
		model = glm::scale(model, glm::vec3(0.06f, 0.8f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		// Pilar Derecho (Cubo Café)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.48f, -0.35f, -3.0f));
		model = glm::scale(model, glm::vec3(0.06f, 0.8f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		// (Pirámide triangular Amarillo
		shaderAmarillo.useShader();
		uniformModel = shaderAmarillo.getModelLocation();
		uniformProjection = shaderAmarillo.getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, -0.15f, -3.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.26f, 0.24f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		// (Pirámide triangular Rojo
		shaderRojo.useShader();
		uniformModel = shaderRojo.getModelLocation();
		uniformProjection = shaderRojo.getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, -0.39f, -3.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.26f, 0.24f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		// (Pirámide triangular Verde
		shaderVerde.useShader();
		uniformModel = shaderVerde.getModelLocation();
		uniformProjection = shaderVerde.getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, -0.63f, -3.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.26f, 0.24f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();






		//Con variables porque agiliza moverlo ya que es el que más se me complico
		float s = 0.30f; // Distancia desde el centro a las esquinas
		float sy = -0.51f; // Altura central de la figura

		//HOLOCRÓN
		Shader& shaderMagenta = shaderList[4];
		Shader& shaderAzul = shaderList[2];
		// (Pirámide triangular Amarillo
		shaderAmarillo.useShader();
		uniformModel = shaderAmarillo.getModelLocation();
		uniformProjection = shaderAmarillo.getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-s, sy + s, -3.02f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.16f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.18f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		// (Pirámide triangular Rojo
		shaderRojo.useShader();
		uniformModel = shaderRojo.getModelLocation();
		uniformProjection = shaderRojo.getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(s, sy + s, -3.02f));
		model = glm::rotate(model, glm::radians(315.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.16f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.18f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		// (Pirámide triangular Magenta
		shaderMagenta.useShader();
		uniformModel = shaderMagenta.getModelLocation();
		uniformProjection = shaderMagenta.getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-s, sy - s, -3.02f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.16f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.18f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		// (Pirámide triangular Verde
		shaderVerde.useShader();
		uniformModel = shaderVerde.getModelLocation();
		uniformProjection = shaderVerde.getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(s, sy - s, -3.02f));
		model = glm::rotate(model, glm::radians(225.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.16f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.18f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		// Cubo Azul
		shaderAzul.useShader();
		uniformModel = shaderAzul.getModelLocation();
		uniformProjection = shaderAzul.getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, sy, -3.01f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.355f, 0.355f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); // Cubo
		// Cubo Café
		shaderCafe.useShader();
		uniformModel = shaderCafe.getModelLocation();
		uniformProjection = shaderCafe.getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, sy, -3.00f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.18f, 0.18f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); // Cubo





		//Pirámides cuadrangular
		// (Pirámide triangular Verde
		shaderVerde.useShader();
		uniformModel = shaderVerde.getModelLocation();
		uniformProjection = shaderVerde.getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.50f, -0.60f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		// (Pirámide triangular Rojo
		shaderRojo.useShader();
		uniformModel = shaderRojo.getModelLocation();
		uniformProjection = shaderRojo.getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.80f, -0.60f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		// (Pirámide triangular Magenta
		shaderMagenta.useShader();
		uniformModel = shaderMagenta.getModelLocation();
		uniformProjection = shaderMagenta.getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, -0.30f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		// (Pirámide triangular Amarillo
		shaderAmarillo.useShader();
		uniformModel = shaderAmarillo.getModelLocation();
		uniformProjection = shaderAmarillo.getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, -0.60f, -3.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();




		// Para el ejercicio 1 de las iniciales
		
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		model = glm::mat4(1.0f);
		// Posicionar en la parte superior para que no interfiera con el resto del escenario
		model = glm::translate(model, glm::vec3(0.0f, 0.45f, -3.0f));
		model = glm::scale(model, glm::vec3(0.65f, 0.65f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		// Renderizar la malla de las iniciales (índice 8 en meshColorList)
		meshColorList[8]->RenderMeshColor();




		/*
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));

		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();
		*/

		/*
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();
		*/


		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/