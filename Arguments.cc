#include <algorithm>
#include <exception>
#include <iostream>

#include <GL/glut.h>

#include "Arguments.h"

using namespace std;

Arguments::Arguments() :
  m_optsDesc("Allowed options"),
  points(100), duration(0), resample(1.0F),
  xmin(-5.0F), xmax(5.0F), ymin(-5.0F), ymax(5.0F),
  lights(false), axis(false), falseColors(false), imageColors(false),
  renderVideo(false), verbose(false), loop(false), help(false),
  drawModeStr("lines"), saveDir(".")

{
  m_optsDesc.add_options()
    ("help", "produce help message")
    ("verbose", "Print extra info")
    ("axis,a", "Show xyz-axis and light path")
    ("lights,l", "Enable lighting")
    ("loop", "Loop video")
    ("video,v", "Render as video [used with multiple --images OR if --function is f(x,y,t)]")
    ("save-dir,s", po::value<string>(&saveDir), "Directory where dumped frames are saved")
    ("draw-mode,d", po::value<string>(&drawModeStr), "Select between points, lines, or filled polygons")
    ("false-colors", "Use a false color scale")
    ("points,p", po::value<int>(&points), "Total number of points(vertices)\n")
    ("images,i", po::value< vector<string> >(&images), "Use image file(s) from command line")
    ("resample,r", po::value<float>(&resample), "Scale image(s) before rendering [used with --images]")
    ("image-colors", "Use original image colors [used with --images]\n")
    ("function,f", po::value<string>(&functionCode), "Supply a (one line) function body f(x,y,t)")
    ("xmin", po::value<float>(&xmin), "Minimum x value [used with --function]")
    ("xmax", po::value<float>(&xmax), "Maximum x value [used with --function]")
    ("ymin", po::value<float>(&ymin), "Minimum y value [used with --function]")
    ("ymax", po::value<float>(&ymax), "Maximum y value [used with --function]")
    ("duration", po::value<int>(&duration), "Duration in seconds [used with '--function --video']");
}

void Arguments::parse(int argc, char** argv)
{
  po::positional_options_description p;
  p.add("images", -1);
  po::store(po::command_line_parser(argc, argv).options(m_optsDesc).positional(p).run(), m_varMap);
  po::notify(m_varMap);    

  if (m_varMap.count("images") && m_varMap.count("function")) {
    cerr << "\nYou can only use one of --images or --function" << endl;
    help = true;
  }
  if (!(m_varMap.count("images") || m_varMap.count("function"))) {
    cerr << "\nYou have to use one of --images or --function, but not both" << endl;
    help = true;
  }
  if (images.size() == 1 && m_varMap.count("video")) {
    cerr << "\nYou can't use --video with only one image." << endl;
    help = true;
  }
  if (m_varMap.count("function") && m_varMap.count("video") && !m_varMap.count("duration")) {
    cerr << "\nYou have to specify --duration with --video" << endl;
    help = true;
    // -f -v      : Without -d, this is an error
    // -f         : Render function at 't = 0'              | -d 0 -v false
    // -f -d T    : Render function for t=[0,T], no video   | -d T -v false
    // -f -d T -v : Render function for t=[0,T], with video | -d T -v true
  }
  if (m_varMap.count("false-colors") && m_varMap.count("image-colors")) {
    cerr << "\nYou can only use one of --false-colors or --image-colors" << endl;
    help = true;
  }

  if (drawModeStr == "point")
    drawMode = GL_POINT;
  else if (drawModeStr == "line")
    drawMode = GL_LINE;
  else if (drawModeStr == "fill")
    drawMode = GL_FILL;
  else {
    cerr << "\nDefinition of --draw-mode as point|line|fill is needed" << endl;
    help = true;
  }

  if (m_varMap.count("function"))
    mode = PLOT;
  else
    mode = IMAGE;

  if (m_varMap.count("help"))         help        = true;
  if (m_varMap.count("lights"))       lights      = true;
  if (m_varMap.count("axis"))         axis        = true;
  if (m_varMap.count("video"))        renderVideo = true;
  if (m_varMap.count("verbose"))      verbose     = true;
  if (m_varMap.count("loop"))         loop        = true;
  if (m_varMap.count("false-colors")) falseColors = true;
  if (m_varMap.count("image-colors")) imageColors = true;
}

void Arguments::printHelp() const
{
  cout << "\n" << m_optsDesc << endl;
}

void Arguments::printAllValues() const
{
  int size = min( (int)images.size(), 3 );

  cout << "lights\t\t= "      << string(lights?"yes":"no") << endl;
  cout << "axis\t\t= "        << string(axis?"yes":"no") << endl;
  cout << "video\t\t= "       << string(renderVideo?"yes":"no") << endl;
  cout << "verbose\t\t= "     << string(verbose?"yes":"no") << endl;
  cout << "loop\t\t= "        << string(loop?"yes":"no") << endl;
  cout << "save-dir\t= "      << saveDir << endl;
  cout << "draw-mode\t= "     << drawModeStr << endl;
  cout << "false-colors\t= "  << string(falseColors?"yes":"no") << endl;

  switch (mode) {
    case IMAGE:
      cout << "resample\t= "      << resample << endl;
      cout << "image-colors\t= "  << string(imageColors?"yes":"no") << endl;
      cout << "images\t\t= ";
      for (int i=0; i<size; i++)
        cout << images[i] << " ";
      if (images.size() > 3)
        cout << "..." << endl;
      else
        cout << endl;
      break;
    case PLOT:
      cout << "functionCode\t= "  << functionCode << endl;
      cout << "points\t\t= "      << points << endl;
      cout << "duration\t= "      << duration << endl;
      cout << "x:["<<xmin<<","<<xmax<<"] y:["<<ymin<<","<<ymax<<"]" << endl;
      break;
  }
}

void Arguments::printGivenValues() const
{
  if (m_varMap.count("resample")) cout << "resample = " << m_varMap["resample"].as<float>() << endl;
  if (m_varMap.count("lights")) cout << "lights = " << string(lights?"yes":"no") << endl;
  if (m_varMap.count("axis")) cout << "axis = " << string(axis?"yes":"no") << endl;
  // ...
  //Unfinished
}

