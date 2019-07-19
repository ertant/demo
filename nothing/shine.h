#pragma once

#include "BaseSection.h"

class Shine : BaseSection
{
private:
    LPDIRECT3DSTATEBLOCK9   m_State;
    CMesh   m_ShinyThing;
    CMesh   m_CenterObject;

    D3DXVECTOR3 m_Eye;
    D3DXVECTOR3 m_LookAt;
    D3DXVECTOR3 m_UpVect;

    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;

    D3DXMATRIX tweenMatrix;

    int filter;

    void SetLight();
    void RenderScene();

public:
    Shine(void);
    ~Shine(void);

    virtual void Release();
    virtual void FrameMove( DWORD time , DWORD delta);
    virtual void Render();
    virtual HRESULT ResetDevice();
};
