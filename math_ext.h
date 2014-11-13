#ifndef MATH_EXT_H
#define MATH_EXT_H

#define EPS_MIN 1e-6f
#define EPS 1e-3f

#define DEGTORADCONST 0.017453292519943295769236907684886
#define RADTODEGCONST 57.295779513082320876798154814105
#define RADIANS(degreeMeasure) (degreeMeasure*DEGTORADCONST)
#define DEGREES(radianMeasure) (radianMeasure*RADTODEGCONST)
#define FREE(str) if(str){free(str);str=0;}
#define DESTROY(OBJ) if(OBJ){delete (OBJ); (OBJ)=0;}
#define DESTROY_ARRAY(ARR) if(ARR){delete[] (ARR); (ARR)=0;}
#define OVERWRITE(OBJ,WITHME) {if(OBJ) delete(OBJ); (OBJ)=WITHME;}

// 
#define FLOOR(x) ( ((x)>0) ? ((int)(x)) : (((int)(x))-1) )
#define CEIL(x) ( ((x)<0) ? ((int)(x)) : ((int)(x)+1) )

// if you're sure it's positive
#define FLOORPOS(x) ((int)(x))
#define CEILPOS(x) ((int)(x)+1)

// if you have axis 0, these two defines give you 1 & 2, for example
#define OTHERAXIS1(axis) ((axis+1)%3)
#define OTHERAXIS2(axis) ((axis+2)%3)

// Gives the biggest of x,y, or z
#define max3(x,y,z) std::max(std::max(x,y),z)
#define min3(x,y,z) std::min(std::min(x,y),z)

inline bool IsPow2( int x )
{
	// the x && in front ensure 0 isn't considered a power of 2
	return x && ((x & (x-1))==0) ;
}

// The 'i' is for int, there is a log2 for double in stdclib
inline unsigned int log2i( unsigned int x )
{
	unsigned int log2Val = 0 ;
	// Count push off bits to right until 0
	// 101 => 10 => 1 => 0
	// which means hibit was 3rd bit, its value is 2^3
	while( x>>=1 ) log2Val++;  // div by 2 until find log2.  log_2(63)=5.97, so
	// take that as 5, (this is a traditional integer function!)
	// eg x=63 (111111), log2Val=5 (last one isn't counted by the while loop)
	return log2Val ;
}

// Returns the value of the highest bit in the number.
// Logically 2^( floor(log2(x)) ) gives the result,
// If the number is a power of 2, then
// it's just the number itself, if x isn't a
// power of 2, its the next lower power of 2.
inline unsigned int hibit( unsigned int x )
{
	unsigned int log2Val = log2i( x ) ; // e.g. 5
	return  (1 << log2Val) ;   // finds 2^5=32, 2^0=1
}

// ( n ) =    n!/
// ( k )  k!(n-k)!
//
// also:
// (n) = ( n )
// (k)   ( n-k )
int binomial( int n, int k ) ;

// (0=>+1, 1=>-1)
inline int NEG( bool neg ){
	return -2*neg+1 ;
}

// ++++ => +1
// ---- => -1
inline int signum( float val ){
	//return f > 0 ? 1.f : -1.f ;
	if( val > 0 )  return 1 ;
	else if( val < 0 )  return -1 ;
	else return 0 ;
}

inline int signum( int i ){
	if( i > 0 ) return 1 ;
	else if( i < 0 ) return -1 ;
	else return 0 ;
}

inline bool isNear( float a, float b )
{
	return fabsf( a - b ) <= EPS_MIN ;
}

inline bool isNear( short a, short b, short tolerance )
{
	return abs( a - b ) <= tolerance ;
}

// returns true if `val` is within `eps` units of `reference`
inline bool isNear( float val, float reference, float eps ) 
{
	return fabsf( val-reference ) < eps ;
}

//!! dangerous because caller might use reversed order
template <typename T>
inline bool isBetweenOrdered( T val, T lowerBound, T upperBound ) {
	return lowerBound <= val && val <= upperBound ;
}

// (a ≤ b ≤ c) can work if (a ≤ b) returns -inf if true and +inf if false (which forces 2nd comparison to fail).
// but that spoils short cct eval.
template <typename T>
inline bool isBetween( T val, T boundA, T boundB )
{
	// if( boundA > boundB ) swap(boundA,boundB) ;  return isBetweenOrdered( val, boundA, boundB ) ;
	return (boundA <= val && val <= boundB) ||
		(boundB <= val && val <= boundA) ;
}

inline float lerp( float t, float A, float B ){
	return A + (B-A)*t ; // == A*(1.f-t) + B*t ;
}
inline float unlerp( float v, float A, float B ){
	return (v-A)/(B-A) ;
}

inline float randFloat() {
	return (float)rand()/RAND_MAX ; 
}

// -1,1 => -1 + ( rand between 0 and 2 )
inline float randFloat( float low, float high ) {
	return low + (high-low)*randFloat() ;
}

// low of 0
inline float randFloat( float high ) {
	return high*randFloat() ;
}

// assumes low=0
inline int randInt( int high ) {
	if( !high ) return high ;
	return rand() % high ;
}

// between low and (high-1)
inline int randInt( int low, int high ) {
	// modulus 0 is an error
	if( low==high ) return low ;
	return low + rand() % (high-low) ;
}

// maxVal is 1 over the max
inline vector<int> randomOrder( int maxVal )
{
	vector<int> results ;

	// fisher-yates
	vector<int> indices( maxVal ) ;

	// "fill the hat with numbers", in order
	for( int i = 0 ; i < maxVal ; i++ )
		indices[i] = i ;

	// indices:  0 1 2 3 4

	// Now push random indices into songIndexShuffledOrder
	// do this n times.
	for( int i = 0 ; i < maxVal ; i++ )
	{
		// starts: 5
		int numsRem = maxVal - i ;

		// random number on [0,4]
		int index = randInt( 0, numsRem ) ;

		// say you got 3, then you will add 3 into the order array:
		// indices:    0 1 2 3 4
		// songOrder:  3
		results.push_back( indices[index] ) ;

		// This next step is what prevents repeat.
		// Now swap indices, the BACK elt with the JUST SELECTED elt.
		// indices:    0 1 2 4 //3
		// Now on the next iteration, __3 will no longer be selected__,
		// because i=1, so the max # will be 3.
		::swap( indices[index], indices[numsRem-1] ) ;
	}

	return results ;
}

inline int randSign()
{
	return -1 + 2*randInt(0,2) ; // -1+0 or -1+2=+1
}


#endif