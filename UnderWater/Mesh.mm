#import "Mesh.h"
#import <Foundation/Foundation.h>
#import <GLUT/glut.h>
#import <OpenGL/glu.h>
#import <fstream>
#import <math.h>
#import <vector>
#import <stdio.h>
using namespace std;

Mesh::Mesh() {}

Mesh::~Mesh() {}

void Mesh::Draw() const
{
    
    glVertexPointer(3, GL_FLOAT, sizeof(vertex_t), &(tmesh->vertices[0]));
    glNormalPointer(GL_FLOAT, sizeof(vertex_t), &(tmesh->normals[0]));
    glTexCoordPointer(2, GL_FLOAT, 0, &(tmesh->textures[0]));
    
    glDrawElements(GL_TRIANGLES, (int)tmesh->num_faces * 3, GL_UNSIGNED_INT,
                   &(tmesh->faces[0]));
}

void Mesh::openFile()
{
    NSURL *myURL = [NSURL fileURLWithPath:@"/Users/admin/Downloads/UnderWater/UnderWater/DerivedData/UnderWater/Build/Products/Debug/BlueTaT.jpg"];
    CFURLRef url = (__bridge CFURLRef)(myURL);
    CGImageSourceRef myImageSourceRef = CGImageSourceCreateWithURL(url, NULL);
    if (!myImageSourceRef) {
        NSLog(@"No image");
    }
    CGImageRef myImageRef = CGImageSourceCreateImageAtIndex(myImageSourceRef, 0, NULL);
    CFDataRef dataRef = CGDataProviderCopyData(CGImageGetDataProvider(myImageRef));
    const unsigned char *data = CFDataGetBytePtr(dataRef);
    NSUInteger width = CGImageGetWidth(myImageRef);
    NSUInteger height = CGImageGetHeight(myImageRef);
    cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
    cout<< "width: "<< width <<", "<<"height: "<<height<<endl;
    cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
}

