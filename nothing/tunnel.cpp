#include "Tunnel.h"
#include "resource.h"

Tunnel::Tunnel(void)
    {

    }

Tunnel::~Tunnel(void)
    {
    }

void Tunnel::FrameMove( DWORD time , DWORD delta)
    {
    m_Tunnel.RotateMeshY( DEGREE(delta * 0.05f) );

    m_LookAt.x = 200 + sin(time * 0.002f) * 30.0f;

    m_LookAt.y = cos(time * 0.002f) * sin(time * 0.003f) * 40.0f;
    m_LookAt.y -= 20.0f;
    m_LookAt.z = cos(time * 0.0005f) * sin(time * 0.003f) * 40.0f;
    
    D3DXMatrixIdentity( &m_Ship.Matrix );

    m_Ship.ScaleMesh(0.03f,0.03f,0.03f);
    m_Ship.RotateMeshX( DEGREE( sin( time * 0.002f) * 5 ) );
    m_Ship.RotateMeshY( DEGREE(150) );
    m_Ship.RotateMeshZ( DEGREE(-5) );
    m_Ship.Translate(3.0f, -1.2f,-135.0f);

    D3DXMATRIX orientation;

    D3DXMatrixTranslation(&orientation, 0, delta * -0.05f,0);
    scaleBarMatrix *= orientation;

    D3DXMatrixTranslation(&orientation,delta * -0.05f,0,0);
    textMatrix *= orientation;

    if ( (time / 1000) == 108 )
        Finished = TRUE;
    }

void Tunnel::Release()
    {
    SAFE_RELEASE(m_State);

    m_Tunnel.Release();
    }

HRESULT Tunnel::ResetDevice()
    {
    HRESULT m_HR;

    m_Eye = D3DXVECTOR3(0,0,-140);
    m_LookAt = D3DXVECTOR3(180,0,0);
    m_LookAt2 = D3DXVECTOR3(180,0,0);
    m_UpVect = D3DXVECTOR3(0,1,0);

    
    D3DXMatrixIdentity(&scaleBarMatrix);
    D3DXMATRIX orientation;
    D3DXMatrixTranslation(&orientation, 50, 0,0);
    scaleBarMatrix *= orientation;

    D3DXMatrixTranslation(&textMatrix,850,180,0);

    if ( FAILED( m_HR =  m_Tunnel.LoadFromResource(m_Device,MESH_TUNNEL) ) )
        return m_HR;

    if ( FAILED( m_HR = m_Ship.LoadFromResource(m_Device,MESH_SPACESHIP) ) )
        return m_HR;

    if( FAILED( m_HR = m_Device->BeginStateBlock()))
        return m_HR;

    m_Device->SetRenderState(           D3DRS_AMBIENT,                  D3DCOLOR_COLORVALUE(0.35, 0.35, 0.35, 0));
    m_Device->SetRenderState(           D3DRS_ZENABLE,                  TRUE);
    m_Device->SetRenderState(           D3DRS_CLIPPING,                 TRUE);
    m_Device->SetRenderState(           D3DRS_CULLMODE,                 D3DCULL_NONE);
    m_Device->SetRenderState(           D3DRS_LIGHTING,                 TRUE);
    m_Device->SetRenderState(           D3DRS_ZFUNC,                    D3DCMP_LESS);
    m_Device->SetRenderState(           D3DRS_ZWRITEENABLE,             TRUE);
    m_Device->SetRenderState(           D3DRS_ALPHABLENDENABLE,         TRUE);
    m_Device->SetRenderState(           D3DRS_BLENDOP,                  D3DBLENDOP_ADD );
    m_Device->SetRenderState(           D3DRS_SRCBLEND,                 D3DBLEND_SRCALPHA);
    m_Device->SetRenderState(           D3DRS_DESTBLEND,                D3DBLEND_INVSRCALPHA);
    m_Device->SetRenderState(           D3DRS_CLIPPING,                 TRUE);
    m_Device->SetRenderState(           D3DRS_LIGHTING,                 TRUE);
    m_Device->SetRenderState(           D3DRS_CULLMODE,                 D3DCULL_NONE);
    m_Device->SetRenderState(           D3DRS_FILLMODE,                 D3DFILL_SOLID);
    m_Device->SetRenderState(           D3DRS_VERTEXBLEND,              D3DVBF_DISABLE );

    m_Device->SetTextureStageState(0,   D3DTSS_COLOROP,                 D3DTOP_SELECTARG1 );
    m_Device->SetTextureStageState(0,   D3DTSS_COLORARG1,               D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,   D3DTSS_COLORARG2,               D3DTA_DIFFUSE);
    m_Device->SetTextureStageState(0,   D3DTSS_ALPHAOP,                 D3DTOP_SELECTARG1);
    m_Device->SetTextureStageState(0,   D3DTSS_ALPHAARG1,               D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,   D3DTSS_TEXCOORDINDEX,           D3DTSS_TCI_PASSTHRU);
    m_Device->SetTextureStageState(0,   D3DTSS_TEXTURETRANSFORMFLAGS,   D3DTTFF_DISABLE);

    if ( FAILED( m_HR = m_Device->EndStateBlock(&m_State) ) )
        return m_HR;

    return S_OK;
    }

