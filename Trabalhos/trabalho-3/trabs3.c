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

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

GLint rot = 0;
static GLfloat spin = 0.0;
GLfloat fovy = 45.0;
GLint width, height;
GLdouble zoom = 1.0;
GLdouble move_cx = 0.0;
GLdouble move_cz = 0.0;
GLint programId;
GLint escalador = 1;

GLint loadShader(GLuint shaderId, const char *path) {
	FILE *file = fopen(path,"r");
	if (file == NULL) {
		fprintf(stderr, "Couldn't open file %s for reading\n", path);
		return GL_FALSE;
	}

	int codeBufferSize = 4096;
	int codeSize = 0;
	char *code = (char*)malloc(codeBufferSize * sizeof(char));
	if (code == NULL) {
		fprintf(stderr, "Failed to request %d bytes of memory\n", codeBufferSize);
		return GL_FALSE;
	}

	int rl = 256;
	int r = 0;
	int i = 0;
	while ((r = fread(&code[i], 1, rl, file)) > 0) {
		i += r;
		if ((i + rl) > (codeBufferSize - 1)) {
			codeBufferSize *= 2;
			char *newCode = (char*)malloc(codeBufferSize * sizeof(char));
			if (newCode == NULL) {
				fprintf(stderr, "Failed to request %d bytes of memory\n", codeBufferSize);
				free(code);
				return GL_FALSE;
			}

			for (int j = 0; j < i; j++) {
				code[j] = newCode[j];
			}
			free(code);
			code = newCode;
		}
	}

	code[i] = '\0';

	//printf("%s\n", code);

	const char *src = code;
	glShaderSource(shaderId, 1, &src, NULL);
	glCompileShader(shaderId);

	GLint ret = GL_FALSE;
	int infoLogLength;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &ret);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		char msg[4096];
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, msg);
		printf("%s\n", msg);
	}

}