GLuint Mesh::loadTexture(const char *imagepath)
{
    CFStringRef cfStr = CFURLCopyPath(CFBundleCopyBundleURL(CFBundleGetMainBundle()));
    const char *basePath = CFStringGetCStringPtr(cfStr, CFStringGetSystemEncoding());
    char filepath [512];
    strcpy(filepath, basePath);
    strcat(filepath, imagepath);

//    NSCharacterSet *characterset=[NSCharacterSet characterSetWithCharactersInString:@"\r"];
//    NSString *filePath = [[NSString stringWithFormat:@"%s", filepath] stringByTrimmingCharactersInSet:characterset];
//
//    NSURL *myURL = [NSURL fileURLWithPath:[[NSString stringWithFormat:@"%s", filepath] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"\r"]]];
//    CFURLRef url = (CFURLRef)CFBridgingRetain([NSURL fileURLWithPath:[[NSString stringWithFormat:@"%s", filepath] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"\r"]]]);
    CGImageSourceRef myImageSourceRef = CGImageSourceCreateWithURL((CFURLRef)CFBridgingRetain([NSURL fileURLWithPath:[[NSString stringWithFormat:@"%s", filepath] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"\r"]]]), NULL);
    if (!myImageSourceRef) {
        NSLog(@"No image found");
    }
    CGImageRef myImageRef = CGImageSourceCreateImageAtIndex(myImageSourceRef, 0, NULL);
    CFDataRef dataRef = CGDataProviderCopyData(CGImageGetDataProvider(myImageRef));
    const unsigned char *imageData = CFDataGetBytePtr(dataRef);
    NSUInteger width = CGImageGetWidth(myImageRef);
    NSUInteger height = CGImageGetHeight(myImageRef);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    // "Bind" the newly created texture : all future texture functions will
    // modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Typical Texture Generation Using Data From The Bitmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    // crank out the texture
    // OpenGL likes the GL_BGRA + GL_UNSIGNED_INT_8_8_8_8_REV combination
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA,
//                 GL_UNSIGNED_INT_8_8_8_8_REV, imageData);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 width,
                 height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 imageData);
    return textureID;
}

 GLuint Mesh::loadBMP(const char * imagepath){
 // Data read from the header of the BMP file
 unsigned char header[54]; // Each BMP file begins by a 54-bytes header
 unsigned int dataPos;     // Position in the file where the actual data
 unsigned int width, height; unsigned int imageSize;   // = width*height*3
 // Actual RGB data
 unsigned char * data;
 
     CFStringRef cfStr = CFURLCopyPath(CFBundleCopyBundleURL(CFBundleGetMainBundle()));
     const char *basePath = CFStringGetCStringPtr(cfStr, CFStringGetSystemEncoding());
     char fullPath [PATH_MAX];
     strcpy(fullPath, basePath);
     strcat(fullPath, imagepath);
 
  NSCharacterSet *characterset=[NSCharacterSet characterSetWithCharactersInString:@"\r"];
  NSString *filePath = [[NSString stringWithFormat:@"%s", fullPath] stringByTrimmingCharactersInSet:characterset];
     
//  NSURL *myURL = [NSURL fileURLWithPath:[[NSString stringWithFormat:@"%s", filePath] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"\r"]]];
//  CFURLRef url = (CFURLRef)CFBridgingRetain([NSURL fileURLWithPath:[[NSString stringWithFormat:@"%s", filePath] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"\r"]]]);
     CGImageSourceRef myImageSourceRef = CGImageSourceCreateWithURL((CFURLRef)CFBridgingRetain([NSURL fileURLWithPath:[[NSString stringWithFormat:@"%s", fullPath] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"\r"]]]), NULL);
     if (!myImageSourceRef) {
         NSLog(@"No image found");
     }
     CGImageRef myImageRef = CGImageSourceCreateImageAtIndex(myImageSourceRef, 0, NULL);
     CFDataRef dataRef = CGDataProviderCopyData(CGImageGetDataProvider(myImageRef));
     const unsigned char *imageData = CFDataGetBytePtr(dataRef);
     
 FILE * file = fopen([filePath UTF8String],"r+");

 if (!file){printf("Image could not be opened: %d \n",  errno); return 0;}
 if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
 printf("Not a correct BMP file\n");
 return false;
 }
 if ( header[0]!='B' || header[1]!='M' ){
 printf("Not a correct BMP file\n");
 return 0;
 }
 // Read ints from the byte array
 dataPos    = *(int*)&(header[0x0A]);
 imageSize  = *(int*)&(header[0x22]);
 width      = *(int*)&(header[0x12]);
 height     = *(int*)&(header[0x16]);
 // Some BMP files are misformatted, guess missing information
 if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red,
// Green and Blue component if (dataPos==0)      dataPos=54;
 // Create a buffer
 data = new unsigned char [imageSize];
     
 // Read the actual data from the file into the buffer
 fread(data,1,imageSize,file);
 
 //Everything is in memory now, the file can be closed
 fclose(file);
 // Create one OpenGL texture
 GLuint textureID;
 glGenTextures(1, &textureID);
 
 
 // "Bind" the newly created texture : all future texture functions will
 // modify this texture glBindTexture(GL_TEXTURE_2D, textureID);
 // Typical Texture Generation Using Data From The Bitmap
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
 
 
 glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 
 
 // crank out the texture
 glTexImage2D(GL_TEXTURE_2D,
 0,
 3,
 width,
 height,
 0,
 GL_RGB,
 GL_UNSIGNED_BYTE,
 data);
 
 // Give the image to OpenGL
 //    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR,GL_UNSIGNED_BYTE, data);
 //    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
 //    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
 return  textureID;
 }

