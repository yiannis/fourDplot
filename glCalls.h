#ifndef GLCALLS_H
#define GLCALLS_H

#include <string>

void initLights();
void initFunc();
void displayFunc();
void idleFuncSpin();
void mouseFuncTimer( int button, int state, int x, int y );
void specialKeysFunc(int key, int x, int y);
void keyboardFunc( unsigned char key, int x, int y );
void dumpFrame( std::string file = "", std::string dir = "" );
void cleanup();

#endif // GLCALLS_H
