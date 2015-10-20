#ifndef __geom_h
#define __geom_h

#include <float.h>
#include <stdbool.h>


typedef struct _point2d {
  int x,y; 
} point2D;

typedef struct _triplet {
	point2D one, two, three;
} triplet;

typedef struct _slope {
	float s;
	int leftPoint;
	int rightPoint;
} slope;



void quicksort(slope* slopes, int p, int r);
int partition(slope* slopes, int p, int r);
int checkDuplicate(point2D* p, int i, int j, int k);
void insertSlope(slope* slopes, float s, int index, int leftPoint, int rightPoint);
void computeSlopes(point2D* p, slope* slopes, int i, int n);
void checkForCollinearity(point2D* p, slope s1, slope s2, int* ncol);

/* returns the signed area of triangle abc. The area is positive if c
   is to the left of ab, and negative if c is to the right of ab
 */
int signed_area2D(point2D a, point2D b, point2D c); 


/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2D p, point2D q, point2D r);


/* return 1 if c is  strictly left of ab; 0 otherwise */
int left (point2D a, point2D b, point2D c); 


/* return all triplets of colinear points as an array using the
   straightforward algorithm that runs in cubic time 
*/
void find_collinear_straightforward(point2D* p, int n);



/* return all triplets of colinear points as an array using the
   an improved algorithm 
*/
void find_collinear_improved(point2D* p, int n);


#endif


