#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <vector>
#include <cmath>

enum colors { white = 0, black, blue, red, green, pink, light_blue, yellow, orange };

class Color;
extern const Color WHITE, BLACK, BLUE, RED, GREEN, PINK, LIGHT_BLUE, YELLOW, ORANGE;

///Color [holds its own values] <=> pixel [holds pointer to its values]
///  /\                               /\
///  ||  -> array                     ||  -> pArray
///  \/                               \/
///point [holds its own values] <=> vector [holds pointer to its values]

class Color {
	private:
		float *rgba;

	public:
		Color() {
			rgba = new float[4]; 
			rgba[0] = 0;
			rgba[1] = 0; 
			rgba[2] = 0; 
			rgba[3] = 1;
		}
		Color( float r, float g, float b, float a = 1 ) {
			rgba = new float[4];
			rgba[0] = r;
			rgba[1] = g;
			rgba[2] = b;
			rgba[3] = a;
		}
		Color( double r, double g, double b, double a = 1 ) {
			rgba = new float[4];
			rgba[0] = r;
			rgba[1] = g;
			rgba[2] = b;
			rgba[3] = a;
		}
		Color( int r, int g, int b, int a = 255 ) {
			rgba = new float[4];
			rgba[0] = r/255.0;
			rgba[1] = g/255.0;
			rgba[2] = b/255.0;
			rgba[3] = a/255.0;
		}
		Color( const float* cp ) { 
			rgba[0] = cp[0];
			rgba[1] = cp[1];
			rgba[2] = cp[2];
			rgba[3] = cp[3];
		}
		Color( const Color& c ) {
			rgba = new float[4];
			rgba[0] = c.rgba[0];
			rgba[1] = c.rgba[1];
			rgba[2] = c.rgba[2];
			rgba[3] = c.rgba[3];
		}
		Color( colors c ) {
			rgba = new float[4];
			switch (c) {
				case white:
					*this = WHITE;
					break;
				case black:
					*this = BLACK;
					break;
				case blue:
					*this = BLUE;
					break;
				case red:
					*this = RED;
					break;
				case green:
					*this = GREEN;
					break;
				case pink:
					*this = PINK;
					break;
				case light_blue:
					*this = LIGHT_BLUE;
					break;
				case yellow:
					*this = YELLOW;
					break;
				case orange:
					*this = ORANGE;
					break;
				default:
					throw "Unknown Color!";
					break;
			}
		}
		~Color() { delete[] rgba; }

		// Assignments modify & return lvalue.
		// operator= can only be a member function:
		Color& operator=( const Color& right ) {
			// Handle self-assignment:
			if(this == &right) return *this;

			rgba[0] = right.rgba[0];
			rgba[1] = right.rgba[1];
			rgba[2] = right.rgba[2];
			rgba[3] = right.rgba[3];
			return *this;
		}
		float* operator()() const { return rgba; }
		float r() const { return rgba[0]; }
		float g() const { return rgba[1]; }
		float b() const { return rgba[2]; }
		float a() const { return rgba[3]; }
		void print() const { std::cout<<'['<<rgba[0]<<','<<rgba[1]<<','<<rgba[2]<<','<<rgba[3]<<']'; }
};

inline float distance( Color x1, Color x2 )
{
	return sqrt( pow( x1.r()-x2.r(), 2 ) + pow( x1.g()-x2.g(), 2 ) + pow( x1.b()-x2.b(), 2 ) );
}

/// Represents a false color scale.
class ColorIndex {
	private:
		std::vector<Color> m_colorScale; /// < The list of false colors

		float m_top; ///< Maximum intensity value
		float m_bottom; ///< Minimum intensity value
		int m_N; ///< ID if last color

	public:
    /// Takes the minimum and maximum intensity values and the false color list
		ColorIndex( float min, float max, const std::vector<Color> cs ) :
      m_colorScale(cs), m_top(max), m_bottom(min), m_N(cs.size()-1)
		{}
		~ColorIndex() {}

		Color index( float value ) const;
};

#endif //COLOR_H
