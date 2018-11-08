#include <math.h>
#include <OpenGL/OpenGL.h>
//#include <GL/glu.h>

#include <Math/vjVec3.h>
#include <Math/vjCoord.h>
#include <math.h>
#include "Mesh.h"
#include "atlantis.h"

#include "MPApp.h"

Mesh			g_Mesh;
bool			g_Wire	= false;
fishRec LeopardShark[NUM_SHARKS];

fishRec BlueTang[NUM_TANGS];
fishRec Kumanomi[NUM_KUMAN];

model * sea_floor;


void draw_model(model* tmesh)
{
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);		
    glBindTexture(GL_TEXTURE_2D,tmesh->textId);
	glEnable(GL_TEXTURE_2D);


	glVertexPointer( 3, GL_FLOAT, sizeof( vertex_t ), &(tmesh->vertices[0]));
    glNormalPointer( GL_FLOAT, sizeof(vertex_t ), &(tmesh->normals[0]));
	glTexCoordPointer(2 , GL_FLOAT , 0 ,&(tmesh->textures[0]));
	
	glDrawElements( GL_TRIANGLES, (int)tmesh->num_faces * 3,
					GL_UNSIGNED_INT, &(tmesh->faces[0]) );
	
}

void invert_normals(const struct model *tmesh)
{
	int k,kmax;
for (k=0, kmax=tmesh->num_vert; k<kmax; k++) {
    /* skip vertices which have no triangles or only degenerated ones */
    if (tmesh->normals[k].x != 0 ||tmesh->normals[k].y != 0 || tmesh->normals[k].z != 0) {
      //__normalize_v(tmesh->normals[k]);
	 neg_v(&(tmesh->normals[k]), &(tmesh->normals[k]));
    }
  }
}

void DrawBox(vertex_t  bBox0 ,vertex_t  bBox1 )
{
	vertex_t p_Min = bBox0;
	vertex_t p_Max = bBox1;

	vertex_t p_Min1= p_Min ;
	p_Min1.y = p_Max.y ;
	vertex_t p_Min2= p_Max ;
	p_Min2.x = p_Min.x;
	vertex_t p_Min3= p_Min ;
	p_Min3.z = p_Max.z;

	vertex_t p_Max1= p_Max ;
	p_Max1.z = p_Min.z;
	vertex_t p_Max2= p_Min ;
	p_Max2.x = p_Max.x;
	vertex_t p_Max3= p_Max ;
	p_Max3.y = p_Min.y;

	glPushMatrix();
    glLineWidth(2.0);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDisable( GL_LIGHTING );
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	glVertex3fv(&p_Min.x);
	glVertex3fv(&p_Min1.x);
	glVertex3fv(&p_Min2.x);
	glVertex3fv(&p_Min3.x);

	glVertex3fv(&p_Max.x);
	glVertex3fv(&p_Max1.x);
	glVertex3fv(&p_Max2.x);
	glVertex3fv(&p_Max3.x);

	glVertex3fv(&p_Min.x);
	glVertex3fv(&p_Min3.x);
	glVertex3fv(&p_Max3.x);
	glVertex3fv(&p_Max2.x);

	glVertex3fv(&p_Max.x);
	glVertex3fv(&p_Min2.x);
	glVertex3fv(&p_Min1.x);
	glVertex3fv(&p_Max1.x);
	glEnd(); 
	glPopMatrix();
	glEnable( GL_LIGHTING );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

}
void DrawAxis()
{

    // Draw a coordinate axis "on" the box
    glLineWidth(3.0f);
    glPushMatrix();
    glDisable( GL_LIGHTING );

    glBegin(GL_LINES);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0,0,0);
    glVertex3f(0.5,0,0);

    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0,0,0);
    glVertex3f(0,0.5,0);

    //glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0,0,0);
    glVertex3f(0,0,0.5);
    glEnd();
    glPopMatrix();
	glEnable( GL_LIGHTING );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	  
}
//----------------------------------------------
//  Draw the scene.
//----------------------------------------------
void MPApp::draw()
{
	int	i ;
	bool found = false;
  
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushAttrib(GL_LIGHTING_BIT);


    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	/*
	float x	= g_Zoom * cos( g_Pitch ) * sin( g_Yaw );
	float y	= g_Zoom * sin( g_Pitch );
	float z	= g_Zoom * cos( g_Pitch ) * cos( g_Yaw );
	gluLookAt( x, y, z, 0.0, 0.0 , 0.0, 0.0, 1.0, 0.0 );
*/
	glEnable( GL_LIGHTING );
	if( g_Wire )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	else
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	 glPushMatrix();
	 glTranslatef(0 , -1.5 , 0);
     glScalef(3.0f,3.0f,3.0f);
	
	 draw_model(sea_floor);
     glPopMatrix();


	 for (i = 0; i < NUM_SHARKS; i++) {
        glPushMatrix();
		FishTransform(&LeopardShark[i]);
		DrawShark(&LeopardShark[i], 0);
		glPopMatrix();
    }
	

	for (i = 0; i < NUM_TANGS; i++) {
		glPushMatrix();
		FishTransform(&BlueTang[i]);
		DrawTang(&BlueTang[i], 0);
		glPopMatrix();
	}

	for (i = 0; i < NUM_KUMAN; i++) {
		glPushMatrix();
		FishTransform(&Kumanomi[i]);
		DrawKumanomi(&Kumanomi[i], 0);
		glPopMatrix();
	}

	vertex_t  bBox0 ,bBox1;
    bBox0.x = -1.5 , bBox0.y = -1.5 , bBox0.z = -1.5 ;
    bBox1.x =  1.5 , bBox1.y = 1.5 , bBox1.z =  1.5 ;
    DrawBox(bBox0 , bBox1);
	
	DrawAxis();
	
 }

