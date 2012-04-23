#include "Lexer.h"
#include "Parser.h"

#include "Surface.h"

using namespace std;

//////// Surface { //////// 
Surface::~Surface()
{
	delete[] m_vertices;
	delete[] m_normals;
	delete[] m_colors;
}

void Surface::createNormals()
{ //FIX ME
	if (m_normals == NULL)
		m_normals = new float[3*m_points];
	ArrayPoint3D<float> N( m_normals, m_xPoints, m_yPoints, 3 );
	ArrayPoint3D<float> V( m_vertices, m_xPoints, m_yPoints, 3 );

	for (int i=1; i<m_yPoints-1; i++)
		for (int j=1; j<m_xPoints-1; j++) {
			N.x(i,j) =	(V.y(i-1,j)-V.y(i,j-1))*(V.z(i,j)-V.z(i,j-1))-(V.y(i,j)-V.y(i,j-1))*(V.z(i-1,j)-V.z(i,j-1))+
							(V.y(i-1,j)-V.y(i,j))*(V.z(i,j+1)-V.z(i,j))-(V.y(i,j+1)-V.y(i,j))*(V.z(i-1,j)-V.z(i,j))+
							(V.y(i,j)-V.y(i,j-1))*(V.z(i+1,j)-V.z(i,j-1))-(V.y(i+1,j)-V.y(i,j-1))*(V.z(i,j)-V.z(i,j-1))+
							(V.y(i,j+1)-V.y(i,j))*(V.z(i+1,j)-V.z(i,j))-(V.y(i+1,j)-V.y(i,j))*(V.z(i,j+1)-V.z(i,j));
			N.y(i,j) =	(V.x(i,j)-V.x(i,j-1))*(V.z(i-1,j)-V.z(i,j-1))-(V.x(i-1,j)-V.x(i,j-1))*(V.z(i,j)-V.z(i,j-1))+
							(V.x(i,j+1)-V.x(i,j))*(V.z(i-1,j)-V.z(i,j))-(V.x(i-1,j)-V.x(i,j))*(V.z(i,j+1)-V.z(i,j))+
							(V.x(i+1,j)-V.x(i,j-1))*(V.z(i,j)-V.z(i,j-1))-(V.x(i,j)-V.x(i,j-1))*(V.z(i+1,j)-V.z(i,j-1))+
							(V.x(i+1,j)-V.x(i,j))*(V.z(i,j+1)-V.z(i,j))-(V.x(i,j+1)-V.x(i,j))*(V.z(i+1,j)-V.z(i,j));
			N.z(i,j) =	(V.x(i-1,j)-V.x(i,j-1))*(V.y(i,j)-V.y(i,j-1))-(V.x(i,j)-V.x(i,j-1))*(V.y(i-1,j)-V.y(i,j-1))+
							(V.x(i-1,j)-V.x(i,j))*(V.y(i,j+1)-V.y(i,j))-(V.x(i,j+1)-V.x(i,j))*(V.y(i-1,j)-V.y(i,j))+
							(V.x(i,j)-V.x(i,j-1))*(V.y(i+1,j)-V.y(i,j-1))-(V.x(i+1,j)-V.x(i,j-1))*(V.y(i,j)-V.y(i,j-1))+
							(V.x(i,j+1)-V.x(i,j))*(V.y(i+1,j)-V.y(i,j))-(V.x(i+1,j)-V.x(i,j))*(V.y(i,j+1)-V.y(i,j));
		}
	for (int j=1,i=0; j<m_xPoints-1; j++) {//First line
		N.x(i,j) =	(V.y(i,j)-V.y(i,j-1))*(V.z(i+1,j)-V.z(i,j-1))-(V.y(i+1,j)-V.y(i,j-1))*(V.z(i,j)-V.z(i,j-1))+
						(V.y(i,j+1)-V.y(i,j))*(V.z(i+1,j)-V.z(i,j))-(V.y(i+1,j)-V.y(i,j))*(V.z(i,j+1)-V.z(i,j));
		N.y(i,j) =	(V.x(i+1,j)-V.x(i,j-1))*(V.z(i,j)-V.z(i,j-1))-(V.x(i,j)-V.x(i,j-1))*(V.z(i+1,j)-V.z(i,j-1))+
						(V.x(i+1,j)-V.x(i,j))*(V.z(i,j+1)-V.z(i,j))-(V.x(i,j+1)-V.x(i,j))*(V.z(i+1,j)-V.z(i,j));
		N.z(i,j) =	(V.x(i,j)-V.x(i,j-1))*(V.y(i+1,j)-V.y(i,j-1))-(V.x(i+1,j)-V.x(i,j-1))*(V.y(i,j)-V.y(i,j-1))+
						(V.x(i,j+1)-V.x(i,j))*(V.y(i+1,j)-V.y(i,j))-(V.x(i+1,j)-V.x(i,j))*(V.y(i,j+1)-V.y(i,j));
	}
	for (int j=1,i=m_yPoints-1; j<m_xPoints-1; j++) {//Last line
		N.x(i,j) =	(V.y(i-1,j)-V.y(i,j-1))*(V.z(i,j)-V.z(i,j-1))-(V.y(i,j)-V.y(i,j-1))*(V.z(i-1,j)-V.z(i,j-1))+
						(V.y(i-1,j)-V.y(i,j))*(V.z(i,j+1)-V.z(i,j))-(V.y(i,j+1)-V.y(i,j))*(V.z(i-1,j)-V.z(i,j));
		N.y(i,j) =	(V.x(i,j)-V.x(i,j-1))*(V.z(i-1,j)-V.z(i,j-1))-(V.x(i-1,j)-V.x(i,j-1))*(V.z(i,j)-V.z(i,j-1))+
						(V.x(i,j+1)-V.x(i,j))*(V.z(i-1,j)-V.z(i,j))-(V.x(i-1,j)-V.x(i,j))*(V.z(i,j+1)-V.z(i,j));
		N.z(i,j) =	(V.x(i-1,j)-V.x(i,j-1))*(V.y(i,j)-V.y(i,j-1))-(V.x(i,j)-V.x(i,j-1))*(V.y(i-1,j)-V.y(i,j-1))+
						(V.x(i-1,j)-V.x(i,j))*(V.y(i,j+1)-V.y(i,j))-(V.x(i,j+1)-V.x(i,j))*(V.y(i-1,j)-V.y(i,j));
	}
	for (int i=1,j=0; i<m_yPoints-1; i++) {//First column
		N.x(i,j) =	(V.y(i-1,j)-V.y(i,j))*(V.z(i,j+1)-V.z(i,j))-(V.y(i,j+1)-V.y(i,j))*(V.z(i-1,j)-V.z(i,j))+
						(V.y(i,j+1)-V.y(i,j))*(V.z(i+1,j)-V.z(i,j))-(V.y(i+1,j)-V.y(i,j))*(V.z(i,j+1)-V.z(i,j));
		N.y(i,j) =	(V.x(i,j+1)-V.x(i,j))*(V.z(i-1,j)-V.z(i,j))-(V.x(i-1,j)-V.x(i,j))*(V.z(i,j+1)-V.z(i,j))+
						(V.x(i+1,j)-V.x(i,j))*(V.z(i,j+1)-V.z(i,j))-(V.x(i,j+1)-V.x(i,j))*(V.z(i+1,j)-V.z(i,j));
		N.z(i,j) =	(V.x(i-1,j)-V.x(i,j))*(V.y(i,j+1)-V.y(i,j))-(V.x(i,j+1)-V.x(i,j))*(V.y(i-1,j)-V.y(i,j))+
						(V.x(i,j+1)-V.x(i,j))*(V.y(i+1,j)-V.y(i,j))-(V.x(i+1,j)-V.x(i,j))*(V.y(i,j+1)-V.y(i,j));
	}
	for (int i=1,j=m_xPoints-1; i<m_yPoints-1; i++) {//Last column
		N.x(i,j) =	(V.y(i-1,j)-V.y(i,j-1))*(V.z(i,j)-V.z(i,j-1))-(V.y(i,j)-V.y(i,j-1))*(V.z(i-1,j)-V.z(i,j-1))+
						(V.y(i,j)-V.y(i,j-1))*(V.z(i+1,j)-V.z(i,j-1))-(V.y(i+1,j)-V.y(i,j-1))*(V.z(i,j)-V.z(i,j-1));
		N.y(i,j) =	(V.x(i,j)-V.x(i,j-1))*(V.z(i-1,j)-V.z(i,j-1))-(V.x(i-1,j)-V.x(i,j-1))*(V.z(i,j)-V.z(i,j-1))+
						(V.x(i+1,j)-V.x(i,j-1))*(V.z(i,j)-V.z(i,j-1))-(V.x(i,j)-V.x(i,j-1))*(V.z(i+1,j)-V.z(i,j-1));
		N.z(i,j) =	(V.x(i-1,j)-V.x(i,j-1))*(V.y(i,j)-V.y(i,j-1))-(V.x(i,j)-V.x(i,j-1))*(V.y(i-1,j)-V.y(i,j-1))+
						(V.x(i,j)-V.x(i,j-1))*(V.y(i+1,j)-V.y(i,j-1))-(V.x(i+1,j)-V.x(i,j-1))*(V.y(i,j)-V.y(i,j-1));
	}

	int i,j;
	i=0;
	j=0;
	N.x(0,0) = (V.y(i,j+1)-V.y(i,j))*(V.z(i+1,j)-V.z(i,j))-(V.y(i+1,j)-V.y(i,j))*(V.z(i,j+1)-V.z(i,j));
	N.y(0,0) = (V.x(i+1,j)-V.x(i,j))*(V.z(i,j+1)-V.z(i,j))-(V.x(i,j+1)-V.x(i,j))*(V.z(i+1,j)-V.z(i,j));
	N.z(0,0) = (V.x(i,j+1)-V.x(i,j))*(V.y(i+1,j)-V.y(i,j))-(V.x(i+1,j)-V.x(i,j))*(V.y(i,j+1)-V.y(i,j));

	i=0;
	j=m_xPoints-1;
	N.x(0,m_xPoints-1) = (V.y(i,j)-V.y(i,j-1))*(V.z(i+1,j)-V.z(i,j-1))-(V.y(i+1,j)-V.y(i,j-1))*(V.z(i,j)-V.z(i,j-1));
	N.y(0,m_xPoints-1) = (V.x(i+1,j)-V.x(i,j-1))*(V.z(i,j)-V.z(i,j-1))-(V.x(i,j)-V.x(i,j-1))*(V.z(i+1,j)-V.z(i,j-1));
	N.z(0,m_xPoints-1) = (V.x(i,j)-V.x(i,j-1))*(V.y(i+1,j)-V.y(i,j-1))-(V.x(i+1,j)-V.x(i,j-1))*(V.y(i,j)-V.y(i,j-1));

	i=m_yPoints-1;
	j=0;
	N.x(m_yPoints-1,0) = (V.y(i-1,j)-V.y(i,j))*(V.z(i,j+1)-V.z(i,j))-(V.y(i,j+1)-V.y(i,j))*(V.z(i-1,j)-V.z(i,j));
	N.y(m_yPoints-1,0) = (V.x(i,j+1)-V.x(i,j))*(V.z(i-1,j)-V.z(i,j))-(V.x(i-1,j)-V.x(i,j))*(V.z(i,j+1)-V.z(i,j));
	N.z(m_yPoints-1,0) = (V.x(i-1,j)-V.x(i,j))*(V.y(i,j+1)-V.y(i,j))-(V.x(i,j+1)-V.x(i,j))*(V.y(i-1,j)-V.y(i,j));

	i=m_yPoints-1;
	j=m_xPoints-1;
	N.x(m_yPoints-1,m_xPoints-1) = (V.y(i-1,j)-V.y(i,j-1))*(V.z(i,j)-V.z(i,j-1))-(V.y(i,j)-V.y(i,j-1))*(V.z(i-1,j)-V.z(i,j-1));
	N.y(m_yPoints-1,m_xPoints-1) = (V.x(i,j)-V.x(i,j-1))*(V.z(i-1,j)-V.z(i,j-1))-(V.x(i-1,j)-V.x(i,j-1))*(V.z(i,j)-V.z(i,j-1));
	N.z(m_yPoints-1,m_xPoints-1) = (V.x(i-1,j)-V.x(i,j-1))*(V.y(i,j)-V.y(i,j-1))-(V.x(i,j)-V.x(i,j-1))*(V.y(i-1,j)-V.y(i,j-1));
/*	for (int j=1; j<m_xPoints-1; j++) {
		//First line, last line
		N.x(0,j) = N.x(m_yPoints-1,j) = 0;
		N.y(0,j) = N.y(m_yPoints-1,j) = 0;
		N.z(0,j) = N.z(m_yPoints-1,j) = 1;
	}
	for (int i=1; i<m_yPoints-1; i++) {
		//First column, last column
		N.x(i,0) = N.x(i,m_xPoints-1) = 0;
		N.y(i,0) = N.y(i,m_xPoints-1) = 0;
		N.z(i,0) = N.z(i,m_xPoints-1) = 1;
	}*/
//	N.x(0,0) = N.x(0,m_xPoints-1) = N.x(m_yPoints-1,0) = N.x(m_yPoints-1,m_xPoints-1) = 0;
//	N.y(0,0) = N.y(0,m_xPoints-1) = N.y(m_yPoints-1,0) = N.y(m_yPoints-1,m_xPoints-1) = 0;
//	N.z(0,0) = N.z(0,m_xPoints-1) = N.z(m_yPoints-1,0) = N.z(m_yPoints-1,m_xPoints-1) = 1;

	float length;
	int size = 3*m_points;
	for (int i=0; i<size; i+=3) {
		length = sqrt( m_normals[i]*m_normals[i]+m_normals[i+1]*m_normals[i+1]+m_normals[i+2]*m_normals[i+2] );
		if (length != 0) {
			m_normals[i+0] /= length;
			m_normals[i+1] /= length;
			m_normals[i+2] /= length;
		} else { //FIXME: should a normal be [0 0 0]?
			m_normals[i+0] = 0;
			m_normals[i+1] = 0;
			m_normals[i+2] = 0;
		}
	}
}

