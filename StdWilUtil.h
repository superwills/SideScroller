#ifndef STDWILUTIL_H
#define STDWILUTIL_H

#include <math.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <list>
#include <map>
using namespace std;

#include "Log.h"

typedef unsigned int UInt32;
typedef unsigned char Byte;

#define every(intVal,cycle) (intVal%cycle==0)
#define YesOrNo(x) (x?"Yes":"No")

// skip is a synonym for "skipping" a loop iteration
#define skip continue

template<typename T>
bool contains( const list<T>& container, const T& elt )
{
	return find( container.begin(), container.end(), elt ) != container.end() ;
}

template<typename T>
bool contains( const vector<T>& container, const T& elt )
{
	return find( container.begin(), container.end(), elt ) != container.end() ;
}

template<typename T,typename S>
bool contains( const map<T,S>& container, const T& elt )
{
	return container.find( elt ) != container.end() ;
}

template <typename T> inline T& clamp( T& x, T minVal, T maxVal ) {
	if( x < minVal ) x = minVal ;
	else if( x > maxVal ) x = maxVal ;
	return x ;
}

template <typename T> inline T clampCopy( T x, T minVal, T maxVal ) {
	if( x < minVal ) return minVal ;
	else if( x > maxVal ) return maxVal ;
	else return x ;
}

template <typename T> inline T& clampBelow( T& x, T minVal ) {
	if( x < minVal ) x = minVal ;
	return x ;
}

template <typename T> inline T clampBelowCopy( T x, T minVal ) {
	if( x < minVal ) x = minVal ;
	return x ;
}

// for when x is an r-value
//inline float clampBelow( float x, float minVal ) {
//  if( x < minVal ) x = minVal ;
//  return x ;
//}
template <typename T> inline T& clampAbove( T& x, T maxVal ) {
	if( x > maxVal ) x = maxVal ;
	return x ;
}

template <typename T> inline T clampAboveCopy( T x, T maxVal ) {
	if( x > maxVal ) x = maxVal ;
	return x ;
}

// clamp below by 0
inline float clamp_0( float x ) {
	if( x < 0.0f ) return 0.0f ;
	else return x ;
}

// clamp below by 0 and above by 1
inline float clamp_01( float x ) {
	if( x < 0.0f ) return 0.0f ;
	else if( x > 1.0f ) return 1.0f ;
	else return x ;
}

// clamp below by -1 and above by +1
inline float clamp_11( float x ) {
	if( x < -1.0f ) return -1.0f ;
	else if( x > 1.0f ) return 1.0f ;
	else return x ;
}

// It's hard to decide what happens at 0.  Here i made it so
// 1 and 0 don't have the same sign.  neither do -1 and 0.
// 0 is considered as a distinct sign than +/-.  So also,
// 1 and 0 don't have a different sign.  and -1 and 0 don't have a different sign.
// 
inline bool sameSign( float a, float b ){
	return (a > 0.f && b > 0.f) || (a < 0.f && b < 0.f) ;
}

inline bool signDiffers( float a, float b ){
	//         b     |              a    (a>0 && b < 0)
	//    a          0     b             (a<0 && b > 0)
	return (a > 0.f && b < 0.f) || (a < 0.f && b > 0.f) ; // || (a != 0.f && b != 0.f) ;
	// the last condition is "one of a or b is not 0".  ie NOT BOTH are 0.
	// in the context of the previous checks, all that checks is they are NOT BOTH 0,
	// if they  ae both 0, then they are the "same sign" (no sign).
	//(a == 0 && b != 0) || (a != 0 && b == 0)  ;
}

inline bool sameSign( float a, float b, float c ){
	return (a > 0.f && b > 0.f && c > 0.f) ||
		(a < 0.f && b < 0.f && c < 0.f) ;
}

// check NOT same sign.
inline bool signDiffers( float a, float b, float c ){
	return !sameSign( a, b, c ) ;
}

// Ok, I need to get the odd one out. which index one has the different sign?
// Here i'm assuming that you already checked `signDiffers` so at one has
// a different sign.
// 0 means a was different, 1 means b was, and 2 means c was
inline int whichDifferent( float a, float b, float c ){
	if( sameSign(b, c) )  return 0 ;
	else if( sameSign(a, c) )  return 1 ;
	else //if( sameSign(a, b) )  
		return 2 ;
}

// to avoid nans/infs
inline float ratio( float wins, float losses )
{
	if( !wins ) return 0.f ; // avoid nan/infs.
	return wins / (wins+losses);
}

//#define cycleFlag( val, MINVAL, MAXVAL ) ( ++val>(MAXVAL)?val=(MINVAL):val )
inline int cycleFlag( int &val, int minVal, int maxVal )
{
	if( ++val > maxVal )
		val = minVal;

	return val;
}

//#define decycleFlag( val, MINVAL, MAXVAL ) ( --((int)val)<(MINVAL)?val=(MAXVAL):val )
inline int decycleFlag( int &val, int minVal, int maxVal )
{
	if( --val < minVal )
		val = maxVal;

	return val;
}

string makeString( const char *fmt, ... );

string makeString( const char *fmt, va_list args ) ;

string makeTime( double s ) ;


#endif