#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>





void quicksort(slope* slopes, int p, int r)
{
    if ( p < r )
    {
        int j = partition(slopes, p, r);
        quicksort(slopes, p, j - 1);
        quicksort(slopes, j + 1, r);
    }
}


int partition(slope* slopes, int p, int r)
{
    float pivot = slopes[r].s;
    
    while ( p < r )
    {
        while (slopes[p].s < pivot)
            p++;
        
        while (slopes[r].s > pivot)
            r--;
        
        if (slopes[p].s == slopes[r].s)
        {
            p++;
        }
        else if (p < r)
        {
            slope tmp = slopes[p];
            slopes[p] = slopes[r];
            slopes[r] = tmp;
        }
    }
    
    return r;
}


/* **************************************** */
/* returns the signed area of triangle abc. The area is positive if c
   is to the left of ab, and negative if c is to the right of ab
 */
int signed_area2D(point2D a, point2D b, point2D c) {
  int triArea = (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2;
  return triArea;
}


/* **************************************** */
/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2D p, point2D q, point2D r) {
  //calculate triangular area that the triplets make
  //if the area is 0, then they are collinear
  if (signed_area2D(p, q, r) == 0)
  {
    return 1;
  }
  return 0;
}



/* **************************************** */
/* return 1 if c is  strictly left of ab; 0 otherwise */
int left (point2D a, point2D b, point2D c) {
  
  return 1; 
}

int checkDuplicate(point2D* p, int i, int j, int k)
{
  if (p[i].x == p[j].x && p[i].y == p[j].y)
  {
    return 1;
  }
  else if (p[i].x == p[k].x && p[i].y == p[k].y)
  {
    return 1;
  }
  else if (p[j].x == p[k].x && p[j].y == p[k].y)
  {
    return 1;
  }
  return 0;
}

/* **************************************** */
/* return all triplets of colinear points as an array using the
   straightforward algorithm that runs in cubic time 
*/
void find_collinear_straightforward(point2D* p, int n) {

  assert(p); 
  int ncol=0; //nb distinct  collinear triplets
  //triplet* allTriplets = 
  //for each triplet
  for (int i = 0; i < n; ++i)
  {
    for (int j = i + 1; j < n; ++j)
    {
      for (int k = j + 1; k < n; ++k)
      {
        //test to see if they are collinear
        if(collinear(p[i], p[j], p[k]))
        {
          if (!checkDuplicate(p, i, j, k))
          {
            printf("Collinear triplet: (%d, %d) (%d, %d) (%d, %d)\n", p[i].x, p[i].y, p[j].x, p[j].y, p[k].x, p[k].y);
              ncol++;
          }
        }
      }
    }
  }


  printf("find_collinear_straightforward: total %d distinct collinear triplets (out of max %ld triplets)\n", ncol, (long int) ((long int)n*(long int)(n-1)*(long int)(n-2))/6);
  fflush(stdout); 
}




int vertical (point2D a, point2D b) {

  if (a.x == b.x) return 1; 
  return 0; 
}

float calcSlope (point2D left, point2D right)
{
  return (float)(right.y - left.y) / (right.x - left.x);
}

void insertSlope(slope* slopes, float s, int index, int leftPoint, int rightPoint)
{
  slopes[index].s = s;
  slopes[index].leftPoint = leftPoint;
  slopes[index].rightPoint = rightPoint;
}


void computeSlopes(point2D* p, slope* slopes, int i, int n)
{
  int index = 0;
  for (int j = (i+1); j < n; j++)
  {
      if (p[i].x == 276 || p[i].x == 276)
      {
        printf("%d,%d\n", p[i].x, p[j].x);
      }
  //always want to calculate slope from left to right on the x-axis
      //to keep consistency
      int rightPoint, leftPoint;

      if (p[i].x > p[j].x)
      {
        rightPoint = i;
        leftPoint = j;
      }
      else {
        rightPoint = j;
        leftPoint = i;
      }

      //check if slope is vertical
      if (vertical(p[i], p[j]))
      {
        insertSlope(slopes, FLT_MAX, index, leftPoint, rightPoint);
      }
      else 
      {
        insertSlope(slopes, calcSlope(p[leftPoint], p[rightPoint]), index, leftPoint, rightPoint);
      }

      index++;
  }
}

