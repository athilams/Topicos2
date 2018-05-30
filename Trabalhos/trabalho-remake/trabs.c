/*
COMANDOS

Scroll: Zoom in/Zoom out
F1: Aumento do tamanho dos planetas
F2: Redução do tamanho dos planetas
Mouse
	Esquerdo: Inicia a rotação
	Direto: Pausa a rotação
	Meio: Reinicia a rotação

Shift Esquerdo: Reset de zoom
Control Esquerdo: Reset de posição da câmera

Control Direto: Redução da velocidade de rotação
Shift Direto: Aumento da velocidade de rotação

*/

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

GLint rot = 0;
static GLfloat spin = 0.0;
GLfloat fovy = 45.0;
GLfloat velocidade = 0.1;
GLdouble zoom = 1.0;
GLint width, height;
GLint programId;
GLint escalador = 3;
GLdouble move1 = 0.0;
GLdouble move2 = 0.0;

GLint loadShader(GLuint shaderId, const char *path);
GLuint loadShaders(const char *vsPath, const char *fsPath);

void display(void)
{
	if (rot) {
		spin = spin + velocidade;
		if (spin > 360.0)
			spin = spin - 360.0;
   }
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programId);

	glPushMatrix();//universo


	// SOL
	GLfloat cor1[4] = {1.0f, 1.0f, 0.0f, 1.0f};
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, cor1);
	glMaterialfv(GL_FRONT, GL_EMISSION, cor1);

	glPushMatrix();
		glutSolidSphere(185.0, 30.0, 30.0);
	glPopMatrix();

	GLfloat preto[4] = {.0f,.0f, .0f, 1.0f};

	// WIRES
	GLfloat laranja[4] = {1.0, 0.5, 0.0, 1.0};
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, laranja);
		glRotatef(spin*400, 1.0, 0.5, 0.5);
		glutWireSphere(250.0, 35.0, 5.0);

		glMaterialfv(GL_FRONT, GL_EMISSION, laranja);
		glRotatef(spin*500, 1.0, 0.0, 0.5);
		glutWireSphere(250.0, 5.0, 25.0);

		glMaterialfv(GL_FRONT, GL_EMISSION, laranja);
		glRotatef(spin*600, 1.0, 0.5, 1.0);
		glutWireSphere(250.0, 15.0, 35.0);

		glMaterialfv(GL_FRONT, GL_EMISSION, laranja);
		glRotatef(spin*700, 0.5, 1.0, 1.0);
		glutWireSphere(250.0, 25.0, 15.0);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, preto);


	// MERCURIO
	GLfloat cor2[4] = {1.0,0.5,0.31, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cor2);

	glPushMatrix();
		glRotatef(spin*9, 0.0, 1.0, 0.0);
		glPushMatrix();
			glTranslatef(600.0,0.0,0.0);
			glutSolidSphere(escalador*5.0, 15.0, 15.0);
			//glRotatef(spin, 1.0, 0.0, 0.0);
		glPopMatrix();
	glPopMatrix();

	
	// VENUS
	GLfloat cor3[4] = {1.0,0.98,0.804, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cor3);

	glPushMatrix();
		glRotatef(spin*8, 0.0, 1.0, 0.0);
		glPushMatrix();
			glTranslatef(800.0,0.0,0.0);   
			glutSolidSphere(escalador*12.5, 15.0, 15.0);
			//glRotatef(spin, 1.0, 0.0, 0.0);
		glPopMatrix();
	glPopMatrix();


	// TERRA
	GLfloat cor4[4] = {0.0,.8,.74, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cor4);

	glPushMatrix();
		glRotatef(spin*7, 0.0, 1.0, 0.0);
		glPushMatrix();
			glTranslatef(1200.0,0.0,0.0);
			glutSolidSphere(escalador*13.0, 15.0, 15.0);
			glRotatef(spin*2, 0.0, 1.0, 0.0);
			// LUA
			GLfloat cor5[4] = {1.0, 1.0, 1.0, 1.0};
			glMaterialfv(GL_FRONT, GL_DIFFUSE, cor5);

			glPushMatrix();
				glTranslatef(escalador*30.0, 0.0, 0.0);
				glutSolidSphere(escalador*4.2, 15.0, 15.0);
				//glRotatef(spin, 1.0, 0.0, 0.0);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();


	// MARTE
	GLfloat cor6[4] = {1.0,0.271,0.0, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cor6);

	glPushMatrix();
		glRotatef(spin*6, 0.0, 1.0, 0.0);
		glPushMatrix();
			glTranslatef(2000.0,0.0,0.0);
			glutSolidSphere(escalador*7.0, 15.0, 15.0);
			//glRotatef(spin, 0.0, 1.0, 0.0);
		glPopMatrix();
	glPopMatrix();


	// JUPITER
	GLfloat cor7[4] = {0.914,0.588,0.478, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cor7);

	glPushMatrix();
		glRotatef(spin*5, 0.0, 1.0, 0.0);
		glPushMatrix();
			glTranslatef(3600.0,0.0,0.0);
			glutSolidSphere(escalador*120.0, 15.0, 15.0);
			//glRotatef(spin, 1.0, 0.0, 0.0);
		glPopMatrix();
	glPopMatrix();


	// SATURNO
	GLfloat cor8[4] = {0.941,0.902,0.549, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cor8);

	glPushMatrix();
		glRotatef(spin*4, 0.0, 1.0, 0.0);
		glPushMatrix();
			glTranslatef(4750.0,0.0,0.0);
			glutSolidSphere(escalador*100.0, 15.0, 15.0);
			//glRotatef(spin, 0.0, 1.0, 0.0);

			// ANEIS
			GLfloat cor9[4] = {0.941,0.902,0.549, 1.0};
			glMaterialfv(GL_FRONT, GL_DIFFUSE, cor9);

			glPushMatrix();
				glRotatef(80.0, 1.0, 0.0, 0.0);
				glutSolidTorus(escalador*15.0, escalador*180.0, 3.0, escalador*20.0);
				//glRotatef(spin, 1.0, 0.0, 0.0);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();


	// URANO
	GLfloat cor10[4] = {0.902,0.902,0.98, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cor10);

	glPushMatrix();
		glRotatef(spin*3, 0.0, 1.0, 0.0);
		glPushMatrix();
			glTranslatef(6200.0,0.0,0.0);
			glutSolidSphere(escalador*50.0, 15.0, 15.0);
			//glRotatef(spin, 1.0, 0.0, 0.0);
		glPopMatrix();
	glPopMatrix();


	// NETUNO
	GLfloat cor11[4] = {0.0,0.749, 1.0, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cor11);

	glPushMatrix();
		glRotatef(spin*2, 0.0, 1.0, 0.0);
		glPushMatrix();
			glTranslatef(8200.0,0.0,0.0);
			glutSolidSphere(escalador*48.0, 15.0, 15.0);
			//glRotatef(spin, 1.0, 0.0, 0.0);
		glPopMatrix();  
	glPopMatrix();

	glPopMatrix();//ultimo

	glUseProgram(0);
	glutSwapBuffers();
	glutPostRedisplay();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void reshape(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (float)w/(float)h, 300.0, 28000.0);
	//glFrustum(-w*0.2, w*0.2, -h*0.2, h*0.2, 10.0, 70.0);
	//glOrtho(-w, w, -h, h, -50000.0, 50000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0.0+move1, 15000.0, 0.0+move2,   0.0, 0.0, 0.0,   1.0, 0.0, 0.0); // visão superior
	//gluLookAt(15000.0, 0.0+move1, 0.0+move2,   0.0, 0.0, 0.0,   0.0, 1.0, 0.0); // visão lateral
	gluLookAt(15000.0, 15000.0+move1, 0.0+move2,   0.0, 0.0, 0.0,   0.0, 1.0, 0.0); // visão isometrica
	//printf("%d %d\n", w, h);
	float position[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	float ambient[4] =  {0.2f, 0.2f, 0.2f, 1.0f};
	float diffuse[4] =  {0.8f, 0.8f, 0.8f, 1.0f};
	float specular[4] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	GLfloat shine[1] = {3};
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
}

