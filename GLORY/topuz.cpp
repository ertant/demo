#include "topuz.h"
#include <d3dx9.h>

void topuz::FrameMove()
{
    frame++;
}

void topuz::SyncMeta( DWORD data )
{
    if ( data == 0x90 )
        Finished = TRUE;
}

void topuz::Cleanup()
{
}

HRESULT topuz::ResetDevice()
{
    HRESULT m_HR;

    BaseSection::ResetDevice();

    frame = 0;

    if ( FAILED( m_HR = D3DXLoadMeshFromX("res\\cure.x",D3DXMESH_SYSTEMMEM,m_Device,&m_ObjectAdjacency,&m_ObjectMaterials ,NULL,&m_ObjectNumMaterial,&m_Mesh) ) )
        return m_HR;

    if( FAILED( m_HR = m_Device->BeginStateBlock()))
        return m_HR;

    m_Device->SetRenderState(D3DRS_CLIPPING,TRUE);
    m_Device->SetRenderState(D3DRS_DITHERENABLE, FALSE );
    m_Device->SetRenderState(D3DRS_SPECULARENABLE, FALSE );
    m_Device->SetRenderState(D3DRS_LIGHTING,TRUE);
    m_Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
    m_Device->SetRenderState(D3DRS_ZENABLE,TRUE);
    m_Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
    m_Device->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

    m_Device->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_COLORVALUE(0.35, 0.35, 0.35, 1.0));
    m_Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);

    D3DMATERIAL9 m_Material;
    ZeroMemory( &m_Material, sizeof(D3DMATERIAL9) );

    m_Material.Diffuse.r = m_Material.Ambient.r = 1;
    m_Material.Diffuse.g = m_Material.Ambient.g = 1;
    m_Material.Diffuse.b = m_Material.Ambient.b = 1;

    m_Device->SetMaterial(&m_Material);

    m_Device->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_PASSTHRU);
    m_Device->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);

    //m_Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
    //m_Device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
    //m_Device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_CURRENT);

    m_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

 /*   m_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    m_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);*/


    m_Eye = D3DXVECTOR3(0,0,-200);
    m_LookAt = D3DXVECTOR3(0,0,0);
    m_UpVect = D3DXVECTOR3(0,1,0);
    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;
    
    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);
    D3DXMatrixPerspectiveFovLH(&m_Proj, D3DX_PI / 4, 1.3f, 1, 1000);

    m_Device->SetTransform(D3DTS_VIEW,&m_View);
    m_Device->SetTransform(D3DTS_PROJECTION,&m_Proj);

    m_Device->EndStateBlock(&m_State);

    return S_OK;
}


void topuz::Render()
{
    if ( SUCCEEDED(this->m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_XRGB(80,80,80),1.0f,0 ) ) )
    {
        if( SUCCEEDED( m_Device->BeginScene() ) )
        {
            //m_Background->Render(m_BackgroundTexture, 1.0f);            

            m_State->Apply();

            m_Device->LightEnable(0, TRUE);

            D3DXMATRIX m_Orientation;
            D3DXMATRIX m_MeshMatrix;

            D3DXMatrixIdentity(&m_Orientation);
            D3DXMatrixIdentity(&m_MeshMatrix);

            m_MeshMatrix *= m_Orientation;

            D3DXMatrixRotationZ(&m_Orientation, sinf(frame / 87.0f) + cosf(frame / 97.0f) + 10);
            m_MeshMatrix *= m_Orientation;

            D3DXMatrixRotationY(&m_Orientation, frame / 57.0f * -1);
            m_MeshMatrix *= m_Orientation;

            m_Device->SetTransform(D3DTS_WORLD,&m_MeshMatrix);

            m_Eye.z = (sinf(((float)frame)/50) * 20);
            m_Eye.z += 170;

            D3DXMATRIX m_View;
            D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);

            D3DLIGHT9 light;
            ZeroMemory(&light, sizeof(D3DLIGHT9));

            light.Type = D3DLIGHT_POINT;
            light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 155.0f;
            light.Range = 120.0f;
            light.Attenuation0 = 1.0f;
            light.Attenuation1 = 1.0f;
            light.Position = 0.5 * m_Eye;
            light.Position.x += cos((float) frame / 50 ) * 20;
            
            m_Device->SetLight(0, &light);

            m_Device->SetTransform(D3DTS_VIEW,&m_View);

            m_Device->SetTexture(0,m_TextureCache->Textures[TextureCache::TEX2]);

            m_Device->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_PASSTHRU);

            m_Mesh->DrawSubset(0);            
            m_Mesh->DrawSubset(1);

            RECT m_Rect;
            SetRect(&m_Rect,0,0,0,0);

            D3DXVECTOR2 m_Pos(150,200);
            D3DXVECTOR2 m_Scale(1,1);

            m_Scale.y = 1 + sin((float)frame / 60) * 0.05f;
            m_Scale.x = 1 - sin((float)frame / 60) * 0.05f;
            m_Pos.y /= m_Scale.y;
            m_Pos.x /= m_Scale.x;

            m_Writer->Render( TextureCache::TEXT4, &m_Pos , &m_Scale);

        }

        m_Device->EndScene();
    }
    
}