void Surface::createFalseColors( const vector<Color>& clist )
{
	if (m_vertices == NULL)
		return;
	if (m_colors == NULL)
		m_colors = new float[3*m_points];

	ArrayPoint3D<float> C( m_colors, m_xPoints, m_yPoints, 3 );
	ArrayPoint3D<float> V( m_vertices, m_xPoints, m_yPoints, 3 );

	Color pixel;
	ColorIndex cindex( m_zMin, m_zMax, clist );
	for (int i=0; i<m_yPoints; i++)
		for (int j=0; j<m_xPoints; j++) {
			pixel = cindex.index( V.z(i,j) );
			C.r(i,j) = pixel.r();
			C.g(i,j) = pixel.g();
			C.b(i,j) = pixel.b();
		}
}

void Surface::draw( int mode )
{
  //What about glIndexPointer??
	glPushClientAttrib( GL_CLIENT_VERTEX_ARRAY_BIT );
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, m_vertices );
	if (m_colors != NULL) {
		glEnableClientState( GL_COLOR_ARRAY );
		glColorPointer( 3, GL_FLOAT, 0, m_colors );
	}
	if (m_normals != NULL) {
		glEnableClientState( GL_NORMAL_ARRAY );
		glNormalPointer( GL_FLOAT, 0, m_normals );
	}
	glPushAttrib( GL_POLYGON_BIT|GL_CURRENT_BIT );

	if (mode == GL_POINT)
		glDrawArrays( GL_POINTS, 0, m_xPoints*m_yPoints );
	else {
		glPolygonMode( GL_FRONT_AND_BACK, mode );
		for (int i=0; i<m_yPoints-1; i++ ) // indices[width][height] -> [2*m_xPoints][m_yPoints-1]
			glDrawElements( 	GL_TRIANGLE_STRIP, 2*m_xPoints, GL_UNSIGNED_INT, (void *) &m_indices[i*2*m_xPoints] );
	}

	glPopAttrib();
	glPopClientAttrib();
}

