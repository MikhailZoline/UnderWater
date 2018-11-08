#ifdef _WIN32
#include <windows.h>
#endif

#ifndef __MAC__
#define __MAC__ 1
#endif

#ifdef __MAC__

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/GLUT.h>

#else

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#endif
using namespace std;
#include <iostream>
#include <math.h>
#include "Mesh.h"
#include "atlantis.h"

using namespace std;

float			g_Yaw	= 0.0f;
float			g_Pitch	= 0.0f;
float			g_Zoom	= 7.0f;
float			g_PointZ;
int				g_MouseX, g_MouseY, g_Button;
Mesh			g_Mesh;
enum			Mode { MODE_VIEW, MODE_POINT } g_Mode = MODE_VIEW;
bool			g_Wire	= false;
int				g_Level	= 1;
int             g_tail=0;
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
	glEnable(GL_TEXTURE_2D) ;


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
void	Display()
{
	int	i ;
	bool found = false;
    g_tail++;
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushAttrib(GL_LIGHTING_BIT);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	float x	= g_Zoom * cos( g_Pitch ) * sin( g_Yaw );
	float y	= g_Zoom * sin( g_Pitch );
	float z	= g_Zoom * cos( g_Pitch ) * cos( g_Yaw );
	gluLookAt( x, y, z, 0.0, 0.0 , 0.0, 0.0, 1.0, 0.0 );

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

	vertex_t  bBox0,bBox1;
    bBox0.x = -1.5 , bBox0.y = -1.5 , bBox0.z = -1.5 ;
    bBox1.x =  1.5 , bBox1.y = 1.5 , bBox1.z =  1.5 ;
    DrawBox(bBox0 , bBox1);
	
	DrawAxis();
	
    glutSwapBuffers();
}


void	Reshape( int width, int height )
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60.0, (double)width / height, 0.1, 1000000.0 );
	glMatrixMode( GL_MODELVIEW );
	glViewport( 0, 0, width, height );
}


void	OnMouseButton( int Button, int State, int x, int y )
{

	g_Mode	= MODE_VIEW;
	g_MouseX	= x;
	g_MouseY	= y;
	g_Button	= Button;
}


void	OnMouseMove( int x, int y )
{
	float	DiffX = g_MouseX - x;
	float	DiffY = g_MouseY - y;

	switch( g_Mode )
	{
	case MODE_VIEW:
		{
			if( g_Button == GLUT_LEFT_BUTTON )
			{
				g_Pitch	+= DiffY * 0.01f;
				g_Yaw	+= DiffX * 0.01f;
			}
			else if( g_Button == GLUT_RIGHT_BUTTON )
			{
				float	k;
				if( DiffX + DiffY > 0 )
					k = sqrt( DiffX * DiffX + DiffY * DiffY );
				else
					k = -sqrt( DiffX * DiffX + DiffY * DiffY );

				if( g_Zoom * ( 1 + k * 0.01 ) > 0.0 )
					g_Zoom	*= 1 + k * 0.01;
			}
		}
		break;

	case MODE_POINT:

		break;
	}
	g_MouseX = x;
	g_MouseY = y;
}


void	OnKeyPressed( unsigned char Key, int x, int y )
{
	char	FileName[260];
    
	switch( Key )
	{
	case 'w':
	case 'W':
		g_Wire	= !g_Wire;
		if( g_Wire )
			cout << "Mode fils de fer\n";
		else
			cout << "Mode remplissage\n";
		break;

	case 's':
	case 'S':
		cout << "Sauvegarde du mesh subdivise sous: ";
		cin >> FileName;
	
		break;

	case 'c':
	case 'C':
		cout << "Sauvegarde du mesh de controle sous: ";
		cin >> FileName;
	
		break;

	case '+':
	case '=':
		if( g_Level < 7 )
			g_Level++;
		cout << "Subdivision niveau " << g_Level << endl;
	
		break;

	case '-':
	case '_':
		if( g_Level > 1 )
			g_Level--;
		cout << "Subdivision niveau " << g_Level << endl;
	
		break;

	case' ':
		
	    break;
	
	case 27: // ESC.
		exit( 0 );
	}
}

void
Animate(void)
{
    int i;

    for (i = 0; i < NUM_SHARKS; i++) {
        SharkPilot(&LeopardShark[i]);
        SharkMiss(i);
    }
     glutPostRedisplay();

    for (i = 0; i < NUM_TANGS; i++) {
        TangPilot(&BlueTang[i]);
        //SharkMiss(i);
    }
    for (i = 0; i < NUM_KUMAN; i++) {
        KumanomiPilot(&Kumanomi[i]);
        //SharkMiss(i);
    }

    glutPostRedisplay();

	/*
    WhalePilot(&momWhale);
    momWhale.phi++;
    WhalePilot(&babyWhale);
    babyWhale.phi++;
	*/
}


void	Init()
{

	float	a[4]	=	{ 0.4f, 0.4f, 0.4f, 1.0f };
	float	d[4]	=	{ 0.75f, 0.75f, 0.75f, 1.0f };
	float	s[4]	=	{ 0.7f, 0.7f, 0.7f, 1.0f };
	float	lp[4]	=	{ 5.0f, -25.0f, -25.0f, 1.0f };

	glLightfv( GL_LIGHT0, GL_POSITION, lp );
	glEnable( GL_LIGHT0 );

	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, a );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, d );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, s );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 50.0f );

	glShadeModel( GL_SMOOTH );
	//glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
	glClearColor(0.0f, 0.4f, 0.75f, 0.0f);
	//glClearColor( 0.25f, 0.25f, 0.25f, 0.0f );
	//glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glEnable( GL_NORMALIZE );
	glEnable( GL_AUTO_NORMAL );
	glEnable( GL_LIGHTING );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
}

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

int	main( int argc, char** argv )
{
	glutInit( &argc, argv );
	glutInitWindowSize( 800, 600 );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutCreateWindow( "NewAtlantis" );

	int i  ;

	if( argc == 2 )
		g_Mesh.Load( argv[1] );
	else
    
    g_Mesh.Load( "LeopardSharkTS.raw" );
//    invert_normals(g_Mesh.tmesh);
    for(  i = 0 ;  i < NUM_SHARKS ; i++ )
    LeopardShark[i].tmesh= g_Mesh.tmesh;



    g_Mesh.Load( "BlueTangTS.raw" );
//    invert_normals(g_Mesh.tmesh);
    for (i=0 ; i < NUM_TANGS ; i++)
    BlueTang[i].tmesh= g_Mesh.tmesh;


	g_Mesh.Load( "KumanomiTS.raw" );
//    invert_normals(g_Mesh.tmesh);
	for (i=0 ; i < NUM_KUMAN ; i++)
	Kumanomi[i].tmesh= g_Mesh.tmesh;
  
	g_Mesh.createSeaFloor();
//    invert_normals(g_Mesh.tmesh);
	sea_floor = g_Mesh.tmesh;
	
    cout<<"Loading Texture : SAND1.jpg"<<endl;
    sea_floor->textId = g_Mesh.loadTexture("SAND1.jpg");;
    cout<<"Fin de creation de fond de la mer\n";
	
	Init();
	InitFishs();

	glutDisplayFunc( Display );
	glutMouseFunc( OnMouseButton );
	glutMotionFunc( OnMouseMove );
	glutKeyboardFunc( OnKeyPressed );
    glutIdleFunc( Animate );
	glutReshapeFunc( Reshape );
	glutMainLoop();

	return	0;
}
