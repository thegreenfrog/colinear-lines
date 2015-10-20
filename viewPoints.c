/* view1.c 

Laura Toma

What it does:  draws a set of points in the default 2D projection and with no transformations 

*/

#include "geom.h"
#include "rtimer.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


GLfloat red[3] = {1.0, 0.0, 0.0};
GLfloat green[3] = {0.0, 1.0, 0.0};
GLfloat blue[3] = {0.0, 0.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat gray[3] = {0.5, 0.5, 0.5};
GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat magenta[3] = {1.0, 0.0, 1.0};
GLfloat cyan[3] = {0.0, 1.0, 1.0};

GLint fillmode = 0;



/* forward declarations of functions */
void display(void);
void keypress(unsigned char key, int x, int y);
void main_menu(int value);



/* global variables */
const int WINDOWSIZE = 500; 
point2D*  points;
int n;  



/* ****************************** */
/* initialize  the array of points stored in global variable points[] with random points */
void initialize_points_random() {
  //points must be allocated 
  assert(points); 
  
  int i; 
  for (i=0; i<n; i++) {
    points[i].x = random() % WINDOWSIZE; 
    points[i].y = random() % WINDOWSIZE;
  }
}

/* ****************************** */
/* print the array of points stored in global variable points[]*/
void print_points() {
  assert(points); 
  int i; 
  printf("points: ");
  for (i=0; i<n; i++) {
    printf("[%3d,%3d] ", points[i].x, points[i].y);
  }
  printf("\n");
  fflush(stdout);  //flush stdout, weird sync happens when using gl thread
}


/* ****************************** */
int main(int argc, char** argv) {

  //read number of points from user
  if (argc!=2) {
    printf("usage: viewPoints <nbPoints>\n");
    exit(1); 
  }
  n = atoi(argv[1]); 
  printf("you entered n=%d\n", n);
  assert(n >0); 

  //allocate global arrays of n points 
  points = (point2D*)malloc(n*sizeof(point2D));
  assert(points); 
  initialize_points_random();
  //print_points();

  Rtimer rt1; 
  rt_start(rt1); 
  find_collinear_straightforward(points,n); 
  rt_stop(rt1); 
  char buf [1024]; 
  rt_sprint(buf,rt1);
  printf("finding all triplets of collinear points, n=%d, straightforward alg:  %s\n\n", n, buf);
  fflush(stdout); 

  Rtimer rt2; 
  rt_start(rt2); 
  find_collinear_improved(points,n); 
  rt_stop(rt2); 
  rt_sprint(buf,rt2);
  printf("finding all triplets of collinear points, n=%d, improved algo:  %s\n\n", n, buf);
  fflush(stdout); 
  

  /* initialize GLUT  */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(WINDOWSIZE, WINDOWSIZE);
  glutInitWindowPosition(100,100);
  glutCreateWindow(argv[0]);

  /* register callback functions */
  glutDisplayFunc(display); 
  glutKeyboardFunc(keypress);

  /* init GL */
  /* set background color black*/
  glClearColor(0, 0, 0, 0);   
  /* here we can enable depth testing and double buffering and so
     on */

  
  /* give control to event handler */
  glutMainLoop();
  return 0;
}


/* ****************************** */
/* draw the array of points stored in global variable points[] 
   each point is drawn as a small square 
  
*/
void draw_points(){

  const int R= 1;
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //set color 
  glColor3fv(yellow);   
  
  assert(points);
  int i;
  for (i=0; i<n; i++) {
    //draw a small square centered at (points[i].x, points[i].y)
    glBegin(GL_POLYGON);
    glVertex2f(points[i].x -R,points[i].y-R);
    glVertex2f(points[i].x +R,points[i].y-R);
    glVertex2f(points[i].x +R,points[i].y+R);
    glVertex2f(points[i].x -R,points[i].y+R);
    glEnd();
  }

}



/* ****************************** */
void display(void) {

  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity(); //clear the matrix


  /* the default GL window is [-1,1]x[-1,1] with the origin in the
     center the points are in the range (0,0) to (WINSIZE,WINSIZE), so
     they need to be mapped to [-1,1]x [-1,1] */
  glScalef(2.0/WINDOWSIZE, 2.0/WINDOWSIZE, 1.0);  
  glTranslatef(-WINDOWSIZE/2, -WINDOWSIZE/2, 0); 
  draw_points();

  /* execute the drawing commands */
  glFlush();
}



/* ****************************** */
void keypress(unsigned char key, int x, int y) {
  switch(key) {
  case 'q':
    exit(0);
    break;
  } 
}


/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
     
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   gluOrtho2D(0.0, (GLdouble) width, 0.0, (GLdouble) height); 
}


