#ifndef VECTORF_H
#define VECTORF_H

#include <stdio.h>
#include <float.h>
#include <math.h>
#include "StdWilUtil.h"
#include "math_ext.h"

// NAN is the worst thing that can happen to a computation.
// NAN fails any comparison test ( 0 < nan is false, 0 > nan is false, 0 == nan is false )
// NAN is toxic (0*nan=nan, 0+nan=nan), the basic result of nan is to mess EVERYTHING UP.
// If NAN_SAFETY is defined, all divisions which result in NAN
// are given +inf instead.
#define NAN_MOD_SAFETY
#define DEBUG_STOP_ON_NAN

// If x and y are BOTH 0, put HUGE in the place of division.
#define NAN_DIV(x,y) ( (!x && !y) ? HUGE : x/y )

// See https://gist.github.com/superwills/6159033
// for the matrix characters

union Vector2f
{
	struct { float x,y; } ;
	float elts[2] ;

	Vector2f():x(0.f),y(0.f){}
	Vector2f( float ix, float iy ):x(ix),y(iy){}
	Vector2f( float iv ):x(iv),y(iv){}

	// makes compiler consider class has "nontrivial" copy assignment operator  
	// inline Vector2f& operator=( const Vector2f& v ) {
	//   x=v.x, y=v.y ;
	//   return *this ;
	// }

	inline Vector2f& operator=( float v ) {
		x=y=v;
		return *this ;
	}

	static inline Vector2f random() { return Vector2f( randFloat(), randFloat() ) ;  }

	static inline Vector2f random(float min, float max) {
		return Vector2f( randFloat(min,max), randFloat(min,max) ) ;
	}

	//CONST
	inline void print() const {
		printf( "(%.2f %.2f) ",x,y ) ;
	}
	inline void print( const char* msg ) const {
		printf( "%s (%.2f %.2f)",msg,x,y ) ;
	}
	inline void println() const {
		printf( "(%.2f %.2f)\n",x,y ) ;
	}
	inline void println( const char* msg ) const {
		printf( "%s (%.2f %.2f)\n",msg,x,y ) ;
	}

	inline float max() const { return ::max( x,y ) ; }
	inline float min() const { return ::min( x,y ) ; }
	inline int minIndex() const { 
		if( x <= y )  return 0 ;
		else return 1 ;
	}
	inline int maxIndex() const { 
		if( x >= y )  return 0 ;
		else  return 1 ;
	}
	inline int fabsMinIndex() const { 
		Vector2f c = fabsCopy() ;
		if( c.x <= c.y )  return 0 ;
		else return 1 ;
	}
	inline int fabsMaxIndex() const { 
		Vector2f c = fabsCopy() ;
		if( c.x >= c.y ) return 0 ;
		else  return 1 ;
	}

	inline float getAvg() const { return (x+y)/2.f ; }
	inline float sum() const { return x+y ; }

	inline Vector2f operator+( const Vector2f& o ) const {
		return Vector2f(x+o.x,y+o.y);
	}
	inline Vector2f operator-() const {
		return Vector2f(-x,-y);
	}
	inline Vector2f operator-( const Vector2f& o ) const {
		return Vector2f(x-o.x,y-o.y);
	}

	inline Vector2f operator*( const Vector2f& o ) const {
		return Vector2f(x*o.x,y*o.y);
	}
	inline Vector2f operator*( float s ) const {
		return Vector2f(x*s,y*s);
	}

