#include "Yildiz.h"

void Yildiz::FrameMove()
{
    number++;

#if(_DEBUG)
    if ( number > 200 )
        opacity_vel = 0.01f;
#endif

    opacity += opacity_vel;
}

void Yildiz::SyncMeta( DWORD data )
{
    if ( data == 0x40 )
        opacity_vel = 0.01f;

    if ( data == 0x50 )
        Finished = TRUE;
}

void Yildiz::Cleanup()
{
}

HRESULT Yildiz::ResetDevice()
{
    HRESULT m_HR;

    BaseSection::ResetDevice();

    opacity = 0.0f;
    opacity_vel = 0.0f;
    number = 0;

    if ( FAILED( m_HR = m_Mesh.Load( m_Device, "res\\aa.x" ) ) )
        return m_HR;

    m_Mesh.States = new LPDIRECT3DSTATEBLOCK9[1];

    m_Device->BeginStateBlock();

    m_Device->SetRenderState(D3DRS_CLIPPING,TRUE);
    m_Device->SetRenderState(D3DRS_LIGHTING,FALSE);
    m_Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
    m_Device->SetRenderState(D3DRS_ZENABLE,TRUE);
    m_Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
    m_Device->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

    m_Device->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_XRGB(0x255,0x255,0x255));
    m_Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);

    D3DMATERIAL9 m_Material;
    ZeroMemory( &m_Material, sizeof(D3DMATERIAL9) );

    m_Material.Diffuse.r = m_Material.Ambient.r = 1;
    m_Material.Diffuse.g = m_Material.Ambient.g = 1;
    m_Material.Diffuse.b = m_Material.Ambient.b = 1;

    m_Device->SetMaterial(&m_Material);

    m_Device->SetTexture(0,m_TextureCache->Textures[TextureCache::GALAXY]);

    m_Device->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_CAMERASPACENORMAL);
    m_Device->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);

    m_Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE );
    m_Device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);

    D3DXVECTOR3 m_Eye = D3DXVECTOR3(0,0,-15);
    D3DXVECTOR3 m_LookAt = D3DXVECTOR3(0,0,0);
    D3DXVECTOR3 m_UpVect = D3DXVECTOR3(0,1,0);
    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;
    
    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);
    D3DXMatrixPerspectiveFovLH(&m_Proj, D3DX_PI / 4, 1.3f, 1, 100);

    m_Device->SetTransform(D3DTS_VIEW,&m_View);
    m_Device->SetTransform(D3DTS_PROJECTION,&m_Proj);

    m_Device->EndStateBlock(&m_Mesh.States[0]);

    return S_OK;
}


void Yildiz::Render()
{
    if ( SUCCEEDED(this->m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_XRGB(0,0,0),1.0f,0 ) ) )
    {
        if( SUCCEEDED( m_Device->BeginScene() ) )
        {
            if ( opacity > 0 )
                m_Background->Render(m_TextureCache->Textures[TextureCache::YILDIZBACK], opacity);            

            m_Mesh.States[0]->Apply();

            D3DXMatrixIdentity(&m_Mesh.Matrix);

            m_Mesh.ScaleMesh( 0.9f );

            m_Mesh.RotateMeshZ( number / 50.0f );
            m_Mesh.RotateMeshY( -45 );

            m_Mesh.Translate( -3.5f,-1.2f,0.0f );

            m_Device->SetTransform(D3DTS_WORLD,&m_Mesh.Matrix);

            m_Mesh.Render();

            RECT m_Rect;
            SetRect(&m_Rect,0,0,0,0);

            D3DXVECTOR2 m_Pos(150,200);
            D3DXVECTOR2 m_Scale(1,1);

            m_Scale.y = 1 + sin((float)number / 60) * 0.05f;
            m_Scale.x = 1 - sin((float)number / 60) * 0.05f;
            m_Pos.y /= m_Scale.y;
            m_Pos.x /= m_Scale.x;

            m_Writer->Render( TextureCache::TEXT1, &m_Pos , &m_Scale);
        }

        m_Device->EndScene();
    }
    
}