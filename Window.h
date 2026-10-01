#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	GLfloat getarticulacion5() { return articulacion5; }
	GLfloat getarticulacion6() { return articulacion6; }

	GLfloat getSatX() { return satX; }
	GLfloat getSatY() { return satY; }
	GLfloat getSatZ() { return satZ; }

	GLfloat getRotPanel1() { return rotPanel1; }
	GLfloat getRotPanel2() { return rotPanel2; }
	GLfloat getRotPanel3() { return rotPanel3; }

	GLfloat getRotHolo1() { return rotHolo1; }
	GLfloat getRotHolo2() { return rotHolo2; }
	GLfloat getRotHolo3() { return rotHolo3; }
	GLfloat getRotHolo4() { return rotHolo4; }
	GLfloat getRotHolo5() { return rotHolo5; }
	GLfloat getRotHolo6() { return rotHolo6; }
	GLfloat getRotHolo7() { return rotHolo7; }
	GLfloat getRotHolo8() { return rotHolo8; }

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat rotax,rotay,rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6;
	// Variables de posición del satélite
	GLfloat satX, satY, satZ;
	// Variables de rotación de los paneles
	GLfloat rotPanel1, rotPanel2, rotPanel3;
	// Variables de rotación independiente de las 8 esquinas del holocrón
	GLfloat rotHolo1, rotHolo2, rotHolo3, rotHolo4, rotHolo5, rotHolo6, rotHolo7, rotHolo8;

	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