void mouse(int button, int state, int x, int y)
{
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){
				rot = 1;
			}
		break;
		case GLUT_MIDDLE_BUTTON:
			if(state == GLUT_DOWN){
				spin = 0.0;
			}
		break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN)
				rot = 0;
		break;
		case 3:
			fovy *= 0.95;
			reshape(width, height);
		break;
		case 4:
			fovy *= 1.05;
			fovy = (fovy >= 100.0)?100.0:fovy;
			reshape(width, height);
		break;
		default:
			//printf("button: %d\n",button);
		break;
	}
}

void motion(int x, int y) {

}

void special(int key, int x, int y) {
	switch(key){
	case GLUT_KEY_LEFT:
		move2+= 1000.0;
		reshape(width, height);
	break;
	case GLUT_KEY_DOWN:
		move1-= 1000.0;
		reshape(width, height);
	break;
	case GLUT_KEY_RIGHT:
		move2-= 1000.0;
		reshape(width, height);
	break;
	case GLUT_KEY_UP:
		move1+= 1000.0;
		reshape(width, height);
	break;
	case 114:
		move1 = 0.0;
		move2 = 0.0;
		reshape(width, height);
	break;
	case 112:
		fovy = 45.0;
		reshape(width, height);
	break;
	case 113:
		velocidade+= 0.05;
	break;
	case 115:
		velocidade-= 0.05;
	break;
	case GLUT_KEY_F1:
		escalador++;
	break;
	case GLUT_KEY_F2:
		escalador--;
	break;
	default:
		printf("KEY: %d\n",key);
	break;
	}
}

GLint loadShader(GLuint shaderId, const char *path){
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


GLuint loadShaders(const char *vsPath, const char *fsPath)
{

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
	if(infoLogLength > 0){
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

void initShaders()
{
	GLenum err = glewInit();
	if(GLEW_OK != err)
		fprintf(stderr, "%s\n", glewGetErrorString(err));

	programId = loadShaders("vertex.glsl", "fragment.glsl");
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sistema Solar");
	initShaders();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);
	glutSpecialFunc(special);
	glutMainLoop();
	return 0;
}