void Surface::createIndices( int xPoints, int yPoints, GLuint* &indices )
{
  const_indicesCacheIter ci;
	for (ci = s_glIndicesCache.begin(); ci != s_glIndicesCache.end(); ci++)
		if (ci->x == xPoints && ci->y == yPoints) {
			indices = ci->indices;
			return;
		}
	
	int size=2*xPoints*(yPoints-1);
	glIndices aux;

	aux.x = xPoints;
	aux.y = yPoints;
	aux.indices = new GLuint[size];
	indices = aux.indices;

  std::clog << "Creating new indices array of [" << size << "]" << std::endl;

	aux.indices[0] = 0;
	for (int i=1; i<size; i+=2 ) {
		aux.indices[i] = aux.indices[i-1]+xPoints;
		aux.indices[i+1] = aux.indices[i]-xPoints+1;
		//std::cerr<<aux.indices[i]<<" "<<aux.indices[i+1]<<" "; //DEBUG
	}
	s_glIndicesCache.push_back( aux );
}

void Surface::clearIndices()
{
  for (indicesCacheIter ci = s_glIndicesCache.begin(); ci != s_glIndicesCache.end(); ci++)
    if (ci->indices)
      delete[] ci->indices;

  s_glIndicesCache.clear();
}

/// Create the static cache of Surface objects.
/// If the user specifies to play as video, then all the frames are
/// prerendered in the cache (as Sufrace objects), and
/// 'vector Surface::s_surfaceCache' has a 1-1 relationship with,
/// 'vector SurfaceImage::s_files', so s_currentSurfaceIndex == s_currentFrameIndex.
/// Else, the cache keeps a size of one surface only: s_currentSurfaceIndex == 0,
/// s_currentFrameIndex = 't'/imageID.
/// At first, using the first image frame, or function at 't=0'.
/// Then every next Surface takes its place.
void Surface::initSurfaceCache( const Arguments& argv, int FPS )
{
  s_surfaceCacheOn = argv.renderVideo;
  s_surfaceMode = argv.mode;
  s_drawMode = argv.drawMode;
  s_falseColors = argv.falseColors;
  s_lights = argv.lights;
  s_loop = argv.loop;
  s_log = argv.verbose;
  s_currentSurfaceIndex = 0;
  s_currentFrameIndex = 0;

  if (argv.falseColors) {
    s_colors.push_back( BLUE );
    s_colors.push_back( LIGHT_BLUE );
    s_colors.push_back( YELLOW );
    s_colors.push_back( RED );
    clog << "Creating false colors" << endl;
  }

  switch (s_surfaceMode) {
    case IMAGE:
      SurfaceImage::s_resample = argv.resample;
      SurfaceImage::s_files = argv.images;
      s_NRframes = SurfaceImage::s_files.size();

      // Set the cache size
      s_surfaceCacheSize = s_surfaceCacheOn ? SurfaceImage::s_files.size() : 1;
      s_surfaceCache = vector<Surface*>(s_surfaceCacheSize);

      for (int i = 0; i<s_surfaceCacheSize; i++) {
        SurfaceImage::create( i, i );
      }
      break;
    case PLOT:
      SurfaceFunction::s_xmin = argv.xmin;
      SurfaceFunction::s_xmax = argv.xmax;
      SurfaceFunction::s_ymin = argv.ymin;
      SurfaceFunction::s_ymax = argv.ymax;
      SurfaceFunction::s_points = argv.points;

      // Parse the input function code and set the SurfaceFunction::Interpreter
      SurfaceFunction::s_input.str(argv.functionCode);

      SurfaceFunction::s_code["x"] = 0;
      SurfaceFunction::s_code["y"] = 0;
      SurfaceFunction::s_code["t"] = 0;

      // Set the total number of frames allowed
      s_NRframes = (argv.duration == 0) ? 1 : FPS * argv.duration;
      // Set the cache size
      s_surfaceCacheSize = s_surfaceCacheOn ? s_NRframes : 1;
      s_surfaceCache = vector<Surface*>(s_surfaceCacheSize);

      for (int i=0; i<s_surfaceCacheSize; i++) {
        SurfaceFunction::create( i, i ); 
      }
      break;
  }
}