void Tunnel::SetLight()
    {
    m_Device->LightEnable(0, TRUE);

    D3DLIGHT9 light;
    ZeroMemory(&light, sizeof(D3DLIGHT9));

    light.Type = D3DLIGHT_POINT;
    light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 255.0f;
    light.Specular.r = light.Specular.g = light.Specular.b = 255.0f;

    light.Range = 230.0f;
    light.Attenuation1 = 1.0f;
    light.Attenuation2 = 0.7f;
    //light.Position = D3DXVECTOR3(0,0,-250);
    light.Position = m_Eye;

    m_Device->SetLight(0, &light);
    }

void Tunnel::RenderScene()
    {
    m_State->Apply();


    D3DXMatrixPerspectiveFovLH(&m_Proj, /*D3DX_PI / 4*/ 2.4f, 1.3f, 1, 1000);

    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);

    m_Device->SetTransform(             D3DTS_PROJECTION,               &m_Proj);
    m_Device->SetTransform(             D3DTS_VIEW,                     &m_View);

    m_Device->SetTexture( 0, m_TextureCache->Textures[TextureCache::HALU] );
    m_Device->SetTextureStageState(0,   D3DTSS_COLOROP,                 D3DTOP_SELECTARG1 );

    m_Tunnel.Render();

    m_Device->SetTextureStageState(0,   D3DTSS_COLOROP,                 D3DTOP_MODULATE );

    SetLight();

    D3DXMatrixPerspectiveFovLH(&m_Proj, D3DX_PI / 4, 1.3f, 1, 1000);
    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt2,&m_UpVect);

    m_Device->SetTransform(             D3DTS_PROJECTION,               &m_Proj);
    m_Device->SetTransform(             D3DTS_VIEW,                     &m_View);

    m_Device->SetTextureStageState(0,   D3DTSS_COLOROP,                 D3DTOP_MODULATE);
    m_Device->SetTextureStageState(0,   D3DTSS_TEXCOORDINDEX,           D3DTSS_TCI_SPHEREMAP);
    m_Device->SetTexture( 0, m_TextureCache->Textures[TextureCache::WEST] );
    
    m_Ship.Render();
    }

void Tunnel::Render()
    {
    m_Device->BeginScene();

    m_Effects->StartGlow(D3DCOLOR_COLORVALUE(0,0,0,0),TRUE);

    RenderScene();

    m_Effects->EndGlow(0.01f,TRUE);

    m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_COLORVALUE(0,0,0,0),1.0f,0 );

    RenderScene();

    m_Effects->RenderGlow();

    m_Effects->RenderSprite(m_TextureCache->Textures[TextureCache::SCALEBAR],&scaleBarMatrix,D3DCOLOR_COLORVALUE(1,1,1,1));

    m_Effects->RenderSprite( m_TextureCache->Textures[TextureCache::TEXT2], &textMatrix, D3DCOLOR_COLORVALUE(1,1,1,0.4f) );

    m_Effects->RenderFilledTexture( m_TextureCache->Textures[TextureCache::FILTER5]);

    m_Device->EndScene(); 
    }