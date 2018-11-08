#ifndef _MESH_H_
#define _MESH_H_

#import <iostream>
#import <OpenGL/gl.h>
#import "geomutils.h"
#import "3dmodel.h"

#define  mXDim  50
#define  mYDim  50



/// Definition d'un mesh.
/**
Un mesh est simplement un ensemble de triangles.  Ces triangles sont stockes
a l'aide de deux tableaux.  Le premiers tableau contient la liste des vertex
et le second contient la liste des index permettant de construire les
triangles.  Ces index sont en fait le numero du vertex dans le premier
tableau.  Par exemple, 2 triangles adjacents sont stockes comme suit:

Tableau des points:		(0,0,0) (2,0,0) (1,1,0) (1,-1,0)
Tableau des triangles:	(0,1,2) (1,0,3)
Tableau des textures:	(0,0) (1,1) (2,2) (3,3) 

*/
class	Mesh
{

public:
	/// Constructeur.
	Mesh();

	/// Destructeur.
	~Mesh();

	/// Charge une definition de mesh.
	/**
	Le format du fichier est le suivant:
	[Nombre de points] [Nombre de triangle]
    // Vertices
	x1 y1 z1
	x2 y2 z2
	...
	xn yn zn
    // Triangles (connection de vertices)
	i11, i12, i13
	i21, i22, i23
	...
	in1, in2, in3
	// Coordones de texture 
    s1 t1
	s2 t2
	s3 t3
	...
	sn tn

	@param FileName le nom du fichier.
	*/
	void	Load( const std::string &FileName );
    
    GLuint  loadTexture(const char * imagepath);
    
    GLuint  loadBMP(const char * imagepath);
    
    void    openFile();

	// Creer un maillage de fond 
	void    createSeaFloor();
   

	/// Calcule les normales de chaque vertex du mesh.
	void	ComputeNormals();

	/// Dessine le mesh.
	void	Draw() const;

	// Get min, max values of all verts
	void    setMinMax();

	struct model *tmesh;

protected:
   // Get the height value for the given x and y coords
   inline float getHeight(float x, float y, float curTime);
	
private:
	
};

#endif

