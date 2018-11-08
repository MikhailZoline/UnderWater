#ifdef _WIN32
#include <windows.h>
#endif

//#include <GLUT/gl.h>
#include <OpenGL/gl.h>


#include <math.h>

#include "atlantis.h"
#include "3dmodel.h"

void
DrawTang(fishRec * fish, int wire=0)
{
	
	int i;
	fish->htail = (int) (fish->htail - (int) (5.0 * fish->v) ) % 360 ;
	float thrash = 0.025 ;
//	float thrash = 0.5 * fish->v;
	vertex_t g_Min3 = fish->tmesh->segment_list[3].bBox[0];
	vertex_t g_Min2 = fish->tmesh->segment_list[2].bBox[0];
	vertex_t g_Min1 = fish->tmesh->segment_list[1].bBox[0];
	vertex_t g_Min0 = fish->tmesh->segment_list[0].bBox[0];
	vertex_t g_Max0 = fish->tmesh->segment_list[0].bBox[1];

	vertex_t p_Min = fish->tmesh->segment_list[4].bBox[0];
	vertex_t p_Max = fish->tmesh->segment_list[4].bBox[1];


	vertex_t g_WMin = g_Min2 ;
	vertex_t g_WMax = g_Max0 ;

	float dist_X = g_WMax.x - g_WMin. x;

	for( i = 0; 
		i < fish->tmesh->segment_list[4].n_vertices; i++ )
		{
			int p = fish->tmesh->segment_list[4].vertices[i];
	   		vertex_t base = fish->tmesh->base_vertices[p];
			vertex_t* v = &fish->tmesh->vertices[p];
			v->y=base.y;
			v->x=base.x;
			v->z=base.z;

		//	v->y +=  pow(fabs(p_Min.x - v->x)/fabs(p_Max.x - p_Min.x ), 1.7)*0.005*sin( M_PI*( v->x - p_Min.x )/fabs(p_Max.x - p_Min.x ) + fish->htail* RRAD);
			v->x +=  pow(fabs(p_Min.x - v->x)/fabs(p_Max.x - p_Min.x ), 1.7)*0.020*cos( M_PI*( v->x - p_Min.x )/fabs(p_Max.x - p_Min.x ) + fish->htail*2* RRAD);
			v->z +=  pow(fabs(p_Min.x - v->x)/fabs(p_Max.x - p_Min.x ), 1.7)*0.0105*sin( M_PI*( v->x - p_Min.x )/fabs(p_Max.x - p_Min.x ) + fish->htail* RRAD);
		}

	for( i = 0; 
		i < fish->tmesh->segment_list[5].n_vertices; i++ )
		{
			int p = fish->tmesh->segment_list[5].vertices[i];
	   		vertex_t base = fish->tmesh->base_vertices[p];
			vertex_t* v = &fish->tmesh->vertices[p];
			v->y=base.y;
			v->x=base.x;
			v->z=base.z;

		//	v->y +=  pow(fabs(p_Min.x - v->x)/fabs(p_Max.x - p_Min.x ), 1.7)*0.005*sin( M_PI*( v->x - p_Min.x )/fabs(p_Max.x - p_Min.x ) + fish->htail* RRAD);
			v->x +=  pow(fabs(p_Min.x - v->x)/fabs(p_Max.x - p_Min.x ), 1.7)*0.020*cos( M_PI*fabs( v->x - p_Min.x )/fabs(p_Max.x - p_Min.x ) + fish->htail*2* RRAD);
			v->z -=  pow(fabs(p_Min.x - v->x)/fabs(p_Max.x - p_Min.x ), 1.7)*0.0105*sin( M_PI*fabs( v->x - p_Min.x )/fabs(p_Max.x - p_Min.x ) + fish->htail* RRAD);
		}

		
		for( i = 0; 
		i < fish->tmesh->segment_list[3].n_vertices; i++ )
		{
			int p = fish->tmesh->segment_list[3].vertices[i];
	   		vertex_t base = fish->tmesh->base_vertices[p];
			vertex_t* v = &fish->tmesh->vertices[p];
			v->z=base.z;
		 // v->z += pow(fabs(g_Min3.x - v->x)/(g_Max0.x - g_Min3.x ), 2.5)*0.011*sin( M_PI*( v->x - g_Min2.x )/dist_X +fish->htail* RRAD );
			v->z += pow(fabs(g_Min3.x - v->x)/(dist_X), 1.5)*0.027*sin( M_PI*( v->x - g_Min3.x )/dist_X +fish->htail* RRAD );
		}

		for( i = 0; 
		i < fish->tmesh->segment_list[2].n_vertices; i++ )
		{
			int p = fish->tmesh->segment_list[2].vertices[i];
	   		vertex_t base = fish->tmesh->base_vertices[p];
			vertex_t* v = &fish->tmesh->vertices[p];
			v->z=base.z;
		//  v->z += pow(fabs(g_Min3.x - v->x)/(g_Max0.x - g_Min3.x ), 2.5)*0.011*sin( M_PI*( v->x - g_Min2.x )/dist_X + fish->htail* RRAD );
			v->z += pow(fabs(g_Min3.x - v->x)/(dist_X), 1.5)*0.029*sin( M_PI*fabs( v->x - g_Min3.x )/dist_X + fish->htail* RRAD );
		}
		for( i = 0; 
		i < fish->tmesh->segment_list[1].n_vertices; i++ )
		{
			int p = fish->tmesh->segment_list[1].vertices[i];
	   		vertex_t base = fish->tmesh->base_vertices[p];
			vertex_t* v = &fish->tmesh->vertices[p];
			v->z=base.z;
		//	v->z += pow(fabs(g_Min3.x - v->x)/(g_Max0.x - g_Min3.x ), 2.5)*0.025*sin( M_PI*( v->x - g_Min2.x )/dist_X +fish->htail* RRAD );
			v->z +=  pow(fabs(g_Min3.x - v->x)/(dist_X), 1.45)*0.032*sin( M_PI*( v->x - g_Min3.x )/dist_X + fish->htail* RRAD);
		}

		for( i = 0; 
		i < fish->tmesh->segment_list[0].n_vertices; i++ )
		{
			int p = fish->tmesh->segment_list[0].vertices[i];
	   		vertex_t base = fish->tmesh->base_vertices[p];
			vertex_t* v = &fish->tmesh->vertices[p];
			v->z=base.z;
		//  v->z += pow(fabs(g_Min3.x - v->x)/(g_Max0.x - g_Min3.x ), 2.5)*0.025 *sin( M_PI*( v->x - g_Min2.x )/dist_X + fish->htail* RRAD )+
		//  pow((v->x - g_Min0.x  )/(g_Max0.x - g_Min0.x ), 1.7) *0.025*sin( M_PI*( v->x - g_Min2.x )/dist_X + fish->htail* RRAD );

			v->z +=pow(fabs(g_Min3.x - v->x)/(dist_X), 1.45)*0.033*sin( M_PI*( v->x - g_Min3.x )/dist_X + fish->htail* RRAD );

			v->z +=pow(fabs(g_Min1.x - v->x)/(dist_X), 2.0)*0.035*sin( M_PI*( v->x - g_Min3.x )/dist_X + fish->htail* RRAD );

		} 


/***********************************************************/
/*
		
	for( i = 0; 
	i < fish->tmesh->segment_list[2].n_vertices; i++ )
	{
		int p = fish->tmesh->segment_list[2].vertices[i];
	   	vertex_t base =fish->tmesh->base_vertices[p];
		vertex_t* v = &fish->tmesh->vertices[p];
		v->z=base.z;
		v->z += pow(fabs(g_Min2.x - v->x)/(g_Min1.x - g_Min2.x ), 1.7) * thrash
				*sin( M_PI*( v->x - g_Min2.x )/dist_X + fish->htail* RRAD );
	}

	for( i = 0; 
	i < fish->tmesh->segment_list[1].n_vertices; i++ )
	{
		int p = fish->tmesh->segment_list[1].vertices[i];
	   	vertex_t base = fish->tmesh->base_vertices[p];
		vertex_t* v = &fish->tmesh->vertices[p];
		v->z=base.z;
		v->z +=  thrash * sin( M_PI*( v->x - g_Min2.x )/dist_X + fish->htail* RRAD );
	}

	for( i = 0; 
	i < fish->tmesh->segment_list[0].n_vertices; i++ )
	{
		int p = fish->tmesh->segment_list[0].vertices[i];
	   	vertex_t base = fish->tmesh->base_vertices[p];
		vertex_t* v = &fish->tmesh->vertices[p];
		v->z=base.z;
		v->z += thrash * sin( M_PI*( v->x - g_Min2.x )/dist_X + fish->htail* RRAD );
	}

*/
/*******************************************************************/
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);		
    glBindTexture(GL_TEXTURE_2D, fish->tmesh->textId);
	glEnable(GL_TEXTURE_2D) ;


	glPushMatrix();
//	glScalef(1.75, 1.75, 1.75);
	glRotatef(-90 , 0 , 1 , 0);
	glVertexPointer( 3, GL_FLOAT, sizeof( vertex_t ), &(fish->tmesh->vertices[0]));
    glNormalPointer( GL_FLOAT, sizeof(vertex_t ), &(fish->tmesh->normals[0]));
	glTexCoordPointer(2 , GL_FLOAT , 0 ,&(fish->tmesh->textures[0]));
	
	glDrawElements( GL_TRIANGLES, (int)fish->tmesh->num_faces * 3,
					GL_UNSIGNED_INT, &(fish->tmesh->faces[0]) );
	glPopMatrix();
}

