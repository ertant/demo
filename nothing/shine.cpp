#include "Shine.h"
#include "resource.h"

Shine::Shine(void)
    {

    }

Shine::~Shine(void)
    {
    }

void Shine::FrameMove( DWORD time , DWORD delta)
    {
    m_Effects->Cube.RotateMeshXYZ( 
        DEGREE( delta * 0.05f ) * sin( time * 0.0004f ), 
        DEGREE( delta * 0.05f ) * sin( time * 0.0003f ), 
        DEGREE( delta * 0.05f ) * cos( time * 0.0002f )
        );

    m_ShinyThing.RotateMeshXYZ(
        DEGREE( delta * 0.05f ) * cos( time * 0.0001f ),
        DEGREE( delta * 0.04f ) * sin( time * 0.0007f ),
        DEGREE( delta * 0.07f ) * cos( time * 0.0003f ) );

    D3DXMATRIX orientation;

    D3DXMatrixRotationX(&orientation, 0.001f );
    tweenMatrix *= orientation;
    D3DXMatrixRotationY(&orientation, 0.001f );
    tweenMatrix *= orientation;
    D3DXMatrixRotationZ(&orientation, 0.002f );
    tweenMatrix *= orientation;

    if ( (time / 1000) == 53 )
        {
        if ( filter != 1 )
            m_Effects->Flash();
        filter = 1;
        }
    else if ( (time / 1000) == 68 )
        {
        if ( filter != 2 )
            m_Effects->Flash();
        filter = 2;
        }

    if ( (time / 1000) == 82 )
        Finished = TRUE;
    }

void Shine::Release()
    {
    SAFE_RELEASE(m_State);

    m_ShinyThing.Release();
    m_CenterObject.Release();
    }

HRESULT Shine::ResetDevice()
    {
	m_State = NULL;

    HRESULT m_HR;

    if ( FAILED( m_HR =  m_ShinyThing.LoadFromResource(m_Device,MESH_SACMA) ) )
        return m_HR;

    D3DXMatrixIdentity(&tweenMatrix);
    D3DXMATRIX orientation;

    filter = 0;

    //D3DXMatrixScaling(&orientation, 2.5f ,2.5f ,2.5f );
    //tweenMatrix *= orientation;


    m_ShinyThing.SetFVF( D3DFVF_XYZB1 | D3DFVF_TEX1 | D3DFVF_NORMAL );

    Vertex_PosTextNormBlend * vertices = (Vertex_PosTextNormBlend *)m_ShinyThing.LockVertices();

    FLOAT fMinX =  1e10f;
    FLOAT fMaxX = -1e10f;

    for(DWORD i = 0; i < m_ShinyThing.numVertices; i++)
        {
        if( vertices[i].x < fMinX ) 
            fMinX = vertices[i].x;
        if( vertices[i].x > fMaxX ) 
            fMaxX = vertices[i].x;
        }

    for(DWORD i = 0; i < m_ShinyThing.numVertices; i++)
        {
        FLOAT a = ( vertices[i].x - fMinX ) / ( fMaxX - fMinX );
        vertices[i].blend = 1.0f-sinf(a*D3DX_PI*1.0f) / 4;
        }

    m_ShinyThing.Unlock();

    m_Eye = D3DXVECTOR3(0,0,-90);
    m_LookAt = D3DXVECTOR3(0,0,0);
    m_UpVect = D3DXVECTOR3(0,1,0);

    D3DXMatrixPerspectiveFovLH(&m_Proj, /*D3DX_PI / 4*/ 2.3f, 1.3f, 1, 1000);

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

    m_Device->SetTextureStageState(0,   D3DTSS_COLOROP,                 D3DTOP_SELECTARG1 );
    m_Device->SetTextureStageState(0,   D3DTSS_COLORARG1,               D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,   D3DTSS_COLORARG2,               D3DTA_DIFFUSE);
    m_Device->SetTextureStageState(0,   D3DTSS_ALPHAOP,                 D3DTOP_SELECTARG1);
    m_Device->SetTextureStageState(0,   D3DTSS_ALPHAARG1,               D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,   D3DTSS_TEXCOORDINDEX,           D3DTSS_TCI_PASSTHRU);
    m_Device->SetTextureStageState(0,   D3DTSS_TEXTURETRANSFORMFLAGS,   D3DTTFF_DISABLE);

    m_Device->SetTransform(             D3DTS_PROJECTION,               &m_Proj);

    if ( FAILED( m_HR = m_Device->EndStateBlock(&m_State) ) )
	{
		return m_HR;
	}

    return S_OK;
    }

