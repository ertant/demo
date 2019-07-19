#if !defined(BACKGROUND__INCLUDED_)
#define BACKGROUND__INCLUDED_

#include <d3dx9.h>
#include "vertextypes.h"

class Background
{
private:
    LPDIRECT3DVERTEXBUFFER9 m_Vertices;
    LPDIRECT3DINDEXBUFFER9 m_Indices;
    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;
    D3DXMATRIX m_World;
    LPDIRECT3DSTATEBLOCK9   m_State;
    LPDIRECT3DSTATEBLOCK9   m_OldState;
    Vertex_PositionedTextured* m_VerticeData;
    D3DMATERIAL9* m_Material;

protected:
    LPDIRECT3DDEVICE9 m_Device;

public:

    Background();

    void SetDevice(LPDIRECT3DDEVICE9 device);

    virtual void Render(LPDIRECT3DTEXTURE9 texture,float opacity);
    virtual void ResetDevice();
};


#endif