void Surface::deleteSurfaceCache()
{
  for (int i=0; i<s_surfaceCacheSize; i++)
    delete s_surfaceCache[i];

  clearIndices();
}

void Surface::drawCurrent()
{
  s_surfaceCache[s_currentSurfaceIndex]->draw( s_drawMode );
}

bool Surface::next()
{
  if (s_NRframes == 1)
    return false;

  if (s_surfaceCacheOn) {
    // If at end, and no loop, do nothing
    if ((s_currentSurfaceIndex == s_surfaceCacheSize-1) && !s_loop)
      return false;

    // Advance to next index
    if (s_currentSurfaceIndex == s_surfaceCacheSize-1) {
      s_currentFrameIndex = s_currentSurfaceIndex = 0;
    } else {
      s_currentSurfaceIndex++;
      s_currentFrameIndex++;
    }
  } else { // Cache holds only one Surface
    // If at end, and no loop, do nothing
    if ((s_currentFrameIndex == s_NRframes-1) && !s_loop)
      return false;

    // Remove old surface
    delete s_surfaceCache[0];

    // Advance to next frame
    if (s_currentFrameIndex == s_NRframes-1)
      s_currentFrameIndex = 0;
    else
      s_currentFrameIndex++;

    // Create new Surface
    Surface::create( 0, s_currentFrameIndex ); 
  }
  if (s_log)
    clog << "Surface::next(): Frame[" << s_currentFrameIndex << "]: of '"<<s_NRframes<<"' ready." << endl;

  return true;
}

