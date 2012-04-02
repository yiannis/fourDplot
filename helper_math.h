#ifndef HELPER_MATH_H
#define HELPER_MATH_H

#include <algorithm>

//////////MATRIX OPERATIONS//////////
/// Wrapper for a C array with 2 dimentions(i,j) that holds a 3D vector or RGB color.
template<class T>
class ArrayPoint3D {
	private:
		T* array;
		int width;
		int height;
		int channels;
	public:
		ArrayPoint3D( T* a, int w, int h, int c ) :array(a), width(w), height(h), channels(c) {}

		T& operator() ( int i, int j, int c ) { return *(array+(i*channels*width+j*channels+c)); }
		T* operator() ( int i, int j ) { return array+(i*channels*width+j*channels); }
		T& x( int i, int j ) { return *(array+(i*channels*width+j*channels+0)); }
		T& y( int i, int j ) { return *(array+(i*channels*width+j*channels+1)); }
		T& z( int i, int j ) { return *(array+(i*channels*width+j*channels+2)); }
		T& r( int i, int j ) { return *(array+(i*channels*width+j*channels+0)); }
		T& g( int i, int j ) { return *(array+(i*channels*width+j*channels+1)); }
		T& b( int i, int j ) { return *(array+(i*channels*width+j*channels+2)); }
};

//////////MATH OPERATIONS//////////
//Use std::max(T&,T&)
template<typename Tp>
inline const Tp&
max(const Tp& a, const Tp& b, const Tp& c ) //3 values
{
	return std::max( std::max( a,b ),c );
}

template<typename Tp>
inline const Tp&
max(const Tp& a, const Tp& b, const Tp& c, const Tp& d ) //4 values
{
	return std::max(std::max(a,b),std::max(c,d));
}

//Use std::min(T&,T&)
template<typename T>
inline const T&
min(const T& a, const T& b, const T& c ) //3 values
{
	return std::min( std::min( a,b ),c );
}

template<typename T>
inline const T&
min(const T& a, const T& b, const T& c, const T& d ) //4 values
{
	return std::min(std::min(a,b),std::min(c,d));
}
//////////MATH OPERATIONS//////////

#endif // HELPER_MATH_H
