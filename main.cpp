#include <GL/gl.h>
#include <Gl/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <string>
#include <windows.h>
#include "include/BmpLoader.h"
#include "include/glext.h"

float angle = 60.0;
float x_val = -20, z_val = 0;
float mov = 0;
float randombuild;
const double PI = 3.14159265389;
//float x=0.0f,z=5.0f;
//float lx=0.0f,lz=-1.0f;
double dist = sqrt(1 / 3.0);
//float zvalue = 0;
//float deltaAngle = 0.0f;
//float deltaMove = 0;
float ex=-38.0, ey=5.0, ez=0.0;
float lx=-20.0, ly=0.0, lz=0.0;
float buildpos = 0;
float buildheight[] = {4, 3, 2, 4, 5, 4.3, 3.6, 13, 5.1, 8};
float houseloc[] = {12, 16, 15, 15, 14, 17, 19, 17, 18, 16, 20};
GLfloat xPlane[] = { 1.0f, 5.0f, 0.0f, 0.0f };
bool car = false;
bool paused = false;
bool light0 = true;
bool light1 = true;
int menuFlag = 0;
unsigned int ID;


//lighting
const GLfloat light_ambient[]  = { 0.6f, 0.6f, 0.6f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 10.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 40.0f };

const GLfloat light1_ambient[]  = { 0.1f, 0.1f, 0.1f, 1.0f };

GLfloat carlight_position[] = { x_val, -1, z_val, 1.0 };
GLfloat carlight_direction[] = { 0, 1, 0.0 };

//GLfloat ctrlpoints[4][3] = {
//        {510, 0, -7}, {510, 10, -4},
//        {510, 10, 4}, {510, 0, 7}};

//control points for curved tunnel
GLfloat ctrlpoints[4][4][3] = {
   {{510, -1.5, -7}, {510, 10, -4},
        {510, 10, 4}, {510, -1.5, 7}},
   {{540, -1.5, -7}, {540, 10, -4},
        {540, 10, 4}, {540, -1.5, 7}},
   {{570, -1.5, -7}, {570, 10, -4},
        {570, 10, 4}, {570, -1.5, 7}},
   {{600, -1.5, -7}, {600, 10, -4},
        {600, 10, 4}, {600, -1.5, 7}}
};


    ///function declarations

void display();
void reshape(int, int);
void timer(int);

void drawCar();
void drawBuilding(float, int);
void drawSphereTree();
void drawConicTree();
void drawHouse();
void drawStreetLamp();

static void drawBox(GLfloat size, GLenum type);
void renderBitmapString(float,float,float,void *font, char *string);


void processNormalKeys(unsigned char key, int x, int y);
void LoadTexture(const char*filename);


