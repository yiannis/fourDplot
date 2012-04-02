#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include <GL/glut.h>

#include "Arguments.h"
#include "Surface.h"
#include "Color.h"

#include "glCalls.h"

using namespace std;

extern Arguments args;

// Constants //
static const GLfloat mat_specular[] = { 0.3, 0.3, 0.3, 1.0 };
static const GLfloat mat_shininess[] = { 20.0 };
//static const GLfloat light_position[] = { 1, 1, 1, 0 };
static const GLfloat light_position[] = { 0.5, 0.5, 1, 0 };
static const GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };

extern const unsigned int FPS[] = { 5, 10, 15, 30, 50, 60, 75, 100, 120, 150, 200 };
// Constants //


// Globals (sorry, but this is just a glut demo!)
static float spinz = -45, spinx = -35, spiny = 0, spin_step = 1, spin_dir = 0;
static Color bgColor = LIGHT_BLUE; ///< The background color
static int win_width,  ///< The current window width
           win_height; ///< The current window height
static bool videoOn = false; ///< If playing video
static float L,   ///< 
             Ox,  ///<
             Oy,  ///<
             Oz;  ///<
static unsigned int videoDelay = 30; ///< in milliseconds

int fpsIndex = 3; ///< Default is 30fps
int fpsValues = (sizeof FPS)/sizeof(unsigned int);
// Globals //


void initLights()
{
  if (!args.lights)
    return;

  clog << "Init lights" << endl;
	glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
	glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );
	glMaterialfv( GL_FRONT, GL_DIFFUSE , white_light );
	if (args.falseColors || args.imageColors) {
		clog << "Enable color material" << endl;
		glEnable( GL_COLOR_MATERIAL );
		//glColorMaterial( GL_FRONT_AND_BACK, GL_DIFFUSE );
		//glColorMaterial( GL_FRONT_AND_BACK, GL_SPECULAR );
	}
	glLightfv( GL_LIGHT0, GL_POSITION, light_position );
	glLightfv( GL_LIGHT0, GL_DIFFUSE , white_light );
	glLightfv( GL_LIGHT0, GL_SPECULAR, white_light );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
}

void setProjection( float mult )
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	L = Surface::getCurrentSurface()->Lmax(); L *= mult;
  Ox = Surface::getCurrentSurface()->Ox();
  Oy = Surface::getCurrentSurface()->Oy();
  Oz = Surface::getCurrentSurface()->Oz();
	glOrtho( Ox-L, Ox+L, Oy-L, Oy+L, Oz-L, Oz+L );
	glMatrixMode( GL_MODELVIEW );
}

void initFunc()
{
	if (args.drawMode == GL_LINE || args.drawMode == GL_POINT)
		bgColor = BLACK;
	glClearColor( bgColor.r(), bgColor.g(), bgColor.b(), bgColor.a() );

  setProjection(1.0F);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	//gluLookAt( 5, 5, 5, 0, 0, 0, -125, -125, 250 );
	//xoffset = (surfaces[0]->xMin()+surfaces[0]->xMax()) / -2.0; == -Ox
	//yoffset = (surfaces[0]->yMin()+surfaces[0]->yMax()) / -2.0; == -Oy
	//cerr<<"move [x,y,z]="<<"["<<xoffset<<","<<yoffset<<",0]"<<endl;

	glEnable( GL_DEPTH_TEST );
}