GLuint loadShaders(const char *vsPath, const char *fsPath){

	GLuint vsId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsId = glCreateShader(GL_FRAGMENT_SHADER);

	loadShader(vsId, vsPath);
	loadShader(fsId, fsPath);

	GLuint progId = glCreateProgram();
	glAttachShader(progId, vsId);
	glAttachShader(progId, fsId);
	glLinkProgram(progId);

	GLint ret = GL_FALSE;
	int infoLogLength;
	glGetProgramiv(progId, GL_LINK_STATUS, &ret);
	glGetProgramiv(progId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		char msg[4096];
		glGetProgramInfoLog(progId, infoLogLength, NULL, msg);
		printf("%s\n", msg);
	}

	glDetachShader(progId, vsId);
	glDetachShader(progId, fsId);

	glDeleteShader(vsId);
	glDeleteShader(fsId);

	glUseProgram(progId);

	GLuint t0 = glGetUniformLocation(progId, "normalTexture");
	GLuint t1 = glGetUniformLocation(progId, "colorTexture");

	glUniform1i(t0, 0);
	glUniform1i(t1, 1);

	glUseProgram(0);

	return progId;
}
void display(void)
{
   if (rot) {
     spin = spin + 0.1;
     if (spin > 360.0)
       spin = spin - 360.0;
   }

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#if 1
   
   glUseProgram(programId);


   glPushMatrix();
   glScalef(zoom, zoom, zoom);
   //glTranslatef(move_cx, 0.0, move_cz); 
   
   glPushMatrix();
   //glTranslatef(0.0, zoom, 0.0);
   GLfloat cor1[4] = {1.0f, 1.0f, 0.0f, 1.0f};  
   GLfloat shine[1] = {25};
   //glMaterialfv(GL_FRONT, GL_DIFFUSE, cor1);
   glMaterialfv(GL_FRONT, GL_SHININESS, shine);
   glMaterialfv(GL_FRONT, GL_EMISSION, cor1);
   glRotatef(spin*9, 0.0, 1.0, 0.0);
   //glColor3f(0.8,0.8,0.0);
   glutSolidSphere(185.0, 30.0, 30.0);

   float preto[4] = {.0f,.0f, .0f, 1.0f};  
   glMaterialfv(GL_FRONT, GL_EMISSION, preto);

   //mercurio
   

   glPushMatrix();
   GLfloat cor2[4] = {1.0,0.5,0.31, 1.0};
   glMaterialfv(GL_FRONT, GL_DIFFUSE, cor2);
   glRotatef(spin, 1.0, 0.0, 0.0);
   glTranslatef(600.0/escalador,0.0,0.0);   
   glutSolidSphere(5.0, 15.0, 15.0);   
   glPopMatrix();
   
   glPopMatrix();

   glPushMatrix();
   //glTranslatef(0.0, zoom, 0.0);
   glRotatef(spin*8, 0.0, 1.0, 0.0);
   glutSolidSphere(1.0, 1.0, 1.0);
	
   //venus
   glPushMatrix();
   GLfloat cor3[4] = {1.0,0.98,0.804, 1.0};
   glMaterialfv(GL_FRONT, GL_DIFFUSE, cor3);
   glRotatef(spin, 1.0, 0.0, 0.0);
   glTranslatef(800.0,0.0,0.0);   
   glutSolidSphere(12.5, 15.0, 15.0);
   glPopMatrix();

   glPopMatrix();   

   glPushMatrix();
   //glTranslatef(0.0, zoom, 0.0);
   
   glRotatef(spin*7, 0.0, 1.0, 0.0);
   glutSolidSphere(1.0, 1.0, 1.0);
    
   //terra
   glPushMatrix();
   glTranslatef(1200.0/escalador,0.0,0.0);
   glRotatef(spin*2, 0.0, 1.0, 0.0);
   GLfloat cor4[4] = {0.0,.8,.74, 1.0};
   glMaterialfv(GL_FRONT, GL_DIFFUSE, cor4);
   glutSolidSphere(13.0, 15.0, 15.0);

   //lua
   glPushMatrix();
   glTranslatef(30.0, 0.0, 0.0);
   GLfloat cor5[4] = {1.0, 1.0, 1.0, 1.0};
   glMaterialfv(GL_FRONT, GL_DIFFUSE, cor5);
   glutSolidSphere(4.2, 15.0, 15.0);
   glPopMatrix();

   glPopMatrix();

   glPopMatrix();


   glPushMatrix();
   //glTranslatef(0.0, zoom, 0.0);
   glRotatef(spin*6, 0.0, 1.0, 0.0);
   glutSolidSphere(1.0, 1.0, 1.0);

   //marte
   GLfloat cor6[4] = {1.0,0.271,0.0, 1.0};
   glPushMatrix();
   glMaterialfv(GL_FRONT, GL_DIFFUSE, cor6);
   glTranslatef(2000.0/escalador,0.0,0.0);
   glRotatef(spin, 0.0, 1.0, 0.0);   
   glutSolidSphere(7.0, 15.0, 15.0);   
    

   /*//phobos
   glPushMatrix();
   glTranslatef(25.0, 0.0, 0.0);
   glColor3f(1.0, 1.0, 1.0);
   glutSolidSphere(1.4, 15.0, 15.0);
   glPopMatrix();

   //deimos
   glPushMatrix();
   glTranslatef(10.0, 0.0, 0.0);
   glColor3f(.502, .502, 0.0);
   glutSolidSphere(.75, 15.0, 15.0);
   glPopMatrix(); */

   glPopMatrix();

   glPopMatrix();



   glPushMatrix();
   //glTranslatef(0.0, zoom, 0.0);
   glRotatef(spin*5, 0.0, 1.0, 0.0);
   glutSolidSphere(1.0, 1.0, 1.0);

   //jupiter
   GLfloat cor7[4] = {0.914,0.588,0.478, 1.0};
   glPushMatrix();
   glMaterialfv(GL_FRONT, GL_DIFFUSE, cor7);
   glTranslatef(3600.0,0.0,0.0);
   glRotatef(spin, 1.0, 0.0, 0.0);   
   glutSolidSphere(120.0, 15.0, 15.0);
   glPopMatrix();

   glPopMatrix();

  
   glPushMatrix();
   //glTranslatef(0.0, zoom, 0.0);
   glRotatef(spin*4, 0.0, 1.0, 0.0);
   glutSolidSphere(1.0, 1.0, 1.0);

   //saturno   
   glPushMatrix();
   GLfloat cor8[4] = {0.941,0.902,0.549, 1.0};
   glMaterialfv(GL_FRONT, GL_DIFFUSE, cor8);
   glTranslatef(4750.0,0.0,0.0);
   glRotatef(spin, 0.0, 1.0, 0.0);   
   glutSolidSphere(100.0, 15.0, 15.0);
    
   
   //aneis
   glPushMatrix();
   GLfloat cor9[4] = {0.941,0.902,0.549, 1.0};
   glMaterialfv(GL_FRONT, GL_DIFFUSE, cor9);
   glRotatef(80.0, 1.0, 0.0, 0.0);
   glutSolidTorus(15.0, 180.0, 3.0, 20.0);
   glPopMatrix();
   
   glPopMatrix();

   glPopMatrix(); 
   
   glPushMatrix();
   //glTranslatef(0.0, zoom, 0.0);
   glRotatef(spin*3, 0.0, 1.0, 0.0);
  
 glutSolidSphere(1.0, 1.0, 1.0);

   //urano
   GLfloat cor10[4] = {0.902,0.902,0.98, 1.0};   
   glPushMatrix();
   glMaterialfv(GL_FRONT, GL_DIFFUSE, cor10);
   glTranslatef(6200.0,0.0,0.0);
   glRotatef(spin, 1.0, 0.0, 0.0);   
   glutSolidSphere(50.0, 15.0, 15.0);
   glPopMatrix();

   glPopMatrix();
 

   glPushMatrix();
   //glTranslatef(zoom, 0.0, 0.0);
   glRotatef(spin*2, 0.0, 1.0, 0.0);
   glutSolidSphere(1.0, 1.0, 1.0);

   //netuno
   GLfloat cor11[4] = {0.0,0.749, 1.0, 1.0};
   glMaterialfv(GL_FRONT, GL_DIFFUSE, cor11);
   glPushMatrix();
   glTranslatef(8200.0,0.0,0.0);
   glRotatef(spin, 1.0, 0.0, 0.0);   
   glutSolidSphere(48.0, 15.0, 15.0);
   glPopMatrix();  

   glPopMatrix();

   


   glRotatef(spin*500, 1.0, 0.5, 0.5);
   GLfloat cor12[4] = {1.0, 0.5, 0.0, 1.0};
   glMaterialfv(GL_FRONT, GL_EMISSION, cor12);
   glutWireSphere(225.0, 35.0, 5.0);

  
   glMaterialfv(GL_FRONT, GL_EMISSION, preto);


   glRotatef(spin*700, 1.0, 0.0, 0.5);
   GLfloat cor13[4] = {.90, 0.55, 0.0, 1.0};
   glMaterialfv(GL_FRONT, GL_EMISSION, cor13);
   glutWireSphere(235.0, 5.0, 25.0);

   glMaterialfv(GL_FRONT, GL_EMISSION, preto);


   glRotatef(spin*600, 1.0, 0.5, 1.0);
   GLfloat cor14[4] = {0.95, 0.45, 0.0, 1.0};
   glMaterialfv(GL_FRONT, GL_EMISSION, cor14);
   glutWireSphere(245.0, 15.0, 35.0);

  
   glMaterialfv(GL_FRONT, GL_EMISSION, preto);


   glRotatef(spin*800, 0.5, 1.0, 1.0);
   GLfloat cor15[4] = {.89, 0.53, 0.0, 1.0};
   glMaterialfv(GL_FRONT, GL_EMISSION, cor15);
   glutWireSphere(255.0, 25.0, 15.0);

   glMaterialfv(GL_FRONT, GL_EMISSION, preto);

   glPopMatrix();

   glUseProgram(0);

#endif
   glutSwapBuffers();
   
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
}