void init()
{
    glClearColor(0, 0, 0, 1.0);

//    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
//    glEnable(GL_MAP1_VERTEX_3);

    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
           0, 1, 12, 4, &ctrlpoints[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

//    gluLookAt(	10, -9.5, 7,
//			5, -5,  0,
//			0.0f, 1.0f,  0.0f);

    gluLookAt(	ex, ey, ez,
			lx, ly,  lz,
			0.0f, 1.0f, 0.0f);

    //ground
    glBegin(GL_QUADS);

    glColor3f(0, 0, 1);
    glVertex3f(-100.0f, -1.7f, -30.0f);
    glVertex3f(-100.0f, -1.7f,  30.0f);

    glColor3f(0.1, 0.9, 0.1);
    glVertex3f( 500.0f, -1.7f,  30.0f);
    glVertex3f( 500.0f, -1.7f, -30.0f);

    glEnd();

    //road
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 1);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glBegin(GL_QUADS);
        //road
    glColor3f(1, 1, 1);
    glVertex3f(-100.0f, -1.5f, -7.0f);
        glTexCoord2f(1,1);
    glVertex3f(-100.0f, -1.5f,  7.0f);
        glTexCoord2f(1,0);
    glColor3f(1, 1, 1);
    glVertex3f( 700.0f, -1.5f,  7.0f);
        glTexCoord2f(0,0);
    glVertex3f( 700.0f, -1.5f, -7.0f);
        glTexCoord2f(0,1);

    glEnd();

    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
        drawCar();
    glPopMatrix();

    srand(3);

    ///code for testing each object individually
//    glPushMatrix();
//        glTranslatef(30, 0, 15);
//        drawBuilding(12);
//    glPopMatrix();
//
//    glPushMatrix();
//        glTranslatef(0, 0, -10);
//        drawSphereTree();
//    glPopMatrix();

//    glPushMatrix();
//        glTranslatef(10, 0, -10);
//        drawConicTree();
//    glPopMatrix();

//    glPushMatrix();
//        glTranslatef(20, 0, -10);
//        drawHouse();
//    glPopMatrix();


    ///building loop
    for(int i=0; i<10; i++){
        //else glTranslatef(buildpos, 0, 0);
        glPushMatrix();
            glTranslatef((float)i*20, 0, 15);
            drawBuilding(buildheight[i], (i%4)+3);
        glPopMatrix();
    }

    ///streetlamps loop
    for(int i=0; i<20; i++){
        glPushMatrix();
            glTranslatef((float)i*25, 0, -10);
            drawStreetLamp();
        glPopMatrix();
    }

    ///small houses loop
    int house = 27;
    for(int i=0; i<10; i++){
        glPushMatrix();
            glTranslatef((float)house*8, 0, houseloc[i]);
            drawHouse();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(10+(float)house*8, 0, -houseloc[10-i]);
            drawHouse();
        glPopMatrix();
        house += 3;
    }

    ///trees loops
    for(int i=0; i<6; i++){
        glPushMatrix();
            glTranslatef((float)i*30, 0, -15);
            drawSphereTree();
        glPopMatrix();
    }
//
    for(int i=0; i<6; i++){
        glPushMatrix();
            glTranslatef(15 + (float)i*30, 0, -15);
            drawConicTree();
        glPopMatrix();
    }

    int trees = 27;
    for(int i=0; i<5; i++){
        glPushMatrix();
            glTranslatef(24+(float)trees*8, 0, -houseloc[i]);
            drawSphereTree();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(34+(float)trees*8, 0, houseloc[i]);
            drawConicTree();
        glPopMatrix();
        trees += 6;
    }

    ///text on the screen
    char percent[5];
    snprintf(percent, sizeof(percent), "%f", 100*x_val/700.0);
//    percent[6] = '%';

    glColor3f(1,1,1);
//    renderBitmapString(x_val+30, 10, 0, GLUT_BITMAP_HELVETICA_18, percent);
    if(x_val>=0) {renderBitmapString(x_val+30, 15, -20, GLUT_BITMAP_HELVETICA_18, percent);
    renderBitmapString(x_val+30, 15, -17, GLUT_BITMAP_HELVETICA_18, "%");}
    renderBitmapString(190, 10, -2, GLUT_BITMAP_HELVETICA_18, "Checkpoint #1");
    renderBitmapString(500, 10, -2, GLUT_BITMAP_HELVETICA_18, "Checkpoint #2");
    renderBitmapString(630, 10, -2, GLUT_BITMAP_HELVETICA_18, "Checkpoint #3");
    renderBitmapString(715, 10, -2, GLUT_BITMAP_HELVETICA_18, "Complete");
    renderBitmapString(725, 8, -2, GLUT_BITMAP_HELVETICA_18, "Congratulations!");
    renderBitmapString(0, 10, -2, GLUT_BITMAP_HELVETICA_18, "Welcome");
    renderBitmapString(0, 9, -3, GLUT_BITMAP_HELVETICA_18, "Press 'K' to start");
    renderBitmapString(0, 8, -4, GLUT_BITMAP_HELVETICA_18, "'P' to pause while running");
    renderBitmapString(100, 8, -8, GLUT_BITMAP_HELVETICA_18, "'A' & 'D' to move left & right");
//    renderBitmapString(20, 8, -4, GLUT_BITMAP_HELVETICA_18, "'P' to pause while running");
    if(paused) renderBitmapString(x_val+10, 8, -4, GLUT_BITMAP_HELVETICA_18, "Paused");

    ///the curved tunnel at the end
    glColor3f(0.0, 0.5, 0.5);
    float tunnel = 0;

    glPushMatrix();
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho(-20, 20, -20, 20, -20, 50);
    gluPerspective(60, 1, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, 0);

    if(x_val <= 700 && car){
        x_val+=0.5;
        ex+=0.5;
        lx+=0.5;
    }
    //zvalue--;

}

