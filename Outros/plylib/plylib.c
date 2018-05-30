#include "plylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


ply_model* load_ply_model(const char* f) {

  FILE *file = fopen(f,"r");
  if (file == NULL) return NULL;

  #define BUFFER_SIZE 1024
  char line[BUFFER_SIZE];

  // header
  char format[128];
  int version[2];
  if (fscanf(file,"ply\nformat %s %d.%d\n", format, version, version+1) != 3) {
    printf("Failed[%d]\n",__LINE__);
    fclose(file);
    return NULL;
  }

  while (fgets(line, BUFFER_SIZE, file) != NULL) {
    if (strncmp(line,"comment",7) == 0) {
      //printf("comment: %s",&line[8]);
    } else break;
  }

  int nVertices = 0;
  if (sscanf(line,"element vertex %d",&nVertices) != 1) {
    printf("Failed[%d]\n",__LINE__);
    fclose(file);
    return NULL;
  }

  while (fgets(line, BUFFER_SIZE, file) != NULL) {
    if (strncmp(line,"property",7) == 0) {
      //printf("property: %s",&line[8]);
    } else break;
  }

  int nTriangles = 0;
  if (sscanf(line,"element face %d",&nTriangles) != 1) {
    printf("Failed[%d]\n",__LINE__);
    fclose(file);
    return NULL;
  }

  while (fgets(line, BUFFER_SIZE, file) != NULL) {
    if (strncmp(line,"property",7) == 0) {
      //printf("property: %s",&line[8]);
    } else break;
  }

  if (strncmp(line,"end_header",10) != 0) {
    printf("Failed[%d]\n",__LINE__);
    fclose(file);
    return NULL;
  }

  ply_model *m = malloc(sizeof(ply_model));
  if (m == NULL) {
    fclose(file);
    return NULL;
  }
  m->nVertices = nVertices;
  m->nTriangles = nTriangles;

  m->vertices = malloc(nVertices*sizeof(vec3f));
  if (m->vertices == NULL) {
    fclose(file);
    free(m);
    return NULL;
  }

  m->triangles = malloc(nTriangles*sizeof(triangle));
  if (m->triangles == NULL) {
    fclose(file);
    free(m->vertices);
    free(m);
    return NULL;
  }

  int i;
  for (i=0;i<nVertices && fgets(line, BUFFER_SIZE,file) != NULL;i++) {
    float x,y,z;
    sscanf(line,"%f %f %f", &x, &y, &z);
    m->vertices[i].x = x;
    m->vertices[i].y = y;
    m->vertices[i].z = z;
  }

  for (i=0;i<nTriangles && fgets(line, BUFFER_SIZE,file) != NULL;i++) {
    int sides;
    int a,b,c;
    sscanf(line,"%d %d %d %d", &sides, &a, &b, &c);
    m->triangles[i].a = a;
    m->triangles[i].b = b;
    m->triangles[i].c = c;
  }

  return m;
}

void   delete_ply_model(ply_model *m) {
  if (m == NULL) return;

  free(m->triangles);
  free(m->vertices);
  free(m);
}

