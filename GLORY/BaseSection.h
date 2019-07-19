#if !defined(BASESECTION__INCLUDED_)
#define BASESECTION__INCLUDED_

#include <d3dx9.h>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#include "background.h"
#include "texturecache.h"
#include "vertexTypes.h"
#include "textwriter.h"

class BaseSection 
{
protected:
    LPDIRECT3DDEVICE9 m_Device;
    Background* m_Background;
    TextureCache* m_TextureCache;
    TextWriter* m_Writer;

public:
    BaseSection();

    BOOL Finished;

    void SetDevice(LPDIRECT3DDEVICE9 device,TextureCache * cache);
    void SetBackground(Background* background);

    virtual void FrameMove();
    virtual void Render();
    virtual HRESULT ResetDevice();

    virtual void SyncMeta( DWORD data );
    virtual void Cleanup();
};

#endif