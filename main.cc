#include "Arguments.h"
#include "Surface.h"
#include "glCalls.h"

extern unsigned int FPS[];
extern int fpsIndex;

// Globals
static const int WIN_WIDTH = 800, WIN_HEIGHT = 600;

Arguments args;

int main(int argc, char** argv)
{
  args.parse(argc, argv);

  if (args.help) {
    args.printHelp();
    return 1;
  } else {
    args.printAllValues();
  }

  Surface::initSurfaceCache( args, FPS[fpsIndex] );

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
  glutInitWindowSize( WIN_WIDTH, WIN_HEIGHT );
  glutCreateWindow( argv[0] );

  try {
    initLights();
    initFunc();
    glutMouseFunc( mouseFunc );
    glutKeyboardFunc( keyboardFunc );
    glutSpecialFunc( specialKeysFunc );
    glutDisplayFunc( displayFunc );
    glutMainLoop();
  } catch (std::exception &e) {
    std::cerr << argv[0] << ": " << e.what() << "\n";
    return 1;
  }

  return 0;
}
