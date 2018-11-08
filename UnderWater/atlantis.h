


/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI (3.14159265358979323846f)
#endif
#define RAD 57.295
#define RRAD 0.01745
/* Some other utility macros.
 */
#define SINF(n)			((float)sin((double)(n)))
#define COSF(n)			((float)cos((double)(n)))
#define FABSF(n)		((float)fabs((double)(n)))


#undef MAX
#undef MIN
#undef ABS
#define MAX(a,b)((a)>(b)?(a):(b))
#define MIN(a,b)((a)<(b)?(a):(b))
#define ABS(a)((a)<0 ? -(a) : (a))


#define NUM_SHARKS 1

#define NUM_TANGS 2

#define NUM_KUMAN 1

#define SHARKSIZE 2
#define SHARKSPEED 0.0075

#define WHALESPEED 0.005
#define TANGSPEED 0.0035
#define KUMANOSPEED 0.0010 //0.0030

#define RMAX      1.5

typedef struct _fishRec {
    float x, y, z, phi, theta, psi, v;
    float xt, yt, zt;
    float htail, vtail;
	float dpsi;
    int spurt, attack;
	struct model* tmesh;
} fishRec;




extern void FishTransform(fishRec *);
extern void WhalePilot(fishRec *);
extern void TangPilot(fishRec *);
extern void KumanomiPilot(fishRec *);
extern void SharkPilot(fishRec *);
extern void SharkMiss(int);
extern void DrawWhale(fishRec * , int );
extern void DrawShark(fishRec * , int );
extern void DrawDolphin(fishRec *, int);
extern void DrawTang(fishRec * , int );
extern void DrawKumanomi(fishRec * , int );
