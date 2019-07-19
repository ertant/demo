#if !defined(BASESECTION__INCLUDED_)
#define BASESECTION__INCLUDED_

#include <d3dx9.h>
#include "macros.h"

#include "texturecache.h"
#include "vertexTypes.h"
#include "cmesh.h"
#include "effects.h"

class BaseSection 
{
protected:
    LPDIRECT3DDEVICE9 m_Device;
    TextureCache* m_TextureCache;
    Effects* m_Effects;

public:
    BaseSection();

    BOOL Finished;

    void SetDevice(LPDIRECT3DDEVICE9 device,TextureCache * cache,Effects * effects);

    virtual void FrameMove(DWORD time,DWORD delta);
    virtual void Render();
    virtual HRESULT ResetDevice();

    virtual void Release();
};

#endif