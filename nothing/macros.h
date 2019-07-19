#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef DEGREE
#define DEGREE(angle)            ( angle * (D3DX_PI / 180) )
#endif


#ifndef V

    #if defined(DEBUG) | defined(_DEBUG)
        #define V(exp)						\
			if (FAILED( exp ))				\
				DebugBreak();
    #else
        #define V(exp)  exp
    #endif

#endif