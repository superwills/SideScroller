#ifndef RECTF_H
#define RECTF_H

struct RectF
{
  // x,y is TOP LEFT corner of the rectangle
  float x,y,w,h ;
  
  RectF():x(0),y(0),w(0),h(0){}
  RectF( float ixPts, float iyPts, float iwPts, float ihPts ):x(ixPts),y(iyPts),w(iwPts),h(ihPts){}
  RectF( const Vector2f& pos, const Vector2f& size ):
    x(pos.x),y(pos.y),w(size.x),h(size.y){}
  
  Vector2f randomPoint() const {
    return Vector2f( randFloat( x, x+w ), randFloat( y, y+w ) ) ;
  }
  RectF randomRect( float width, float height ) const {
    return RectF( randFloat( x, x+w-width ), randFloat( y, y+h-height ), width, height ) ;
  }
  
  // Allows you to position your rectangle from the TL corner
  // instead of usual BL
  static RectF FromTopLeft( float ixPts, float iyPts, float iwPts, float ihPts ){
    return RectF( ixPts, iyPts-ihPts, iwPts, ihPts ) ;
  }
  static RectF FromMinsAndMaxes( const Vector2f& mins, const Vector2f& maxes ){
    return RectF( mins.x,mins.y,maxes.x-mins.x,maxes.y-mins.y ) ;
  }
  
  inline bool hit( const Vector2f& v ) const {
    return ( v.x >= left() && v.x <= right() && v.y <= bottom() && v.y >= top() ) ;
  }
  inline bool hitX( float ix ) const {
    return ( ix >= left() && ix <= right() ) ;
  }
  inline bool hitY( float iy ) const {
    return ( iy <= bottom() && iy >= top() ) ;
  }
  
  inline bool hit( const RectF& o ) const {
    // These are the miss situations
    bool hitted = !( right() < o.left()   || // definite miss
              left() > o.right()   ||
              bottom() < o.top()   ||
              top() > o.bottom() ) ;
    
    //if( hitted )      puts( "HIT" ) ;
    //else       puts( "MISS" ) ;
    return hitted ;
  }
  
  // WHAT IS THE OUTSIDE WALLS DISTANCE BETWEEN THE 2?
  inline Vector2f outsideWallDistance( const RectF& o ) const {
    // ________   ________
    // | this |   |   o  |
    // |______|   |      |
    //            |______|
    // 
    
    // If the walls OVERLAP, the distance is 0.
    Vector2f dist ;
    if( isLeftOf(o) )
      dist.x = o.left() - right() ; // +++ - I have to go +x to reach o
    else if( isRightOf(o) )
      dist.x = o.right() - left() ; // --- I have to go -x to reach o
      
    if( isBelowOf( o ) )
      dist.y = top() - o.bottom(); // +++ I have to go +y to reach o
    else if( isOnTopOf( o ) )
      dist.y = bottom() - o.top(); // --- I have to go -y to reach o
      
    return dist ;
  }
  
  inline bool isLeftOf( const RectF& o ) const {
    // ________   ________
    // | this |   |   o  |
    // |______|   |      |
    //            |______|
    // 
    return right() < o.left() ;
  }
  
  inline bool isRightOf( const RectF& o ) const {
    // my left wall is bigger than your right wall means i'm right of you
    // ________   ________
    // | o    |   | this |
    // |______|   |      |
    //            |______|
    // 
    return left() > o.right() ;
  }
  
  inline bool isBelowOf( const RectF& o ) const {
    // top below o's bottom
    return top() > o.bottom() ;
  }
  
  inline bool isOnTopOf( const RectF& o ) const {
    // bottom on top of o's top
    return bottom() < o.top() ;
  }
  
  // Default considers origin BOTTOM LEFT.
  inline float left() const { return x ; }
  inline float right() const { return x+w ; }
  
  //!! The GL coordinate system is ALWAYS origin in lowerleft.
  // so if you want to work in upside down coordinates for textboxes,
  // you still have to invert it back at EOD
  inline float bottom() const { return y + h ; }
  inline float top() const { return y ; } 
  
  inline float midX() const { return x + w/2 ; }
  inline float midY() const { return y + h/2 ; }
  
  inline bool isCanonical() const { 
    return -1.f <= x && x <= 1.f && 
           -1.f <= y && y <= 1.f && 
              0 <= w && w <= 2.f && 
              0 <= h && h <= 2.f ;
  }
  // Considers origin @ bottom left.
  inline Vector2f bottomLeft() const {
    return Vector2f( left(), bottom() ) ;
  }
  inline Vector2f bottomRight() const {
    return Vector2f( right(), bottom() ) ;
  }
  inline Vector2f topLeft() const {
    return Vector2f( left(), top() ) ;
  }
  inline Vector2f topRight() const {
    return Vector2f( right(), top() ) ;
  }
  
