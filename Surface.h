#ifndef SURFACE_H
#define SURFACE_H


#include <iostream>
#include <vector>
#include <map>
#include <cmath>

#include <GL/glut.h>
#include <Magick++.h>

#include "Compiler.h"

#include "Arguments.h"
#include "Color.h"
#include "helper_math.h"

/// \file Surface.h
/// \brief A Surface class represents a sampled 'real' surface
/// The 'real' surface could be an image \see SurfaceImage, or a 3D
/// surface function of type z = f(x,y), \see SurfaceFunction.


/// The abstract base class of all surfaces.
class Surface {
  protected:
		float m_xMin, ///< Min x-value (image indices / function local coordinates)
          m_xMax, ///< Max x-value (image indices / function local coordinates)
          m_yMin, ///< Min y-value (image indices / function local coordinates)
          m_yMax, ///< Max y-value (image indices / function local coordinates)
          m_zMin, ///< Min z-value (image indices / function local coordinates)
          m_zMax; ///< Max z-value (image indices / function local coordinates)
		int m_points,   ///< Total #of points (vertices)
        m_polygons, ///< Total number of triangles
        m_xPoints,  ///< points (vertices) on x-axis, width
        m_yPoints;  ///< points (vertices) on y-axis, height
		float *m_vertices,
          *m_normals,
          *m_colors;
		GLuint *m_indices; ///< Pointer to an indices buffer in s_glIndicesCache

	public:
    Surface() :
      m_vertices(NULL), m_normals(NULL), m_colors(NULL), m_indices(NULL)
    {}
		Surface( float xmin, float xmax, float ymin, float ymax, int points ) :
      m_xMin(xmin), m_xMax(xmax), m_yMin(ymin), m_yMax(ymax), m_points(points),
      m_vertices(NULL), m_normals(NULL), m_colors(NULL), m_indices(NULL)
    {}

		~Surface();

		void createNormals();
		void createFalseColors( const std::vector<Color>& );

		//void zoom_in( float ) = 0; //TODO
		//void zoom_out( float ) = 0; //TODO

		void draw( int mode = GL_POINT );

		float xMin() const { return m_xMin; }
		float xMax() const { return m_xMax; }
		float yMin() const { return m_yMin; }
		float yMax() const { return m_yMax; }
		float zMin() const { return m_zMin; }
		float zMax() const { return m_zMax; }
    float Lx() const { return m_xMax - m_xMin; }
    float Ly() const { return m_yMax - m_yMin; }
    float Lz() const { return m_zMax - m_zMin; }
    float Ox() const { return (m_xMax + m_xMin)/2.0F; }
    float Oy() const { return (m_yMax + m_yMin)/2.0F; }
    float Oz() const { return (m_zMax + m_zMin)/2.0F; }
    float Lmax() const { return max( Lx(), Ly(), Lz() ); }

    // static methods
    static void initSurfaceCache( const Arguments& argv, int FPS );
    static void deleteSurfaceCache();
		static void drawCurrent();
    static bool next();
    static bool previous();
    static void first();
    static const Surface* getCurrentSurface();

  protected:
    static bool s_falseColors, s_lights, s_log;
    static std::vector<Color> s_colors; ///< The false color scale
		static std::vector<Surface*> s_surfaceCache; ///< Static cache of surfaces

		static void createIndices( int x, int y, GLuint* &indices );

		virtual void createVertices() = 0;

	private: // Static Members //
    // Methods
    static void clearIndices();
		static void create(int cacheIndex, int frameIndex);

    // Data
		struct glIndices { unsigned int x,y; GLuint* indices; }; // TODO glIndexArray

    typedef std::vector<glIndices>::const_iterator const_indicesCacheIter;
    typedef std::vector<glIndices>::iterator indicesCacheIter;
    //typedef std::vector<Surface*>::const_iterator const_surfaceCacheIter;
    //typedef std::vector<Surface*>::iterator surfaceCacheIter;

		static std::vector<glIndices> s_glIndicesCache; ///< Static cache of indices arrays
    //static surfaceCacheIter s_currentSurface;
    static int s_currentSurfaceIndex, ///< The index of the current surface (on the cache)
               s_surfaceCacheSize,
               s_drawMode;
    static bool s_surfaceCacheOn, s_loop;
    static Mode s_surfaceMode;
    static int s_currentFrameIndex, ///< The index of the current frame (in time/image#)
               s_NRframes;          ///< The number of all frames
};

/// SurfaceImage
class SurfaceImage : public Surface {
	private:
    Magick::Image* m_image; ///< A 3D surface will be created, representing this image

	public:
    SurfaceImage( const std::string& file, float ratio );
		~SurfaceImage();

    static void create(int cacheIndex, int fileIndex);

    static float s_resample; ///< Image resample ratio
		static std::vector<std::string> s_files; ///< The list of image files

  protected:
		virtual void createVertices();

    void createImageColors();
};

/// SurfaceFunction
class SurfaceFunction : public Surface {
  private:
    Compiler *m_func; ///< This pointer in not owned by the SurfaceFunction class

	public:
    SurfaceFunction( float xmin, float xmax, float ymin, float ymax, int points, Compiler *code );
		~SurfaceFunction();

    static void create(int cacheIndex, int timeIndex);

    static int s_points;
    static std::istringstream *s_input; ///< The input stream holding the function source code
    static Compiler *s_code; ///< The parsed function used to describe the 3D surface
    static float s_xmin, s_xmax, s_ymin, s_ymax;

  protected:
		virtual void createVertices();
};
#endif //SURFACE_H
