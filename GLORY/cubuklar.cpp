#include "cubuklar.h"

void cubuklar::FrameMove()
{
    number++;

//#if(_DEBUG)
//    if (number > 200)
//        opacity_vel = -0.02f;
//#endif

    if ( opacity != 0 )
    {
        opacity += opacity_vel;

        if ( opacity < 0 )
            opacity = 0;
    }
}

void cubuklar::SyncMeta( DWORD data )
{
    if ( data == 0x70 )
        opacity_vel = -0.02f;

    if ( data == 0x80 )
        Finished = TRUE;
}

void cubuklar::Cleanup()
{
}


HRESULT cubuklar::ResetDevice()
{
    HRESULT m_HR;

    BaseSection::ResetDevice();

    number = 0;
    opacity = 1.0f;
    opacity_vel = 0.0f;

    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;
    D3DMATERIAL9 m_Material;

    D3DXCreateSprite(m_Device,&m_Sprite);

    if ( FAILED( m_HR = m_Taslar.Load( m_Device, "res\\taslar.x" ) ) )
        return m_HR;

    m_Taslar.States = new LPDIRECT3DSTATEBLOCK9[2];

    m_Device->BeginStateBlock();

    m_Device->SetRenderState(D3DRS_CLIPPING,TRUE);
    m_Device->SetRenderState(D3DRS_LIGHTING,TRUE);
    m_Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
    m_Device->SetRenderState(D3DRS_ZENABLE,TRUE);
    m_Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
    m_Device->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

    m_Device->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_COLORVALUE(1, 1, 1, 1.0));
    m_Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);

    m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);

    ZeroMemory( &m_Material, sizeof(D3DMATERIAL9) );

    m_Material.Diffuse.r = m_Material.Ambient.r = 1;
    m_Material.Diffuse.g = m_Material.Ambient.g = 1;
    m_Material.Diffuse.b = m_Material.Ambient.b = 1;

    m_Device->SetMaterial(&m_Material);

    m_Device->SetTexture(0,m_TextureCache->Textures[TextureCache::CHROME]);

    m_Device->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_CAMERASPACENORMAL);
    m_Device->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);

    m_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    m_Eye = D3DXVECTOR3(0,0,-255);
    m_LookAt = D3DXVECTOR3(0,0,0);
    m_UpVect = D3DXVECTOR3(0,1,0);
    
    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);
    D3DXMatrixPerspectiveFovLH(&m_Proj, D3DX_PI / 4, 1.3f, 1, 1000);

    m_Device->SetTransform(D3DTS_VIEW,&m_View);
    m_Device->SetTransform(D3DTS_PROJECTION,&m_Proj);

    m_Device->EndStateBlock(&m_Taslar.States[0]);

//---------------------------------------------------------------------------

    m_Device->BeginStateBlock();

    m_Device->SetRenderState(D3DRS_CLIPPING,TRUE);
    m_Device->SetRenderState(D3DRS_LIGHTING,TRUE);
    m_Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
    m_Device->SetRenderState(D3DRS_ZENABLE,TRUE);
    m_Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
    m_Device->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
    
    m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
    m_Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    m_Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCCOLOR);

    m_Device->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_COLORVALUE(1, 1, 1, 1.0));
    m_Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);

    ZeroMemory( &m_Material, sizeof(D3DMATERIAL9) );

    m_Material.Diffuse.r = m_Material.Ambient.r = 1;
    m_Material.Diffuse.g = m_Material.Ambient.g = 1;
    m_Material.Diffuse.b = m_Material.Ambient.b = 1;

    m_Device->SetMaterial(&m_Material);

    m_Device->SetTexture(0,m_TextureCache->Textures[TextureCache::CHROME]);

    m_Device->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_CAMERASPACENORMAL);
    m_Device->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);

    m_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    m_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    //m_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
    //m_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    //m_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    m_Device->EndStateBlock(&m_Taslar.States[1]);

    return m_HR;
}


