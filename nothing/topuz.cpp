#include "topuz.h"
#include <d3dx9.h>
#include "CQuad.h"
#include <strsafe.h>
#include "resource.h"



void topuz::FrameMove(DWORD time , DWORD delta)
    {
    float angle = 0.005f;
    DWORD second = time / 1000;

    if ( lastSecond == 0 )
        lastSecond = second;

    if ( ( second - lastSecond )  > 0 )
        m_Effects->Flash();

    angle += (second - lastSecond) * 80;

    m_Eye.z -=  0.04f * delta;
    m_Eye.x = (cos((float)time / 1000) * 150) - 250.0f;
    m_Eye.y = (sin((float)time / 1000) * 150) - 280.0f;

    m_Eye.z += (second - lastSecond) * 40.0f;

    if ( second != lastSecond )
        lastSecond = second;

    if ( (time / 500) > 26 )
        m_PresentsAlpha += 0.001f * delta;

    if ( m_PresentsAlpha > 1.0f )
        m_PresentsAlpha = 1.0f;

    if ( second == 27 )
        Finished = TRUE;

    m_Effects->Cube.RotateMeshXYZ( 
        (DEGREE( delta * 0.05f ) * sin( time * 0.0004f )) + DEGREE((second - lastSecond) * 180), 
        (DEGREE( delta * 0.05f ) * sin( time * 0.0003f )) + DEGREE((second - lastSecond) * 180), 
        (DEGREE( delta * 0.05f ) * cos( time * 0.0002f )) + DEGREE((second - lastSecond) * 180)
        );

    m_Object.RotateMeshZ( DEGREE(angle));
    m_Object.RotateMeshY( DEGREE(angle));
    m_Object.RotateMeshX( DEGREE(angle));

    D3DXMATRIX orientation;

    D3DXMatrixRotationX(&orientation, 0.0005f );
    tweenMatrix *= orientation;
    D3DXMatrixRotationY(&orientation, 0.0002f );
    tweenMatrix *= orientation;
    D3DXMatrixRotationZ(&orientation, 0.0003f );
    tweenMatrix *= orientation;
    }

void topuz::Release()
    {
    SAFE_RELEASE(m_State);
    m_Object.Release();
    }