void Mesh::Load(const std::string &FileName)
{
    ifstream f(FileName.c_str());
    int NbVertices;
    int NbTriangles;
    int NbSections;
    int i, j;
    char TextureName[32];
    /* Allocate space and initialize mesh */
    f >> NbVertices;
    f >> NbTriangles;
    f >> NbSections;
    f.getline(TextureName, 32);
    cout << "Loading Model : " << FileName.c_str() << endl;
    tmesh = (model *)calloc(1, sizeof(*tmesh));
    if (tmesh == NULL)
        cout << "****** Out of Memory error *******\n";
    tmesh->num_faces = NbTriangles;
    tmesh->num_vert = NbVertices;
    tmesh->num_segm = NbSections;
    tmesh->vertices =
    (vertex_t *)malloc(sizeof(*(tmesh->vertices)) * NbVertices);
    tmesh->base_vertices =
    (vertex_t *)malloc(sizeof(*(tmesh->vertices)) * NbVertices);
    tmesh->faces = (face_t *)malloc(sizeof(*(tmesh->faces)) * NbTriangles);
    tmesh->textures =
    (texture_t *)malloc(sizeof(*(tmesh->textures)) * NbVertices);
    tmesh->segment_list =
    (segment *)malloc(sizeof(*(tmesh->segment_list)) * NbSections);
    
    for (i = 0; i < NbVertices; i++) {
        float x, y, z;
        f >> x >> y >> z;
        tmesh->vertices[i].x = x;
        tmesh->vertices[i].y = y;
        tmesh->vertices[i].z = z;
        tmesh->base_vertices[i] = tmesh->vertices[i];
    }
    
    for (i = 0; i < NbTriangles; i++) {
        int v1, v2, v3;
        f >> v1 >> v2 >> v3;
        
        tmesh->faces[i].f0 = v1;
        tmesh->faces[i].f1 = v2;
        tmesh->faces[i].f2 = v3;
    }
    
    for (i = 0; i < NbVertices; i++) {
        float s, t;
        f >> s >> t;
        tmesh->textures[i].s = s;
        tmesh->textures[i].t = 1 - t;
    }
    
    for (i = 0; i < NbSections; i++) {
        cout << "Load section " << i << endl;
        f >> NbVertices;
        tmesh->segment_list[i].vertices = (int *)malloc(
                                                        sizeof(*(tmesh->segment_list[i].vertices)) * NbVertices);
        tmesh->segment_list[i].n_vertices = NbVertices;
        
        for (j = 0; j < NbVertices; j++)
            f >> tmesh->segment_list[i].vertices[j];
    }
    f.close();
    
    setMinMax();
    
    cout << " MIN " << tmesh->bBox[0].x << " , " << tmesh->bBox[0].y << " , "
    << tmesh->bBox[0].z << endl;
    cout << " MAX " << tmesh->bBox[1].x << " , " << tmesh->bBox[1].y << " , "
    << tmesh->bBox[1].z << endl;
    cout << dist_v(&tmesh->bBox[0], &tmesh->bBox[1]) << endl;
    
    ComputeNormals();
    
    cout << "Loading Texture :" << TextureName << endl;
    int count = strlen(TextureName);
    tmesh->textId = TextureName[count - 2] == 'p' ?  loadBMP(&TextureName[1]) : loadTexture(&TextureName[1]) ;
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, tmesh->textId);
    glEnable(GL_TEXTURE_2D);
    
    cout << "Fin de load" << endl;
}

