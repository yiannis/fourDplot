#include <iostream>

#include "Color.h"

/// Returns an interpolated color, at the given intensity value.
/// The m_colorScale indices are mapped linearly with the
/// given intensity range, so that m_colorScale[0] <-> m_bottom
/// and m_colorScale[m_N] <-> m_top.
/// The color vector OC between OA and AB, is interpolated by:
/// x{C}^i = x{A}^i + d / |AB| * (x{B}^i - x{A}^i)
Color ColorIndex::index( float value ) const
{
  if (value > m_top || value < m_bottom) {
    std::cerr << "Value: '"<<value<<"', out of range ["<<m_bottom<<","<<m_top<<"]" << std::endl;
    return WHITE;
  }

  float ci = m_N*(value-m_bottom)/(m_top-m_bottom); ///< color index, mapped to [m_bottom,m_top]
  float d = ci - floor(ci); ///< Fractional part of 'ci'
  int i1 = (int)floor(ci);
  int i2 = (int)ceil(ci);

  if (i1 < 0 || i1 > m_N) {
    std::cerr << "i1: '"<<i1<<"', out of range [0,"<<m_N<<"]" << std::endl;
    return WHITE;
  }
  if (i2 < 0 || i2 > m_N) {
    std::cerr << "i2: '"<<i2<<"', out of range [0,"<<m_N<<"]" << std::endl;
    return WHITE;
  }

  // Colors are treated as 3D vectors in the RGB color cube.
	Color OA = m_colorScale[i1], OB = m_colorScale[i2];

	float AB = distance( OA, OB );

	return Color( OA.r()+d/AB*(OB.r()-OA.r()),
                OA.g()+d/AB*(OB.g()-OA.g()),
                OA.b()+d/AB*(OB.b()-OA.b()) );
}


const Color WHITE( 1.0, 1.0, 1.0 );
const Color BLACK( 0.0, 0.0, 0.0 );
const Color RED( 1.0, 0.0, 0.0 );
const Color GREEN( 0.0, 1.0, 0.0 );
const Color BLUE( 0.0, 0.0, 1.0 );
const Color PINK( 244, 29, 244 );
const Color LIGHT_BLUE( 16, 242, 227 );
const Color YELLOW( 242, 231, 29 );
const Color ORANGE( 252, 150, 25 );
