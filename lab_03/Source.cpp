#include <glut.h>
#include <iostream>
#include <locale.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

// ------- ��������� ���� ----------------------------
#define WIN_WIDTH 750 // ������ ����
#define WIN_HEIGHT 750 // ������ ����

// -------------- ��������� �������� ------------------------
GLfloat octX = 100.0; // �������� ���������� �� ��
GLfloat octY = 150.0; // �������� ���������� �� ��
GLfloat octZ = 100.0; // �������� ���������� �� �z

static bool autoRotateROctY = false; // �������������� �������� �� �������
static bool autoRotateLOctY = false; // �������������� �������� ������ �������
GLint rotateOctCount = 0; // ������� ��� ������ ������� ��������

GLfloat rotateOctX = 20.0; // ����������� �������� �� Ox
GLfloat rotateOctY = 20.0; // ����������� �������� �� O�

GLfloat sideShift = 10.0; // ���������� ����������� ������

// ������� ������ ������ ��������
GLfloat point1[3] = { 0.0, 0.0, 0.0 };
GLfloat point2[3] = { 0.0, 0.0, 0.0 };
GLfloat point3[3] = { 0.0, 0.0, 0.0 };

GLint colorMode = 0; // ����� ������ (0 - �����, 1 - ���� ��������, 2 - 8 �������)

static GLuint tex[9]; // ������ �������

static bool isBlend = false; // ������������

// -------------------- ��������� ��������� ����� ---------------------

static bool autoRotateLightR = false;
static bool autoRotateLightL = false;
GLfloat rotateLight = 1.0; // ���� ��������
GLint rotateSphereCount = 0; // ������� ��� ������ ������� ��������
static bool isLight = true; // ���������

// ----------------- DISPLAY LIST PARAM ---------------------

static bool isListOct = false; // display list
static GLuint list; // ���������� ��� ������
GLint stripes = 100; // ���������� �������

// ----------------- ������ ��������� ----------------------

GLint speed = 1; // �������� ��������

// ------------------- ������������ ------------------------
// ������� ������� �������� �� ������ "Calculating a Surface Normal"
// � ����� www.khronos.org/opengl/wiki
void getNormal(float p1[3], float p2[3], float p3[3], float vNormal[3])
{
    float v1[3], v2[3]; // ��� ������������� ����������
    float normal; // �����

    v1[0] = p2[0] - p1[0];
    v1[1] = p2[1] - p1[1];
    v1[2] = p2[2] - p1[2];

    v2[0] = p3[0] - p1[0];
    v2[1] = p3[1] - p1[1];
    v2[2] = p3[2] - p1[2];

    vNormal[0] = v1[1] * v2[2] - v1[2] * v2[1];
    vNormal[1] = v1[2] * v2[0] - v1[0] * v2[2];
    vNormal[2] = v1[0] * v2[1] - v1[1] * v2[0];

    normal = sqrt(vNormal[0] * vNormal[0] + vNormal[1] * vNormal[1] + vNormal[2] * vNormal[2]);
    // ������������
    vNormal[0] /= normal;
    vNormal[1] /= normal;
    vNormal[2] /= normal;
}

// --------------------- ��������� ����� --------------------
void drawSphere()
{
    glColor3f(1.0, 1.0, 1.0); // ����� �������� ����� ����
    glTranslatef(0.0, 0.0, 200.0); // ��������� ����� �� �����
    glutWireSphere(10.0, 10.0, 10.0); // ����������� ������� ������� Wire Sphere
}

// ---------------- ��������� ������ DISPLAY LIST -----------
void sideList()
{
    point1[0] = octX + sideShift;      
    point1[1] = point1[2] = sideShift;

    point2[0] = point2[2] = sideShift; 
    point2[1] = octY + sideShift;

    point3[0] = point3[1] = sideShift; 
    point3[2] = octZ + sideShift;

    float norm[3] = { 0.0 , 0.0 , 0.0 }; // ������ ��� �������
    getNormal(point1, point2, point3, norm); // ��������� �������
    glNormal3fv(norm); //������� � ����������� ��������� ����������

    // ���������� ������
    for (int i = 0; i < stripes; i++)
    {
        glBegin(GL_POLYGON);
            glVertex3f(point1[0] - i, point1[1] + i * 1.5, point1[2]);
            glVertex3f(point3[0], point3[1] + i * 1.5, point3[2] - i);
            glVertex3f(point1[0] - (i - 1), point1[1] + (i + 1) * 1.5, point1[2]);
            glVertex3f(point3[0], point3[1] + (i + 1) * 1.5, point3[2] - (i - 1));
            i += 0.1;
        glEnd();
    }
}

