#pragma once

#include "BaseSection.h"

class Uzun : BaseSection
{
private:
    LPDIRECT3DSTATEBLOCK9   m_State;
    CMesh   m_Borular;
    CMesh   m_Topuk;

    D3DXVECTOR3 m_Eye;
    D3DXVECTOR3 m_LookAt;
    D3DXVECTOR3 m_UpVect;

    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;

    D3DXMATRIX textMatrix;
    D3DXMATRIX textMatrix2;

    D3DXMATRIX backMatrix;


    void SetLight(float range,float diffuse);

    void RenderScene();

public:
    Uzun(void);
    ~Uzun(void);

    virtual void Release();
    virtual void FrameMove( DWORD time , DWORD delta);
    virtual void Render();
    virtual HRESULT ResetDevice();
};
