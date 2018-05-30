// Lucas Fernandes Gauer 15102821
// Trabalho 1
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

GLint rot = 0;
static GLfloat spin = 0.0;
GLfloat fovy = 45.0;
GLfloat velo = 1.0;
GLint width, height;
GLdouble zoom = 1.0;
GLdouble move_cx = 0.0;
GLdouble move_cy = 0.0;

void display(void)
{
    if (rot)
    {
        spin = spin + velo;
        if (spin > 360.0)
            spin = spin - 360.0;
    }
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(move_cx, move_cy, 0.0);

    //Sol
    glPushMatrix();
    glRotatef(spin*1, 0.0, 1.0, 0.0);
    glColor3f(0.9,0.9,0.0);
    glutSolidSphere(50, 50, 50);
    
	//Terra
    glPushMatrix();
    glTranslatef(150.0,0.0,0.0);
    glRotatef(-20, 0, 0, 1);
    glRotatef(spin*5, 0.0, 1.0, 0.0);
    glColor3f(0, 0.5, 1.0);
    glutSolidSphere(10.0, 20.0, 20.0);

	//Lua
    glPushMatrix();
    glTranslatef(20.0, 0.0, 0.0);
    glColor3f(0.9, 0.9, 0.9);
    glutSolidSphere(4, 10.0, 10.0);
    glPopMatrix();// lua

    glPopMatrix();// terra

    glPopMatrix();// sol

    glPopMatrix();// main

    glutSwapBuffers();

    glutPostRedisplay();
}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
   width = w;
   height = h;
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(fovy, (float)w/(float)h, 200.0, 600.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0.0,0.0,-400.0, 0.0,0.0,0.0, 0.0,1.0,0.0);
}

void mouse(int button, int state, int x, int y)
{
	switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            rot = 1;
            printf("LEFT BUTTON\n");
        }
        break;
    case GLUT_MIDDLE_BUTTON:
        if (state == GLUT_DOWN)
        {
            fovy = 25.0;
            reshape(width, height);
            printf("MIDDLE BUTTON\n");
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
        {
            rot = 0;
            printf("RIGHT BUTTON\n");
        }
        break;
    case 3:
        fovy *= 0.99;
        reshape(width, height);
        break;
    case 4:
        fovy *= 1.01;
        fovy = (fovy >= 100.0)?100.0:fovy;
        reshape(width, height);
        break;
    default:
        break;
    }
}

void motion(int x, int y)
{
    //printf("Motion: %d %d\n", x, y);
}

void special(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_LEFT:
      move_cx+= 10.0;
      break;
    case GLUT_KEY_DOWN:
      move_cy-= 10.0;
      break;
    case GLUT_KEY_RIGHT:
      move_cx-= 10.0;
      break;
    case GLUT_KEY_UP:
      move_cy+= 10.0;
      break;
    default:
      break;
  }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH |GLUT_RGB);
   glutInitWindowSize(1280, 720);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Sol, Terra e Lua");
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutPassiveMotionFunc(motion);
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}
