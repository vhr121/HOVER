#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <GL/glut.h>

#define BUFSIZE 512

#define TORUS		1
#define TETRAHEDRON	2
#define ICOSAHEDRON	3
#define SPHERE      4


enum {
  LIGHT_OFF, LIGHT_RED, LIGHT_WHITE, LIGHT_GREEN
} LightValues;

GLfloat red_light[] =
{1.0, 0.0, 0.0, 1.0}, green_light[] =
{0.0, 1.0, 0.0, 1.0}, white_light[] =
{1.0, 1.0, 1.0, 1.0};
GLfloat left_light_position[] =
{-1.0, 0.0, 1.0, 0.0}, right_light_position[] =
{1.0, 0.0, 1.0, 0.0};
GLuint selectBuf[BUFSIZE];

int W = 500, H = 500;
GLfloat x, y;
int locating = 0;
int theObject = 0;
int menu_inuse = 0;
int mouse_state = 0;

char *objectNames[] =
{"Nothing", "Torus", "Tetrahedron", "Icosahedron","sphere"};


int shade_model = GL_SMOOTH;
char *left_light, *right_light;
char *ico_material, *sphere_material, *torus_material;

void output(GLfloat x, GLfloat y, char *format,...)
{
  va_list args;
  char buffer[200], *p;

  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  glPushMatrix();
  glTranslatef(x, y, 0);
  for (p = buffer; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  glPopMatrix();
}

/* Initialize material property and light source. */
void myinit(void)
{
  GLfloat light_ambient[] =
  {0.2, 0.2, 0.2, 1.0};
  GLfloat light_diffuse[] =
  {1.0, 1.0, 1.0, 1.0};
  GLfloat light_specular[] =
  {1.0, 1.0, 1.0, 1.0};
  GLfloat light_position[] =
  {1.0, 1.0, 1.0, 0.0};

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glEnable(GL_LIGHT0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  
  
  glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);
  glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glLineWidth(1.0);
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* degrees field of view */ 50.0,
    /* aspect ratio */ 1.0, /* Z near */ 1.0, /* Z far */ 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */
  glTranslatef(0.0, 0.0, -1.0);
  
  
  
  
  

  glSelectBuffer(BUFSIZE, selectBuf);

  glNewList(TORUS, GL_COMPILE);
  glutSolidTorus(0.275, 0.85, 10, 15);
  glEndList();
  glNewList(TETRAHEDRON, GL_COMPILE);
  glutSolidTetrahedron();
  glEndList();
  glNewList(ICOSAHEDRON, GL_COMPILE);
  glutSolidIcosahedron();
  glEndList();
 
  glNewList(SPHERE,GL_COMPILE);
  glutSolidSphere(0.75,25,25);
  glEndList();
}

void highlightBegin(void)
{
  static GLfloat blue[4] =
  {0,1,1,1};

  glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
  glColor3f(0,0,1);
}

void highlightEnd(void)
{
  glPopAttrib();
}

void draw(void)
{
  glPushMatrix();
  glScalef(1.3, 1.3, 1.3);
  glRotatef(20.0, 1.0, 0.0, 0.0);

  glLoadName(2);
  glPushMatrix();
  if (theObject == 2)
    highlightBegin();
  glTranslatef(-0.75, -0.75, 0.0);
  glRotatef(79.0, 1.0, 1.0, 0.0);
  glCallList(TETRAHEDRON);
  if (theObject == 2)
    highlightEnd();
  glPopMatrix();

  glLoadName(1);
  glPushMatrix();
  if (theObject == 1)
    highlightBegin();
  glTranslatef(-0.75, 0.5, 0.0);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  glCallList(TORUS);
  if (theObject == 1)
    highlightEnd();
  glPopMatrix();

  glLoadName(3);
  glPushMatrix();
  if (theObject == 3)
    highlightBegin();
  glTranslatef(0.75, 0.0, -1.0);
  glCallList(ICOSAHEDRON);
  if (theObject == 3)
    highlightEnd();
  glPopMatrix();
  
  glLoadName(4);
  glPushMatrix();
  if (theObject == 4)
    highlightBegin();
  glTranslatef(1, -1.85, -1.5);
  glCallList(SPHERE);
  if (theObject == 4)
    highlightEnd();
  glPopMatrix();


  glPopMatrix();
  
  glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 3000, 0, 3000);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
  glutSwapBuffers();
}

void myortho(void)
{
  if (W <= H)
    glOrtho(-2.5, 2.5, -2.5 * (GLfloat) H / (GLfloat) W,
      2.5 * (GLfloat) H / (GLfloat) W, -10.0, 10.0);
  else
    glOrtho(-2.5 * (GLfloat) W / (GLfloat) H,
      2.5 * (GLfloat) W / (GLfloat) H, -2.5, 2.5, -10.0, 10.0);
}

/*  processHits() prints out the contents of the 
 *  selection array.
 */