bool Surface::previous()
{
  if (s_NRframes == 1)
    return false;

  if (s_surfaceCacheOn) {
    // If at begining, and no loop, do nothing
    if ((s_currentSurfaceIndex == 0) && !s_loop)
      return false;

    // Advance to next index
    if (s_currentSurfaceIndex == 0)
      s_currentFrameIndex = s_currentSurfaceIndex = s_surfaceCacheSize-1;
    else {
      s_currentSurfaceIndex--;
      s_currentFrameIndex--;
    }
  } else { // Cache holds only one Surface
    // If at begining, and no loop, do nothing
    if ((s_currentFrameIndex == 0) && !s_loop)
      return false;

    // Remove old surface
    delete s_surfaceCache[0];

    // Advance to next frame
    if (s_currentFrameIndex == 0)
      s_currentFrameIndex = s_NRframes-1;
    else
      s_currentFrameIndex--;

    // Create new Surface
    Surface::create( 0, s_currentFrameIndex ); 
  }
  if (s_log)
    clog << "Surface::previous(): Frame[" << s_currentFrameIndex << "]: of '"<<s_NRframes<<"' ready." << endl;

  return true;
}

void Surface::first()
{
  if (s_NRframes == 1)
    return;
  if (s_currentFrameIndex == 0)
    return;

  s_currentSurfaceIndex = s_currentFrameIndex = 0;

  if (!s_surfaceCacheOn) {
    // Remove old surface
    delete s_surfaceCache[0];

    // Create new Surface
    Surface::create( 0, s_currentFrameIndex ); 
  }
}

