#pragma once

#include "BaseSection.h"

class Greets : BaseSection
{
private:
    LPDIRECT3DSTATEBLOCK9   m_State;
    CMesh   m_Top;
    CMesh   m_Monitors;

    D3DXVECTOR3 m_Eye;
    D3DXVECTOR3 m_LookAt;
    D3DXVECTOR3 m_UpVect;

    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;

    float vel;
    int textureIndex;

    D3DXMATRIX textMatrix;

    D3DXVECTOR3 m_TopAngle;

    void SetLight();
    void RenderScene();

public:
    Greets(void);
    ~Greets(void);

    virtual void Release();
    virtual void FrameMove( DWORD time , DWORD delta);
    virtual void Render();
    virtual HRESULT ResetDevice();
};