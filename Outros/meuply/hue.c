#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "plylib.c"

ply_model *objeto = NULL;

static GLfloat spin = 0.1;
GLint width, height;
GLint programId;
GLfloat fovy = 15.0;
GLint rot = 0;

GLint loadShader(GLuint shaderId, const char *path);
GLuint loadShaders(const char *vsPath, const char *fsPath);

void display(void)
{
	if (rot) {
		spin = spin + 0.1;
		if (spin > 360.0)
			spin = spin - 360.0;
   }
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programId);

	glPushMatrix();
	
	if (objeto != NULL) {
		glPushMatrix();
		glRotatef(spin*10, 0.0, 1.0, 0.0);
		glScalef(100.0,100.0,100.0);
		glBegin(GL_TRIANGLES);
		int i;
		for (i=0;i<objeto->nTriangles;i++) {
			glVertex3f(objeto->vertices[objeto->triangles[i].a].x,objeto->vertices[objeto->triangles[i].a].y,objeto->vertices[objeto->triangles[i].a].z);
			glVertex3f(objeto->vertices[objeto->triangles[i].b].x,objeto->vertices[objeto->triangles[i].b].y,objeto->vertices[objeto->triangles[i].b].z);
			glVertex3f(objeto->vertices[objeto->triangles[i].c].x,objeto->vertices[objeto->triangles[i].c].y,objeto->vertices[objeto->triangles[i].c].z);
		}
		glEnd();
		glPopMatrix();
	}

	
	glPushMatrix();
	glTranslatef(60.0,0.0,0.0);
	//glutSolidSphere(50.0, 25.0, 15.0);
	glPopMatrix();

	glPopMatrix();
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
	gluPerspective(fovy, (float)w/(float)h, 1.0, 2000.0);
	//glFrustum(-w*0.2, w*0.2, -h*0.2, h*0.2, 10.0, 70.0);
	//glOrtho(-w, w, -h, h, -50000.0, 50000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(100.0, 0.0, 100.0,   0.0, 10.0, 0.0,   0.0, 1.0, 0.0);
	float position[4] = {1000.0f, 1000.0f, 1000.0f, 1.0f};
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
	GLfloat laranja[4] = {1.0, 0.5, 0.0, 1.0};
	//glMaterialfv(GL_FRONT, GL_EMISSION, laranja);
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

	objeto = load_ply_model(argv[1]);

	initShaders();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);
	glutSpecialFunc(special);
	glutMainLoop();

	delete_ply_model(objeto);

	return 0;
}