void cubuklar::Render()
{
    if ( SUCCEEDED(this->m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_XRGB(0,0,0),1.0f,0 ) ) )
    {
        if( SUCCEEDED( m_Device->BeginScene() ) )
        {



            D3DXMATRIX m_TransformMatrix;
            D3DXMATRIX m_TransformMatrix2;

            D3DXMatrixIdentity(&m_TransformMatrix);
            D3DXMatrixIdentity(&m_TransformMatrix2);

            //D3DXMatrixTranslation(&m_TransformMatrix2, -100, -200, 0 );
            D3DXMatrixTranslation(&m_TransformMatrix2, -256, -256, 0 );
            m_TransformMatrix *= m_TransformMatrix2;

            D3DXMatrixRotationZ(&m_TransformMatrix2, number / 60.0f );
            m_TransformMatrix *= m_TransformMatrix2;

            D3DXMatrixScaling(&m_TransformMatrix2,2,2,2);
            m_TransformMatrix *= m_TransformMatrix2;

            D3DXMatrixTranslation(&m_TransformMatrix2, 400, 300, 0 );
            m_TransformMatrix *= m_TransformMatrix2;

            m_Sprite->DrawTransform(m_TextureCache->Textures[TextureCache::RENKLI],NULL,&m_TransformMatrix,D3DCOLOR_COLORVALUE( 1, 1, 1, opacity  ));

            D3DXMatrixIdentity(&m_TransformMatrix);
            D3DXMatrixIdentity(&m_TransformMatrix2);

            //D3DXMatrixTranslation(&m_TransformMatrix2, -100, -200, 0 );
            D3DXMatrixTranslation(&m_TransformMatrix2, -256, -256, 0 );
            m_TransformMatrix *= m_TransformMatrix2;

            D3DXMatrixRotationZ(&m_TransformMatrix2, -1 * number / 40.0f );
            m_TransformMatrix *= m_TransformMatrix2;

            D3DXMatrixScaling(&m_TransformMatrix2,2,2,2);
            m_TransformMatrix *= m_TransformMatrix2;

            D3DXMatrixTranslation(&m_TransformMatrix2, 400, 300, 0 );
            m_TransformMatrix *= m_TransformMatrix2;

            m_Sprite->DrawTransform(m_TextureCache->Textures[TextureCache::RENKLI],NULL,&m_TransformMatrix,D3DCOLOR_COLORVALUE( 1, 1, 1, opacity / 2 ));

            D3DXMatrixIdentity(&m_Taslar.Matrix);

            for(int i=1; i >= 0; i--)
            {
                if ( i == 1 )
                    m_Taslar.RotateMeshXYZ( 0, number / 45.0f * -1 , number / 50.0f );

                m_Taslar.States[i]->Apply();

                m_Eye.z = (sinf(((float)number)/50) * 50);
                m_Eye.z += 170;
                m_Eye.y = (sinf(((float)number)/40) * 80);

                D3DXMATRIX m_View;
                D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);

                m_Device->SetTransform(D3DTS_VIEW,&m_View);

                if ( opacity < 1.0f )
                {
                    D3DMATERIAL9 m_Material;

                    ZeroMemory( &m_Material, sizeof(D3DMATERIAL9) );

                    m_Material.Diffuse.r = m_Material.Ambient.r = opacity;
                    m_Material.Diffuse.g = m_Material.Ambient.g = opacity;
                    m_Material.Diffuse.b = m_Material.Ambient.b = opacity;

                    m_Device->SetMaterial(&m_Material);
                }            

                if ( i == 0 )
                    m_Taslar.ScaleMesh( 0.9f, 0.9f, 0.9f );

                m_Device->SetTransform(D3DTS_WORLD, &m_Taslar.Matrix );

                m_Taslar.Render();
            }

            //m_Taslar.States[0]->Apply();
            //m_Taslar.ScaleMesh( 0.9f, 0.9f, 0.9f );
            //m_Device->SetTransform(D3DTS_WORLD, &m_Taslar.Matrix );
            //m_Taslar.Render();


            
        }

        m_Device->EndScene();
    }
    
}