	inline Vector2f operator/( const Vector2f& o ) const {
		return Vector2f(x/o.x,y/o.y).checkNaN() ;

	}
	inline Vector2f operator/( float s ) const {
		return Vector2f(x/s,y/s).checkNaN() ;
	}
	inline float cross( const Vector2f& o ) const {
		return x*o.y-y*o.x ;
	}
	inline float dot( const Vector2f& o ) const {
		return x*o.x+y*o.y ;
	}
	inline float len() const {
		return sqrtf( x*x+y*y ) ;
	}
	inline float len2() const {
		return x*x+y*y ;
	}
	inline Vector2f& setLen( float iLen ){
		if( float length = len() )
		{
			*this *= iLen / length;
		}
		return *this;
	}
	inline Vector2f& setAngle( float a ){
		// put on x-axis
		x = len();
		
		// now rotate
		float newX = x * cosf( a );
		float newY = x * sinf( a );
		x=newX, y=newY;
		return *this;
	}
	inline Vector2f normalizedCopy() const {
		return Vector2f( *this ).normalize() ;
	}
	inline float angleWith( const Vector2f& o ) const {
		return acosf( this->normalizedCopy().dot(o.normalizedCopy()) ) ;
	}
	// Returns + if this leads o.
	// more expensive than unsigned angle,
	// (2x atan2f)
	inline float signedAngleWith( const Vector2f& o ) const {
		float aThis = atan2f( y, x );
		float aO = atan2f( o.y, o.x ) ;

		//info( "lead=%.2f lag=%.2f, ", aThis, aO ) ;
		return aThis - aO ;
		// When 
	}
	// proximity
	inline bool isNear( const Vector2f& o ) const{
		return fabsf(x-o.x)<EPS_MIN && fabsf(y-o.y)<EPS_MIN ;
	}
	inline bool isNear( const Vector2f& o, const Vector2f& maxDiff ) const{
		return fabsf(x-o.x)<maxDiff.x && fabsf(y-o.y)<maxDiff.y ;
	}
	inline bool isCanonical() const {
		return -1.f <= x && x <= 1.f && 
			-1.f <= y && y <= 1.f ;
	}
	inline bool isNaN() const {
		return _isnan(x) || _isnan(y) ;
	}
	// You have to figure out what to do with nan.  nan
	// only results from 0.f/0.f, so USUALLY it should mean inf.
	inline Vector2f& checkNaN() {
		// Getting NAN is extremely bad.  it poisons other operations, fails every comparison test,
		// and generally messes your program up.  because nan is toxic, the effect could perculate and
		// eventually bubble into some really far place, you won't notice the bug was 0.f/0.f until about
		// 2 hours later.  so here we prevent NAN perculation, and in the event off odd program behavior,
		// its a good idea to set breakpoints here.
		if( _isnan( x ) )
			x = HUGE ;
		if( _isnan( y ) )
			y = HUGE ;
		return *this ;
	}

	// Exact equality
	inline bool operator==( const Vector2f& o ) const{
		return x==o.x && y==o.y ;
	}
	inline bool operator!=( const Vector2f& o ) const{
		return x!=o.x || y!=o.y ;
	}



	// Returns TRUE if "this" is closer to A
	// than B
	inline bool isCloserTo( const Vector2f& a, const Vector2f& thanB ){
		return ( *this-a ).len2() < ( *this-thanB ).len2() ;
	}

	// gets the tangential and normal components of THIS along fN
	// f MUST BE NORMALIZED.
	// It gives you the component of this along fN.
	// If that's negative, THEN THIS IS MORE THAN 90 DEG FROM FN.
	inline float parallelPerpX( const Vector2f& fN, Vector2f &vParallel, Vector2f &vPerp ) const
	{
		// assuming fN is normalized
		float compParallel = fN.dot( *this ) ;
		vParallel = fN * compParallel ;
		vPerp = (*this) - vParallel ;
		return compParallel ;
	}

	// The perpendicular vector is 
	inline void parallelPerpComponents( const Vector2f& fN, float &compParallel, float &compPerp ) const
	{
		// assuming fN is normalized
		compParallel = fN.dot( *this ) ;
		compPerp = fN.cross( *this ) ;
	}

	// This is the CCW 90 deg rotated perp. ( y,-x ) is CW rotated.
	inline Vector2f getPerpendicular() const {
		return Vector2f( -y,x ) ;
	}