// --------------- ��������� �������� DISPLAY LIST ----------
void drawOctList()
{
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    sideList(); // ������ �����

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    sideList();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    sideList();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(270, 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 0.0);
    sideList();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 1.0);
    sideList();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    glColor3f(0.0, 1.0, 1.0);
    sideList();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(270, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.5, 0.0);
    sideList();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(360, 0.0, 1.0, 0.0);
    glColor3f(0.25, 0.25, 0.25);
    sideList();
    glPopMatrix();
    glPopMatrix();
}

// --------------------- ��������� ������ -------------------
void side(int countSide = 0)
{
    glBegin(GL_TRIANGLES);

    point1[0] = octX + sideShift;
    point1[1] = point1[2] = sideShift;

    point2[0] = point2[2] = sideShift;
    point2[1] = octY + sideShift;

    point3[0] = point3[1] = sideShift;
    point3[2] = octZ + sideShift;

    float norm[3] = { 0.0 , 0.0 , 0.0 }; // ������ ��� �������
    getNormal(point1, point2, point3, norm);  // ��������� �������
    glNormal3fv(norm); // ������� � ����������� ��������� ����������

    if (colorMode % 3) // ���� ������� ����� �������
    {
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(point1);

        glTexCoord2f(1.0, 0.0);
        glVertex3fv(point2);

        glTexCoord2f(0.0, 1.0);
        glVertex3fv(point3);
    }
    else if (!(colorMode % 3) && countSide) // ���� ����� ���������� ����������� ����� (����� �������� �����)
    {
        glColor3f(0.0, 0.0, 1.0);
        glVertex3fv(point1);

        glColor3f(0.0, 1.0, 0.0);
        glVertex3fv(point2);

        glColor3f(1.0, 0.0, 0.0);
        glVertex3fv(point3);
    }
    else // ������ ������� ����� (����� �������� � ������� drawOct())
    {
        glVertex3fv(point1);
        glVertex3fv(point2);
        glVertex3fv(point3);
    }

    glEnd();
}

// --------------------- ��������� �������� -----------------
void drawOct()
{
    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[0]); // �������� �������� � ���� ���������������
    if ((colorMode % 3) == 1) glBindTexture(GL_TEXTURE_2D, tex[8]); 
    glPushMatrix();
    if (!(colorMode % 3)) glColor3f(1.0, 0.0, 0.0);
    side(); // ������ �����
    
    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[1]);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    if (!(colorMode % 3)) glColor3f(0.0, 1.0, 0.0);
    side();
    glPopMatrix();

    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[2]);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    if (!(colorMode % 3)) glColor3f(0.0, 0.0, 1.0);
    side();
    glPopMatrix();

    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[3]);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(270, 0.0, 1.0, 0.0);
    if (!(colorMode % 3)) glColor3f(1.0, 1.0, 0.0);
    side();
    glPopMatrix();

    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[4]);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    if (!(colorMode % 3)) glColor3f(1.0, 0.0, 1.0);
    side();
    glPopMatrix();

    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[5]);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    if (!(colorMode % 3)) glColor3f(0.0, 1.0, 1.0);
    side();
    glPopMatrix();

    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[6]);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(270, 0.0, 1.0, 0.0);
    if (!(colorMode % 3)) glColor3f(1.0, 0.5, 0.0);
    side();
    glPopMatrix();

    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[7]);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(360, 0.0, 1.0, 0.0);
    side(7); // ���� ����� �������� ����� �������� int, ������� side() �����, ��� ��� ���������� �����
    glPopMatrix();
    glPopMatrix();
}