const Surface* Surface::getCurrentSurface()
{
  return s_surfaceCache[s_currentSurfaceIndex];
}

void Surface::create(int cacheIndex, int frameIndex)
{
  switch (s_surfaceMode) {
    case IMAGE:
      SurfaceImage::create( cacheIndex, frameIndex );
      break;
    case PLOT:
      SurfaceFunction::create( cacheIndex, frameIndex ); 
      break;
  }
}

// protected static data
vector<Surface::glIndices> Surface::s_glIndicesCache;
bool Surface::s_falseColors, Surface::s_lights, Surface::s_log;
std::vector<Color> Surface::s_colors;
std::vector<Surface*> Surface::s_surfaceCache;

// private static data
int Surface::s_currentSurfaceIndex,
    Surface::s_surfaceCacheSize,
    Surface::s_drawMode;
bool Surface::s_surfaceCacheOn, Surface::s_loop;
Mode Surface::s_surfaceMode;
int Surface::s_currentFrameIndex,
    Surface::s_NRframes;
//////// } Surface //////// 


//////// SurfaceImage { //////// 
SurfaceImage::SurfaceImage( const string& file, float ratio )
{
  m_image = new Magick::Image();
	m_image->read( file );
	if (ratio != 1.0) //FIXME
		m_image->scale( Magick::Geometry( ratio*m_image->columns(), ratio*m_image->rows()) );
	m_image->flip();
	m_xPoints = m_image->columns();
	m_yPoints = m_image->rows();
	m_points = m_xPoints*m_yPoints;

  createImageColors(); // Needed for createVertices()
  createVertices();

	createIndices( m_xPoints, m_yPoints, m_indices );
}

