#include "VertexTypes.h"
#include "CQuad.h"
#include "macros.h"

CQuad::CQuad(void)
    {
    }

CQuad::~CQuad(void)
    {
    }

void CQuad::Release()
    {
    m_Vertices->Release();
    m_Indices->Release();
    m_State->Release();
    }

HRESULT CQuad::SetDevice(LPDIRECT3DDEVICE9 device)
    {
    m_Device = device;
    HRESULT m_HR;

    if( FAILED( m_HR = m_Device->CreateVertexBuffer(6*sizeof(Vertex_PositionedTextured),D3DUSAGE_WRITEONLY,Vertex_PositionedTextured::FVF, D3DPOOL_DEFAULT,&m_Vertices,NULL)))
        return m_HR;

    Vertex_PositionedTextured* m_VerticeData;

    if( FAILED(m_Vertices->Lock(0,0,(void**)&m_VerticeData,0)))
        DebugBreak();

    float width = 20.0f;
    float height = 20.0f;

    UpdateVertex( m_VerticeData, 0,    0.0f,   0.0f,   0.0f,   1.0f,   1.0f );
    UpdateVertex( m_VerticeData, 1,    width,  0.0f,   0.0f,   0.0f,   1.0f );
    UpdateVertex( m_VerticeData, 2,    0.0f,   height, 0.0f,   1.0f,   0.0f );
    UpdateVertex( m_VerticeData, 3,    0.0f,   height, 0.0f,   1.0f,   0.0f );
    UpdateVertex( m_VerticeData, 4,    width,  height, 0.0f,   0.0f,   0.0f );
    UpdateVertex( m_VerticeData, 5,    width,  0.0f,   0.0f,   0.0f,   1.0f );

    m_Vertices->Unlock();

    if( FAILED( m_HR = m_Device->CreateIndexBuffer(6 * sizeof(int),D3DUSAGE_WRITEONLY,D3DFMT_INDEX32,D3DPOOL_DEFAULT,&m_Indices,NULL)))
        return m_HR;

    int* m_IndiceData;

    if( FAILED(m_Indices->Lock(0,0,(void**)&m_IndiceData,0)))
        DebugBreak();

    for(int m_Index = 0; m_Index < 6; m_Index++)
        m_IndiceData[m_Index] = m_Index;

    m_Indices->Unlock();

    SetStraightMatrix();

    if ( FAILED( m_HR = m_Device->CreateStateBlock(D3DSBT_ALL,&m_State) ) )
        return m_HR;

    return S_OK;
    }

void CQuad::SetStraightMatrix()
    {

    D3DXVECTOR3 m_Eye = D3DXVECTOR3(0,0,6);
    D3DXVECTOR3 m_LookAt = D3DXVECTOR3(0,0,0);
    D3DXVECTOR3 m_UpVect = D3DXVECTOR3(0,1,0);

    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);
    D3DXMatrixPerspectiveFovLH(&m_Proj, 90.0f, 1.0f, 1.0f, 90000.0f);

    D3DXMatrixIdentity(&m_World);
    D3DXMatrixTranslation(&m_World,-10.0f,-10.0f,0.0f);

    }

void CQuad::Transform(D3DXMATRIX * transform)
    {
    m_World *= *transform;
    }

void CQuad::Render(LPDIRECT3DTEXTURE9 texture)
{
	if ( texture != NULL )
		m_Device->SetTexture(0,texture);

	m_State->Capture();

	m_Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	m_Device->SetRenderState(D3DRS_ZENABLE,FALSE);
	m_Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	m_Device->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
	m_Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
	m_Device->SetRenderState(D3DRS_LIGHTING,FALSE);

	m_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	m_Device->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_PASSTHRU);

	m_Device->SetTransform(D3DTS_VIEW,&m_View);
	m_Device->SetTransform(D3DTS_PROJECTION,&m_Proj);
	m_Device->SetTransform(D3DTS_WORLD,&m_World);

	m_Device->SetStreamSource(0,m_Vertices,0,sizeof(Vertex_PositionedTextured));
	m_Device->SetIndices(m_Indices);
	m_Device->SetFVF(Vertex_PositionedTextured::FVF);

	V(m_Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,6,0,2) );

	m_State->Apply();
}