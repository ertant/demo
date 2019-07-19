#pragma once

#include "BaseSection.h"

class Planes : BaseSection
{
private:
    LPDIRECT3DSTATEBLOCK9   m_State;
    CMesh   m_PlanesSey;

    D3DXVECTOR3 m_Eye;
    D3DXVECTOR3 m_LookAt;
    D3DXVECTOR3 m_UpVect;

    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;

    D3DXMATRIX textMatrix;
    D3DXMATRIX textMatrix2;

    int planeCount;
    D3DXVECTOR4 * planes;

    float glow;

    void RenderScene();

public:
    Planes(void);
    ~Planes(void);

    virtual void Release();
    virtual void FrameMove( DWORD time , DWORD delta);
    virtual void Render();
    virtual HRESULT ResetDevice();
};
