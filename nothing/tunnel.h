#pragma once

#include "BaseSection.h"

class Tunnel : BaseSection
{
private:
    LPDIRECT3DSTATEBLOCK9   m_State;
    CMesh   m_Tunnel;
    CMesh   m_Ship;

    D3DXVECTOR3 m_Eye;
    D3DXVECTOR3 m_LookAt;
    D3DXVECTOR3 m_LookAt2;
    D3DXVECTOR3 m_UpVect;

    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;

    void SetLight();
    void RenderScene();

    D3DXMATRIX textMatrix;

    D3DXMATRIX scaleBarMatrix;

public:
    Tunnel(void);
    ~Tunnel(void);

    virtual void Release();
    virtual void FrameMove( DWORD time , DWORD delta);
    virtual void Render();
    virtual HRESULT ResetDevice();
};
