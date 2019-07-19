#include "CMesh.h"
#include "macros.h"
#include <d3dx9.h>
#include <strsafe.h>

CMesh::CMesh(void)
    {
        UseMaterial = TRUE;
        UseMatrix = TRUE;

        vb = NULL;
        Mesh = NULL;
        Materials = NULL;
    }

CMesh::~CMesh(void)
    {
    }

HRESULT CMesh::LoadFromResource(LPDIRECT3DDEVICE9 device,WORD name)
    {
    HRESULT m_HR;

    m_Device = device;

    ID3DXBuffer*            m_ObjectAdjacency;
    ID3DXBuffer*            m_ObjectMaterialsBuffer;

    if ( FAILED( m_HR = D3DXLoadMeshFromXResource(
        NULL,
        MAKEINTRESOURCE(name),
        "MESH",
        D3DXMESH_MANAGED,
        device,
        &m_ObjectAdjacency,
        &m_ObjectMaterialsBuffer ,
        NULL,
        &NumMaterials,
        &Mesh ) ) )
        {
        return m_HR;
        }

    V(Mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
        (DWORD*)m_ObjectAdjacency->GetBufferPointer(),
        NULL,NULL,NULL ));

    // copy materials
    Materials = new D3DMATERIAL9[NumMaterials];

    D3DXMATERIAL* materialBuffer = (D3DXMATERIAL*)m_ObjectMaterialsBuffer->GetBufferPointer();

    for(DWORD index =0; index < NumMaterials ; index++)
        Materials[index] = materialBuffer[index].MatD3D;

    SAFE_RELEASE(m_ObjectAdjacency);
    SAFE_RELEASE(m_ObjectMaterialsBuffer);

    D3DXMatrixIdentity(&Matrix);

    return S_OK;
    }

HRESULT CMesh::Load( LPDIRECT3DDEVICE9 device, char * meshFile )
    {
    HRESULT m_HR;
    ID3DXBuffer*            m_ObjectAdjacency;
    ID3DXBuffer*            m_ObjectMaterialsBuffer;

    m_Device = device;

    if ( FAILED( m_HR = D3DXLoadMeshFromX(
        meshFile,
        D3DXMESH_MANAGED,
        device,
        &m_ObjectAdjacency,
        &m_ObjectMaterialsBuffer ,
        NULL,
        &NumMaterials,
        &Mesh) ) )
        return m_HR;

    Mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
        (DWORD*)m_ObjectAdjacency->GetBufferPointer(),
        NULL,NULL,NULL );

   // copy materials
    Materials = new D3DMATERIAL9[NumMaterials];

    D3DXMATERIAL* materialBuffer = (D3DXMATERIAL*)m_ObjectMaterialsBuffer->GetBufferPointer();

    for(DWORD index =0; index < NumMaterials ; index++)
        Materials[index] = materialBuffer[index].MatD3D;

    SAFE_RELEASE(m_ObjectAdjacency);
    SAFE_RELEASE(m_ObjectMaterialsBuffer);

    D3DXMatrixIdentity(&Matrix);

    return S_OK;
    }

void CMesh::Render(DWORD subSet)
{
	if ( UseMaterial == TRUE)
	{
		V(m_Device->SetMaterial(&Materials[subSet]));
	}

	V(Mesh->DrawSubset(subSet));
}

void CMesh::SetMatrix()
    {
    if ( UseMatrix )
        m_Device->SetTransform(D3DTS_WORLD,&Matrix);
    }

void CMesh::Render()
    {
    SetMatrix();

    for(DWORD i = 0; i < NumMaterials; i++)
        {
        Render(i);
        }
    }

void ** CMesh::LockVertices()
    {
    
    void** vertices;

    numVertices = Mesh->GetNumVertices();
    Mesh->GetVertexBuffer(&vb);

    vb->Lock(0,0,(void**)&vertices,0);

    return vertices;
    }

void CMesh::Unlock()
    {
    vb->Unlock();
    vb->Release();
    }

HRESULT CMesh::SetFVF( DWORD fvf )
    {
    HRESULT m_HR;

    LPD3DXMESH newMesh = NULL;

    if ( FAILED( m_HR = Mesh->CloneMeshFVF(
        D3DXMESH_MANAGED,
        fvf,
        m_Device,
        &newMesh) ) )
        {
        SAFE_RELEASE(newMesh);
        return m_HR;
        }

    SAFE_RELEASE(Mesh);

    Mesh = newMesh;

    D3DXComputeNormals( Mesh, NULL );

    return S_OK;
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

void CMesh::Release()
    {
    SAFE_RELEASE(vb);
    //SAFE_RELEASE(m_Mesh);
    }
