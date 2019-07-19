#include <d3d9.h>
#include <d3dx9math.h>

#include "tunnel.h"

void Tunnel::FrameMove()
{
    frame+=0.1f;
}

void Tunnel::SyncMeta( DWORD data )
{
    if ( data == 0x60 )
        Finished = TRUE;
}

HRESULT Tunnel::ResetDevice()
    {
    HRESULT m_HR;
    BaseSection::ResetDevice();

    m_AngleFactor = D3DX_PI / 180;

    m_TunnelHalkaAdedi = 24;
    m_TunnelHalkaNoktaAdedi = 24;
    m_TunnelHiz = 5.2f;
    m_TunnelTextureHiz = 0.15f;
    m_TunnelHalkaAralik = -1.3f;

    m_TunnelIndexCount = ( 2 * 3 * m_TunnelHalkaNoktaAdedi );
    m_TunnelVertexCount = ( 2 * 3 * m_TunnelHalkaNoktaAdedi );

    m_TunnelHareketOrani = 0.27f;
    m_TunnelFarSpeed = 5.0f;
    m_TunnelX1 = -0.65f;
    m_TunnelX2 = 1.4f;
    m_TunnelY1 = 0.7f;
    m_TunnelY2 = -0.9f;

    m_TunnelTextureScale = 4.0f;
    frame = 0.0f;

    if( FAILED( m_HR = m_Device->CreateIndexBuffer(m_TunnelIndexCount * sizeof(int),D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC ,D3DFMT_INDEX32,D3DPOOL_DEFAULT,&m_Indices,NULL)))
        return m_HR;

    int* m_IndiceData;

    if( FAILED( m_HR = m_Indices->Lock(0,0,(void**)&m_IndiceData,0)))
        return m_HR;

    for(int m_Index = 0; m_Index < m_TunnelIndexCount; m_Index++)
        m_IndiceData[m_Index] = m_Index;

    if( FAILED( m_HR = m_Indices->Unlock()))
        return m_HR;

    if( FAILED( m_HR = m_Device->CreateVertexBuffer(m_TunnelVertexCount*sizeof(Vertex_PosColTextNorm),D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,Vertex_PosColTextNorm::FVF, D3DPOOL_DEFAULT,&m_Vertices,NULL)))
        return m_HR;

    //m_Device.RenderState.AlphaTestEnable = true;
    //m_Device.RenderState.ReferenceAlpha = 0x08;
    //m_Device.RenderState.AlphaFunction = Compare.GreaterEqual;

    if( FAILED( m_HR = m_Device->BeginStateBlock()))
        return m_HR;

    m_Device->SetRenderState(D3DRS_LIGHTING,FALSE);
    m_Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
    m_Device->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_XRGB(10,10,10));
    //m_Device->SetRenderState(D3DRS_ZENABLE,TRUE);
    //m_Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
    m_Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);

    m_Device->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_PASSTHRU);
    m_Device->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);

    m_Device->SetTexture(0,m_TextureCache->Textures[TextureCache::TUNNEL]);

    m_Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
    m_Device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);

    D3DXVECTOR3 m_Eye = D3DXVECTOR3(0,0,0.01f);
    D3DXVECTOR3 m_LookAt = D3DXVECTOR3(0,0,0);
    D3DXVECTOR3 m_UpVect = D3DXVECTOR3(0,1,0);
    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;

    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);
    D3DXMatrixPerspectiveFovLH(&m_Proj, D3DX_PI / 4, 1.3f, 1, 100);

    m_Device->SetTransform(D3DTS_VIEW,&m_View);
    m_Device->SetTransform(D3DTS_PROJECTION,&m_Proj);

    m_Device->EndStateBlock(&m_State);

    return S_OK;
    }

