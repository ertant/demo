#include "Arms.h"
#include "resource.h"

Arms::Arms(void)
    {

    }

Arms::~Arms(void)
    {
    }

void Arms::FrameMove( DWORD time , DWORD delta)
    {
    m_Effects->Cube.RotateMeshXYZ( 
        sin( time * 0.0002f ) * 0.0004f,
        cos( time * 0.0001f ) * 0.0004f, 
        sin( time * 0.0003f ) * 0.0004f
        );

    D3DXMATRIX orientation;

    D3DXMatrixRotationY(&orientation,delta * 0.0003f);
    m_Rotation *= orientation;
    D3DXMatrixRotationX(&orientation,delta * 0.0002f);
    m_Rotation *= orientation;
    D3DXMatrixRotationZ(&orientation,delta * 0.0001f);
    m_Rotation *= orientation;

    D3DXMatrixIdentity(&leftThing.Matrix);
    leftThing.Matrix *= m_Rotation;
    leftThing.Translate(-170,-50,-180);

    D3DXMatrixTranslation(&orientation,delta * -0.05f,0,0);
    textMatrix *= orientation;

    MoveArms(time, delta);

    if ( (time / 1000) == 137 )
        Finished = TRUE;
    }

void Arms::MoveArmPoint( Vertex_PosColSize * vertices , int verticeIndex, int arm, int armPoint  )
    {
    float moveScale = 50.0001f;

    float xdelta = vertices[ verticeIndex - 1 ].x - vertices[verticeIndex].x;
    float ydelta = vertices[ verticeIndex - 1 ].y - vertices[verticeIndex].y;
    float zdelta = vertices[ verticeIndex - 1 ].z - vertices[verticeIndex].z;

    //if( xdelta < ArmPointSpace )
    //    xdelta = 0;
    //else
    xdelta /= moveScale;

    //if( ydelta < ArmPointSpace )
    //    ydelta = 0;
    //else
    ydelta /= moveScale;

    //if( zdelta < ArmPointSpace )
    //    zdelta = 0;
    //else
    zdelta /= moveScale;

    float degree = DEGREE( (360 / ((float)(NumArms-1)) ) * arm );
    float zdegree = DEGREE( 360 * degree );

    armPoint++;

    xdelta -= cos( degree ) * ( armPoint * ArmPointSpace) * 0.78f;
    ydelta -= sin( degree ) * ( armPoint * ArmPointSpace) * 0.78f;
    zdelta -= cos( zdegree ) * ( armPoint * ArmPointSpace) * 0.78f;

    vertices[verticeIndex].x += xdelta;
    vertices[verticeIndex].y += ydelta;
    vertices[verticeIndex].z += zdelta;

    vertices[verticeIndex].size = (NumArmPoints - armPoint) * 0.04f;
    }

void Arms::MoveArms(DWORD time, DWORD delta)
    {
    Vertex_PosColSize* m_VerticeData;

    if( FAILED(m_Vertices->Lock(0,0,(void**)&m_VerticeData,0)))
        DebugBreak();

    m_VerticeData[0].x = sin( time * 0.002f ) * 180;
    m_VerticeData[0].y = cos( time * 0.001f ) * 180;
    //m_VerticeData[0].z = sin( time * 0.003f ) * 180;

    for(int arm = 0; arm < NumArms; arm++)
        {
        int verticeIndex = (arm * NumArmPoints);

        m_VerticeData[verticeIndex].x = m_VerticeData[ 0 ].x;
        m_VerticeData[verticeIndex].y = m_VerticeData[ 0 ].y;
        m_VerticeData[verticeIndex].z = m_VerticeData[ 0 ].z;
        m_VerticeData[verticeIndex].size = m_VerticeData[ 0 ].size;

        for(int armPoint = 1; armPoint < NumArmPoints; armPoint++)
            {
            int verticeIndex = (arm * NumArmPoints) + armPoint;

            MoveArmPoint(m_VerticeData, verticeIndex, arm, armPoint);
            }
        }

    m_Vertices->Unlock();
    }

void Arms::Release()
    {
    SAFE_RELEASE(m_State);
    }


float fPointSize = 3.0f,fPointScaleB = 1.0f;