SurfaceImage::~SurfaceImage()
{
  delete m_image;
}

/// Create the surface vertices from the image colors.
/// The vertice height(z-axis) is taken as the distance of the
/// pixel's RGB color from the RGB cube origin.
void SurfaceImage::createVertices()
{
	if (m_image->colorSpace() != Magick::RGBColorspace)
		throw string("Only rgb images are supported!");

  if (m_colors == NULL)
		throw string("No color data!");

	if (m_vertices == NULL)
		m_vertices = new float[3*m_points];

  int c = -1; ///< image channel
	int width = m_xPoints;
	int height = m_yPoints;
	const float step = 1.0/(width>height?width-1:height-1);
	ArrayPoint3D<float> V( m_vertices, width, height, 3 );
	ArrayPoint3D<float> C( m_colors, width, height, 3 );
	if (c == -1) {
		m_zMin = m_zMax = (C.r(0,0)+C.g(0,0)+C.b(0,0))/3.0;
		for (int i=0; i<height; i++)
			for (int j=0; j<width; j++) {
				V.z(i,j) = (C.r(i,j)+C.g(i,j)+C.b(i,j))/3.0;
				V.y(i,j) = i*step;
				V.x(i,j) = j*step;

				if (V.z(i,j) > m_zMax) m_zMax = V.z(i,j);
				if (V.z(i,j) < m_zMin) m_zMin = V.z(i,j);
			}
	} else {
		m_zMin = m_zMax = C(0,0,c);
		for (int i=0; i<height; i++)
			for (int j=0; j<width; j++) {
				V.z(i,j) = C(i,j,c);
				V.y(i,j) = i*step;
				V.x(i,j) = j*step;

				if (V.z(i,j) > m_zMax) m_zMax = V.z(i,j);
				if (V.z(i,j) < m_zMin) m_zMin = V.z(i,j);
			}
	}
	
	m_xMin = V.x(0,0);
	m_yMin = V.y(0,0);
	m_xMax = V.x(height-1,width-1);
	m_yMax = V.y(height-1,width-1);
}

/// Get the image pixel colors and use them as the OpenGL vertex RGB colors.
void SurfaceImage::createImageColors()
{
  if (m_colors == NULL)
    m_colors = new float[3*m_points];

	switch (m_image->colorSpace()) {
		case Magick::RGBColorspace:
			m_image->write( 0, 0, m_xPoints, m_yPoints, "RGB", Magick::FloatPixel, m_colors );
			break;
		case Magick::GRAYColorspace:
			//temp.write( 0, 0, width, height, "R", Magick::FloatPixel, Data );
		default:
			throw string("Colorspace not implemented yet!");
			break;
	}
}

