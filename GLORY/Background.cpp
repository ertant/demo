#include "background.h"

Background::Background()
    {

    }

void Background::SetDevice(LPDIRECT3DDEVICE9 device)
    {
    m_Device = device;
    }

void Background::Render(LPDIRECT3DTEXTURE9 texture,float opacity)
    {
    //m_OldState->Capture();
    m_State->Apply();

    m_Material->Ambient.r = opacity;
    m_Material->Ambient.g = opacity;
    m_Material->Ambient.b = opacity;

    m_Device->SetMaterial(m_Material);

    m_Device->SetTexture(0,texture);

    m_Device->SetTransform(D3DTS_VIEW,&m_View);
    m_Device->SetTransform(D3DTS_PROJECTION,&m_Proj);
    m_Device->SetTransform(D3DTS_WORLD,&m_World);

    m_Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,6,0,2);

    //m_OldState->Apply();
    }



void Background::ResetDevice()
    {
    if( FAILED(m_Device->CreateVertexBuffer(6*sizeof(Vertex_PositionedTextured),D3DUSAGE_WRITEONLY,Vertex_PositionedTextured::FVF, D3DPOOL_DEFAULT,&m_Vertices,NULL)))
        DebugBreak();    

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

    if( FAILED(m_Device->CreateIndexBuffer(6,D3DUSAGE_WRITEONLY,D3DFMT_INDEX32,D3DPOOL_DEFAULT,&m_Indices,NULL)))
        DebugBreak();

    int* m_IndiceData;

    if( FAILED(m_Indices->Lock(0,0,(void**)&m_IndiceData,0)))
        DebugBreak();

    for(int m_Index = 0; m_Index < 6; m_Index++)
        m_IndiceData[m_Index] = m_Index;

    m_Indices->Unlock();

    D3DXVECTOR3 m_Eye = D3DXVECTOR3(0,0,6);
    D3DXVECTOR3 m_LookAt = D3DXVECTOR3(0,0,0);
    D3DXVECTOR3 m_UpVect = D3DXVECTOR3(0,1,0);

    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);
    D3DXMatrixPerspectiveFovLH(&m_Proj, 90.0f, 1.0f, 0.001f, 90000.0f);

    D3DXMatrixIdentity(&m_World);
    D3DXMatrixTranslation(&m_World,-10.0f,-10.0f,0.0f);

    m_Material = new D3DMATERIAL9();

    //m_Device->CreateStateBlock(D3DSBT_ALL,&m_OldState);

    if( FAILED(m_Device->BeginStateBlock()))
        DebugBreak();

    m_Device->SetRenderState(D3DRS_LIGHTING,TRUE);
    m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
    //m_Device->SetRenderState(D3DRS_SPECULARENABLE,TRUE);
    //m_Device->SetRenderState(D3DRS_SPECULARMATERIALSOURCE,D3DMCS_MATERIAL);
    //m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
    //m_Device->SetRenderState(D3DRS_SRCBLENDALPHA,D3DBLEND_SRCCOLOR);
    //m_Device->SetRenderState(D3DRS_DESTBLENDALPHA,D3DBLEND_DESTCOLOR);

    m_Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
    m_Device->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_XRGB(255,255,255));

    m_Device->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_PASSTHRU);
    m_Device->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);

    m_Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
    m_Device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);

    m_Device->SetStreamSource(0,m_Vertices,0,sizeof(Vertex_PositionedTextured));
    m_Device->SetIndices(m_Indices);
    m_Device->SetFVF(Vertex_PositionedTextured::FVF);

    m_Device->EndStateBlock(&m_State);
    }