HRESULT Arms::ResetDevice()
    {
    HRESULT m_HR;

    m_Eye = D3DXVECTOR3(0,0,-340);
    m_LookAt = D3DXVECTOR3(0,0,0);
    m_UpVect = D3DXVECTOR3(0,1,0);

    D3DXMatrixIdentity(&m_Rotation);

    if ( FAILED( m_HR =  leftThing.LoadFromResource(m_Device,MESH_SACMA) ) )
        return m_HR;

    D3DXMatrixTranslation(&textMatrix,850,320,0);

    NumArms = 30;
    NumArmPoints = 30;
    ArmPointSpace = 0.01f;

    VerticeCount = NumArms * NumArmPoints;

    if( FAILED( m_HR = m_Device->CreateVertexBuffer(VerticeCount * sizeof(Vertex_PosColSize),D3DUSAGE_WRITEONLY,Vertex_PositionedTextured::FVF, D3DPOOL_DEFAULT,&m_Vertices,NULL)))
        return m_HR;

    Vertex_PosColSize* m_VerticeData;

    if( FAILED(m_Vertices->Lock(0,0,(void**)&m_VerticeData,0)))
        DebugBreak();

    if( FAILED( m_HR = m_Device->CreateIndexBuffer(VerticeCount * sizeof(int),D3DUSAGE_WRITEONLY,D3DFMT_INDEX32,D3DPOOL_DEFAULT,&m_Indices,NULL)))
        return m_HR;

    int* m_IndiceData;
    int m_IndiceIndex = 0;

    if( FAILED(m_Indices->Lock(0,0,(void**)&m_IndiceData,0)))
        DebugBreak();

    for(int arm = 0; arm < NumArms; arm++)
        {
        float degree = (360 / ((float)(NumArms-1)) ) * arm;
        float armangle = DEGREE( degree );
        float zdegree = DEGREE( 360 * degree );

        for(int armPoint = 0; armPoint < NumArmPoints; armPoint++)
            {
            int verticeIndex = (arm * NumArmPoints) + armPoint ;

            UpdateVertex(m_VerticeData , verticeIndex,
                cos( armangle ) * ( armPoint * ArmPointSpace),
                sin( armangle ) * ( armPoint * ArmPointSpace),
                sin( zdegree ) * ( armPoint * ArmPointSpace),
                D3DCOLOR_COLORVALUE(1,1,1,1),
                1
                );

            m_IndiceData[m_IndiceIndex++] = verticeIndex;
            }
        }

    m_Vertices->Unlock();

    //for(int m_Index = 0; m_Index < VerticeCount; m_Index++)
    //    m_IndiceData[m_Index] = m_Index;

    m_Indices->Unlock();

    D3DXMatrixPerspectiveFovLH(&m_Proj, /*D3DX_PI / 4*/ 1.4f, 1.3f, 1, 1000);

    if( FAILED( m_HR = m_Device->BeginStateBlock()))
        return m_HR;

    m_Device->SetRenderState(           D3DRS_AMBIENT,                  D3DCOLOR_COLORVALUE(0.35, 0.35, 0.35, 0));
    m_Device->SetRenderState(           D3DRS_ZENABLE,                  FALSE);
    m_Device->SetRenderState(           D3DRS_CLIPPING,                 TRUE);
    m_Device->SetRenderState(           D3DRS_LIGHTING,                 FALSE);
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

    m_Device->SetRenderState(           D3DRS_POINTSPRITEENABLE,        TRUE );
    m_Device->SetRenderState(           D3DRS_POINTSCALEENABLE,         TRUE );

    m_Device->SetRenderState(           D3DRS_POINTSIZE,                *((DWORD*)&fPointSize));
    m_Device->SetRenderState(           D3DRS_POINTSCALE_B,             *((DWORD*)&fPointScaleB));

    m_Device->SetTextureStageState(0,   D3DTSS_COLOROP,                 D3DTOP_SELECTARG1 );
    m_Device->SetTextureStageState(0,   D3DTSS_COLORARG1,               D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,   D3DTSS_ALPHAOP,                 D3DTOP_SELECTARG1);
    m_Device->SetTextureStageState(0,   D3DTSS_ALPHAARG1,               D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,   D3DTSS_TEXCOORDINDEX,           D3DTSS_TCI_PASSTHRU);
    m_Device->SetTextureStageState(0,   D3DTSS_TEXTURETRANSFORMFLAGS,   D3DTTFF_DISABLE);

    m_Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    m_Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

    m_Device->SetTransform(             D3DTS_PROJECTION,               &m_Proj);

    if ( FAILED( m_HR = m_Device->EndStateBlock(&m_State) ) )
        return m_HR;

    return S_OK;
    }

void Arms::RenderScene()
    {
    m_State->Apply();

    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);

    m_Device->SetTransform(D3DTS_VIEW,&m_View);
    m_Device->SetTransform(D3DTS_WORLD,&m_Rotation);

    m_Device->SetTexture( 0, m_TextureCache->Textures[TextureCache::CHROME] );

    m_Device->SetStreamSource(0,m_Vertices,0,sizeof(Vertex_PosColSize));
    m_Device->SetIndices(m_Indices);
    m_Device->SetFVF(Vertex_PosColSize::FVF);

    m_Device->DrawIndexedPrimitive( D3DPT_POINTLIST,0,0,VerticeCount,0,VerticeCount);
    }

void Arms::Render()
    {
    m_Device->BeginScene();

    m_Effects->StartGlow(D3DCOLOR_COLORVALUE(0,0,0,0),TRUE);

    RenderScene();

    m_Effects->EndGlow(0.01f,TRUE);

    m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_COLORVALUE(0,0,0,0),1.0f, 0 );

    m_Effects->DrawSkyCube(m_TextureCache->Textures[TextureCache::MAVI],2.385f,1.0f,1.3f);
    m_Effects->DrawSkyCube(m_TextureCache->Textures[TextureCache::MAVI],2.985f,1.0f,1.3f);

    RenderScene();

    leftThing.Render();

    m_Effects->RenderGlow();

    m_Effects->RenderSprite( m_TextureCache->Textures[TextureCache::TEXT3], &textMatrix, D3DCOLOR_COLORVALUE(1,1,1,0.4f) );

    m_Effects->RenderFilledTexture( m_TextureCache->Textures[TextureCache::FILTER1]);

    m_Device->EndScene(); 
    }