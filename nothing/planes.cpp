#include "Planes.h"
#include "resource.h"

Planes::Planes(void)
    {

    }

Planes::~Planes(void)
    {
    }

void Planes::FrameMove( DWORD time , DWORD delta)
    {

        for(int plane = 0; plane < planeCount; plane++)
        {
        planes[plane].x  -= delta * 0.02f;
        
        if ( planes[plane].x < -80 )
            planes[plane].x = 80 ; //+ ( (float)rand() / (float)RAND_MAX ) * 80.0f;
        }

    glow += 0.00001f;

    if ( glow > 0.02f )
        glow = 0.01f;


    if ( (time / 1000) == 219 )
        Finished = TRUE;
    }

void Planes::Release()
    {
    SAFE_RELEASE(m_State);
    }

HRESULT Planes::ResetDevice()
    {
    HRESULT m_HR;

    glow = 0.01f;

    planeCount = 60;

    planes = new D3DXVECTOR4[planeCount];

    int max = 100;
    int min = 0;
    
    srand( 123456 );
    for(int plane = 0; plane < planeCount; plane++)
        {
        //planes[plane].x  = ( (float)rand() / (float)RAND_MAX ) * 650.0f;
        //planes[plane].y  = 200 + ( (float)rand() / (float)RAND_MAX ) * 20.0f;
        //planes[plane].z  = 400.0f + ( (float)rand() / (float)RAND_MAX ) * 150.0f;
        //planes[plane].w  = 0.0f;

        planes[plane].x  = -80 + ( (float)rand() / (float)RAND_MAX ) * 180.0f;
        planes[plane].y  =  10;  //10 + ( (float)rand() / (float)RAND_MAX ) * 2.0f;
        planes[plane].z  =  -10 + ( (float)rand() / (float)RAND_MAX ) * -40.0f;
        planes[plane].w  = 0.0f;
        }

    m_Eye = D3DXVECTOR3(0,0,-15);
    m_LookAt = D3DXVECTOR3(0,0,0);
    m_UpVect = D3DXVECTOR3(0,1,0);

    D3DXMatrixPerspectiveFovLH(&m_Proj,/*D3DX_PI / 4*/ 2.3f, 1.3f, 1, 1000);

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
    m_Device->SetRenderState(           D3DRS_SPECULARENABLE,           TRUE);
    
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

void Planes::RenderScene()
    {
    m_State->Apply();
    
    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);

    m_Device->SetTransform(D3DTS_VIEW,&m_View);

    for(int plane = 0; plane < planeCount; plane++)
        {
        D3DXMATRIX orientation;
        D3DXMATRIX object;

        float zValue = 100 / planes[plane].z ; 

        D3DXMatrixIdentity(&object);

        //D3DXMatrixScaling(&orientation, zValue, zValue, zValue );
        //object *= orientation;

        D3DXMatrixTranslation(&orientation, planes[plane].x, planes[plane].y, planes[plane].z );
        object *= orientation;

        D3DXMatrixRotationX(&orientation, DEGREE(-11));
        object *= orientation;

        m_Effects->Quad.SetStraightMatrix();
        m_Effects->Quad.Transform( &object );
        m_Effects->Quad.Render( m_TextureCache->Textures[TextureCache::TEST] );

        //m_Effects->RenderSprite(  m_TextureCache->Textures[TextureCache::TEST], &object, D3DCOLOR_COLORVALUE(1,1,1,1) );
        }
    }

void Planes::Render()
    {
    m_Device->BeginScene();

    m_Effects->StartGlow(D3DCOLOR_COLORVALUE(0,0,0,0),true);

    RenderScene();

    m_Effects->EndGlow(glow,FALSE);

    m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_COLORVALUE(0,0,0,0),1.0f,0 );

    

    

    RenderScene();

    m_Effects->RenderGlow();

    //m_Effects->RenderSprite( m_TextureCache->Textures[TextureCache::TEXT5], &textMatrix, D3DCOLOR_COLORVALUE(1,1,1,0.4f) );
    //m_Effects->RenderSprite( m_TextureCache->Textures[TextureCache::TEXT6], &textMatrix2, D3DCOLOR_COLORVALUE(1,1,1,0.4f) );

    //m_Effects->RenderFilledTexture( m_TextureCache->Textures[TextureCache::FILTER1]);

    m_Device->EndScene(); 
    }