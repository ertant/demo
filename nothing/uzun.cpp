#include "Uzun.h"
#include "resource.h"

Uzun::Uzun(void)
    {

    }

Uzun::~Uzun(void)
    {
    }

void Uzun::FrameMove( DWORD time , DWORD delta)
    {
    m_Borular.Translate( 0,0, delta * -0.039f);

    m_Effects->Cube.RotateMeshXYZ( 
        DEGREE( delta * 0.05f ) * sin( time * 0.0004f ), 
        DEGREE( delta * 0.05f ) * sin( time * 0.0003f ), 
        DEGREE( delta * 0.05f ) * cos( time * 0.0002f )
        );

    D3DXMATRIX orientation;
    D3DXMatrixTranslation(&orientation,delta * -0.05f,0,0);
    textMatrix *= orientation;
    D3DXMatrixTranslation(&orientation,delta * -0.03f,0,0);
    textMatrix2 *= orientation;

    m_Eye.z = -25 + sin( time * 0.0007f ) * 50.0f;
    m_Eye.x = sin( time * 0.0002f ) * 16.0f;
    m_Eye.y = cos( time * 0.0002f ) * 16.0f;

    if ( (time / 1000) == 219 )
        Finished = TRUE;
    }

void Uzun::Release()
    {
    SAFE_RELEASE(m_State);

    m_Borular.Release();
    }

HRESULT Uzun::ResetDevice()
    {
    HRESULT m_HR;

    V(m_Borular.LoadFromResource( m_Device, MESH_BORUTUNNEL));
    V(m_Topuk.LoadFromResource( m_Device, MESH_TB ));

    D3DXMatrixTranslation(&textMatrix,850,150,0);
    D3DXMatrixTranslation(&textMatrix2,1050,350,0);

    D3DXMATRIX orientation;

    D3DXMatrixIdentity(&backMatrix);
    
    D3DXMatrixScaling(&orientation,1,0.4f,1);
    backMatrix *= orientation;

    D3DXMatrixTranslation(&orientation,0,100,0);
    backMatrix *= orientation;

    D3DXMatrixIdentity(&m_Borular.Matrix);

    m_Borular.RotateMeshY( DEGREE(90)) ;
    m_Borular.Translate(0,0,1000);
    
    m_Topuk.RotateMeshY( DEGREE(-90)) ;

    m_Eye = D3DXVECTOR3(-10,18,-25);
    m_LookAt = D3DXVECTOR3(0,0,0);
    m_UpVect = D3DXVECTOR3(0,1,0);

    D3DXMatrixPerspectiveFovLH(&m_Proj,/*D3DX_PI / 4*/ 0.785f, 1.3f, 1, 2000);

    if( FAILED( m_HR = m_Device->BeginStateBlock()))
        return m_HR;

    m_Device->SetRenderState(           D3DRS_AMBIENT,                  D3DCOLOR_COLORVALUE(0.95, 0.95, 0.95, 0));
    m_Device->SetRenderState(           D3DRS_ZENABLE,                  TRUE);
    m_Device->SetRenderState(           D3DRS_CLIPPING,                 TRUE);
    m_Device->SetRenderState(           D3DRS_LIGHTING,                 TRUE);
    m_Device->SetRenderState(           D3DRS_ZFUNC,                    D3DCMP_LESSEQUAL);
    m_Device->SetRenderState(           D3DRS_ZWRITEENABLE,             TRUE);
    m_Device->SetRenderState(           D3DRS_ALPHABLENDENABLE,         TRUE);
    m_Device->SetRenderState(           D3DRS_BLENDOP,                  D3DBLENDOP_ADD );
    m_Device->SetRenderState(           D3DRS_SRCBLEND,                 D3DBLEND_SRCALPHA);
    m_Device->SetRenderState(           D3DRS_DESTBLEND,                D3DBLEND_INVSRCALPHA);
    m_Device->SetRenderState(           D3DRS_CLIPPING,                 TRUE);
    m_Device->SetRenderState(           D3DRS_CULLMODE,                 D3DCULL_NONE);
    m_Device->SetRenderState(           D3DRS_FILLMODE,                 D3DFILL_SOLID);
    m_Device->SetRenderState(           D3DRS_SPECULARENABLE,           TRUE);
    m_Device->SetRenderState(           D3DRS_SHADEMODE,                D3DSHADE_PHONG);
        
    m_Device->SetTextureStageState(0,   D3DTSS_COLOROP,                 D3DTOP_MODULATE );
    m_Device->SetTextureStageState(0,   D3DTSS_COLORARG1,               D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,   D3DTSS_COLORARG2,               D3DTA_DIFFUSE);

    m_Device->SetTextureStageState(0,   D3DTSS_ALPHAOP,                 D3DTOP_SELECTARG1);
    m_Device->SetTextureStageState(0,   D3DTSS_ALPHAARG1,               D3DTA_TEXTURE);

    m_Device->SetTextureStageState(0,   D3DTSS_TEXCOORDINDEX,           D3DTSS_TCI_PASSTHRU);
    m_Device->SetTextureStageState(0,   D3DTSS_TEXTURETRANSFORMFLAGS,   D3DTTFF_DISABLE);

    m_Device->SetTransform(             D3DTS_PROJECTION,               &m_Proj);

    if ( FAILED( m_HR = m_Device->EndStateBlock(&m_State) ) )
        return m_HR;

    return S_OK;
    }