HRESULT topuz::ResetDevice()
    {
    HRESULT m_HR;

    lastSecond = 0;
    m_PresentsAlpha = 0.0f;
    m_Effects->Flash();

    if ( FAILED( m_HR = BaseSection::ResetDevice() ) )
        return m_HR;

    m_Eye = D3DXVECTOR3(0,0,-225);
    m_LookAt = D3DXVECTOR3(0,1,0);
    m_UpVect = D3DXVECTOR3(0,1,0);

    D3DXMatrixIdentity(&m_PresentsMatrix);

    //D3DXMATRIX orientation;
    ////D3DXMatrixScaling(&orientation,1.4f,1.4f,1.4f);
    ////m_PresentsMatrix *= orientation;
    //D3DXMatrixTranslation(&orientation,144,44,0);
    //m_PresentsMatrix *= orientation;

    if ( FAILED( m_HR = m_Object.LoadFromResource(m_Device,MESH_ACAYIP) ) )
        return m_HR;


    m_Object.SetFVF( D3DFVF_XYZB1 | D3DFVF_TEX1 | D3DFVF_NORMAL );

    Vertex_PosTextNormBlend * vertices = (Vertex_PosTextNormBlend *)m_Object.LockVertices();

    FLOAT fMinX =  1e10f;
    FLOAT fMaxX = -1e10f;

    for(DWORD i = 0; i < m_Object.numVertices; i++)
        {
        float len = D3DXVec3Length( &D3DXVECTOR3( vertices[i].x ,vertices[i].y,vertices[i].z) ) ;

        if( len < fMinX ) 
            fMinX = len;
        if( len > fMaxX ) 
            fMaxX = len;
        }

    for(DWORD i = 0; i < m_Object.numVertices; i++)
        {
        float len = D3DXVec3Length( &D3DXVECTOR3( vertices[i].x ,vertices[i].y,vertices[i].z) ) ;
        // Set the blend factors for the vertices
        FLOAT a = ( len - fMinX ) / ( fMaxX - fMinX );
        vertices[i].blend = 1.0f-sinf(a*D3DX_PI*1.0f) / 4;
        }

    m_Object.Unlock();

    D3DXMatrixIdentity(&tweenMatrix);

    m_Object.UseMaterial = FALSE;

    if( FAILED( m_HR = m_Device->BeginStateBlock()))
        return m_HR;

    m_Device->SetRenderState(           D3DRS_AMBIENT,                  D3DCOLOR_COLORVALUE(0.35, 0.35, 0.35, 0));
    m_Device->SetRenderState(           D3DRS_ZENABLE,                  TRUE);
    m_Device->SetRenderState(           D3DRS_CLIPPING,                 TRUE);
    m_Device->SetRenderState(           D3DRS_CULLMODE,                 D3DCULL_NONE);
    m_Device->SetRenderState(           D3DRS_LIGHTING,                 FALSE);
    m_Device->SetRenderState(           D3DRS_ZFUNC,                    D3DCMP_LESS);
    m_Device->SetRenderState(           D3DRS_ZWRITEENABLE,             TRUE);
    m_Device->SetRenderState(           D3DRS_ALPHABLENDENABLE,         TRUE);
    m_Device->SetRenderState(           D3DRS_BLENDOP,                  D3DBLENDOP_ADD );
    m_Device->SetRenderState(           D3DRS_SRCBLEND,                 D3DBLEND_SRCALPHA);
    m_Device->SetRenderState(           D3DRS_DESTBLEND,                D3DBLEND_INVSRCALPHA);
    m_Device->SetRenderState(           D3DRS_CLIPPING,                 TRUE);
    m_Device->SetRenderState(           D3DRS_CULLMODE,                 D3DCULL_NONE);
    m_Device->SetRenderState(           D3DRS_FILLMODE,                 D3DFILL_SOLID);
    m_Device->SetRenderState(           D3DRS_VERTEXBLEND,              D3DVBF_DISABLE );

    m_Device->SetTextureStageState(0,   D3DTSS_COLOROP,                 D3DTOP_SELECTARG1 );
    m_Device->SetTextureStageState(0,   D3DTSS_COLORARG1,               D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,   D3DTSS_ALPHAOP,                 D3DTOP_SELECTARG1);
    m_Device->SetTextureStageState(0,   D3DTSS_ALPHAARG1,               D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,   D3DTSS_TEXCOORDINDEX,           D3DTSS_TCI_CAMERASPACENORMAL);
    m_Device->SetTextureStageState(0,   D3DTSS_TEXTURETRANSFORMFLAGS,   D3DTTFF_DISABLE);

    if ( FAILED( m_HR = m_Device->EndStateBlock(&m_State) ) )
        return m_HR;

    return S_OK;
    }

void topuz::RenderScene(BOOL glow)
    {
    if ( FAILED(m_State->Apply() ) )
        DebugBreak();

    m_Device->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_1WEIGHTS);
    m_Device->SetTransform(D3DTS_WORLD1,&tweenMatrix);

    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);
    D3DXMatrixPerspectiveFovLH(&m_Proj, D3DX_PI / 4, 1.3f, 1, 1000);

    m_Device->SetTransform(D3DTS_VIEW,&m_View);
    m_Device->SetTransform(D3DTS_PROJECTION,&m_Proj);

    if ( glow == FALSE )
        {
        m_Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
        m_Device->SetTexture( 0, m_TextureCache->Textures[TextureCache::LINES] );
        }
    else
        {
        m_Device->SetTexture( 0, m_TextureCache->Textures[TextureCache::TEX1] );
        }

    m_Object.Render();

    m_Device->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_DISABLE);

    if ( ( glow == FALSE) && ( m_PresentsAlpha > 0.0f) )
        {
        //m_Effects->RenderSprite( m_TextureCache->Textures[TextureCache::LOGO], &m_PresentsMatrix, D3DCOLOR_COLORVALUE(1,1,1,m_PresentsAlpha));
        m_Effects->RenderFilledTexture( m_TextureCache->Textures[TextureCache::LOGO], D3DCOLOR_COLORVALUE(1,1,1,m_PresentsAlpha));
        }
    }


void topuz::Render()
    {
    m_Device->BeginScene();

    m_Effects->StartGlow(D3DCOLOR_COLORVALUE(0,0,0,0),true);

    RenderScene(TRUE);

    m_Effects->EndGlow(0.01f,TRUE);

    m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_COLORVALUE(0,1,0,0),1.0f,0 );

    m_Effects->DrawSkyCube(m_TextureCache->Textures[TextureCache::STARS],2.385f,1.0f,1.3f);

    m_Effects->RenderGlow();

    RenderScene(FALSE);

    m_Effects->RenderFilledTexture( m_TextureCache->Textures[TextureCache::FILTER1]);

    m_Device->EndScene();

    }
