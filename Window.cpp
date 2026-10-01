#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;

	satX = 0.0f; satY = 0.0f; satZ = 0.0f;
	rotPanel1 = 0.0f; rotPanel2 = 0.0f; rotPanel3 = 0.0f;
	rotHolo1 = 0.0f; rotHolo2 = 0.0f; rotHolo3 = 0.0f; rotHolo4 = 0.0f;
	rotHolo5 = 0.0f; rotHolo6 = 0.0f; rotHolo7 = 0.0f; rotHolo8 = 0.0f;
	
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica XX: Nombre de la práctica", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	
	if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}

	




	// PATA DELANTERA DERECHA (Teclas F y G)
	if (key == GLFW_KEY_F)
	{
		if (theWindow->articulacion1 < 4.5f) {
			theWindow->articulacion1 += 0.1;
		}
	}
	if (key == GLFW_KEY_G)
	{
		if (theWindow->articulacion1 > -4.5f) {
			theWindow->articulacion1 -= 0.1;
		}
	}

	// PATA DELANTERA IZQUIERDA (Teclas H y J)
	if (key == GLFW_KEY_H)
	{
		if (theWindow->articulacion2 < 4.5f) {
			theWindow->articulacion2 += 0.1;
		}
	}
	if (key == GLFW_KEY_J)
	{
		if (theWindow->articulacion2 > -4.5f) {
			theWindow->articulacion2 -= 0.1;
		}
	}

	// PATA TRASERA DERECHA (Teclas K y L)
	if (key == GLFW_KEY_K)
	{
		if (theWindow->articulacion3 < 4.5f) {
			theWindow->articulacion3 += 0.1;
		}
	}
	if (key == GLFW_KEY_L)
	{
		if (theWindow->articulacion3 > -4.5f) {
			theWindow->articulacion3 -= 0.1;
		}
	}

	// PATA TRASERA IZQUIERDA (Teclas Z y X)
	if (key == GLFW_KEY_Z)
	{
		if (theWindow->articulacion4 < 4.5f) {
			theWindow->articulacion4 += 0.1;
		}
	}
	if (key == GLFW_KEY_X)
	{
		if (theWindow->articulacion4 > -4.5f) {
			theWindow->articulacion4 -= 0.1;
		}
	}





	// --- MOVIMIENTO SATÉLITE ---
	// Ejes X y Y con Flechas Direccionales
	if (key == GLFW_KEY_UP) { theWindow->satY += 0.2f; }
	if (key == GLFW_KEY_DOWN) { theWindow->satY -= 0.2f; }
	if (key == GLFW_KEY_RIGHT) { theWindow->satX += 0.2f; }
	if (key == GLFW_KEY_LEFT) { theWindow->satX -= 0.2f; }
	// Eje Z con teclas N y M
	if (key == GLFW_KEY_N) { theWindow->satZ -= 0.2f; }
	if (key == GLFW_KEY_M) { theWindow->satZ += 0.2f; }

	// --- ROTACIÓN PANELES SATÉLITE --- (Teclas C, V, B)
	if (key == GLFW_KEY_C) { theWindow->rotPanel1 += 2.0f; }
	if (key == GLFW_KEY_V) { theWindow->rotPanel2 += 2.0f; }
	if (key == GLFW_KEY_B) { theWindow->rotPanel3 += 2.0f; }

	// --- ROTACIÓN ESQUINAS HOLOCRÓN --- (Teclas numéricas 1 al 8)
	if (key == GLFW_KEY_1) { theWindow->rotHolo1 += 2.0f; }
	if (key == GLFW_KEY_2) { theWindow->rotHolo2 += 2.0f; }
	if (key == GLFW_KEY_3) { theWindow->rotHolo3 += 2.0f; }
	if (key == GLFW_KEY_4) { theWindow->rotHolo4 += 2.0f; }
	if (key == GLFW_KEY_5) { theWindow->rotHolo5 += 2.0f; }
	if (key == GLFW_KEY_6) { theWindow->rotHolo6 += 2.0f; }
	if (key == GLFW_KEY_7) { theWindow->rotHolo7 += 2.0f; }
	if (key == GLFW_KEY_8) { theWindow->rotHolo8 += 2.0f; }







	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
	





	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