// -------------------- ��������� ����� ---------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������� ������ ����� � �������
    glLoadIdentity(); // �������� ������� ������� �� ���������

    if (isLight) glEnable(GL_LIGHT0); // �������� �������� ����� GL_LIGHT0
    else glDisable(GL_LIGHT0); // ��������� �������� ����� GL_LIGHT0

    if (isBlend) // ���� ������� ����� ������������ 
    {
        glEnable(GL_BLEND); // �������� ����� ����������
        glDepthMask(GL_FALSE); // ��������� ������ � ����� �������
        glBlendFunc(GL_SRC_ALPHA, GL_ONE); // ������� ���������� ��� ��������������, ������������ �� �������� �����
    }
    else // ���� ����� ������������ ��������
    {
        glDepthMask(GL_TRUE); // ��������� ������ � ����� �������
        glDisable(GL_BLEND); // ��������� ����� ����������
    }

    if (colorMode % 3) // ���� ������� ����� �������
    {
        glEnable(GL_TEXTURE_2D); // ��������� ��������� ��������
        glPushMatrix();
        glRotatef(rotateOctX, 1.0, 0.0, 0.0);
        glRotatef(rotateOctY, 0.0, 1.0, 0.0);
        drawOct(); // ������ �������
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
    else // ���� ������� ����� �����������
    {
        glPushMatrix();
        glRotatef(rotateOctX, 1.0, 0.0, 0.0); // ������� �� O�
        glRotatef(rotateOctY, 0.0, 1.0, 0.0); // ������� �� O�
        if (!isListOct)
            drawOct(); // ������ �������
        else
            glCallList(list); // �������� ��������� ������ �����������
        glPopMatrix();
    }

    glRotatef(rotateLight, 0.0, 1.0, 0.0); // �������� ����� ������������ 'Oy' ��� ����� 1
    drawSphere(); // ��������� �����
    float lightpos[] = { 0.0, 0.0, 200.0, 0.0 }; // ���������� GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos); // ������� ��������� ��������� �����
    glEnable(GL_LIGHTING); // ��������� ���������

    glutSwapBuffers(); // ���������� ������, ��������� ���� ������� ����� ����, ��� ��� ��� ������������
    glFlush(); // ���������� ��� ������� �� ������������, �� ��������� ���������� ���������� ������. 
}

// ------------- ������� ����������� ����������� -----------------
void reshape(int w, int h)
{
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT); // ������� ������� ������ (0,0) - ����� ������ ����, (w, h) - ������ � ������
    glMatrixMode(GL_PROJECTION); // ������� ������������� ��������;
                                 // ������� �������� ������ ��� ����� �������������� ���������� �������
                                 // �� ��������� ������ (� ������� ����������)
    glLoadIdentity(); // �������� ������� ������� �� ���������
    glOrtho(-WIN_WIDTH / 2, WIN_WIDTH / 2, -WIN_HEIGHT / 2, WIN_HEIGHT / 2, -500, 500); // ������� ��������������� (������������) ��������
    glMatrixMode(GL_MODELVIEW); // ����� ����� ����������� ������ �������������� �������� �����
    glLoadIdentity();
}

// -------------- ������ --------------------
void timer(int value)
{
    if (autoRotateROctY) // Ox
    {
        rotateOctY += speed;
        if (rotateOctY > 360) rotateOctY = speed;
    }
    else if (autoRotateLOctY) // Oy
    {
        rotateOctY -= speed;
        if (rotateOctY < -360) rotateOctY = -speed;
    }

    if (autoRotateLightR) // Ox
    {
        rotateLight += speed;
        if (rotateLight > 360) rotateLight = speed;
    }
    else if (autoRotateLightL) // Oy
    {
        rotateLight -= speed;
        if (rotateLight < -360) rotateLight = -speed;
    }

    glutPostRedisplay(); //����������� ������
    glutTimerFunc(10, timer, 1); //10 - ����� � �������������, 1 - ������������� �������
}

// -------------- ���������� ������ --------------
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'r': // �������� ��������
            if (rotateOctCount == 0)
            {
                autoRotateROctY = true;
                rotateOctCount++;
            }
            else if (rotateOctCount == 1)
            {
                autoRotateROctY = false;
                autoRotateLOctY = true;
                rotateOctCount++;
            }
            else {
                autoRotateLOctY = false;
                rotateOctCount = 0;
            }
            break;

        case 'd': // ������� �������
            rotateOctY += 5;
            break;

        case 'a': // ������� ������
            rotateOctY -= 5;
            break;

        case 'w': // ������ �����
            rotateOctX += 5;
            break;

        case 's': // ������ ����
            rotateOctX -= 5;
            break;

        case 'z': // �������� ������ �����
            rotateLight = (rotateLight >= 0) ? rotateLight + 5 : 360;
            break;

        case 'x': // �������� ������ ������
            rotateLight = (rotateLight <= 0) ? 360 : rotateLight - 5;
            break;

        case 'e': // �������� �����
            if (rotateSphereCount == 0)
            {
                autoRotateLightR = true;
                rotateSphereCount++;
            }
            else if (rotateSphereCount == 1)
            {
                autoRotateLightR = false;
                autoRotateLightL = true;
                rotateSphereCount++;
            }
            else {
                autoRotateLightL = false;
                rotateSphereCount = 0;
            }
            break;
        
        case 'l' : // ��������� �����
            isLight = isLight ? false : true;
            break;

        case 'b':
            isBlend = isBlend ? false : true;
            break;

        case 't':
            colorMode += 1;
            break;

        case 'q':
            isListOct = isListOct ? false : true;
            break;
        
        default:
            break;
    }

        glutPostRedisplay();
}