/*
void MPApp::draw()
{
   // --- Setup for drawing --- //
   glClearColor(0.0f, 0.4f, 0.75f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
      glScalef(5.0f,5.0f,5.0f);
	  draw_model(sea_floor);
   glPopMatrix();
   
   glPushMatrix();
      glScalef(5.0f,5.0f,5.0f);
	  glTranslatef(0.0,1.0,0.0);
	  glColor3f(0.0f, 0.5f, 0.65f);
	  draw_model(LeopardShark[0].tmesh);
   glPopMatrix();
   
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
   glDisable(GL_LIGHTING);
   glBegin(GL_QUADS);
   
   glVertex3f(-1.5,0,-1.5);
   glVertex3f(-1.5,3,-1.5);
   glVertex3f(-1.5,3, 1.5);
   glVertex3f(-1.5,0, 1.5);

   glVertex3f( 1.5,0,-1.5);
   glVertex3f( 1.5,3,-1.5);
   glVertex3f( 1.5,3, 1.5);
   glVertex3f( 1.5,0, 1.5);

   glVertex3f(-1.5,0,-1.5);
   glVertex3f(-1.5,3,-1.5);
   glVertex3f( 1.5,3,-1.5);
   glVertex3f( 1.5,0,-1.5);
   
   glVertex3f(-1.5,0, 1.5);
   glVertex3f(-1.5,3, 1.5);
   glVertex3f( 1.5,3, 1.5);
   glVertex3f( 1.5,0, 1.5);

   glEnd();

   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   glEnable(GL_LIGHTING);
  	 
}
*/
void
InitFishs(void)
{
    int i;
    float r;
    for (i = 0; i < NUM_SHARKS; i++) {
    r = rand();
	LeopardShark[i].x = 0.75 + 0.75*r/r ;
	r = rand();
    LeopardShark[i].y = 0.05+ 0.75*r/r;
	r = rand();
    LeopardShark[i].z = 0.75*r/r ;
	LeopardShark[i].htail= 0;
    LeopardShark[i].v = 3;

    }
 for (i = 0; i < NUM_TANGS; i++) {
    r = rand();
	BlueTang[i].x = 0.5 + 0.75*r/r ;
	r = rand();
    BlueTang[i].y = 0.25+ 0.75*r/r;
	r = rand();
    BlueTang[i].z = 0.5 + 0.75*r/r ;
	BlueTang[i].htail= 0;
	BlueTang[i].xt = 0;
    BlueTang[i].yt = 0;
    BlueTang[i].zt = -0.75;
    BlueTang[i].v = 0.5;

    }

 for (i = 0; i < NUM_KUMAN; i++) {
    r = rand();
	Kumanomi[i].x = 0.05 + 0.75*r/r ;
	r = rand();
    Kumanomi[i].y = 0.05+ 0.75*r/r;
	r = rand();
    Kumanomi[i].z = 0.05 + 0.75*r/r ;
	Kumanomi[i].htail= 0;
	Kumanomi[i].xt = 0;
    Kumanomi[i].yt = 0;
    Kumanomi[i].zt = 0.75;
    Kumanomi[i].v = 0.3;

    }

}

