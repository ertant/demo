#pragma once

#include "d3d9.h"

class CQuad
{
private:
    LPDIRECT3DVERTEXBUFFER9 m_Vertices;
    LPDIRECT3DINDEXBUFFER9 m_Indices;
    LPDIRECT3DDEVICE9 m_Device;
    LPDIRECT3DSTATEBLOCK9   m_State;

public:

    void Release();

    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;
    D3DXMATRIX m_World;

    void SetStraightMatrix();

    void Transform(D3DXMATRIX * transform);

    HRESULT SetDevice(LPDIRECT3DDEVICE9 device);

    void Render(LPDIRECT3DTEXTURE9 texture);

    CQuad(void);
    ~CQuad(void);
};
