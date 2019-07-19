#include "Titrek.h"
#include "resource.h"

Titrek::Titrek(void)
    {
    }

Titrek::~Titrek(void)
    {
    }

DWORD lastSecond;

void Titrek::FrameMove( DWORD time , DWORD delta)
    {
    D3DXMATRIX m_Orientation;

    D3DXMatrixRotationY(&m_Orientation,delta * 0.0005f);
    m_Oda.Matrix *= m_Orientation;
    m_CenterObject.Matrix *= m_Orientation;

    //if ( m_Eye.z < -10)
    //    m_Eye.z +=  0.0034f * delta;

    m_Eye.x = sin(time * .0002f) * 15.0f;
    m_Eye.y = cos(time * .0003f) * 15.0f;
    m_Eye.z = 10 + cos(time * .0002f) * 3.0f;

    fov = (sin(time * .0005f) * 1.0f) + 1.7f;

    D3DXMatrixTranslation(&m_Orientation,delta * -0.08f,0,0);
    textMatrix *= m_Orientation;

    DWORD second = time / 1000;

    if ( lastSecond == 0 )
        lastSecond = second;

    //fov -= (second - lastSecond) * 0.5f;

    //m_Eye.x -= (second - lastSecond) * 10.0f;

    //if ( m_Eye.z < -19 )
    //    m_Eye.z = -19;

    if ( second != lastSecond )
        lastSecond = second;

    if ( (time / 1000) == 41 )
        Finished = TRUE;
    }


void Titrek::Release()
    {
    SAFE_RELEASE(m_State);
    m_Oda.Release();
    m_CenterObject.Release();
    }

HRESULT Titrek::ResetDevice()
    {

    HRESULT m_HR;

    lastSecond = 0;
    Finished = FALSE;
    fov = 1.585f;

    m_Oda.LoadFromResource(m_Device,MESH_KUTULAR);
    m_Oda.UseMaterial = TRUE;
    
    m_Oda.Materials[0].Diffuse.r = m_Oda.Materials[0].Diffuse.g = m_Oda.Materials[0].Diffuse.b = 0.0f;

    m_CenterObject.LoadFromResource(m_Device,MESH_SPACESHIP);

    D3DXMATRIX orientation;

    m_CenterObject.RotateMeshZ(DEGREE(180));
    //m_CenterObject.Translate(-10,-50,-5);    
    m_CenterObject.Translate(0,-50,-30);    
    m_CenterObject.ScaleMesh(0.2f,0.2f,0.2f);
    
    D3DXMatrixTranslation(&textMatrix,850,380,0);

    m_Eye = D3DXVECTOR3(0,-10,-15.1f);
    m_LookAt = D3DXVECTOR3(0,-10,0);
    m_UpVect = D3DXVECTOR3(0,1,0);

    //D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);
    //D3DXMatrixPerspectiveFovLH(&m_Proj, D3DX_PI / 4 /*1.585f*/, 1.3f, 1, 1000);

    if( FAILED( m_HR = m_Device->BeginStateBlock()))
        return m_HR;

    m_Device->SetRenderState(D3DRS_CLIPPING,TRUE);
    m_Device->SetRenderState(D3DRS_LIGHTING,TRUE);
    m_Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
    m_Device->SetRenderState(D3DRS_ZENABLE,TRUE);
    m_Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
    m_Device->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

    m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

    m_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD );
    m_Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    m_Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

    //m_Device->SetRenderState(D3DRS_STENCILENABLE,FALSE);

    m_Device->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_COLORVALUE(0.65, 0.35, 0.35, 0));
    m_Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
    //m_Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);

    //D3DMATERIAL9 m_Material;
    //ZeroMemory( &m_Material, sizeof(D3DMATERIAL9) );

    //m_Material.Diffuse.r = m_Material.Ambient.r = 1;
    //m_Material.Diffuse.g = m_Material.Ambient.g = 1;
    //m_Material.Diffuse.b = m_Material.Ambient.b = 1;

    //m_Device->SetMaterial(&m_Material);

    m_Device->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_PASSTHRU);
    m_Device->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);

    m_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    m_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    m_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

    if ( FAILED( m_HR = m_Device->EndStateBlock(&m_State) ) )
        return m_HR;

    return S_OK;
    }

void Titrek::SetLight()
    {
    m_Device->LightEnable(0, TRUE);

    D3DLIGHT9 light;
    ZeroMemory(&light, sizeof(D3DLIGHT9));

    light.Type = D3DLIGHT_POINT;
    light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 255.0f;
    light.Range = 660.0f;
    light.Attenuation0 = 0.0f;
    light.Attenuation1 = 1.0f;
    light.Position = D3DXVECTOR3(0,0,0);
    //light.Position = m_Eye;

    m_Device->SetLight(0, &light);
    }


void Titrek::Render()
    {
    m_Device->BeginScene();

    m_Effects->StartGlow(D3DCOLOR_COLORVALUE(0,0,0,0),true);

    m_State->Apply();

    SetLight();

    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);
    D3DXMatrixPerspectiveFovLH(&m_Proj, fov, 1.3f, 1, 1000);

    m_Device->SetTransform(D3DTS_VIEW,&m_View);
    m_Device->SetTransform(D3DTS_PROJECTION,&m_Proj);

    m_Device->SetTexture( 0, m_TextureCache->Textures[TextureCache::LIGHTSQ] );

    m_Oda.Render();

    //m_Device->SetTexture( 0, m_TextureCache->Textures[TextureCache::CHROME] );
    //m_CenterObject.Render();

    m_Effects->EndGlow(0.01f,TRUE);

    m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_COLORVALUE(0,0,0,0),1.0f,0 );

    m_State->Apply();

    m_Device->SetTransform(D3DTS_VIEW,&m_View);
    m_Device->SetTransform(D3DTS_PROJECTION,&m_Proj);

    m_Device->SetTexture( 0, m_TextureCache->Textures[TextureCache::LIGHTSQ] );
    m_Oda.Render();

    m_Effects->RenderGlow();

    m_Device->SetTextureStageState(0,   D3DTSS_TEXCOORDINDEX,           D3DTSS_TCI_SPHEREMAP);
    m_Device->SetTexture( 0, m_TextureCache->Textures[TextureCache::LIGHTSQ] );
    m_CenterObject.Render();

    m_Effects->RenderSprite( m_TextureCache->Textures[TextureCache::TEXT1], &textMatrix, D3DCOLOR_COLORVALUE(1,1,1,0.4f) );

    m_Effects->RenderFilledTexture( m_TextureCache->Textures[TextureCache::FILTER1]);
    
    m_Device->EndScene(); 
    }