void processHits(GLint hits, GLuint buffer[])
{
  GLuint depth = ~0;
  unsigned int i, getThisName;
  GLuint names, *ptr;
  GLuint newObject;

  ptr = (GLuint *) buffer;
  newObject = 0;
  for (i = 0; i < hits; i++) {  /* for each hit  */
    getThisName = 0;
    names = *ptr;
    ptr++;              /* skip # name */
    if (*ptr <= depth) {
      depth = *ptr;
      getThisName = 1;
    }
    ptr++;              /* skip z1 */
    if (*ptr <= depth) {
      depth = *ptr;
      getThisName = 1;
    }
    ptr++;              /* skip z2 */

    if (getThisName)
      newObject = *ptr;
    ptr += names;       /* skip the names list */
  }
  if (theObject != newObject) {
    theObject = newObject;
    glutPostRedisplay();
  }
}

/* ARGSUSED */
void locate(int value)
{
  GLint viewport[4];
  GLint hits;

  if (locating) {
    if (mouse_state == GLUT_ENTERED) {
      (void) glRenderMode(GL_SELECT);
      glInitNames();
      glPushName(-1);

      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      viewport[0] = 0;
      viewport[1] = 0;
      viewport[2] = W;
      viewport[3] = H;
      gluPickMatrix(x, H - y, 5.0, 5.0, viewport);
      myortho();
      glMatrixMode(GL_MODELVIEW);
      draw();
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      hits = glRenderMode(GL_RENDER);
    } else {
      hits = 0;
    }
    processHits(hits, selectBuf);
  }
  locating = 0;
}

void passive(int newx, int newy)
{
  x = newx;
  y = newy;
  if (!locating) {
    locating = 1;
    glutTimerFunc(1, locate, 0);
  }
}

void entry(int state)
{
  mouse_state = state;
  if (!menu_inuse) {
    if (state == GLUT_LEFT) {
      if (theObject != 0) {
        theObject = 0;
        glutPostRedisplay();
      }
    }
  }
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  draw();

  glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_LINE_SMOOTH);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 3000, 0, 3000);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  output(80, 2800, "Automatically names object under mouse");
  output(80, 100, "Located: %s.", objectNames[theObject]);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopAttrib();

  glutSwapBuffers();
}

void myReshape(int w, int h)
{
  W = w;
  H = h;
  glViewport(0, 0, W, H);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  myortho();
  glMatrixMode(GL_MODELVIEW);
}

void polygon_mode(int value)
{
  switch (value) {
  case 1:
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
  case 2:
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    break;
    
   case 3:
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(2);
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    break;
    
  }
  glutPostRedisplay();
}

void mstatus(int status, int newx, int newy)
{
  if (status == GLUT_MENU_NOT_IN_USE) {
    menu_inuse = 0;
    passive(newx, newy);
  } else {
    menu_inuse = 1;
  }
}


void light_select(GLenum which, int value, char **label)
{
  glEnable(which);
  switch (value) {
  case LIGHT_OFF:
    *label = "off";
    glDisable(which);
    break;
  case LIGHT_RED:
    *label = "red";
    glLightfv(which, GL_DIFFUSE, red_light);
    break;
  case LIGHT_WHITE:
    *label = "white";
    glLightfv(which, GL_DIFFUSE, white_light);
    break;
  case LIGHT_GREEN:
    *label = "green";
    glLightfv(which, GL_DIFFUSE, green_light);
    break;
  }
  glutPostRedisplay();
}

void left_light_select(int value)
{
  light_select(GL_LIGHT0, value, &left_light);
}

void right_light_select(int value)
{
  light_select(GL_LIGHT1, value, &right_light);
}

void material(int dlist, GLfloat * ambient, GLfloat * diffuse,
  GLfloat * specular, GLfloat shininess)
{
  glNewList(dlist, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
  glEndList();
}



void main_menu(int value)
{
  if (value == 666)
    exit(0);
  glShadeModel(shade_model = value);
  glutPostRedisplay();
}


int main(int argc, char **argv)
{
  int submenu;
  int left_light_m, right_light_m, torus_m, sphere_m, ico_m;


  glutInit(&argc, argv);
  glutInitWindowSize(W, H);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  
  #define LIGHT_MENU_ENTRIES() \
    glutAddMenuEntry("Disable", LIGHT_OFF); \
    glutAddMenuEntry("Red", LIGHT_RED); \
    glutAddMenuEntry("White", LIGHT_WHITE); \
    glutAddMenuEntry("Green", LIGHT_GREEN);
  
  
  glutCreateWindow("hover");
  myinit();
  glutReshapeFunc(myReshape);
  glutDisplayFunc(display);
  
  left_light_m = glutCreateMenu(left_light_select);
  LIGHT_MENU_ENTRIES();
  right_light_m = glutCreateMenu(right_light_select);
  LIGHT_MENU_ENTRIES();
    
  
  submenu = glutCreateMenu(polygon_mode);
  glutAddMenuEntry("Filled", 1);
  glutAddMenuEntry("Outline", 2);
  glutAddMenuEntry("Points",3);
  
  
  
  glutCreateMenu(main_menu);
  glutAddMenuEntry("Quit", 666);
  glutAddSubMenu("Polygon mode", submenu);
  
  glutAddMenuEntry("Smooth shading", GL_SMOOTH);
  glutAddMenuEntry("Flat shading", GL_FLAT);
  glutAddSubMenu("Left light", left_light_m);
  glutAddSubMenu("Right light", right_light_m);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  
  
  glutPassiveMotionFunc(passive);
  glutEntryFunc(entry);
  glutMenuStatusFunc(mstatus);
  glutMainLoop();
  return 0;             
}
