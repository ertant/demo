#if !defined(TOPUZ__INCLUDED_)
#define TOPUZ__INCLUDED_

#include "BaseSection.h"

class topuz : BaseSection
{
private:
    LPDIRECT3DSTATEBLOCK9   m_State;

    //D3DMATERIAL9*           m_Material;
    //D3DLIGHT9*              m_Light;
    //LPDIRECT3DTEXTURE9      m_ObjectTexture;
    ID3DXBuffer*            m_ObjectAdjacency;
    ID3DXBuffer*            m_ObjectMaterials;
    DWORD                   m_ObjectNumMaterial;
    ID3DXMesh*              m_Mesh;
    //D3DXMATRIX              m_MeshMatrix;
    //LPDIRECT3DTEXTURE9      m_BackgroundTexture;

    D3DXVECTOR3 m_Eye;
    D3DXVECTOR3 m_LookAt;
    D3DXVECTOR3 m_UpVect;

    int frame;

public:
    virtual void SyncMeta( DWORD data );
    virtual void Cleanup();
    virtual void FrameMove();
    virtual void Render();
    virtual HRESULT ResetDevice();
};

#endif