
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
/* with '-ansi' flag and still use the */
/* useful constants from 'math.h' */
#ifndef __USE_BSD
# define __USE_BSD
#endif
#include <math.h>
#include <limits.h>
#include <float.h>

#define DMARGIN 1e10

#ifndef _3D_MODEL
#define _3D_MODEL

typedef struct {
  float x;
  float y;
  float z;
}vertex_t;

typedef struct {
  double x;
  double y;
  double z;
}dvertex_t;

typedef struct {
  int f0;
  int f1;
  int f2;
}face_t;

typedef struct {
  float s;
  float t;
}texture_t;

struct segment {
  int *vertices;    /* Array of indices of the vertex in the list */
  int n_vertices;   /* Number of vertices in the array */
  vertex_t bBox[2]; /* Bounding Box of a segment */
};



struct model {
  int num_faces;
  int num_vert;
  int num_segm; /* Number of segments in the array */
  vertex_t *vertices;
  vertex_t *base_vertices;
  vertex_t *normals; /* Normals for each vertex of the model */
  vertex_t *face_normals;
  texture_t *textures;/*Tex coord for each vertex */
  face_t *faces;
  float *area; /* area of each face */
  float total_area; /* area of the whole model */
  vertex_t bBox[2]; /* bBox[0] is the min  bBox[1] is the max */
  struct segment *segment_list; /* segments of fish model for animation purpose */
  int textId;
};


       
/* A triangle and useful associated information. AB is always the longest side
 * of the triangle. That way the projection of C on AB is always inside AB. */
struct triangle_info {
  dvertex_t a;         /* The A vertex of the triangle */
  dvertex_t b;         /* The B vertex of the triangle */
  dvertex_t c;         /* The C vertex of the triangle. The projection of C
                        * on AB is always inside the AB segment. */
  dvertex_t ab;        /* The AB vector */
  dvertex_t ca;        /* The CA vector */
  dvertex_t cb;        /* The CB vector */
  double ab_len_sqr;   /* The square of the length of AB */
  double ca_len_sqr;   /* The square of the length of CA */
  double cb_len_sqr;   /* The square of the length of CB */
 
  dvertex_t normal;    /* The (unit length) normal of the ABC triangle*/
  
  double s_area;       /* The surface area of the triangle */ 
};

/* A list of triangles with their associated information */
struct triangle_list {
  struct triangle_info *triangles; /* The triangles */
  int n_triangles;                 /* The number of triangles */
  double area;                     /* The total triangle area */
};


 

#ifndef __free_raw_model
#define __free_raw_model(raw_model)                             \
do {                                                            \
    free(((struct model*)raw_model)->vertices);                 \
    free(((struct model*)raw_model)->faces);                    \
    if (((struct model*)raw_model)->normals != NULL)            \
      free(((struct model*)raw_model)->normals);                \
    if (((struct model*)raw_model)->face_normals != NULL)       \
      free(((struct model*)raw_model)->face_normals);           \
    if (((struct model*)raw_model)->area != NULL)               \
      free(((struct model*)raw_model)->area);                   \
    free(((struct model*)raw_model));                           \
} while (0)
#endif

#endif
