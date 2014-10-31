// Contains RAY/SPHERE, but RAY/TRIANGLE was put
// inside the Triangle class
// t BETWEEN 0 AND LEN, __NOT 0 AND 1__.
struct Ray
{
  Vector3f start, end ;
  float len ;
  Vector3f dir, fullLengthDir ;
  
  // This has got to be allowed to avoid ray construction ALWAYS having
  // recomputeDir get called.
  Ray(){}
  
  Ray( const Vector3f& iStart, const Vector3f& iEnd )
  {
    start=iStart ;
    end=iEnd ;
    recomputeDir() ;
  }
  
  // It's rude not to send a normalized vector.
  Ray( const Vector3f& iStart, const Vector3f& iNormalizedDirection, float iLen ){
    start = iStart ;
    dir = iNormalizedDirection ;
    len = iLen ;
    fullLengthDir = iNormalizedDirection*len ;
    end = start + fullLengthDir ;
  }
  
  static Ray FromCenter( const Vector3f& center, const Vector3f& iDir, float halfLen ) {
    return Ray( center - iDir*halfLen, center + iDir*halfLen ) ;
  }
  
  void println( const char* msg ) const {
    printf( "%s, (%f) ", msg, len ) ;
    start.print( " start:" ) ;
    end.print( " end:" ) ;
    
    fullLengthDir.println( " fldir:" ) ;
  }
  
  Ray& recomputeDir() {
    fullLengthDir = end - start ; // compute the full length direction vector
    len = fullLengthDir.len() ;   // keep len
    dir = fullLengthDir/len ;     // normalized dir
    return *this ;
  }
  
  Ray& wrap( const Vector3f& worldSize ) {
    // the ray only wraps if the start wrapped.  they both wrap together.
    Vector3f startWrap = start.wrappedCopy( worldSize ) ;
    Vector3f offsets = startWrap - start ;
    start += offsets ;
    end += offsets ;
    return *this ;
  }
  
  Ray& setStart( const Vector3f& newStart )
  {
    // it keeps the same END so the dir changes
    start = newStart ;
    return recomputeDir();
  }
  
  Ray& setEnd( const Vector3f& newEnd )
  {
    // it keeps the same END so the dir changes
    end = newEnd ;
    return recomputeDir() ;
  }
  
  // MOVES THE END so that the ray still starts at the same pos
  // but has this new length
  // CHANGES: end, len, fullLengthDir
  Ray& setLength( float newLen )
  {
    len=newLen;
    fullLengthDir = dir*len;
    end = start + fullLengthDir;
    return *this ;
  }
  
  // advance or move up along ray
  Ray& advanceStartAlongRay( float t )
  {
    // Change start while keeping end and dir the same
    start += dir * t ;
    len = len - t ; // you have taken t units off len.
    
    // turn dir around, for you have overshot endpoint
    if( len < 0.f ){
      len = - len ;
      dir = - dir ;
    }

    fullLengthDir = dir*len ; // recompute this

    return *this ;
  }
  
  Ray& operator*=( const Vector3f& v ){
    start*=v;  end*=v;
    return recomputeDir() ;
  }
  Ray& operator/=( const Vector3f& v ){
    start/=v;  end/=v;
    return recomputeDir() ;
  }
  Ray& operator+=( const Vector3f& v ){
    start+=v ;  end+=v ;
    return *this ;
  }
  Ray& operator-=( const Vector3f& v ){
    start-=v ;  end-=v ;
    return *this ;
  }
  
  Ray operator/( const Vector3f& v ) const {
    Ray ray( start/v, end/v ) ;
    return ray ;
  }
  Ray operator*( const Vector3f& v ) const {
    Ray ray( start*v, end*v ) ;
    return ray ;
  }
  Ray operator+( const Vector3f& v ) const {
    Ray ray(*this) ; // copying it is cheaper than constructing a new one (no .len() operation)
    ray.start+=v, ray.end+=v ;
    return ray ;
  }
  Ray operator-( const Vector3f& v ) const {
    Ray ray(*this) ;
    ray.start-=v, ray.end-=v ;
    return ray ;
  }
  
  // t should be between 0 and len.
  // if you're using NORMALIZED (0..1) values
  // then manually do start+fullLengthDir*t.
  inline Vector3f at( float t ) const {
    return start + dir*t ;
  }
  
  inline Vector3f at01( float t ) const {
    return start + fullLengthDir*t ;
  }
  
  // 0, 1 or 2 hits.
  int intersectsSphere( const Sphere& sphere, Vector3f& closerPt, Vector3f& fartherPt ) const
  {
    Vector3f f = start - sphere.c ;

    //float a = 1.f ; // dir.dot(dir); // dir is normalized so it will always be a 1.0f
    float b = 2.f*f.dot( dir ) ;
    float c = f.dot( f ) - sphere.r * sphere.r ;
    
    float disc = b*b - 4.f*c ;
    if( disc < 0.f )  return 0 ;
    
    disc = sqrtf( disc ) ;
    float t1 = (-b-disc) / 2.f ; // t1 is ALWAYS SMALLER than t2
    float t2 = (-b+disc) / 2.f ;
    
    //// 4x HIT cases:
    //       -o->       --|-->  |            |  --|->
    // Impale(t1,t2), Poke(t1,t2>len), ExitWound(t1<0, t2), 
    //
    // | -> |
    // CompletelyInside(t1<0, t2>len)
    if( isBetweenOrdered( t1, 0.f, len ) )
    {
      // Poke or, Impale cases
      closerPt = at( t1 ) ;        // t1 hits.
      if( isBetweenOrdered( t2, 0.f, len ) ) {// impale
        fartherPt = at( t2 ) ;
        return 2 ;
      }
      else { // poke
        fartherPt = closerPt ; // the "farther pt" is the same as the closer pt (only 1 intn)
        return 1 ;
      }
    }
    
    else if( isBetweenOrdered( t2, 0.f, len ) ) // only t2 valid, ExitWound, since not impale
    {
      // t1 MUST BE < 0.f here ( t1 happened BEFORE START OF RAY ), you
      // are starting INSIDE the sphere.
      closerPt=fartherPt=at( t2 ) ;
      return 1 ;
    }
    else if( t1 < 0.f && t2 > len ) // CompletelyInside
    {
      // t1 occurred behind start of ray, and t2 occurred BEYOND end of ray
      // technically YOU DID NOT HIT THE SPHERE, but you are inside it.
      // So I return TRUE here and give you the start and end of you ray.
      closerPt = start ;
      fartherPt = end ;
      return 1 ;
    }
    
    // 2x MISS cases:
    //       ->  o                     o ->              
    // FallShort (t1>len,t2>len), Past (t1<0,t2<0)
    return 0 ;
  }
  