void MPApp::initGLState()
{
	int i;
   GLfloat light0_ambient[] = { 0.1f,  0.1f,  0.1f,  1.0f};
   GLfloat light0_diffuse[] = { 0.8f,  0.8f,  0.8f,  1.0f};
   GLfloat light0_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f};
   GLfloat light0_position[] = {0.0f, 0.75f, 0.75f, 0.0f};

   GLfloat mat_ambient[] = { 0.7, 0.7,  0.7,  1.0};
   GLfloat mat_diffuse[] = { 1.0,  0.5,  0.8,  1.0};
   GLfloat mat_specular[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat mat_shininess[] = { 50.0};
   GLfloat mat_emission[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat no_mat[] = { 0.0,  0.0,  0.0,  1.0};

   glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);

   glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
   glMaterialfv( GL_FRONT,  GL_DIFFUSE, mat_diffuse );
   glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
   glMaterialfv( GL_FRONT,  GL_SHININESS, mat_shininess );
   glMaterialfv( GL_FRONT,  GL_EMISSION, no_mat);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);

   glEnableClientState( GL_VERTEX_ARRAY );
   glEnableClientState( GL_NORMAL_ARRAY );
   glEnableClientState( GL_TEXTURE_COORD_ARRAY );

   glClearColor(0.0f, 0.4f, 0.75f, 0.0f);

   g_Mesh.Load( "LeopardSharkTS.raw" );
	//invert_normals(g_Mesh.tmesh);
	for(  i = 0 ;  i < NUM_SHARKS ; i++ )
	LeopardShark[i].tmesh= g_Mesh.tmesh;



	g_Mesh.Load( "BlueTangTS.raw" );
	invert_normals(g_Mesh.tmesh);	
	for (i=0 ; i < NUM_TANGS ; i++)
	BlueTang[i].tmesh= g_Mesh.tmesh;


	g_Mesh.Load( "KumanomiTS.raw" );
	invert_normals(g_Mesh.tmesh);
	for (i=0 ; i < NUM_KUMAN ; i++)
	Kumanomi[i].tmesh= g_Mesh.tmesh;
  
	g_Mesh.createSeaFloor();
	invert_normals(g_Mesh.tmesh);
	sea_floor = g_Mesh.tmesh;
	
	glTexture m_MySeaTexture;
    TextureLoader *pTextureLoader = new TextureLoader();
    cout<<"Loading Texture : SAND1.jpg"<<endl;
    pTextureLoader->LoadTextureFromDisk("SAND1.jpg", &m_MySeaTexture);
    sea_floor->textId=m_MySeaTexture.TextureID;
    cout<<"Fin de creation de fond de la mer\n";

	InitFishs();
	
}

void MPApp::
Animate(void)
{
    int i;

    for (i = 0; i < NUM_SHARKS; i++) {
        SharkPilot(&LeopardShark[i]);
        SharkMiss(i);
    }
	 //glutPostRedisplay();

	for (i = 0; i < NUM_TANGS; i++) {
        TangPilot(&BlueTang[i]);
        //SharkMiss(i);
    }
    for (i = 0; i < NUM_KUMAN; i++) {
        KumanomiPilot(&Kumanomi[i]);
        //SharkMiss(i);
    }

    //glutPostRedisplay();

	/*
    WhalePilot(&momWhale);
    momWhale.phi++;
    WhalePilot(&babyWhale);
    babyWhale.phi++;
	*/
}

