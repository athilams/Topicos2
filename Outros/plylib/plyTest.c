/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 *  double.c
 *  This is a simple double buffered program.
 *  Pressing the left mouse button rotates the rectangle.
 *  Pressing the middle mouse button stops the rotation.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#include "plylib.c"

static GLfloat spin = 0.1;

ply_model *bunny = NULL;

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
#if 1
   glPushMatrix();

   glRotatef(spin, 0.0, 1.0, 0.0);
   if (bunny != NULL) {
     glPushMatrix();
     glScalef(100.0,100.0,100.0);
     glBegin(GL_TRIANGLES);
       int i;
       for (i=0;i<bunny->nTriangles;i++) {
         glVertex3f(bunny->vertices[bunny->triangles[i].a].x,bunny->vertices[bunny->triangles[i].a].y,bunny->vertices[bunny->triangles[i].a].z);
         glVertex3f(bunny->vertices[bunny->triangles[i].b].x,bunny->vertices[bunny->triangles[i].b].y,bunny->vertices[bunny->triangles[i].b].z);
         glVertex3f(bunny->vertices[bunny->triangles[i].c].x,bunny->vertices[bunny->triangles[i].c].y,bunny->vertices[bunny->triangles[i].c].z);
       }
     glEnd();
     glPopMatrix();
   }

   glPushMatrix();
   glTranslatef(20.0,0.0,0.0);
   glRotatef(spin, 1.0, 0.0, 0.0);
   //glutSolidTeapot(5.0);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-20.0,0.0,0.0);
   glRotatef(spin, 0.0, 0.0, 1.0);
   glutSolidSphere(5.0, 5.0, 5.0);
   glPopMatrix();

#if 0
   glBegin(GL_TRIANGLE_FAN);
     glColor3f(1.0, 1.0, 1.0);
     glVertex3f(  0.0,-40.0,0.0f);
     glColor3f(1.0, 0.0, 1.0);
     glVertex3f(-40.0,40.0,0.0f);
     glColor3f(1.0, 1.0, 0.0);
     glVertex3f(-20.0,40.0,0.0f);
     glColor3f(0.0, 1.0, 1.0);
     glVertex3f(  0.0,40.0,0.0f);
     glColor3f(0.0, 0.0, 1.0);
     glVertex3f( 20.0,40.0,0.0f);
     glColor3f(1.0, 0.0, 0.0);
     glVertex3f( 40.0,40.0,0.0f);
   glEnd();
#endif
   glPopMatrix();
#endif
   glutSwapBuffers();
}

void spinDisplay(void)
{
   spin = spin + 0.1;
   if (spin > 360.0)
      spin = spin - 360.0;
   glutPostRedisplay();
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   //glShadeModel (GL_FLAT);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   float position[4] = {100.0f, 100.0f, 100.0f, 1.0f};
   float ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};
   float diffuse[4] = {0.8f, 0.8f, 0.8f, 1.0f};
   float specular[4] = {0.8f, 0.5f, 0.5f, 1.0f};
   glLightfv(GL_LIGHT0, GL_POSITION, position);
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

   glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
   glMaterialf(GL_FRONT, GL_SHININESS, 16.0);
}

void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-50.0, 50.0, -50.0, 50.0, -50.0, 50.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   printf("%d %d\n", w, h);
}

void mouse(int button, int state, int x, int y) 
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
            glutIdleFunc(spinDisplay);
            printf("LEFT\n");
         }
         break;
      case GLUT_MIDDLE_BUTTON:
      case GLUT_RIGHT_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(NULL);
         break;
      default:
         //printf("button: %d\n",button);
         break;
   }
}

void motion(int x, int y) {
	//printf("Motion: %d %d\n", x, y);
}

void special(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_LEFT:
      printf("LEFT\n");
      break;
    case GLUT_KEY_DOWN:
      printf("DOWN\n");
      break;
    case GLUT_KEY_RIGHT:
      printf("RIGHT\n");
      break;
    case GLUT_KEY_UP:
      printf("UP\n");
      break;
    default:
      break;
  }
}
   
/* 
 *  Request double buffer display mode.
 *  Register mouse input callback functions
 */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[1]);

   bunny = load_ply_model(argv[1]);

   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(motion);
   glutSpecialFunc(special);
   glutMainLoop();

   delete_ply_model(bunny);

   return 0;   /* ANSI C requires main to return int. */
}