void processNormalKeys(unsigned char key, int x, int y) {

    switch(key){
        case 27: exit(0);
        case 'r':
            x_val = -20;
            z_val = 0;
            ex = -38;
            lx = -20;
            break;
        case 'k':
            car = true;
            break;
        case 'p':
            car = !car;
            paused = !paused;
            break;
        case 'w':
            ex++;
            x_val++;
            lx++;
            break;
        case 's':
            ex--;
            x_val--;
            lx--;
            break;
        case 'a':
            if(z_val>-4.0) z_val--;
            break;
        case 'd':
            if(z_val<4.0) z_val++;
            break;
        case 'l':
            if(light0) {
                glDisable(GL_LIGHT0);
                light0 = !light0;
            }
            else {
                glEnable(GL_LIGHT0);
                light0 = !light0;
            }
            break;
        case 'o':
            if(light1) {
                glDisable(GL_LIGHT1);
                light1 = !light1;
            }
            else {
                glEnable(GL_LIGHT1);
                light1 = !light1;
            }
            break;
    }
//	switch (key) {
//		case GLUT_KEY_LEFT : deltaAngle = -0.01f; break;
//		case GLUT_KEY_RIGHT : deltaAngle = 0.01f; break;
//		case GLUT_KEY_UP : deltaMove = 0.5f; break;
//		case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
//	}

}

