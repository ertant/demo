#if !defined(TUNNEL__INCLUDED_)
#define TUNNEL__INCLUDED_

#include "BaseSection.h"

class Tunnel : BaseSection
{
private:
    LPDIRECT3DVERTEXBUFFER9 m_Vertices;
    LPDIRECT3DINDEXBUFFER9 m_Indices;
    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;
    LPDIRECT3DSTATEBLOCK9   m_State;
    Vertex_PosColTextNorm* m_VerticeData;
    D3DXMATRIX m_TransformMatrix;    
    D3DLIGHT9* m_Light;

    int m_TunnelHalkaAdedi;
    int m_TunnelHalkaNoktaAdedi;
    float m_TunnelHiz;
    float m_TunnelTextureHiz;
    float m_TunnelHalkaAralik;

    int m_TunnelIndexCount;
    int m_TunnelVertexCount;

    float m_TunnelHareketOrani;
    float m_TunnelFarSpeed;
    float m_TunnelX1;
    float m_TunnelX2;
    float m_TunnelY1;
    float m_TunnelY2;

    float m_TunnelTextureScale;
    float m_AngleFactor;

    float frame;

    void Tunnel::UpdateVertexBuffer();

public:
    virtual void FrameMove();
    virtual void Render();
    virtual HRESULT ResetDevice();
    virtual void SyncMeta( DWORD data );
};

#endif