  // BL corner
  inline Vector2f pos() const {
    return Vector2f( x,y ) ;
  }
  inline Vector2f& xy() {
    return (Vector2f&)x ;
  }
  inline Vector2f& size() {
    return (Vector2f&)w ;
  }
  inline Vector2f centroid() const {
    return Vector2f( midX(), midY() ) ;
  }
  inline Vector2f offsetToCenter( const RectF& inner ) const {
    return centroid() - inner.centroid() ;
  }
  
  // Centers SOME OTHER RECT inside THIS
  inline RectF center( const RectF& inner ) const {
    Vector2f diff = offsetToCenter( inner ) ;
    return RectF( inner.x+diff.x, inner.y+diff.y, inner.w, inner.h ) ;
  }
  inline RectF center( const RectF& inner, const Vector2f& offsets ) const {
    Vector2f diff = offsetToCenter( inner ) ;
    return RectF( inner.x+diff.x+offsets.x, inner.y+diff.y+offsets.y, inner.w, inner.h ) ;
  }
  inline RectF centered( const Vector2f& sizes ) const
  {
    return RectF( x+w/2 - sizes.x/2, y+h/2 - sizes.y/2, sizes.x, sizes.y ) ;
  }
  
  // Sets the rect sent in the bottom left of (this) the parent rect
  inline RectF setInBottomLeft( const RectF& inner, const Vector2f& margin ) const {
    return RectF( left() + margin.x, bottom() + margin.y, inner.w, inner.h ) ;
  }
  inline RectF setInBottomRight( const RectF& inner, const Vector2f& margin ) const {
    return RectF( right() - inner.w - margin.x, bottom() + margin.y, inner.w, inner.h ) ;
  }
  inline RectF setInCenterRight( const RectF& inner, float marginx ) const {
    return RectF( right() - inner.w - marginx, (bottom() + top() - inner.h)/2, inner.w, inner.h ) ;
  }
  inline RectF setInTopLeft( const RectF& inner, const Vector2f& margin ) const {
    return RectF( left() + margin.x, top() - inner.h - margin.y, inner.w, inner.h ) ;
  }
  inline RectF setInTopCenter( const RectF& inner, float margintop ) const {
    return RectF( (left()+right())/2 - inner.w/2, top() - inner.h - margintop, inner.w, inner.h ) ;
  }
  inline RectF setInTopRight( const RectF& inner, const Vector2f& margin ) const {
    return RectF( right() - inner.w - margin.x, top() - inner.h - margin.y, inner.w, inner.h ) ;
  }
  inline RectF setInCenterLeft( const RectF& inner, float marginx ) const {
    return RectF( left() + marginx, (bottom() + top() - inner.h)/2, inner.w, inner.h ) ;
  }
    
  inline RectF operator+( const Vector2f& offset ) {
    return RectF( x+offset.x, y+offset.y, w, h ) ;
  }
  
  inline RectF& setTop( float t ) {
	y = t ;//tl corner
    return *this ;
  }
  inline RectF& setBottom( float b ) {
	h = b-y ; // tl corner
    return *this ;
  }
  inline RectF& setLeft( float l ) {
    x=l;
    return *this ;
  }
  inline RectF& setRight( float r ) {
    w = r-x;
    return *this ;
  }
  inline RectF& setCenter( const Vector2f& v ) {
    Vector2f halfSize = size()/2;
    // when you set the position of the CENTER,
    setLeft( v.x - halfSize.x ) ;
    setRight( v.x + halfSize.x ) ;
    setTop( v.y - halfSize.y ) ;//MUST BE before bottom
    setBottom( v.y + halfSize.y ) ;
    return *this ;
  }
  
  inline RectF& expandRight( float r ) {
    // take width and expand it so x+w is AT LEAST r.
    w = max( w, r-x ) ;
    return *this ;
  }
  inline RectF& expandLeft( float l ) {
    x = min( x, l ) ;
    return *this ;
  }
  inline RectF& expandTop( float t ) {
    y = min( y, t ) ;//tl corner
    return *this ;
  }
  inline RectF& expandBottom( float b ) {
    h = max( b-y, h ); // tl corner
    return *this ;
  }
  
  inline void setMinMax( const Vector2f& min, const Vector2f& max )
  {
    x=min.x;  y=min.y;
    w=max.x-min.x;
    h=max.y-min.y;
  }
  
  // Expands THIS to contain O completely.
  inline RectF& expand( const RectF& o ) {
    // THis is more complicated than just doing min/max
    // because max depends on min (because of width parameter).
    // You have to find the maximal span 
    float maxRight = max( right(), o.right() ) ;
    float minLeft = min( x, o.x ) ;
    w = maxRight - minLeft ;
    x = min( x, o.x ) ;
    
    // what if maxtop is MY top but min is YOUR BOT.
    // We have to x-check this.
    float maxTop = max( top(), o.top() ) ;
    float minBot = min( y, o.y ) ;
    h = maxTop - minBot ;
    y = min( y, o.y ) ;
    return *this ;
  }
  
