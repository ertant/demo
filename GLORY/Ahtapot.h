#include <d3d9.h>
#include <d3dx9math.h>

#if !defined(AHTAPOT__INCLUDED_)
#define AHTAPOT__INCLUDED_

class AhtapotKol
{
private:
    LPDIRECT3DVERTEXBUFFER9 m_Vertices;
    LPDIRECT3DINDEXBUFFER9 m_Indices;
};

class Ahtapot
{
private:
    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;
    D3DXMATRIX m_World;
    LPDIRECT3DSTATEBLOCK9   m_State;

    AhtapotKol* m_Kollar;

    int KolAdedi;

protected:
    LPDIRECT3DDEVICE9 m_Device;

public:

    Ahtapot();

    void SetDevice(LPDIRECT3DDEVICE9 device);

    virtual void Render(LPDIRECT3DTEXTURE9 texture);
    virtual void ResetDevice();
};



#endif