
#ifdef _WIN32
#include <windows.h>
#endif

#include <OpenGL/gl.h>

#include <math.h>

#include "atlantis.h"
#include "3dmodel.h"

void
DrawShark(fishRec * fish, int wire=0)
{
	
	int i;
	fish->htail = (int) (fish->htail - (int) (5.0 * fish->v) + (int)(7.0*fish->dpsi)) % 360 ;

	float thrash = 0.015 * fish->v;
	if(fish->dpsi > 0 )thrash +=0.005* fish->v;
/*
	vertex_t g_Min2 = fish->tmesh->segment_list[2].bBox[0];
	vertex_t g_Min1 = fish->tmesh->segment_list[1].bBox[0];
	vertex_t g_Min0 = fish->tmesh->segment_list[0].bBox[0];
	vertex_t g_Max0 = fish->tmesh->segment_list[0].bBox[1];
	vertex_t g_WMin = g_Min2 ;
	vertex_t g_WMax = g_Max0 ;

	float dist_X = g_WMax.x - g_WMin. x;
		
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

    	vertex_t g_Min3 = fish->tmesh->segment_list[3].bBox[0];
		vertex_t g_Max3 = fish->tmesh->segment_list[3].bBox[1];
    	vertex_t g_Min2 = fish->tmesh->segment_list[2].bBox[0];
		vertex_t g_Max2 = fish->tmesh->segment_list[2].bBox[1];
		vertex_t g_Min1 = fish->tmesh->segment_list[1].bBox[0];
		vertex_t g_Max1 = fish->tmesh->segment_list[1].bBox[1];

		vertex_t g_Min0 = fish->tmesh->segment_list[0].bBox[0];
		vertex_t g_Max0 = fish->tmesh->segment_list[0].bBox[1];		
		float dist_X = g_Max3.x - g_Min0. x;
/*
			for( i = 0; 
		i < fish->tmesh->segment_list[3].n_vertices; i++ )
		{
			int p = fish->tmesh->segment_list[3].vertices[i];
	   		vertex_t base = fish->tmesh->base_vertices[p];
			vertex_t* v = &fish->tmesh->vertices[p];
			v->z=base.z;
		
			v->z += (g_Max3.x - v->x)/(dist_X)*(0.033 * fish->v)*sin( M_PI*(g_Max3.x - v->x)/dist_X + fish->htail* RRAD );
		}

*/
		for( i = 0; 
		i < fish->tmesh->segment_list[2].n_vertices; i++ )
		{
			int p = fish->tmesh->segment_list[2].vertices[i];
	   		vertex_t base = fish->tmesh->base_vertices[p];
			vertex_t* v = &fish->tmesh->vertices[p];
			v->z=base.z;
		
			v->z += (g_Max3.x - v->x)/(dist_X)*(0.035 * fish->v)*sin( M_PI*(g_Max3.x - v->x)/(dist_X*0.975) + (fish->htail+ fish->htail*0.004)* RRAD );
		}
		for( i = 0; 
		i < fish->tmesh->segment_list[1].n_vertices; i++ )
		{
			int p = fish->tmesh->segment_list[1].vertices[i];
	   		vertex_t base = fish->tmesh->base_vertices[p];
			vertex_t* v = &fish->tmesh->vertices[p];
			v->z=base.z;
		
			v->z += (g_Max3.x - v->x)/(dist_X)*(0.037 * fish->v)*sin( M_PI*( g_Max3.x - v->x  )/(dist_X *0.975)+ (fish->htail + fish->htail*0.0045) * RRAD);
		}

		for( i = 0; 
		i < fish->tmesh->segment_list[0].n_vertices; i++ )
		{
			int p = fish->tmesh->segment_list[0].vertices[i];
	   		vertex_t base = fish->tmesh->base_vertices[p];
			vertex_t* v = &fish->tmesh->vertices[p];
			v->z=base.z;

			v->z +=(g_Max3.x - v->x)/(dist_X)*(0.04 * fish->v)*sin( M_PI*( g_Max3.x - v->x )/(dist_X*0.975) + (fish->htail + fish->htail*0.0045) * RRAD );

		} 


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);		
    glBindTexture(GL_TEXTURE_2D, fish->tmesh->textId);
	glEnable(GL_TEXTURE_2D) ;


	glPushMatrix();
	glRotatef(-90 , 0 , 1 , 0 );
	glScalef(2, 2, 2);
	glVertexPointer( 3, GL_FLOAT, sizeof( vertex_t ), &(fish->tmesh->vertices[0]));
    glNormalPointer( GL_FLOAT, sizeof(vertex_t ), &(fish->tmesh->normals[0]));
	glTexCoordPointer(2 , GL_FLOAT , 0 ,&(fish->tmesh->textures[0]));
	
	glDrawElements( GL_TRIANGLES, (int)fish->tmesh->num_faces * 3,
					GL_UNSIGNED_INT, &(fish->tmesh->faces[0]) );
	glPopMatrix();
}