	inline Vector2f fabsCopy() const {
		return Vector2f( fabsf(x), fabsf(y) ) ;
	}




	//NON-CONST
	inline Vector2f& normalize(){
		float length = len() ;

		// I added this debug check man, never take it out.
		if( !length ) {
			error( "Vector2f::normalize() attempt to divide by 0" ) ;
			return *this ;
		}

		return (*this)/=length ;
	}
	inline float safeNormalize(){
		float length = len() ;
		if( length )  (*this)/=length ;
		return length ;
	}
	inline Vector2f& operator+=( const Vector2f& o ){
		x+=o.x,y+=o.y;
		return *this ;
	}
	inline Vector2f& operator-=( const Vector2f& o ){
		x-=o.x,y-=o.y;
		return *this ;
	}
	inline Vector2f& operator*=( const Vector2f& o ){
		x*=o.x,y*=o.y;
		return *this ;
	}
	inline Vector2f& operator*=( float s ){
		x*=s,y*=s;
		return *this ;
	}
	inline Vector2f& operator/=( const Vector2f& o ){
		x/=o.x,y/=o.y;
		return checkNaN() ;
	}
	inline Vector2f& operator/=( float s ){
		x/=s,y/=s;
		return checkNaN() ;
	}
#ifdef NAN_MOD_SAFETY
	// its highly unlikely you'll ask to mod by 0, since
	// modding is usually rare and deliberate.
	inline Vector2f& operator%=( float s ){
		if( !s ) {
			error( "mod by 0" ) ;
			return *this ;
		}
		x=fmodf( x,s );  y=fmodf( y,s );
		return *this ;
	}
	inline Vector2f& operator%=( const Vector2f &o ){
		if( !o.x )  error( "mod by 0 (x)" ) ;
		else x=fmodf( x,o.x );
		if( !o.y )  error( "mod by 0 (y)" ) ;  
		else y=fmodf( y,o.y );
		return *this ;
	}
#else
	inline Vector2f& operator%=( float s ){
		x=fmodf( x,s );  y=fmodf( y,s );
		return *this ;
	}
	inline Vector2f& operator%=( const Vector2f &o ){
		x=fmodf( x,o.x );  y=fmodf( y,o.y );
		return *this ;
	}
#endif
	// This may be faster, but it requires you use the +octant
	// (0,0,0)->(worldSize,worldSize,worldSize)
	inline Vector2f& wrap( const Vector2f& worldSize ){
		*this += worldSize ;
		return *this %= worldSize ;
	}
	inline Vector2f& clampLen( float maxLen ){
		float length = len() ;
		if( length > maxLen ) // also means length > 0, hopefully
			return normalize()*=maxLen ;

		return *this ;
	}
	inline Vector2f& clampComponent( float minVal, float maxVal )
	{
		::clamp( x,minVal,maxVal ) ;
		::clamp( y,minVal,maxVal ) ;
		return *this ;
	}
	inline Vector2f& clampComponentBelow( float below )
	{
		if( x < below ) x=below ;
		if( y < below ) y=below ;
		return *this ;
	}
	inline Vector2f& clampComponentAbove( float above )
	{
		if( x > above ) x=above ;
		if( y > above ) y=above ;
		return *this ;
	}
	inline Vector2f& clampBelow( const Vector2f& below )
	{
		if( x < below.x ) x=below.x ;
		if( y < below.y ) y=below.y ;
		return *this ;
	}
	inline Vector2f& clampAbove( const Vector2f& above )
	{
		if( x > above.x ) x=above.x ;
		if( y > above.y ) y=above.y ;
		return *this ;
	}

	inline Vector2f& fabs()
	{
		x=fabsf(x) ;  y=fabsf(y);
		return *this ;
	}
} ;

inline Vector2f operator-( const Vector2f& v, float s )
{
	return Vector2f(v.x-s,v.y-s);
}

inline Vector2f operator-( float s, const Vector2f& v )
{
	return Vector2f(s-v.x,s-v.y);
}

#endif