void displayFunc()
{
	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

	glPushMatrix();
		if (args.axis) {
			glBegin(GL_LINES);
				//Draw light path
				glColor3fv( WHITE() );
				glVertex3f(0,0,0);
				glVertex3f(light_position[0]*L,
                   light_position[1]*L,
                   light_position[2]*L);
			glEnd();
		}
		glRotatef( spinx, 1.0, 0.0, 0.0 );
		glRotatef( spiny, 0.0, 1.0, 0.0 );
		glRotatef( spinz, 0.0, 0.0, 1.0 );

		if (args.axis) {
			glBegin(GL_LINES);
				//Draw xyz axis
				glColor3fv( RED() );
				glVertex3f(0,0,0); //x-axis
				glVertex3f(Ox+L,0,0);
				glColor3fv( GREEN() );
				glVertex3f(0,0,0); //y-axis
				glVertex3f(0,Oy+L,0);
				glColor3fv( BLUE() );
				glVertex3f(0,0,0); //z-axis
				glVertex3f(0,0,Oz+L);
			glEnd();
		}
		glTranslatef( -Ox, -Oy, -Oz );
		glColor3fv( GREEN() );
    Surface::drawCurrent();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

#if 0
void idleFuncSave()
{
	if (image_id < image_nr-1) {
		dumpFrame( "", save_dir );
		if (verbose) cerr<<"idleFuncSave() -> ["<<image_id<<"]"<<endl;
		if (!surfaceCacheOn) {
			delete surfaces[image_id];
			surfaces[image_id] = NULL;
		}
		image_id++;
		if (surfaces[image_id] == NULL) {
			surfaces[image_id] = new surface( images[image_id], image_colors/*, resample */);
			if (false_colors) surfaces[image_id]->create_false_colors( colors );
			if (lights) surfaces[image_id]->create_normals();
		}
		glutPostRedisplay();
	} else {
		if (verbose) cerr<<"idleFuncSave() -> [NULL]"<<endl;
		glutIdleFunc( NULL ); }
}
#endif

void videoTimer(int value)
{
  if (!videoOn)
    return;

  if (Surface::next()) {
    glutPostRedisplay();
    glutTimerFunc(videoDelay, videoTimer, 0); // next videoTimer call milliseconds later
  } else {
		videoOn = false;
  }
}

void idleFuncSpin()
{
	spinz += spin_dir*spin_step;
	glutPostRedisplay();
}

void mouseFunc( int button, int state, int x, int y )
{
	if ( state == GLUT_DOWN )
		switch (button) {
			case GLUT_LEFT_BUTTON:
				spin_dir=1;
				glutIdleFunc( idleFuncSpin );
				break;
			case GLUT_RIGHT_BUTTON:
				spin_dir=-1;
				glutIdleFunc( idleFuncSpin );
				break;
		}
	else
		glutIdleFunc( NULL );
}

void specialKeysFunc(int key, int x, int y)
{
	switch (key) {
    case GLUT_KEY_LEFT:
			spiny-=5;
			glutPostRedisplay();
			break;
    case GLUT_KEY_RIGHT:
			spiny+=5;
			glutPostRedisplay();
			break;
    case GLUT_KEY_DOWN:
			spinx+=5;
			glutPostRedisplay();
			break;
    case GLUT_KEY_UP:
			spinx-=5;
			glutPostRedisplay();
			break;
		case GLUT_KEY_F11:
			glutFullScreen();
			break;
  }
}

void keyboardFunc( unsigned char key, int x, int y )
{
  static float zoom = 1.0F;

	switch (key) {
		case '4':
			spiny-=5;
			glutPostRedisplay();
			break;
		case '6':
			spiny+=5;
			glutPostRedisplay();
			break;
		case '2':
			spinx+=5;
			glutPostRedisplay();
			break;
		case '8':
			spinx-=5;
			glutPostRedisplay();
			break;
		case '9':
			spinz+=spin_step;
			glutPostRedisplay();
			break;
		case '7':
			spinz-=spin_step;
			glutPostRedisplay();
			break;
		case 'h': // Go to home position
			spinz = -45;
			spinx = -35;
			spiny = 0;
			glutPostRedisplay();
			break;
		case 'b': // GoTo beginning - first frame
      Surface::first();
      glutPostRedisplay();
			break;
#if 0
		case 'a': // Save all image frames
			glutIdleFunc( idleFuncSave );
			break;
#endif
		case 'd': // Save current image frame
			dumpFrame( "", args.saveDir );
			break;
		case 'f':
			spin_step *= 2;
			glutPostRedisplay();
			break;
		case 's':
			spin_step /= 2;
			glutPostRedisplay();
			break;
		case 'n': //render next image
      Surface::next();
      glutPostRedisplay();
			break;
		case 'p': //render previous image
      Surface::previous();
      glutPostRedisplay();
			break;
		case 'v': //Toggle video on/off
			if (videoOn) {
				clog << "Turning video off" << endl;
				videoOn = false;
      } else {
				clog << "Turning video on" << endl;
				videoOn = true;
        glutTimerFunc(0, videoTimer, 0);
      }
			break;
		case '+':
      if (fpsIndex < fpsValues-1) {
        fpsIndex++;
        videoDelay = 1000 / FPS[fpsIndex];
      }
			break;
		case '-':
      if (fpsIndex > 0) {
        fpsIndex--;
        videoDelay = 1000 / FPS[fpsIndex];
      }
			break;
    case 'z':
      //TODO
      zoom *= 0.8F;
      glutPostRedisplay();
      break;
    case 'x':
      //TODO
      zoom *= 1.2F;
      glutPostRedisplay();
      break;
		case 'q':
      cleanup();
			exit(0); //OMG! No cleanup at all...
			break;
		default:
			cerr<<"Unknown key!"<<endl;
			int err=system( "cat menu.txt" );
			break;
	}
}

void dumpFrame( string file, string dir )
{
  static int frame_id = 0; //Number of dumped frame
	int size = win_width*win_height*3;
	float* buf = new float[size];

	glReadPixels( 0, 0, win_width, win_height, GL_RGB, GL_FLOAT, (GLvoid*)buf );

	Magick::Image frame;
	frame.quality(100);
	frame.colorSpace(Magick::RGBColorspace);
	frame.read( win_width, win_height, "RGB", Magick::FloatPixel, buf );
	delete[] buf;

	frame.flip();
	if (dir == "") dir = ".";
	if (file == "") {
		char fileName[128];
		sprintf( fileName, "%s/frame_%04d.jpg", dir.c_str(), frame_id );
		frame.write( fileName );
		frame_id++;
	} else
		frame.write( dir+"/"+file );
}

void cleanup() //TODO
{
  Surface::deleteSurfaceCache();
}
