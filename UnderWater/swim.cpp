/**
 * (c) Copyright 1993, 1994, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * US Government Users Restricted Rights
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */
#ifdef _WIN32
#include <windows.h>
#endif

//#include <GL/gl.h>
#include <OpenGL/gl.h>

#include <math.h>
#include <stdlib.h>  /* For rand(). */
#include <iostream>

#include "atlantis.h"

extern fishRec LeopardShark[NUM_SHARKS];
extern fishRec BlueTang[1];
void
FishTransform(fishRec * fish)
{

    glTranslatef(fish->y, fish->z, -fish->x);

    glRotatef(-fish->psi, 0.0, 1.0, 0.0);	
    glRotatef(fish->theta, 1.0, 0.0, 0.0);
    glRotatef(-fish->phi, 0.0, 0.0, 1.0);

    fish->htail++;
	fish->psi -=1;
}



void
WhalePilot(fishRec * fish)
{

    fish->phi = 0.0;
    fish->theta = -5.0;
    fish->psi -= 0.15;

 //   fish->x += WHALESPEED * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
    fish->y += WHALESPEED * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);
    fish->z += WHALESPEED * fish->v * sin(fish->theta / RAD);
}
void
TangPilot(fishRec * fish)
{
	static int sign = 1;
    float X, Y, Z, tpsi, ttheta, thetal;


    X = fish->xt - fish->x;
    Y = fish->yt - fish->y;
    Z = fish->zt - fish->z;
	if(rand() % 500 > 498){

		fish->zt= 0 -  fish->zt ;

	}  

    ttheta = RAD * atan(Z / (sqrt(X * X + Y * Y)));

    if (ttheta > fish->theta + 0.25) {
        fish->theta += 0.5;
    } else if (ttheta < fish->theta - 0.25) {
        fish->theta -= 0.5;
    }
    if (fish->theta > 90.0) {
        fish->theta = 90.0;
    }
    if (fish->theta < -90.0){
        fish->theta = -90.0;
    }
   

    tpsi = RAD * atan2(Y, X);

    fish->attack = 0;
	

    if (fabs(tpsi - fish->psi) < 10.0) {
        fish->attack = 1;
    } 
	else if (fabs(tpsi - fish->psi) < 45.0) 
	{
        if (fish->psi > tpsi) {
            fish->psi -= 0.5;
            if (fish->psi < -180.0) {
                fish->psi += 360.0;
            }
        } 
		else if (fish->psi < tpsi) {
            fish->psi += 0.5;
            if (fish->psi > 180.0) {
                fish->psi -= 360.0;
            }
        }
    }
	else 
	{
        if (rand() % 50 > 46) {
            sign = 1 - sign;
		}
        fish->psi += sign;
        if (fish->psi > 180.0) {
            fish->psi -= 360.0;
        }
        if (fish->psi < -180.0) {
            fish->psi += 360.0;
        }
    }

    if (fish->attack) {
        if (fish->v < 1.1) {
            fish->spurt = 1;
        }
        if (fish->spurt) {
            fish->v += 0.15;
        }
        if (fish->v > 5.0) {
            fish->spurt = 0;
        }
        if ((fish->v > 1.0) && (!fish->spurt)) {
            fish->v -= 0.15;
        }
    } else {
        if (!(rand() % 600) && (!fish->spurt)) {
            fish->spurt = 1;
        }
        if (fish->spurt) {
            fish->v += 0.025;
        }
        if (fish->v > 3.0) {
            fish->spurt = 0;
        }
        if ((fish->v > 1.0) && (!fish->spurt)) {
            fish->v -= 0.025;
        }
    }

	if( fabs (X) > RMAX || fabs (Y) > RMAX ) 
		fish->psi+=2.5;
	/*
	if( fabs (Z) > RMAX ) 
		fish->theta-=2.5;
*/
    fish->x +=TANGSPEED  * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
	
    
	fish->y +=TANGSPEED  * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);

   
	fish->z +=TANGSPEED  * fish->v * sin(fish->theta / RAD) ;

}
void 
KumanomiPilot(fishRec * fish)
{
	static int sign = 1;
    float X, Y, Z, tpsi, ttheta, thetal;


    X = fish->xt - fish->x;
    Y = fish->yt - fish->y;
    Z = fish->zt - fish->z;
	if(rand() % 500 > 498){

		fish->zt= 0 -  fish->zt ;

	}  

    ttheta = RAD * atan(Z / (sqrt(X * X + Y * Y)));

    if (ttheta > fish->theta + 0.25) {
        fish->theta += 0.5;
    } else if (ttheta < fish->theta - 0.25) {
        fish->theta -= 0.5;
    }
    if (fish->theta > 90.0) {
        fish->theta = 90.0;
    }
    if (fish->theta < -90.0){
        fish->theta = -90.0;
    }
   

    tpsi = RAD * atan2(Y, X);

    fish->attack = 0;
	

    if (fabs(tpsi - fish->psi) < 10.0) {
        fish->attack = 1;
    } 
	else if (fabs(tpsi - fish->psi) < 45.0) 
	{
        if (fish->psi > tpsi) {
            fish->psi -= 0.5;
            if (fish->psi < -180.0) {
                fish->psi += 360.0;
            }
        } 
		else if (fish->psi < tpsi) {
            fish->psi += 0.5;
            if (fish->psi > 180.0) {
                fish->psi -= 360.0;
            }
        }
    }
	else 
	{
        if (rand() % 50 > 46) {
            sign = 1 - sign;
		}
        fish->psi += sign;
        if (fish->psi > 180.0) {
            fish->psi -= 360.0;
        }
        if (fish->psi < -180.0) {
            fish->psi += 360.0;
        }
    }

    if (fish->attack) {
        if (fish->v < 1.1) {
            fish->spurt = 1;
        }
        if (fish->spurt) {
            fish->v += 0.15;
        }
        if (fish->v > 5.0) {
            fish->spurt = 0;
        }
        if ((fish->v > 1.0) && (!fish->spurt)) {
            fish->v -= 0.15;
        }
    } else {
        if (!(rand() % 600) && (!fish->spurt)) {
            fish->spurt = 1;
        }
        if (fish->spurt) {
            fish->v += 0.025;
        }
        if (fish->v > 3.0) {
            fish->spurt = 0;
        }
        if ((fish->v > 1.0) && (!fish->spurt)) {
            fish->v -= 0.025;
        }
    }

	if( fabs (X) > RMAX || fabs (Y) > RMAX ) 
		fish->psi+=2.5;
	/*
	if( fabs (Z) > RMAX ) 
		fish->theta-=2.5;
*/
    fish->x +=KUMANOSPEED  * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
	
    
	fish->y +=KUMANOSPEED  * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);

   
	fish->z +=KUMANOSPEED  * fish->v * sin(fish->theta / RAD) ;

}
void
SharkPilot(fishRec * fish)
{
    static int sign = 1;
    float X, Y, Z, tpsi, ttheta, thetal, psil;

    fish->xt = 0.0;
    fish->yt = 0.0;
    fish->zt = 0.0;

    X = fish->xt - fish->x;
    Y = fish->yt - fish->y;
    Z = fish->zt - fish->z;

	psil = fish->psi;

    ttheta = RAD * atan(Z / (sqrt(X * X + Y * Y)));

    if (ttheta > fish->theta + 0.25) {
        fish->theta += 0.25;
    } else if (ttheta < fish->theta - 0.25) {
        fish->theta -= 0.25;
    }
    if (fish->theta > 90.0) {
        fish->theta = 90.0;
    }
    if (fish->theta < -90.0) {
        fish->theta = -90.0;
    }
   

    tpsi = RAD * atan2(Y, X);

    fish->attack = 0;

    if (fabs(tpsi - fish->psi) < 10.0) {
        fish->attack = 1;
    } else if (fabs(tpsi - fish->psi) < 45.0) {
        if (fish->psi > tpsi) {
            fish->psi -= 0.5;
            if (fish->psi < -180.0) {
                fish->psi += 360.0;
            }
        } else if (fish->psi < tpsi) {
            fish->psi += 0.5;
            if (fish->psi > 180.0) {
                fish->psi -= 360.0;
            }
        }
    } else {
        if (rand() % 100 > 98) {
            sign = 1 - sign;
        }
        fish->psi += sign;
        if (fish->psi > 180.0) {
            fish->psi -= 360.0;
        }
        if (fish->psi < -180.0) {
            fish->psi += 360.0;
        }
    }
	fish->dpsi = fish->psi - psil;

    if (fish->attack) {
        if (fish->v < 1.1) {
            fish->spurt = 1;
        }
        if (fish->spurt) {
            fish->v += 0.2;
        }
        if (fish->v > 5.0) {
            fish->spurt = 0;
        }
        if ((fish->v > 1.0) && (!fish->spurt)) {
            fish->v -= 0.2;
        }
    } else {
        if (!(rand() % 400) && (!fish->spurt)) {
            fish->spurt = 1;
        }
        if (fish->spurt) {
            fish->v += 0.05;
        }
        if (fish->v > 3.0) {
            fish->spurt = 0;
        }
        if ((fish->v > 1.0) && (!fish->spurt)) {
            fish->v -= 0.05;
        }
    }

	if( fabs (X) > RMAX || fabs (Y) > RMAX ) 
		fish->psi+=2.5;

	  fish->x += SHARKSPEED * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
      fish->y += SHARKSPEED * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);
      fish->z += SHARKSPEED * fish->v * sin(fish->theta / RAD);
}

void
SharkMiss(int i)
{
    int j;
    float avoid, thetal;
    float X, Y, Z, R;

    for (j = 0; j < NUM_SHARKS; j++) {
        if (j != i) {
            X = LeopardShark[j].x - LeopardShark[i].x;
            Y = LeopardShark[j].y - LeopardShark[i].y;
            Z = LeopardShark[j].z - LeopardShark[i].z;

            R = sqrt(X * X + Y * Y + Z * Z);

            avoid = 0.1;
            thetal = LeopardShark[i].theta;

            if (R < SHARKSIZE) {
                if (Z > 0.0) {
                    LeopardShark[i].theta -= avoid;
                } else {
                    LeopardShark[i].theta += avoid;
                }
            }
           
        }
    }
}