// Get min, max values of all verts
void Mesh::setMinMax()
{
    int i, j;
    float max[3];
    float min[3];
    min[0] = min[1] = min[2] = FLT_MAX;
    max[0] = max[1] = max[2] = -FLT_MAX;
    
    for (i = 0; i < tmesh->num_vert; ++i) {
        vertex_t pVert = tmesh->base_vertices[i];
        if (pVert.x < min[0])
            min[0] = pVert.x;
        if (pVert.y < min[1])
            min[1] = pVert.y;
        if (pVert.z < min[2])
            min[2] = pVert.z;
        if (pVert.x > max[0])
            max[0] = pVert.x;
        if (pVert.y > max[1])
            max[1] = pVert.y;
        if (pVert.z > max[2])
            max[2] = pVert.z;
    }
    
    tmesh->bBox[0].x = min[0];
    tmesh->bBox[0].y = min[1];
    tmesh->bBox[0].z = min[2];
    tmesh->bBox[1].x = max[0];
    tmesh->bBox[1].y = max[1];
    tmesh->bBox[1].z = max[2];
    
    for (j = 0; j < tmesh->num_segm; j++) {
        
        min[0] = min[1] = min[2] = FLT_MAX;
        max[0] = max[1] = max[2] = -FLT_MAX;
        
        for (i = 0; i < tmesh->segment_list[j].n_vertices; ++i) {
            vertex_t pVert =
            tmesh->base_vertices[tmesh->segment_list[j].vertices[i]];
            if (pVert.x < min[0])
                min[0] = pVert.x;
            if (pVert.y < min[1])
                min[1] = pVert.y;
            if (pVert.z < min[2])
                min[2] = pVert.z;
            if (pVert.x > max[0])
                max[0] = pVert.x;
            if (pVert.y > max[1])
                max[1] = pVert.y;
            if (pVert.z > max[2])
                max[2] = pVert.z;
        }
        
        tmesh->segment_list[j].bBox[0].x = min[0];
        tmesh->segment_list[j].bBox[0].y = min[1];
        tmesh->segment_list[j].bBox[0].z = min[2];
        tmesh->segment_list[j].bBox[1].x = max[0];
        tmesh->segment_list[j].bBox[1].y = max[1];
        tmesh->segment_list[j].bBox[1].z = max[2];
    }
}

// Calculate the vertex normals after loading the mesh.
void Mesh::ComputeNormals()
{
    int i, k, kmax;
    vertex_t n;
    struct triangle_list *tl;
    struct triangle_info *triags;
    face_t *face_i;
    
    /* Initialize and allocate storage */
    int m = tmesh->num_faces;
    tl = (triangle_list *)malloc(sizeof(*tl));
    tl->n_triangles = m;
    triags = (triangle_info *)malloc(sizeof(*tl->triangles) * m);
    tl->triangles = triags;
    tl->area = 0;
    
    /* Convert the triangular model m to a triangle list (without connectivity
     * information) with the associated information. All the information about
     * the triangles (i.e. fields of struct triangle_info) is computed. */
    /* Convert triangles and update global data */
    for (i = 0; i < m; i++) {
        face_i = &(tmesh->faces[i]);
        init_triangle(&(tmesh->vertices[face_i->f0]),
                      &(tmesh->vertices[face_i->f1]),
                      &(tmesh->vertices[face_i->f2]), &(triags[i]));
        tl->area += triags[i].s_area;
    }
    
    /* Computes the normalized vertex normals assuming an oriented model. The
     * triangle information already present in tl are used to speed up the
     * calculation. If the model is not oriented, the resulting normals will be
     * incorrect. Vertices that belong to no triangles or to degenerate ones
     * only have a (0,0,0) normal vector set. */
    
    /* initialize all normals to zero */
    tmesh->normals = (vertex_t *)realloc(
                                         tmesh->normals, tmesh->num_vert * sizeof(*(tmesh->normals)));
    memset(tmesh->normals, 0, tmesh->num_vert * sizeof(*(tmesh->normals)));
    /* add face normals to vertices, weighted by face area */
    for (k = 0, kmax = tmesh->num_faces; k < kmax; k++) {
        vertex_d2f_v(&(tl->triangles[k].normal),
                     &n); /* convert double to float */
        __prod_v(tl->triangles[k].s_area, n, n);
        __add_v(n, tmesh->normals[tmesh->faces[k].f0],
                tmesh->normals[tmesh->faces[k].f0]);
        __add_v(n, tmesh->normals[tmesh->faces[k].f1],
                tmesh->normals[tmesh->faces[k].f1]);
        __add_v(n, tmesh->normals[tmesh->faces[k].f2],
                tmesh->normals[tmesh->faces[k].f2]);
    }
    /* normalize final normals */
    for (k = 0, kmax = tmesh->num_vert; k < kmax; k++) {
        /* skip vertices which have no triangles or only degenerated ones */
        if (tmesh->normals[k].x != 0 || tmesh->normals[k].y != 0 ||
            tmesh->normals[k].z != 0) {
            __normalize_v(tmesh->normals[k]);
            // neg_v(&(tmesh->normals[k]), &(tmesh->normals[k]));
        }
    }
    
    free(tl);
    free(triags);
}