void Tunnel::UpdateVertexBuffer()
    {
    float m_acistep = 360.0f / m_TunnelHalkaAdedi;
    float m_colorfactor = 1.0f;
    float m_colorfactordelta = 1.0f / ( m_TunnelHalkaAdedi + 1 );

    float m_transx = 0.0f;
    float m_transy = 0.0f;
    float m_tunnelhareketi = frame * m_TunnelHiz;
    float m_texturehareketi = frame * m_TunnelTextureHiz;

    D3DXMatrixIdentity(&m_TransformMatrix);

    for(int halka=0; halka < m_TunnelHalkaAdedi; halka++)
        {
        int karanlik = (int)(m_colorfactor * 250.0f);

        int color1 = ( karanlik << 24 ) |
            ( karanlik << 16 ) |
            ( karanlik << 8 ) |
            ( karanlik );

        karanlik = (int)(( m_colorfactor - m_colorfactordelta ) * 250.0f);
        int color2 = ( karanlik << 24 ) |
            ( karanlik << 16 ) |
            ( karanlik << 8 ) |
            ( karanlik );

        m_colorfactor -= m_colorfactordelta;

        D3DXMATRIX m_objMatrix;

        D3DXMatrixTranslation(&m_objMatrix,m_transx,m_transy,m_TunnelHalkaAralik);

        m_TransformMatrix *= m_objMatrix;
        //D3DXMatrixMultiply(&m_TransformMatrix,&m_TransformMatrix,&m_objMatrix);

        int m_VertexIndex = 0;

        if( FAILED(m_Vertices->Lock(0,0,(void**)&m_VerticeData,0)))
            DebugBreak();

        m_transx =   (float) sin(m_tunnelhareketi * m_TunnelX1 * m_AngleFactor ) * m_TunnelHareketOrani;
        m_transy =   (float) cos( m_tunnelhareketi * m_TunnelY1 * m_AngleFactor ) * m_TunnelHareketOrani;
        m_transx -=  (float) cos( m_tunnelhareketi * m_TunnelX2 * m_AngleFactor ) * m_TunnelHareketOrani;
        m_transy -=  (float) sin( m_tunnelhareketi * m_TunnelX2 * m_AngleFactor ) * m_TunnelHareketOrani;

        for( float aci = 0.0f; aci < 360.0f; aci += m_acistep )
            {
            // halkanin koordinatlari
            float x1 = (float) sin( aci * m_AngleFactor );
            float y1 = (float) cos( aci * m_AngleFactor );
            float x2 = x1 + m_transx;
            float y2 = y1 + m_transy;

            // bir sonraki adimin koordinatlarini hesapla
            float x3 = (float) sin( ( aci + m_acistep ) * m_AngleFactor );
            float y3 = (float) cos( ( aci + m_acistep ) * m_AngleFactor );
            float x4 = x3 + m_transx;
            float y4 = y3 + m_transy;

            float u1 = ( aci * m_TunnelTextureScale ) / 360.0f;
            float u2 = ( ( aci + m_acistep ) * m_TunnelTextureScale ) / 360.0f;
            float v1 = ( ( halka * m_TunnelTextureScale ) / m_TunnelHalkaAdedi );
            float v2 = ( ( ( halka + 1 ) * m_TunnelTextureScale ) / m_TunnelHalkaAdedi );

            // texture'i kaydir
            u1 += m_texturehareketi;
            u2 += m_texturehareketi;
            v1 += m_texturehareketi;
            v2 += m_texturehareketi;

            UpdateVertex(m_VerticeData,m_VertexIndex,x1,y1,0.0f,color1,u1,v1);
            m_VertexIndex++;

            UpdateVertex(m_VerticeData,m_VertexIndex,x4,y4,m_TunnelHalkaAralik,color2,u2,v2);
            m_VertexIndex++;

            UpdateVertex(m_VerticeData,m_VertexIndex,x3,y3,0.0f,color1,u2,v1);
            m_VertexIndex++;

            UpdateVertex(m_VerticeData,m_VertexIndex,x4,y4,m_TunnelHalkaAralik,color2,u2,v2);
            m_VertexIndex++;

            UpdateVertex(m_VerticeData,m_VertexIndex,x1,y1,0.0f,color1,u1,v1);
            m_VertexIndex++;

            UpdateVertex(m_VerticeData,m_VertexIndex,x2,y2,m_TunnelHalkaAralik,color2,u1,v2);
            m_VertexIndex++;
            }

        m_Vertices->Unlock();

        //m_Device->SetTexture(0,m_Texture);
        //m_Device->SetTransform(D3DTS_VIEW,&m_View);
        //m_Device->SetTransform(D3DTS_PROJECTION,&m_Proj);
        m_Device->SetTransform(D3DTS_WORLD,&m_TransformMatrix);

        m_Device->SetStreamSource(0,m_Vertices,0,sizeof(Vertex_PosColTextNorm));
        m_Device->SetIndices(m_Indices);
        m_Device->SetFVF(Vertex_PosColTextNorm::FVF);

        m_Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,m_TunnelVertexCount,0,m_TunnelIndexCount / 3);

        m_tunnelhareketi += m_TunnelFarSpeed;
        }
    }

void Tunnel::Render()
    {

    if ( SUCCEEDED(this->m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_XRGB(0,0,0),1.0f,0 ) ) )
        {

        if( SUCCEEDED( m_Device->BeginScene() ) )
            {

            m_State->Apply();

            UpdateVertexBuffer();

            RECT m_Rect;
            SetRect(&m_Rect,0,0,0,0);

            D3DXVECTOR2 m_Pos(250,320);
            D3DXVECTOR2 m_Scale(1,1);

            m_Scale.y = 1 + sin((float)frame / 60) * 0.05f;
            m_Scale.x = 1 - sin((float)frame / 60) * 0.05f;
            m_Pos.y /= m_Scale.y;
            m_Pos.x /= m_Scale.x;

            m_Writer->Render( TextureCache::TEXT3, &m_Pos , &m_Scale);

            }

        m_Device->EndScene();

        }

    }
