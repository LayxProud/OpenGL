#define _USE_MATH_DEFINES
#include <iostream>     
#include <glut.h>
#include <math.h>
using namespace std;

float shift = 0.0; // ������������ ��������
bool isDay = true; // ���� ������� �����

// ������ ��������
void timer(int value)
{
	if (shift >= 2.4) // ������/���� �� �������
	{
		isDay = !isDay; // ����� �����
		shift = 0; // ��������� ������������ ��������
	}

	shift += 0.01;
	glutPostRedisplay(); // �������� �������� ����, ������� ������ ���� ������������, ��� ������ ��������� �����
	glutTimerFunc(25, timer, 0);
}

// ����������� ������������
float light() 
{ 
	if (isDay) {
		if (shift <= 0.8) // ����
		{
			return (shift + 0.4);
		}
		if ((shift > 0.8) && (shift <= 1.2)) // �������
		{
			return (1.2);
		}
		return(2.4 - shift); // �����
	}

	else
	{
		if (shift >= 1.2) // �������
		{
			return(0.4);
		}
		return((shift / 3)); // ����
	}
}

// ������� ���������� �����
void drawTerrain()
{
	glColor3f(0.1 * light(), 0.6 * light(), 0 * light());
	glBegin(GL_QUADS);
	glVertex2f(-1, -1);
	glVertex2f(-1, -0.5);
	glVertex2f(1, -0.5);
	glVertex2f(1, -1);
	glEnd();
}

// ������� ���������� ��������� ����
void drawHouse()
{
	glColor3f(0.3 * light(), 0.2 * light(), 0.05 * light());
	glBegin(GL_QUADS);
	glVertex2f(-0.75, -0.75);
	glVertex2f(-0.75, -0.25);
	glVertex2f(-0.25, -0.25);
	glVertex2f(-0.25, -0.75);
	glEnd();
}

// ������� ���������� ����� ����
void drawRoof()
{
	glColor3f(0.9 * light(), 0.05 * light(), 0.05 * light());
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.8, -0.25);
	glVertex2f(-0.5, 0);
	glVertex2f(-0.2, -0.25);
	glEnd();
}

// ������� ���������� ���� ����
void drawWindow()
{
	// ������
	if (isDay == true) // ��� ����� ���
	{
		glColor3f(1, 1, 1);
	}
	else glColor3f(0.8, 1, 0); // ����� ����� ����

	glBegin(GL_QUADS);
	glVertex2f(-0.6, -0.6);
	glVertex2f(-0.6, -0.4);
	glVertex2f(-0.4, -0.4);
	glVertex2f(-0.4, -0.6);
	glEnd();

	// ������� ����
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(-0.5, -0.6);
	glVertex2f(-0.5, -0.4);
	glVertex2f(-0.6, -0.5);
	glVertex2f(-0.4, -0.5);
	glEnd();
}

// ������� ���������� ������
void drawSun() 
{
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * M_PI; i += M_PI / 12) // i �� 0 �� 2 ��; ������ �������� ���������� ������� � ��������������� ���������
		glVertex2f(cos(i) * 0.2 - 1.2, sin(i) * 0.2 + 0.5);
	glEnd();
}

// ������� ���������� ����
void drawMoon()
{
	// ���������� ������ �����
	glColor3f(1.0, 1.0, 0.0); 
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * 3.14; i += 3.14 / 12)
		glVertex2f(cos(i) * 0.2 - 1.2, sin(i) * 0.2 + 0.5);
	glEnd();

	// ���������� ����� ����
	glColor3f(0.1 + (light() * 0.2), 0.1 + (light() * 0.2), 0.3 + (light() * 0.7));
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * 3.14; i += 3.14 / 12)
		glVertex2f(cos(i) * 0.2 - 1.3, sin(i) * 0.2 + 0.5);
	glEnd();
}

// ������� ���������
void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // ������� ������ �����
	glClearColor(0.1 + (light() * 0.2), 0.1 + (light() * 0.2), 0.3 + (light() * 0.7), 1.0); 
	glLoadIdentity();
	drawTerrain();
	drawHouse();
	drawRoof();
	drawWindow();
	glTranslatef(shift, 0.0, 0.0);
	if (isDay) 
	{ 
		drawSun(); 
	}
	else 
	{ 
		drawMoon(); 
	}
	glFlush(); // ��������� ������
}

// ������� ��� "�����������" ����������� ��� ��������� �������� ����
void reshape(int Width, int Height)
{
	if (Height == 0)
	{
		Height = 1;
	}

	glViewport(0, 0, (GLsizei)Width, (GLsizei)Height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)Width, 0.0, (GLdouble)Height);

	display();
}

// ������� �������
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(500, 200);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Main window");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	printf("Laba 1");
	glutTimerFunc(25, timer, 0);
	glutMainLoop();

	return 0;
}