void Uzun::SetLight(float range, float diffuse)
    {
    m_Device->LightEnable(0, TRUE);

    D3DLIGHT9 light;
    ZeroMemory(&light, sizeof(D3DLIGHT9));

    light.Type = D3DLIGHT_POINT;
    light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = diffuse;
    light.Specular.r = light.Specular.g = light.Specular.b = 10.0f;

    light.Range = range;
    light.Attenuation0 = 0.0f;
    light.Attenuation1 = 1.0f;
    //light.Position = D3DXVECTOR3(0,0,0);    
    light.Position = m_Eye;

    V(m_Device->SetLight(0, &light));
    }

void Uzun::RenderScene()
    {
    m_State->Apply();

    SetLight(420.0,255.0f);
    
    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);

    m_Device->SetTransform(D3DTS_VIEW,&m_View);

    m_Device->SetTexture( 0, m_TextureCache->Textures[TextureCache::METAL] );

    //m_Borular.SetMatrix();

    //for(DWORD subSet = 0; subSet < m_Borular.NumMaterials; subSet++)
    //    {
    //    int textureIndex = TextureCache::ROBOT1;

    //    textureIndex += ( subSet % 3);
    //    m_Borular.Render(subSet);
    //    }

    m_Borular.Render();

    SetLight(260.0f,220.0f);

    m_Topuk.SetMatrix();

    for(DWORD subSet = 0; subSet < m_Topuk.NumMaterials; subSet++)
        {
        int textureIndex = TextureCache::ROBOT1;

        textureIndex += ( subSet % 3);

        m_Device->SetTexture( 0, m_TextureCache->Textures[ textureIndex ]);

        m_Topuk.Render(subSet);
        }

    }

void Uzun::Render()
    {
    m_Device->BeginScene();

	m_State->Apply();

    m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_COLORVALUE(0,1,0,0),1.0f,0 );

    m_Effects->DrawSkyCube(m_TextureCache->Textures[TextureCache::MAVI],0.785f,1.0f,1.3f);

    RenderScene();

    m_Effects->RenderSprite( m_TextureCache->Textures[TextureCache::TEXT5], &textMatrix, D3DCOLOR_COLORVALUE(1,1,1,0.4f) );
    m_Effects->RenderSprite( m_TextureCache->Textures[TextureCache::TEXT6], &textMatrix2, D3DCOLOR_COLORVALUE(1,1,1,0.4f) );

    m_Effects->RenderFilledTexture( m_TextureCache->Textures[TextureCache::FILTER1]);

    m_Device->EndScene(); 
    }