// Get the height value for the given x and y coords
float Mesh::getHeight(float x, float y, float curTime)
{
    const float phase1(20.5f);
    const float offset1(1.2f);
    const float amp1(0.025f);
    const float var1 = x + curTime;
    
    const float phase2(20.6f);
    const float offset2(0.3f);
    const float amp2(0.015);
    const float var2 = y + curTime;
    
    const float phase3(20.0f);
    const float offset3(0.7f);
    const float amp3(0.010f);
    const float var3 = x + (2.0f * y) + curTime;
    
    float ret_val;
    
    ret_val = amp1 * sin((phase1 * var1) + offset1) +
    amp2 * sin((phase2 * var2) + offset2) +
    amp3 * sin((phase3 * var3) + offset3);
    
    return ret_val * 0.3;
}

// Desc:
//  This is a mesh of triangles based on a height grid
//  The grid is of unit size (1.0f x 1.0f) with a variable height
//  on the z-axis.
//
void Mesh::createSeaFloor()
{
    int i, j, k;
    
    int NbVertices = mXDim * mYDim;
    int NbTriangles = (mXDim - 1) * (mYDim - 1) * 2;
    tmesh = (model *)calloc(1, sizeof(*tmesh));
    if (tmesh == NULL)
        cout << "****** Out of Memory error *******\n";
    tmesh->num_faces = NbTriangles;
    tmesh->num_vert = NbVertices;
    tmesh->num_segm = 0;
    tmesh->vertices =
    (vertex_t *)malloc(sizeof(*(tmesh->vertices)) * NbVertices);
    tmesh->base_vertices =
    (vertex_t *)malloc(sizeof(*(tmesh->vertices)) * NbVertices);
    tmesh->faces = (face_t *)malloc(sizeof(*(tmesh->faces)) * NbTriangles);
    tmesh->textures =
    (texture_t *)malloc(sizeof(*(tmesh->textures)) * NbVertices);
    tmesh->segment_list =
    (segment *)malloc(sizeof(*(tmesh->segment_list)) * tmesh->num_segm);
    
    cout << "Creation du fond de la mer" << endl;
    float x, y, z;
    // Update height values of the grid
    for (i = 0, k = 0; i < mXDim; i++) {
        x = (1.0f / float(mXDim)) * float(i); // Compute current x coord
        for (j = 0; j < mYDim; j++) {
            y = (1.0f / float(mYDim)) * float(j);
            z = getHeight(x, y, 0.0);
            tmesh->textures[k].s = x;
            tmesh->textures[k].t = y;
            tmesh->vertices[k].x = x - 0.5;
            tmesh->vertices[k].y = z;
            tmesh->vertices[k].z = -y + 0.5;
            tmesh->base_vertices[k] = tmesh->vertices[k];
            k++;
        }
    }
    // Compute the connection of triangles
    
    for (k = 0, j = 0; j < mYDim - 1; j++) {
        
        for (i = 0; i < mXDim - 1; i++) {
            
            tmesh->faces[k].f0 = i + j * mXDim;
            tmesh->faces[k].f1 = i + j * mXDim + 1;
            tmesh->faces[k].f2 = i + (j + 1) * mXDim;
            
            ++k;
            
            tmesh->faces[k].f0 = i + (j + 1) * mXDim;
            tmesh->faces[k].f1 = i + j * mXDim + 1;
            tmesh->faces[k].f2 = i + (j + 1) * mXDim + 1;
            
            ++k;
        }
    }
    // Compute Normals
    ComputeNormals();
}
