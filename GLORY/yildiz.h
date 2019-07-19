#if !defined(YILDIZ__INCLUDED_)
#define YILDIZ__INCLUDED_

#include "BaseSection.h"
#include "CMesh.h"

class Yildiz : BaseSection
{
private:
    /*
    LPDIRECT3DSTATEBLOCK9   m_State;
   
    D3DMATERIAL9*           m_Material;
    D3DLIGHT9*              m_Light;
    ID3DXBuffer*            m_ObjectAdjacency;
    ID3DXBuffer*            m_ObjectMaterials;
    DWORD                   m_ObjectNumMaterial;
    ID3DXMesh*              m_Mesh;
    D3DXMATRIX              m_MeshMatrix;
    */

    CMesh m_Mesh;

    int number;
    float opacity;
    float opacity_vel;

public:
    virtual void Cleanup();
    virtual void FrameMove();
    virtual void Render();
    virtual HRESULT ResetDevice();
    virtual void SyncMeta( DWORD data );
};

#endif