void reshape(int w, int h)
{
   width = w;
   height = h;
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(fovy, (float)w/(float)h, 300.0, 21000.0);
   //glFrustum(-w*0.2, w*0.2, -h*0.2, h*0.2, 10.0, 70.0);
   //glOrtho(-w, w, -h, h, -50000.0, 50000.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0.0, 15000.0, 0.0,   0.0, 0.0, 0.0,   1.0, 0.0, 0.0); // visão superior
   //gluLookAt(15000.0, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 1.0, 0.0); // visão lateral
   //gluLookAt(10000.0, 10000.0, 0.0,   0.0, 0.0, 0.0,   0.0, 1.0, 0.0); // visão isometrica
   //printf("%d %d\n", w, h);
   float position[4] = {0.0, 0.0f, 0.0, 1.0f};
   float ambient[4] = {.2f, .2f, .2f, 1.0f};
   float diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
   float specular[4] = {0.2f, 0.2f, 0.2f, 1.0f};
   glLightfv(GL_LIGHT0, GL_POSITION, position);
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void mouse(int button, int state, int x, int y) 
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
            rot = 1;
            //glutIdleFunc(spinDisplay);
         }
         break;
      case GLUT_MIDDLE_BUTTON:
      case GLUT_RIGHT_BUTTON:
         if (state == GLUT_DOWN)
            rot = 0;
            //glutIdleFunc(NULL);
         break;
      case 3:
         //fovy *= 1.01;
         //fovy = (fovy >= 100.0)?100.0:fovy;
         //reshape(width, height);
         zoom+=0.01;
         break;
      case 4:
         //fovy *= 0.99;
         //reshape(width, height);
         if (zoom > 0 )
         zoom-=0.01;
         break;
      default:
         printf("button: %d\n",button);
         break;
   }
}

void motion(int x, int y) {

}

void special(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_LEFT:
      move_cz+= 100.0;
      break;
    case GLUT_KEY_DOWN:
      move_cx-= 100.0;
      break;
    case GLUT_KEY_RIGHT:
      move_cz-= 100.0;
      break;
    case GLUT_KEY_UP:
      move_cx+= 100.0;
      break;
    case GLUT_KEY_F1:
      escalador++;
      break;
    case GLUT_KEY_F2:
      escalador--;
      break;
    default:
      break;
  }
}

void initShaders() {
	GLenum err = glewInit();
	if (GLEW_OK != err)
	  fprintf(stderr, "%s\n", glewGetErrorString(err));

	programId = loadShaders("vertex.glsl","fragment.glsl");
}
   
/* 
 *  Request double buffer display mode.
 *  Register mouse input callback functions
 */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (1920, 1080); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[1]);
   initShaders();
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(motion);
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;   /* ANSI C requires main to return int. */
}