void Shine::SetLight()
    {
    m_Device->LightEnable(0, TRUE);

    D3DLIGHT9 light;
    ZeroMemory(&light, sizeof(D3DLIGHT9));

    light.Type = D3DLIGHT_POINT;
    light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 255.0f;

    light.Range = 450.0f;
    light.Attenuation1 = 1.0f;
    //light.Position = D3DXVECTOR3(0,0,-250);
    light.Position = m_Eye;

    m_Device->SetLight(0, &light);
    }

void Shine::RenderScene()
    {
    V(m_State->Apply());

    SetLight();

    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);

    V(m_Device->SetTransform(D3DTS_VIEW,&m_View));

    V(m_Device->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_1WEIGHTS));

    V(m_Device->SetRenderState(           D3DRS_BLENDOP,                  D3DBLENDOP_ADD ));
    V(m_Device->SetRenderState(           D3DRS_DESTBLEND,                D3DBLEND_INVSRCALPHA));
    V(m_Device->SetRenderState(           D3DRS_SRCBLEND,                 D3DBLEND_SRCALPHA));

    V(m_Device->SetTransform(D3DTS_WORLD1,&tweenMatrix));

    V(m_Device->SetTexture( 0, m_TextureCache->Textures[TextureCache::LIGHTSQ] ));

    m_ShinyThing.Render();

    D3DXMATRIX orientation;
    D3DXMATRIX matrix2 = m_ShinyThing.Matrix;
    D3DXMatrixScaling(&orientation,1.01f,1.01f,1.01f);

    matrix2 *= orientation;

    V(m_Device->SetTransform(             D3DTS_WORLD,                    &matrix2 ));
    V(m_Device->SetRenderState(           D3DRS_FILLMODE,                 D3DFILL_WIREFRAME));

    //V(m_Device->SetRenderState(           D3DRS_BLENDOP,                  D3DBLENDOP_ADD ));
    //V(m_Device->SetRenderState(           D3DRS_DESTBLEND,                D3DBLEND_BLENDFACTOR));
    //V(m_Device->SetRenderState(           D3DRS_DESTBLEND,                D3DBLEND_INVBLENDFACTOR));

    V(m_Device->SetRenderState(           D3DRS_BLENDFACTOR,              D3DCOLOR_COLORVALUE(1,1,1,0.5f)));

    m_ShinyThing.UseMatrix = FALSE;
    
    V(m_Device->SetTexture( 0, m_TextureCache->Textures[TextureCache::WHITE] ));

    m_ShinyThing.Render();

    m_ShinyThing.UseMatrix = TRUE;
        
    V(m_Device->SetRenderState(           D3DRS_FILLMODE,                 D3DFILL_SOLID));
    }

void Shine::Render()
    {
    V(m_Device->BeginScene());

	V(m_State->Apply());

    m_Effects->StartGlow(D3DCOLOR_COLORVALUE(0,0,0,0),true);

    m_Effects->DrawSkyCube(m_TextureCache->Textures[TextureCache::HALU],2.385f,1.0f,1.3f);

    RenderScene();

    m_Effects->EndGlow(0.01f,TRUE);

    V(m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_COLORVALUE(0,1,0,0),1.0f,0 ));

    m_Effects->DrawSkyCube(m_TextureCache->Textures[TextureCache::HALU],2.385f,1.0f,1.3f);

    RenderScene();

    m_Effects->RenderGlow();

    if ( filter == 1 )
        {
        m_Effects->RenderFilledTexture( m_TextureCache->Textures[TextureCache::FILTER3]);
        }
    else if ( filter == 2 )
        {
        m_Effects->RenderFilledTexture( m_TextureCache->Textures[TextureCache::FILTER4]);
        }
    else
        {
        m_Effects->RenderFilledTexture( m_TextureCache->Textures[TextureCache::FILTER1]);
        }

    V(m_Device->EndScene()); 
    }