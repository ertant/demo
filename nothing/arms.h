#pragma once

#include "BaseSection.h"

class Arms : BaseSection
{
private:
    LPDIRECT3DSTATEBLOCK9   m_State;

    D3DXVECTOR3 m_Eye;
    D3DXVECTOR3 m_LookAt;
    D3DXVECTOR3 m_UpVect;

    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;

    D3DXMATRIX m_Rotation;

    D3DXMATRIX textMatrix;

    LPDIRECT3DVERTEXBUFFER9 m_Vertices;
    LPDIRECT3DINDEXBUFFER9 m_Indices;

    int NumArms;
    int NumArmPoints;
    float ArmPointSpace;
    int VerticeCount;

    void RenderScene();

    void MoveArmPoint( Vertex_PosColSize * vertice , int index ,int arm, int armPoint );
    void MoveArms(DWORD time, DWORD delta);

    CMesh leftThing;

public:
    Arms(void);
    ~Arms(void);

    virtual void Release();
    virtual void FrameMove( DWORD time , DWORD delta);
    virtual void Render();
    virtual HRESULT ResetDevice();
};