void checkForCollinearity(point2D* p, slope s1, slope s2, int* ncol)
{
  if (p[s1.rightPoint].x == 341 || p[s1.leftPoint].x == 341)
  {
    printf("%d,%d   %d,%d\n", p[s1.rightPoint].x, p[s1.rightPoint].y, p[s1.leftPoint].x, p[s1.leftPoint].y);
  }
  
  if (s1.leftPoint == s2.rightPoint && !(checkDuplicate(p, s1.leftPoint, s1.rightPoint, s2.leftPoint)))
  {
    *ncol += 1;
      printf("Collinear triplet: (%d, %d) (%d, %d) (%d, %d)\n", 
        p[s1.leftPoint].x, p[s1.leftPoint].y, p[s1.rightPoint].x, p[s1.rightPoint].y, 
        p[s2.leftPoint].x, p[s2.leftPoint].y);
      assert(collinear(p[s1.leftPoint], p[s1.rightPoint], p[s2.leftPoint]));
      
    }
  else if (s1.rightPoint == s2.leftPoint && !(checkDuplicate(p, s1.leftPoint, s1.rightPoint, s2.rightPoint)))
  {
    *ncol += 1;
    printf("Collinear triplet: (%d, %d) (%d, %d) (%d, %d)\n", 
      p[s1.leftPoint].x, p[s1.leftPoint].y, p[s1.rightPoint].x, p[s1.rightPoint].y, 
      p[s2.rightPoint].x, p[s2.rightPoint].y);
    assert(collinear(p[s1.rightPoint], p[s1.leftPoint], p[s2.rightPoint]));
  }
  else if (s1.rightPoint == s2.rightPoint && !(checkDuplicate(p, s1.leftPoint, s1.rightPoint, s2.leftPoint)))
  {
    *ncol += 1;
    printf("Collinear triplet: (%d, %d) (%d, %d) (%d, %d)\n", 
      p[s1.leftPoint].x, p[s1.leftPoint].y, p[s1.rightPoint].x, p[s1.rightPoint].y, 
      p[s2.leftPoint].x, p[s2.leftPoint].y);
    assert(collinear(p[s1.rightPoint], p[s1.leftPoint], p[s2.leftPoint]));
  }
  else if (s1.leftPoint == s2.leftPoint && !(checkDuplicate(p, s1.leftPoint, s1.rightPoint, s2.rightPoint)))
  {
    *ncol += 1;
    printf("Collinear triplet: (%d, %d) (%d, %d) (%d, %d)\n", 
      p[s1.leftPoint].x, p[s1.leftPoint].y, p[s1.rightPoint].x, p[s1.rightPoint].y, 
      p[s2.rightPoint].x, p[s2.rightPoint].y);
    assert(collinear(p[s1.rightPoint], p[s1.leftPoint], p[s2.rightPoint]));
  }
}

/* **************************************** */
/* return all triplets of colinear points as an array using the
   improved algorithm that runs in O(n^2 lg n) time
*/
void find_collinear_improved(point2D* p, int n) 
{
  assert(p); 
  int ncol = 0;

  for (int i = 0; i < n; ++i)
  {
    slope* slopes = malloc((n - i) * sizeof(slope));
    assert(slopes);
    //find all slopes stemming from i
    computeSlopes(p, slopes, i, n);
    quicksort(slopes, 0, n - i - 1);
    for (int j = 0; j < (n - i); ++j)
    {
      int idx = j + 1;

      if (idx < (n - i))
      {
        while (slopes[j].s == slopes[idx].s)
        {
          checkForCollinearity(p, slopes[j], slopes[idx], &ncol);
          idx++;
          if (idx == (n - i))
          {
            break;
          }
        }
      }
    } 

  } 

  printf("find_collinear_improved: total %d distinct collinear triplets (out of max %ld triplets)\n", ncol, (long int) ((long int)n*(long int)(n-1)*(long int)(n-2))/6);
  fflush(stdout); 
}



