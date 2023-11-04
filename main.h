//	main.h

#pragma warning( disable:4786)
#pragma warning( disable:4005)
#pragma warning( default:4355)

//add WSOCK32.LIB in Progect\Setings...\Link
#include "winsock.h"

#include <windows.h>
#include <iostream.h>
#include <time.h>
//#include <ctype.h>
#include <math.h>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <direct.h>


//--------------------------------------------------------------------------------------------------

#define DEF_PCONS_INCLUDE
#include "list_h.h"
#undef DEF_PCONS_INCLUDE

using namespace std;


#define DEF_PCONS_CLASS
#include "list_h.h"
#undef DEF_PCONS_CLASS


#define DEF_PCONS_TYPES
#include "list_h.h"
#undef DEF_PCONS_TYPES


#define DEF_PCONS_CONTENT
#include "list_h.h"
#undef DEF_PCONS_CONTENT

//--------------------------------------------------------------------------------------------------

bool Ajax(string Host_name,int port,string method,string heads,string body,string& Response);
V_pCVARIANT::iterator find_pointer(V_pCVARIANT::iterator A,const V_pCVARIANT::iterator B,const CVARIANT&V);

//	main.h	:-|