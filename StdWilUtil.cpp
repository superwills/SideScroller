#include "StdWilUtil.h"

// THESE ARE ABOUT CONVENIENCE, NOT EFFICIENCY.
// There are A LOT of allocations that go on here:
//  - the 4096 chr buffer
//  - the copy of the 4096 chr buffer (which is truncated at
//    exactly the right len)
// If we let msgBuffer be static, it will perform better but
// then won't be threadsafe.
string makeString( const char *fmt, ... )
{
  va_list args ;
  va_start( args, fmt ) ;
  char msgBuffer[ 4096 ] ; // max len.  Can also use a strlen call to avoid over alloc
  vsprintf( msgBuffer, fmt, args ) ;
  return string( msgBuffer ) ; // ALWAYS COPIES WHAT'S IN MSGBUFFER.
}

// Makes std c++ string from fmt and args.
string makeString( const char *fmt, va_list args )
{
  char msgBuffer[ 4096 ] ;  // oops. Had a 623 char error (from shader) and it err-d out.
  vsprintf( msgBuffer, fmt, args ) ;
  return string( msgBuffer ) ; // wrap it up
}

string makeTime( double s )
{
  unsigned long long floorSec = s ;
  
  unsigned long long hours = floorSec / (60 * 60) ;
  int minutes = (floorSec / 60) % 60 ;
  int seconds = floorSec % 60 ;

  double ms = s - floorSec ;
  int msInt = (int)(ms*1000.) ;
  
  if( !hours )
    return makeString( "%d:%02d.%03d", minutes, seconds, msInt ) ;
  else
    return makeString( "%lld:%02d:%02d.%03d", hours, minutes, seconds, msInt ) ;
}