// ----------------- ������� ������� -----------------
void loadTexture()
{
    unsigned char* image[9]; // ������ �����������
    int w[9], h[9], bpp[9]; // ������, ������, ���������� 8-������ ����������� �� �������

    image[0] = stbi_load("img/img1.png", &w[0], &h[0], &bpp[0], STBI_rgb_alpha);
    image[1] = stbi_load("img/img2.png", &w[1], &h[1], &bpp[1], STBI_rgb_alpha);
    image[2] = stbi_load("img/img3.png", &w[2], &h[2], &bpp[2], STBI_rgb_alpha);
    image[3] = stbi_load("img/img4.png", &w[3], &h[3], &bpp[3], STBI_rgb_alpha);
    image[4] = stbi_load("img/img5.png", &w[4], &h[4], &bpp[4], STBI_rgb_alpha);
    image[5] = stbi_load("img/img6.png", &w[5], &h[5], &bpp[5], STBI_rgb_alpha);
    image[6] = stbi_load("img/img7.png", &w[6], &h[6], &bpp[6], STBI_rgb_alpha);
    image[7] = stbi_load("img/img8.png", &w[7], &h[7], &bpp[7], STBI_rgb_alpha);
    image[8] = stbi_load("img/img9.png", &w[8], &h[8], &bpp[8], STBI_rgb_alpha);

    //���������������
    for (int i = 0; i < 9; i++)
    {
        glGenTextures(1, &tex[i]); // �������� ��������������� �������
        glBindTexture(GL_TEXTURE_2D, tex[i]); // �������� � ������������ ����������� ��������
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // ��������, ��� ������������ � ������� ������ ��� �� �����
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w[i], h[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, image[i]); // ��������, ��� ���������� ����������� - ��������
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // ����������
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // ����������
        stbi_image_free(image[i]); // �� �� ����� ��� free()
    }
}

void init()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������� ������� ����� � �������
    glEnable(GL_DEPTH_TEST); // ��������� ����� �������. OpenGL ����� ������������� ��������� �������� ������� ��� ���� ��������� ���� ���������� � ����������� �� ���������
    glEnable(GL_LIGHTING); // ��������� ����������� ���������
    glEnable(GL_LIGHT0); // ��������� ��������� �����
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // ������������� ���������

    GLfloat light_col[] = { 1.0, 1.0, 1.0, 1.0 }; // ���� GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_col); // ������� ������ ��������� ���������� ����
    glEnable(GL_COLOR_MATERIAL); // �������� ���������� ��������� ��������� � ������� �������� �����

    list = glGenLists(1); // ���������� ����������� ����� ������ ������� �����������
    glNewList(list, GL_COMPILE); // ������������ ������; ������ ����� ������ ����������� (GL_COMPILE)
    drawOctList(); // ��������� �������� � ������� ������ �����������
    glEndList(); // ��������� ������ �� �������

    loadTexture(); // ������������ ��������
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "RUSSIAN"); // ��������� ������� ����

    glutInit(&argc, argv); // ������������� ������� ���������� GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // ����� ������ �����������
    // GLUT_DOUBLE - ����� � ���� �������������� � �������������� 2 �������. ����������� ��� ��������, ����� ��������� ������ ��������.
    // GLUT_RGB - ��� ����������� ����������� ���������� ������������ 3 ���������� ����� RGB.
    // GLUT_DEPTH - ����� �������. ���� ����� ������������ ��� ��������� ��������� ����� � 3D ������������ ��� ������ �� ������� ����� ��������.

    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT); // ������� ���������� ����
    glutCreateWindow("Oct"); // �������� ����

    // ����������� ���������� �������
    glutDisplayFunc(display);
    glutTimerFunc(10, timer, 1); // ����� � �������������, �������, ������������ �������
    init();
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    // ���������� �� ������ � �����������
    cout << "�������:" << endl
        << "l: ����./���. ����" << endl
        << "r: �������� ��������" << endl
        << "e: �������� ��������� �����" << endl
        << "w: ������ �������� �����" << endl
        << "a: ������� �������� �����" << endl
        << "s: ������ �������� ����" << endl
        << "d: ������� �������� ������" << endl
        << "z: ������� ��������� ����� �����" << endl
        << "x: ������� ��������� ����� ������" << endl
        << "b: ����./���. ������������" << endl
        << "t: �������� ������� ��������" << endl
        << "q: ����������� ����� display list" << endl;

    glutMainLoop(); // �������� ���� �������

    return 0;
}