  inline RectF& applyDisplacement( const Vector2f& v ) {
    return (*this += v) ;
  }
  
  inline RectF& applyDisplacement( float ix, float iy ) {
    x+=ix ;   y+=iy ;  return *this ;
  }
  
  // For offsetting a rectangle by a vector.
  inline RectF& operator+=( const Vector2f& v ) {
    x+=v.x ;  y+=v.y ; return *this ;
  }
  
  inline RectF& pad( float amount ) {
    float half=amount/2.f ;
    x -= half ;  w += amount ;
    y -= half ;  h += amount ;
    return *this ;
  }
  inline RectF& pad( float amountX, float amountY ) {
    x -= amountX/2.f ;  w += amountX ;
    y -= amountY/2.f ;  h += amountY ;
    return *this ;
  }
  inline RectF& pad( Vector2f amount ) {
    x -= amount.x / 2.f ;  w += amount.x ;
    y -= amount.y / 2.f ;  h += amount.y ;
    return *this ;
  }
  // trbl from html.
  inline RectF& pad( float topPad, float rightPad, float bottomPad, float leftPad ) {
    x -= leftPad ;  w += (rightPad+leftPad) ;
    y -= bottomPad ;  h += (topPad+bottomPad) ;
    return *this ;
  }
  
  // bounds %= 2 makes bounds 2% of itself
  inline RectF& operator%=( const Vector2f& percentage ) {
    Vector2f amt = size()*(1.f-percentage/100.f) ;
    return pad( -amt ) ;
  }
  
  inline RectF paddedCopy( float amount ) const {
    RectF padded = *this;
    return padded.pad( amount ) ;
  }
  inline RectF paddedCopy( float amountX, float amountY ) const {
    RectF padded = *this;
    return padded.pad( amountX, amountY ) ;
  }
  inline RectF paddedCopy( Vector2f amount ) const {
    RectF padded = *this;
    return padded.pad( amount ) ;
  }  
  // trbl from html.
  inline RectF paddedCopy( float topPad, float rightPad, float bottomPad, float leftPad ) const {
    RectF padded = *this ;
    return padded.pad( topPad, rightPad, bottomPad, leftPad ) ;
  }
  
  // You get a new rectangle adjusted as you specify
  inline RectF copy( float dx, float dy, float dw, float dh ) const {
    return RectF( x+dx, y+dy, w+dw, h+dh ) ;
  }
  
  inline RectF sub( float dx, float dy, float newWidth, float newHeight ) const {
    return RectF( x+dx, y+dy, newWidth, newHeight ) ;
  }
  
  // mx is margin x
  inline RectF subTopRight( float mx, float my, float newWidth, float newHeight ) const {
    return RectF( right()-(mx+newWidth), top()-(my+newHeight), newWidth, newHeight ) ;
  }
  // mx is margin x
  inline RectF subTopLeft( float mx, float my, float newWidth, float newHeight ) const {
    return RectF( left()+mx, top()-(my+newHeight), newWidth, newHeight ) ;
  }
    
  inline RectF subBottomRight( float mx, float my, float newWidth, float newHeight ) const {
    return RectF( right()-(mx+newWidth), bottom()+my, newWidth, newHeight ) ;
  }
  // mx is margin x
  inline RectF subBottomLeft( float mx, float my, float newWidth, float newHeight ) const {
    return RectF( left()+mx, bottom()+my, newWidth, newHeight ) ;
  }
  
  // Quad split, 
  void subQuad( RectF subrectsTlTrBrBl[4] ) const
  {
    float halfW=w/2.f, halfH=h/2.f;
    subrectsTlTrBrBl[0] = sub(     0, halfH, halfW, halfH ) ; //tl
    subrectsTlTrBrBl[1] = sub( halfW, halfH, halfW, halfH ) ; //tr
    subrectsTlTrBrBl[2] = sub( halfW,     0, halfW, halfH ) ; //br
    subrectsTlTrBrBl[3] = sub(     0,     0, halfW, halfH ) ; //bl
  }
  void println( const char* msg ) const {
    //printf( "%s: x=%.2f y=%.2f w=%.2f h=%.2f\n", msg,x,y,w,h ) ;
    printf( "%s: l=%.2f r=%.2f b=%.2f t=%.2f (w=%.2f, h=%.2f)\n", msg,left(),right(),bottom(),top(),w,h ) ;
  }
  void println() const {
    this->println( "" ) ;
  }
} ;

#endif