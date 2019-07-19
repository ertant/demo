#pragma once

#include "BaseSection.h"
#include "CQuad.h"

class topuz : BaseSection
{
private:
    LPDIRECT3DSTATEBLOCK9   m_State;

    CMesh                   m_Object;

    D3DXVECTOR3 m_Eye;
    D3DXVECTOR3 m_LookAt;
    D3DXVECTOR3 m_UpVect;

    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;

    float m_PresentsAlpha;

    D3DXMATRIX tweenMatrix;

    DWORD lastSecond;

    D3DXMATRIX m_PresentsMatrix;

    void RenderScene(BOOL glow);

public:
    virtual void Release();
    virtual void FrameMove( DWORD time , DWORD delta );
    virtual void Render();
    virtual HRESULT ResetDevice();
};
