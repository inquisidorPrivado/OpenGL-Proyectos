/*
Práctica 5: Optimización y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;
//Lista de Modelos a importar
Model Rover_M;

Model Cuerpo_M;
Model Brazo_M;
Model PataDelanteraDerecha_M, PataDelanteraIzquierda_M;
Model PataTraseraDerecha_M, PataTraseraIzquierda_M;
Model LlantaDelanteraDerecha_M, LlantaDelanteraIzquierda_M;
Model LlantasTraserasDerechas_01_M, LlantasTraserasDerechas_02_M;
Model LlantasTraserasIzquierdas_01_M, LlantasTraserasIzquierdas_02_M;

//Lista de modelos del satelite
Model Cuerpo_satelite_M;
Model satelite_panel_01_M;
Model satelite_panel_02_M;
Model satelite_panel_03_M;


// Lista de modelos para el Holocron
Model Cuerpo_holocron_M;
Model holocron_esquina_01_M;
Model holocron_esquina_02_M;
Model holocron_esquina_03_M;
Model holocron_esquina_04_M;
Model holocron_esquina_05_M;
Model holocron_esquina_06_M;
Model holocron_esquina_07_M;
Model holocron_esquina_08_M;




//Lista de Skybox a crear
Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);
	//Cargar modelos
	Rover_M = Model();
	Rover_M.LoadModel("Models/RoverModificado.obj");

	// Cargar modelos mios
	Cuerpo_M = Model();
	Cuerpo_M.LoadModel("Models/Cuerpo.obj");

	Brazo_M = Model();
	Brazo_M.LoadModel("Models/Brazo.obj");
	//pata delantera derecha
	PataDelanteraDerecha_M = Model();
	PataDelanteraDerecha_M.LoadModel("Models/PataDelanteraDerecha.obj");

	LlantaDelanteraDerecha_M = Model();
	LlantaDelanteraDerecha_M.LoadModel("Models/LlantaDelanteraDerecha.obj");
	//pata delantera izquierda
	PataDelanteraIzquierda_M = Model();
	PataDelanteraIzquierda_M.LoadModel("Models/PataDelanteraIzquierda.obj");

	LlantaDelanteraIzquierda_M = Model();
	LlantaDelanteraIzquierda_M.LoadModel("Models/LlantaDelanteraIzquierda.obj");
	
	//pata trasera derecha
	PataTraseraDerecha_M = Model();
	PataTraseraDerecha_M.LoadModel("Models/PataTraseraDerecha.obj");

	LlantasTraserasDerechas_01_M = Model();
	LlantasTraserasDerechas_01_M.LoadModel("Models/LlantasTraserasDerechas_01.obj");
	LlantasTraserasDerechas_02_M = Model();
	LlantasTraserasDerechas_02_M.LoadModel("Models/LlantasTraserasDerechas_02.obj");

	//pata trasera izquierda
	PataTraseraIzquierda_M = Model();
	PataTraseraIzquierda_M.LoadModel("Models/PataTraseraIzquierda.obj");

	LlantasTraserasIzquierdas_01_M = Model();
	LlantasTraserasIzquierdas_01_M.LoadModel("Models/LlantasTraserasIzquierdas_01.obj");
	LlantasTraserasIzquierdas_02_M = Model();
	LlantasTraserasIzquierdas_02_M.LoadModel("Models/LlantasTraserasIzquierdas_02.obj");











	//Modelo holocron
	Cuerpo_holocron_M = Model();
	Cuerpo_holocron_M.LoadModel("Models/Holocron/Holocron_cuerpo.obj");

	holocron_esquina_01_M = Model();
	holocron_esquina_01_M.LoadModel("Models/Holocron/esquina_01.obj");
	holocron_esquina_02_M = Model();
	holocron_esquina_02_M.LoadModel("Models/Holocron/esquina_02.obj");
	holocron_esquina_03_M = Model();
	holocron_esquina_03_M.LoadModel("Models/Holocron/esquina_03.obj");
	holocron_esquina_04_M = Model();
	holocron_esquina_04_M.LoadModel("Models/Holocron/esquina_04.obj");
	holocron_esquina_05_M = Model();
	holocron_esquina_05_M.LoadModel("Models/Holocron/esquina_05.obj");
	holocron_esquina_06_M = Model();
	holocron_esquina_06_M.LoadModel("Models/Holocron/esquina_06.obj");
	holocron_esquina_07_M = Model();
	holocron_esquina_07_M.LoadModel("Models/Holocron/esquina_07.obj");
	holocron_esquina_08_M = Model();
	holocron_esquina_08_M.LoadModel("Models/Holocron/esquina_08.obj");

	//Modelo satelite
	Cuerpo_satelite_M = Model();
	Cuerpo_satelite_M.LoadModel("Models/Satelite/satelite_cuerpo.obj");
	
	satelite_panel_01_M = Model();
	satelite_panel_01_M.LoadModel("Models/Satelite/rotar_03.obj");
	satelite_panel_02_M = Model();
	satelite_panel_02_M.LoadModel("Models/Satelite/rotar_02.obj");
	satelite_panel_03_M = Model();
	satelite_panel_03_M.LoadModel("Models/Satelite/rotar_01.obj");
	



	//Crear Skybox con sus 6 texturas
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshListModel[2]->RenderMeshModel();

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS*-------------------*

		// 1. CUERPO (PADRE PRINCIPAL)
		color = glm::vec3(0.0f, 0.0f, 1.0f); // Azul
		model = glm::mat4(1.0);
		// Levantamos el cuerpo a Y = 0.0f para que no atraviese el piso (que está en Y = -2.0f)
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -1.5f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cuerpo_M.RenderModel();

		modelaux2 = model; // GUARDAMOS EL SISTEMA DE COORDENADAS DEL CUERPO

		// --- BRAZO (HIJO DEL CUERPO) --- LISTO
		model = modelaux2;
		// Aplicamos tus 2 espacios hacia arriba en Y
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Brazo_M.RenderModel();


		// --- PATA DELANTERA DERECHA (HIJA DEL CUERPO) --- LISTO
		model = modelaux2;
		// Aplicamos tus 3 espacios hacia la derecha en X
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // GUARDAMOS LA COORDENADA DE LA PATA DERECHA

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataDelanteraDerecha_M.RenderModel();

		// Llanta Delantera Derecha (HIJA DE PATA DEL. DER.)
		model = modelaux;
		// Aplicamos tus 2 espacios de separación extra en X para la llanta
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDelanteraDerecha_M.RenderModel();


		// --- PATA DELANTERA IZQUIERDA (HIJA DEL CUERPO) --- LISTO
		model = modelaux2;
		// 3 espacios hacia la izquierda en X (-3.0f)
		model = glm::translate(model, glm::vec3(-0.0f, -0.5f, 3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataDelanteraIzquierda_M.RenderModel();

		// Llanta Delantera Izquierda (HIJA DE PATA DEL. IZQ.)
		model = modelaux;
		// 2 espacios hacia la izquierda (-2.0f)
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDelanteraIzquierda_M.RenderModel();





		// --- PATA TRASERA DERECHA (HIJA DEL CUERPO) ---
		model = modelaux2;
		// 3 espacios a la derecha
		model = glm::translate(model, glm::vec3(0.0f, -0.2f, -3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataTraseraDerecha_M.RenderModel();

		// Llanta Trasera Derecha 01
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.0f, -2.0f)); // 2 espacios de separación
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantasTraserasDerechas_01_M.RenderModel();

		// Llanta Trasera Derecha 02 
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.0f, -2.0f)); // 2 espacios de separación
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantasTraserasDerechas_02_M.RenderModel();




		// --- PATA TRASERA IZQUIERDA (HIJA DEL CUERPO) ---
		model = modelaux2;
		// 3 espacios hacia el lado izquierdo (Z positivo)
		model = glm::translate(model, glm::vec3(0.0f, -0.2f, 3.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataTraseraIzquierda_M.RenderModel();

		// Llanta Trasera Izquierda 01
		model = modelaux;
		// 2 espacios hacia el lado izquierdo (Z positivo)
		model = glm::translate(model, glm::vec3(0.0f, -0.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantasTraserasIzquierdas_01_M.RenderModel();

		// Llanta Trasera Izquierda 02
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantasTraserasIzquierdas_02_M.RenderModel();

		




		// -----------------------------------------------------------------
		// 2. HOLOCRÓN (Jerarquía y Rotación Independiente)
		// -----------------------------------------------------------------
		model = glm::mat4(1.0);
		// Lo posicionamos donde no choque con el Rover (Puedes ajustar estos números)
		model = glm::translate(model, glm::vec3(15.0f, 2.0f, -5.0f));
		glm::mat4 modelaux_holocron = model; // Matriz Padre

		color = glm::vec3(1.0f, 0.0f, 0.0f); // Color rojo de base
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cuerpo_holocron_M.RenderModel();

		// Creamos los ejes de rotación en diagonal usando los valores X y Y que separaste
		glm::vec3 ejesDiagonales[8] = {
			glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)), glm::normalize(glm::vec3(-1.0f, 1.0f, 0.0f)),
			glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f)), glm::normalize(glm::vec3(-1.0f, -1.0f, 0.0f)),
			glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)), glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f)),
			glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)), glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f))
		};

		// Arreglo de los ángulos leídos desde la ventana
		GLfloat holocronAngles[8] = {
			mainWindow.getRotHolo1(), mainWindow.getRotHolo2(), mainWindow.getRotHolo3(), mainWindow.getRotHolo4(),
			mainWindow.getRotHolo5(), mainWindow.getRotHolo6(), mainWindow.getRotHolo7(), mainWindow.getRotHolo8()
		};

		// Referencias a los modelos de las esquinas
		// Referencias a los modelos de las esquinas
		Model* esquinas[8] = {
			&holocron_esquina_01_M, &holocron_esquina_02_M, &holocron_esquina_03_M, &holocron_esquina_04_M,
			&holocron_esquina_05_M, &holocron_esquina_06_M, &holocron_esquina_07_M, &holocron_esquina_08_M
		};

		glm::vec3 ejesRotacion[8];

		// --- LAS 6 QUE YA QUEDARON PERFECTAS ---
		ejesRotacion[0] = glm::vec3(-1.0f, 1.0f, 1.0f); // Tecla 1 
		ejesRotacion[1] = glm::vec3(1.0f, -1.0f, 1.0f); // Tecla 2 
		ejesRotacion[3] = glm::vec3(1.0f, -1.0f, -1.0f); // Tecla 4 
		ejesRotacion[4] = glm::vec3(-1.0f, 1.0f, -1.0f); // Tecla 5 
		ejesRotacion[6] = glm::vec3(-1.0f, -1.0f, 1.0f); // Tecla 7 
		ejesRotacion[7] = glm::vec3(1.0f, 1.0f, 1.0f); // Tecla 8 

		// --- LAS ÚLTIMAS 2 (Ya intercambiadas) ---
		ejesRotacion[2] = glm::vec3(-1.0f, -1.0f, -1.0f); // Tecla 3 
		ejesRotacion[5] = glm::vec3(1.0f, 1.0f, -1.0f); // Tecla 6 

		// Renderizado de cada esquina
		for (int i = 0; i < 8; i++) {
			model = modelaux_holocron;

			// Cada pieza girará sobre su eje diagonal correcto
			model = glm::rotate(model, glm::radians(holocronAngles[i]), ejesRotacion[i]);

			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			esquinas[i]->RenderModel();
		}





		// -----------------------------------------------------------------
		// 3. SATÉLITE (Jerarquía, Movimiento y Rotación en la Conexión)
		// -----------------------------------------------------------------
		model = glm::mat4(1.0);
		
		model = glm::translate(model, glm::vec3(-16.0f + mainWindow.getSatX(), 4.0f + mainWindow.getSatY(), 0.0f + mainWindow.getSatZ()));
		glm::mat4 modelaux_satelite = model; // Matriz Padre

		color = glm::vec3(0.8f, 0.8f, 0.8f); // Gris claro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cuerpo_satelite_M.RenderModel();

		// Panel 1
		model = modelaux_satelite;
		
		model = glm::rotate(model, glm::radians(mainWindow.getRotPanel1()), glm::vec3(0.0f, 0.0f, 1.0f));//más o menos
		// Separación perfecta
		model = glm::translate(model, glm::vec3(-0.5f, -0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		satelite_panel_01_M.RenderModel();

		// Panel 2
		model = modelaux_satelite;
		model = glm::rotate(model, glm::radians(mainWindow.getRotPanel2()), glm::vec3(0.0f, 1.0f, 1.0f));//nopi
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		satelite_panel_02_M.RenderModel();

		// Panel 3
		model = modelaux_satelite;
		// Diferente eje de rotación para variar el contexto visual
		model = glm::rotate(model, glm::radians(mainWindow.getRotPanel3()), glm::vec3(0.0f, 0.0f, 1.0f));//nopi
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f)); // Separado 1 unidad hacia arriba en Y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		satelite_panel_03_M.RenderModel();














		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
