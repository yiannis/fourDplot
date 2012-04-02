#include "Color.h"

Color ColorIndex::index( float value ) const
{
	int Ni = (int) ((value-bottom)/D);

	Color OA = colorScale[Ni], OB = colorScale[Ni+1];

	float a = (value - bottom - Ni*D)/D;
	float d = distance( OA, OB );
	a *= d;

	float r,g,b;
	r = ( (d-a)*OA.r() + a*OB.r() )/ d;
	g = ( (d-a)*OA.g() + a*OB.g() )/ d;
	b = ( (d-a)*OA.b() + a*OB.b() )/ d;

	return Color( r, g, b );
}

Color WHITE( 1.0, 1.0, 1.0 );
Color BLACK( 0.0, 0.0, 0.0 );
Color RED( 1.0, 0.0, 0.0 );
Color GREEN( 0.0, 1.0, 0.0 );
Color BLUE( 0.0, 0.0, 1.0 );
Color PINK( 244, 29, 244 );
Color LIGHT_BLUE( 16, 242, 227 );
Color YELLOW( 242, 231, 29 );
Color ORANGE( 252, 150, 25 );