void drawCar()
{
    glTranslatef(x_val, 0, z_val);

    glBegin(GL_QUADS);

    //frontbumper
    glColor3f(0.2,0.2,0.2);
    glVertex3f(3.6, 0.5, 1.9);
    glVertex3f(3.6, 0.5, -1.9);
    glVertex3f(3.6, -0.7, -1.9);
    glVertex3f(3.6, -0.7, 1.9);

    //frontleftside
    glColor3f(0.9,0.9,0.9);
    glVertex3f(3.6, 0.5, 1.9);
    glVertex3f(3.6, -0.7, 1.9);
    glVertex3f(2, -0.7, 1.9);
    glVertex3f(2, 0.7, 1.9);

    //hood
    glColor3f(1,1,1);
    glVertex3f(2, 0.7, -1.9);
    glVertex3f(3.6,  0.5, -1.9);
    glVertex3f(3.6, 0.5, 1.9);
    glVertex3f(2, 0.7, 1.9);

    //frontrightside
    glColor3f(1.0,1.0,1.0);
    glVertex3f(2, 0.7, -1.9);
    glVertex3f(2, -0.7, -1.9);
    glVertex3f(3.6, -0.7, -1.9);
    glVertex3f(3.6, 0.5, -1.9);

    //windshield
    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(1, 2.4, 1.9);
    glVertex3f(1, 2.4, -1.9);
    glVertex3f(2, 0.7, -1.9);
    glVertex3f(2, 0.7, 1.9);

    //top
    glColor3f(1.0,1.0,1.0);
    glVertex3f(-3, 2.4, 1.9);
    glVertex3f(-3, 2.4, -1.9);
    glVertex3f(1, 2.4, -1.9);
    glVertex3f(1, 2.4, 1.9);

    //frontleft2
    glColor3f(1.0,1.0,1);
    glVertex3f(1, 2.4, 1.9);
    glVertex3f(2, 0.7, 1.9);
    glVertex3f(2, -0.7, 1.9);
    glVertex3f(1, -0.7, 1.9);

    //frontright2
    glColor3f(1.0,1.0,1.0);
    glVertex3f(1, 2.4, -1.9);
    glVertex3f(1, -0.7, -1.9);
    glVertex3f(2, -0.7, -1.9);
    glVertex3f(2, 0.7, -1.9);

    //midright
    glColor3f(1.0,1,1);
    glVertex3f(-3, 2.4, -1.9);
    glVertex3f(-3, -0.7, -1.9);
    glVertex3f(1, -0.7, -1.9);
    glVertex3f(1, 2.4, -1.9);

    //midleft
    glColor3f(1.0,1.0,1.0);
    glVertex3f(-3, 2.4, 1.9);
    glVertex3f(1, 2.4, 1.9);
    glVertex3f(1, -0.7, 1.9);
    glVertex3f(-3, -0.7, 1.9);

    //backleft
    glColor3f(1,1,1);
    glVertex3f(-3.6, 0.7, 1.9);
    glVertex3f(-3, 2.4, 1.9);
    glVertex3f(-3, -0.7, 1.9);
    glVertex3f(-3.6, -0.7, 1.9);

    //backright
    glColor3f(1,1,1);
    glVertex3f(-3, 2.4, -1.9);
    glVertex3f(-3.6, 0.7, -1.9);
    glVertex3f(-3.6, -0.7, -1.9);
    glVertex3f(-3, -0.7, -1.9);

    //rearwindshield
    glColor3f(0,0,0);
    glVertex3f(-3, 2.4, -1.9);
    glVertex3f(-3, 2.4, 1.9);
    glVertex3f(-3.6, 0.7, 1.9);
    glVertex3f(-3.6, 0.7, -1.9);

    //rearbumper
    glColor3f(1,1,1);
    glVertex3f(-3.6, 0.7, -1.9);
    glVertex3f(-3.6, 0.7, 1.9);
    glVertex3f(-3.6, -0.7, 1.9);
    glVertex3f(-3.6, -0.7, -1.9);

    //bottom
    glColor3f(0.1, 0.1, 0.1);
    glVertex3f(3.6, -0.7, 1.9);
    glVertex3f(3.6, -0.7, -1.9);
    glVertex3f(-3.6, -0.7, -1.9);
    glVertex3f(-3.6, -0.7, 1.9);

    glEnd();

    //wheels
    glPushMatrix();
    glTranslatef(2, -0.5, 2);
    glScalef(1, 1, 0.2);
    glColor3f(0, 0, 0);
    glutSolidSphere(1.0, 15, 10);
    glTranslatef(0, 0, 1);
    glScalef(0.4, 0.4, 0.2);
    glColor3f(0.8, 0.8, 0.8);
    glutSolidSphere(1.0, 8, 8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2, -0.5, -2);
    glScalef(1, 1, 0.2);
    glColor3f(0, 0, 0);
    glutSolidSphere(1.0, 15, 10);
    glTranslatef(0, 0, -1);
    glScalef(0.4, 0.4, 0.2);
    glColor3f(0.8, 0.8, 0.8);
    glutSolidSphere(1.0, 8, 8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2, -0.5, 2);
    glScalef(1, 1, 0.2);
    glColor3f(0, 0, 0);
    glutSolidSphere(1.0, 15, 10);
    glTranslatef(0, 0, 1);
    glScalef(0.4, 0.4, 0.2);
    glColor3f(0.8, 0.8, 0.8);
    glutSolidSphere(1.0, 8, 8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2, -0.5, -2);
    glScalef(1, 1, 0.2);
    glColor3f(0, 0, 0);
    glutSolidSphere(1.0, 15, 10);
    glTranslatef(0, 0, -1);
    glScalef(0.4, 0.4, 0.2);
    glColor3f(0.8, 0.8, 0.8);
    glutSolidSphere(1.0, 8, 8);
    glPopMatrix();

}

void drawBuilding(float yscale, int tex)
{
//    glTranslatef(b_pos, 0, 0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTranslatef(0, yscale/2, 0);
    glScalef(1, yscale, 1);
    glColor3f(0.9, 0.9, 0.9);
    glutSolidCube(10);

    glDisable(GL_TEXTURE_2D);
}