void SurfaceImage::create(int cacheIndex, int fileIndex)
{
  Surface* surface = new SurfaceImage( s_files[fileIndex], s_resample );

  if (Surface::s_falseColors)
    surface->createFalseColors( s_colors );
  if (s_lights)
    surface->createNormals();

  if (s_log)
    clog <<"Create image("<<s_files[fileIndex]<<") surface: "
         <<surface->Lx()<<"x"<<surface->Ly()<<"x"<<surface->Lz()
         <<"@["<<surface->Ox()<<","<<surface->Oy()<<","<<surface->Oz()<<"]\n";

  s_surfaceCache[cacheIndex] = surface;
}

// public static data
float SurfaceImage::s_resample;
std::vector<std::string> SurfaceImage::s_files;
//////// } SurfaceImage //////// 


//////// SurfaceFunction { //////// 
SurfaceFunction::SurfaceFunction( float xmin, float xmax,
                                  float ymin, float ymax,
                                  int points, Interpreter *code ):
  Surface( xmin, xmax, ymin, ymax, points ),
  m_func(code)
{
	float div = (xmax-xmin)/(ymax-ymin);
	m_xPoints = (int) sqrt( points*div );
	m_yPoints = (int) (m_xPoints/div);
  m_points = m_xPoints*m_yPoints;

	createVertices();
	createIndices( m_xPoints, m_yPoints, m_indices );
}

SurfaceFunction::~SurfaceFunction()
{}

#include <cfloat>
void SurfaceFunction::createVertices()
{
  Interpreter& func = *m_func;

  if (s_log)
    clog << "createVertices(): Create vertices for t=" << func["t"] << endl;
	if (m_vertices == NULL)
		m_vertices = new float[3*m_points];

	ArrayPoint3D<float> V( m_vertices, m_xPoints, m_yPoints, 3 );

  func["x"] = 0.0F;
  func["y"] = 0.0F;
	m_zMin = FLT_MAX;
  m_zMax = FLT_MIN;

	float z;
	float xstep = (m_xMax-m_xMin)/(m_xPoints-1);
	float ystep = (m_yMax-m_yMin)/(m_yPoints-1);
	for (int j=0; j<m_xPoints; j++)
		for (int i=0; i<m_yPoints; i++) {
				V.x(i,j) = func["x"] = m_xMin+j*xstep;
				V.y(i,j) = func["y"] = m_yMin+i*ystep;

				V.z(i,j) = z = func.result();
        if (isnan(z) || isinf(z)) {
          V.z(i,j) = 0.0F; //TODO V.z(i,j) = avgFromLocalArea(V,i,j);
          continue;
        }

				if (z > m_zMax) m_zMax = z;
				if (z < m_zMin) m_zMin = z;
    }
}

// Static methods
void SurfaceFunction::create(int cacheIndex, int timeIndex)
{
  s_code["t"] = timeIndex;

  Surface* surface = new SurfaceFunction( s_xmin, s_xmax, s_ymin, s_ymax, s_points, &s_code );

  if (Surface::s_falseColors)
    surface->createFalseColors( s_colors );
  if (s_lights)
    surface->createNormals();

  if (s_log)
    clog << "Create function surface: "
         <<surface->Lx()<<"x"<<surface->Ly()<<"x"<<surface->Lz()
         <<"@["<<surface->Ox()<<","<<surface->Oy()<<","<<surface->Oz()<<"]\n";

  s_surfaceCache[cacheIndex] = surface;
}

// public static data
int SurfaceFunction::s_points;
istringstream SurfaceFunction::s_input;
Interpreter SurfaceFunction::s_code(&s_input);
float SurfaceFunction::s_xmin,
      SurfaceFunction::s_xmax,
      SurfaceFunction::s_ymin,
      SurfaceFunction::s_ymax;
//////// } SurfaceFunction //////// 
