#if !defined(CUBUKLAR__INCLUDED_)
#define CUBUKLAR__INCLUDED_

#include "BaseSection.h"
#include "CMesh.h"

class cubuklar : BaseSection
{
private:
    /*
    LPDIRECT3DSTATEBLOCK9   m_State;
    LPDIRECT3DSTATEBLOCK9   m_State2;

    D3DMATERIAL9           m_Material;
    ID3DXBuffer*            m_ObjectAdjacency;
    ID3DXBuffer*            m_ObjectMaterials;
    DWORD                   m_ObjectNumMaterial;
    ID3DXMesh*              m_Mesh;
    D3DXMATRIX              m_MeshMatrix;
    */

    CMesh m_Taslar;

    int number;

    float opacity;
    float opacity_vel;

    D3DXVECTOR3 m_Eye;
    D3DXVECTOR3 m_LookAt;
    D3DXVECTOR3 m_UpVect;

    LPD3DXSPRITE m_Sprite;

public:
    virtual void Cleanup();
    virtual void FrameMove();
    virtual void Render();
    virtual HRESULT ResetDevice();
    virtual void SyncMeta( DWORD data );
};

#endif