void drawSphereTree()
{
    glTranslatef(0, 7, 0);
    glScalef(1, 1, 1);
    glColor3f(0.15, 0.55, 0.13);
    glutSolidSphere(2.5, 6, 6);

    glTranslatef(0, -7, 0);
    glScalef(0.1, 1.5, 0.1);
    glColor3f(0.64, 0.16, 0.16);
    glutSolidCube(10);

}

void drawConicTree()
{
    glTranslatef(0, 3, 0);
    glScalef(1,1,1);
    glRotatef(-90, 1, 0, 0);
    glColor3f(0.15, 0.55, 0.13);
    glutSolidCone(2.5, 8, 10, 10);

    glRotatef(90, 1, 0, 0);
    glTranslatef(0, -3, 0);
    glScalef(0.1, 1.5, 0.1);
    glColor3f(0.64, 0.16, 0.16);
    glutSolidCube(10);
}

void drawHouse()
{
    glTranslatef(0, 2.5, 0);
    glScalef(1, 1, 1);
    glRotatef(-90, 1, 0, 0);
//    glRotatef(45, 0, 0, 1);
    glColor3f(0.7, 0.2, 0.1);
    glutSolidCone(4, 4, 10, 10);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 2);
    glRotatef(90, 1, 0, 0);
//    glRotatef(45, 0, 0, 1);
    glTranslatef(0, -2.5, 0);
    glScalef(1, 1, 1);
    glColor3f(0.8, 0.8, 0.8);
//    glColor3f(1,1,1);
//    glEnable(GL_TEXTURE_CUBE_MAP);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glutSolidCube(5);
//    glutSolidCube(5);
//    glDisable(GL_TEXTURE_CUBE_MAP);
    glDisable(GL_TEXTURE_2D);
}

void drawStreetLamp()
{
    glTranslatef(0, 7, 0);
    glScalef(1, 1, 1);
    glColor3f(1.0, 1.0, 1.0);
//    glColorMaterial(GL_FRONT, GL_EMISSION);
    glutSolidOctahedron();

    glTranslatef(0, -7, 0);
    glScalef(0.05, 3, 0.05);
    glColor3f(0.1, 0.1, 0.16);
    glutSolidCube(5);

}


void renderBitmapString(float x,float y,float z,void *font,	char *string) {

  char *c;
  glRasterPos3f(x, y,z);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

static void
drawBox(GLfloat size, GLenum type)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  for (i = 5; i >= 0; i--) {
    glBegin(type);
    glNormal3fv(&n[i][0]);
    glVertex3fv(&v[faces[i][0]][0]); glTexCoord2f(1,1);
    glVertex3fv(&v[faces[i][1]][0]); glTexCoord2f(1,0);
    glVertex3fv(&v[faces[i][2]][0]); glTexCoord2f(0,0);
    glVertex3fv(&v[faces[i][3]][0]); glTexCoord2f(0,1);
    glEnd();
  }
}

void APIENTRY
glutSolidCube(GLdouble size)
{
    drawBox(size, GL_QUADS);
}

void LoadTexture(const char*filename)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowPosition(200, 100);
    glutInitWindowSize(650,650);

    glutCreateWindow("CSE 4208 Project 1607036");

    LoadTexture("E:\\C\\test1\\textures\\road.bmp");
    LoadTexture("E:\\C\\test1\\textures\\House1.bmp");
    LoadTexture("E:\\C\\test1\\textures\\building2.bmp");
    LoadTexture("E:\\C\\test1\\textures\\building1.bmp");
    LoadTexture("E:\\C\\test1\\textures\\building.bmp");
    LoadTexture("E:\\C\\test1\\textures\\building3.bmp");
    LoadTexture("E:\\C\\test1\\textures\\building4.bmp");
    LoadTexture("E:\\C\\test1\\textures\\building5.bmp");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    init();

    glutKeyboardFunc(processNormalKeys);

    glEnable(GL_POLYGON_SMOOTH);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);


    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, carlight_position);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, carlight_direction);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);


    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


    glutMainLoop();

}
