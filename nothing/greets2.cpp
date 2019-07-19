#include "Greets2.h"
#include "resource.h"

Greets::Greets(void)
    {

    }

Greets::~Greets(void)
    {
    }

void Greets::FrameMove( DWORD time , DWORD delta)
    {
    m_Effects->Cube.RotateMeshXYZ( 
        DEGREE( delta * 0.05f ) * sin( time * 0.0004f ), 
        DEGREE( delta * 0.05f ) * sin( time * 0.0003f ), 
        DEGREE( delta * 0.05f ) * cos( time * 0.0002f )
        );

    float rotateScale = vel * delta;

    if ( ( m_Eye.y > 0 ) && (!shake) )
        {
        m_Eye.y -= rotateScale;
        }
    else if ( !shake )
        {
        m_TopAngle.y = 0;
        m_TopAngle.x = 0;
        shake = true;
        }

    if ( shake )
        {
        m_TopAngle.y += delta * 0.002f;
        m_Eye.y = sin(m_TopAngle.y) * 30;
        }

    if ( m_Eye.z > -200 )
        m_Eye.z -= rotateScale;

    D3DXMATRIX orientation;
    D3DXMatrixRotationY( &orientation, (delta * 0.002f) );
    m_Merkez.Matrix *= orientation;
    D3DXMatrixRotationY( &orientation, (delta * -0.001f) );
    m_IcCember.Matrix *= orientation;
    D3DXMatrixRotationY( &orientation, (delta * 0.0006f) );
    m_Cember.Matrix *= orientation;
    m_Monitors.Matrix *= orientation;

    if ( ( time / 1000 ) > 1 )
        {
        if ( vel < 1.0 )
            vel+= 0.00001f ;
        }

    if (( time % 2000 ) == 0 )
        {
        textureIndex++;
        if ( textureIndex > TextureCache::GREETS8 )
            textureIndex = TextureCache::GREETS1;
        }

    D3DXMatrixTranslation(&orientation,delta * -0.05f,0,0);
    textMatrix *= orientation;

    if ( (time / 1000) == 165 )
        Finished = TRUE;
    }

void Greets::Release()
    {
    SAFE_RELEASE(m_State);
    }

HRESULT Greets::ResetDevice()
    {
    HRESULT m_HR;

    
    if ( FAILED( m_HR =  m_Merkez.LoadFromResource( m_Device, MESH_GREETSMERKEZ ) ) )
        return m_HR;
    if ( FAILED( m_HR =  m_IcCember.LoadFromResource(m_Device,MESH_GREETSICCEMBER ) ) )
        return m_HR;
    if ( FAILED( m_HR =  m_Cember.LoadFromResource(m_Device,MESH_GREETSCEMBER ) ) )
        return m_HR;
    if ( FAILED( m_HR =  m_Monitors.LoadFromResource(m_Device,MESH_GREETSMONITORS) ) )
        return m_HR;

    //m_Merkez.RotateMeshX( DEGREE(90) );
    //m_IcCember.RotateMeshX( DEGREE(90) );
    //m_Cember.RotateMeshX( DEGREE(90) );
    //m_Monitors.RotateMeshX( DEGREE(90) );

    m_Monitors.UseMatrix = FALSE;

    D3DXMatrixTranslation(&textMatrix,850,350,0);

    vel = 0.0f;
    fov = 2.3f;
    shake = false;
    textureIndex = TextureCache::GREETS1;

    m_TopAngle.x = DEGREE(0);
    m_TopAngle.y = DEGREE(0);
    m_TopAngle.z = DEGREE(0);

    m_Eye = D3DXVECTOR3(0,250,-1);
    m_LookAt = D3DXVECTOR3(0,0,0);
    m_UpVect = D3DXVECTOR3(0,1,0);

    if( FAILED( m_HR = m_Device->BeginStateBlock()))
        return m_HR;

    m_Device->SetRenderState(           D3DRS_AMBIENT,                  D3DCOLOR_COLORVALUE(0.35, 0.35, 0.35, 0));
    m_Device->SetRenderState(           D3DRS_ZENABLE,                  TRUE);
    m_Device->SetRenderState(           D3DRS_CLIPPING,                 TRUE);
    m_Device->SetRenderState(           D3DRS_CULLMODE,                 D3DCULL_NONE);
    m_Device->SetRenderState(           D3DRS_LIGHTING,                 FALSE);
    m_Device->SetRenderState(           D3DRS_ZFUNC,                    D3DCMP_LESSEQUAL);
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

    


    if ( FAILED( m_HR = m_Device->EndStateBlock(&m_State) ) )
        return m_HR;

    return S_OK;
    }

void Greets::SetLight()
    {
    m_Device->LightEnable(0, TRUE);

    D3DLIGHT9 light;
    ZeroMemory(&light, sizeof(D3DLIGHT9));

    light.Type = D3DLIGHT_POINT;
    light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 255.0f;

    light.Range = 750.0f;
    light.Attenuation0 = 0.0f;
    light.Attenuation1 = 1.0f;
    //light.Position = D3DXVECTOR3(0,0,-250);
    light.Position = m_Eye;

    m_Device->SetLight(0, &light);
    }

void Greets::RenderScene(BOOL glow)
    {
    m_State->Apply();

    D3DXMatrixPerspectiveFovLH(&m_Proj, fov, 1.3f, 1, 1000);
    m_Device->SetTransform(             D3DTS_PROJECTION,               &m_Proj);

    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);

    m_Device->SetTransform(D3DTS_VIEW,&m_View);

    m_Device->SetTextureStageState(0,   D3DTSS_COLOROP,                 D3DTOP_SELECTARG1 );
    m_Device->SetTextureStageState(0,   D3DTSS_TEXCOORDINDEX,           D3DTSS_TCI_CAMERASPACENORMAL);
    m_Device->SetTexture( 0, m_TextureCache->Textures[TextureCache::ENVMAP01] );

    m_Merkez.Render();
    m_IcCember.Render();
    m_Cember.Render();
    
    if ( glow == FALSE )
        {
        m_Device->SetTextureStageState(0,   D3DTSS_TEXCOORDINDEX,           D3DTSS_TCI_PASSTHRU);
        m_Device->SetTexture( 0, m_TextureCache->Textures[textureIndex] );
        m_Monitors.Render();
        }
    }

void Greets::Render()
    {
    m_Device->BeginScene();

    m_Effects->StartGlow(D3DCOLOR_COLORVALUE(0,0,0,0),TRUE);

    //m_Effects->DrawSkyCube(m_TextureCache->Textures[TextureCache::HALU],2.385f,1.0f,1.3f);

    RenderScene(TRUE);

    m_Effects->EndGlow(0.01f,TRUE);

    m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_COLORVALUE(0,0,0,0),1.0f,0 );

    m_Effects->DrawSkyCube(m_TextureCache->Textures[TextureCache::STARS],2.385f,1.0f,1.3f);

    m_Effects->RenderGlow();

    RenderScene(FALSE);

    m_Effects->RenderSprite( m_TextureCache->Textures[TextureCache::TEXT4], &textMatrix, D3DCOLOR_COLORVALUE(1,1,1,0.4f) );

    m_Effects->RenderFilledTexture( m_TextureCache->Textures[TextureCache::FILTER1]);

    m_Device->EndScene(); 
    }