  int intersectsSphere( const Sphere& sphere, Vector3f& closerPt ) const
  {
    Vector3f fartherPt ;
    return intersectsSphere( sphere, closerPt, fartherPt ) ;
  }
  
  // Yes or no answer
  bool intersectsSphere( const Sphere& sphere ) const
  {
    //If E is the starting point of the ray,
    //.. and L is the end point of the ray,
    //.. and C is the center of sphere you're testing against
    //.. and r is the radius of that sphere
    Vector3f f = start - sphere.c ; // vector from center sphere to ray start
    
    //float a = 1.f ; // dir.dot(dir); // dir is normalized so it will always be a 1.0f
    float b = 2.f*f.dot( dir ) ;
    float c = f.dot( f ) - sphere.r * sphere.r ;
    
    float disc = b*b - 4.f*c ; //*a ;
    if( disc < 0.f )  return 0 ;

    disc = sqrtf( disc ) ;
    float t1 = (-b-disc) / 2.f ; //originally float t1 = (-b-disc)/(2.f*a);
    float t2 = (-b+disc) / 2.f ;
    
    //// 4x HIT cases:
    //       -o->       --|-->  |            |  --|->
    // Impale(t1,t2), Poke(t1,t2>len), ExitWound(t1<0, t2), 
    //
    // | -> |
    // CompletelyInside(t1<0, t2>len)
    
    // 2x MISS cases:
    //       ->  o                     o ->              
    // FallShort (t1>len,t2>len), Past (t1<0,t2<0)
    
    return( (t1 >= 0.f && t1 <= len) || // Poke, Impale
            (t2 >= 0.f && t2 <= len) || // ExitWound
            (t1 < 0.f  && t2 > len) // CompletelyInside
          ) ;
  }
  
  // Positive if hit, negative if fail.
  // Returns the forward distance from the
  // ray origin to the penetration point.
  float intersectsSphereDistance( const Sphere& sphere ) const
  {
    Vector3f f = start - sphere.c ; // vector from center sphere to ray start
    
    float a = 1.f ; // dir.dot(dir); // dir is normalized so it will always be a 1.0f
    float b = 2.f*f.dot( dir ) ;
    float c = f.dot( f ) - sphere.r * sphere.r ;
    
    float disc = b*b - 4.f*a*c ;
    if( disc >= 0.f )
    {
      disc = sqrtf( disc ) ;
      float t1 = (-b-disc)/(2.f*a);
      float t2 = (-b+disc)/(2.f*a);
      
      if( t1 < 0.f && t2 > len )
      {
        // t1 has to move backward to hit,
        // t2 has to move forward to hit
        // THIS HAPPENS A LOT when you shoot a missile and you're already inside
        // the bv of your target.
        //info( "RAY COMPLETELY INSIDE SPHERE" ) ;
        
        // the t returned is supposed to be ON THE SURFACE OF THE SPHERE,
        // so you could push it out to where the exit point would be..
        // but that would be bad.  maybe just return ORIGIN of ray (0)
        return 0.f ;
      }
      
      else if( t1 >= 0.f && t1 <= len )
      {
        // t1 hit and t1 is closer than t2 (always)
        return t1 ;
      }
      
      else if( t2 >= 0.f && t2 <= len )
      {
        // t2 hit, exit wound
        return t2 ;
      }
    }
    
    // fail
    return -1.0f ;
  }
  
  // t between 0 and 1
  inline float normalizedDistanceToPoint( const Vector3f& pt ) const {
    // http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
    // The 2nd factor of length NORMALIZES the t, so its range is between 0 and 1.
    return - ( ( start - pt ).dot( fullLengthDir ) / fullLengthDir.len2() ) ;
  }
  
  float normalizedDistanceToPoint( const Vector3f& pt, Vector3f& ptOnLine ) const {
    float t = normalizedDistanceToPoint( pt ) ;
    ptOnLine = at( t*len ) ; // do not change t
    return t ;
  }
  
  // the REAL distance
  float distanceToPoint( const Vector3f& pt ) const {
    return len * normalizedDistanceToPoint( pt ) ;
  }
  
  // Gets you the ray's |_ distance to the point.
  float distanceToPoint( const Vector3f& pt, Vector3f& ptOnLine ) const {
    float t = normalizedDistanceToPoint( pt ) ;
    t *= len ;
    ptOnLine = at( t ) ;
    return t ;
  }
  
} ;

inline Ray operator*( const Matrix4f& matrix, const Ray& oRay ) {
  return Ray( matrix * oRay.start, matrix * oRay.end ) ;
}

inline Ray operator*( const Matrix3f& matrix, const Ray& oRay ) {
  return Ray( matrix * oRay.start, matrix * oRay.end ) ;
}