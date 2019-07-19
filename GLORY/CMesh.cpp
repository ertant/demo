#include "CMesh.h"

CMesh::CMesh(void)
{
}

CMesh::~CMesh(void)
{
}

HRESULT CMesh::Load( LPDIRECT3DDEVICE9 device, char * meshFile )
{
    HRESULT m_HR;

    if ( FAILED( m_HR = D3DXLoadMeshFromX(
        meshFile,
        D3DXMESH_SYSTEMMEM,
        device,
        &m_ObjectAdjacency,
        &m_ObjectMaterials ,
        NULL,
        &m_ObjectNumMaterial,
        &m_Mesh) ) )
        return m_HR;

    D3DXMatrixIdentity(&Matrix);

    return S_OK;
}

void CMesh::Render()
{
    for(DWORD i = 0; i < m_ObjectNumMaterial; i++)
        m_Mesh->DrawSubset(i);
}

void CMesh::RotateMeshX( float angle )
{
    D3DXMATRIX m_Orientation;

    D3DXMatrixRotationX(&m_Orientation, angle );

    Matrix *= m_Orientation;
}

void CMesh::RotateMeshY( float angle )
{
    D3DXMATRIX m_Orientation;

    D3DXMatrixRotationY(&m_Orientation, angle );

    Matrix *= m_Orientation;
}

void CMesh::RotateMeshZ( float angle )
{
    D3DXMATRIX m_Orientation;

    D3DXMatrixRotationZ(&m_Orientation, angle );

    Matrix *= m_Orientation;
}

void CMesh::RotateMeshXYZ( float x, float y, float z )
{
    D3DXMATRIX m_Orientation;

    D3DXMatrixRotationX(&m_Orientation, x );
    Matrix *= m_Orientation;
    
    D3DXMatrixRotationY(&m_Orientation, y );
    Matrix *= m_Orientation;

    D3DXMatrixRotationZ(&m_Orientation, z );
    Matrix *= m_Orientation;
}

void CMesh::Translate( float x, float y, float z )
{
    D3DXMATRIX m_Orientation;
    D3DXMatrixTranslation(&m_Orientation,x,y,z);   
    Matrix *= m_Orientation;
}

void CMesh::ScaleMesh( float x, float y, float z )
{
    D3DXMATRIX m_Orientation;

    D3DXMatrixScaling(&m_Orientation, x, y, z );

    Matrix *= m_Orientation;
}

void CMesh::ScaleMesh( float range )
{
    Matrix *= range;
}


