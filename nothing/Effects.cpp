#include "effects.h"
#include <d3dx9.h>
#include "resource.h"
#include "macros.h"

Effects::Effects()
    {
    m_BackBuffer = NULL;
    }

void Effects::RenderFlash()
    {
    m_Device->BeginScene();

    RenderFilledTexture( m_TextureCache->Textures[TextureCache::WHITE], 
        D3DCOLOR_ARGB( (DWORD)(m_FlashAlpha * 255.0f), 
        (DWORD)(m_FlashAlpha * 255.0f), 
        (DWORD)(m_FlashAlpha * 255.0f), 
        (DWORD)(m_FlashAlpha * 255.0f) ) );

    m_Device->EndScene();
    }

void Effects::SetFlashSpeed( float flashSpeed)
    {
        m_FlashSpeed = flashSpeed;
    }

void Effects::Flash()
    {
    m_FlashAlpha = 1.0f;
    }

void Effects::FrameMove(DWORD time,DWORD delta)
    {
    if ( m_FlashAlpha > 0 )
        {
        m_FlashAlpha -= m_FlashSpeed * delta;

        if ( m_FlashAlpha < 0 )
            m_FlashAlpha = 0;

        }
    else if ( m_FlashAlpha > 1 )
        {
        m_FlashAlpha = 1;
        }
    }

void Effects::Release()
    {
    m_Effect->Release();

    for(int i = 0; i < 2; i++)
        {
        m_GlowSurface[i]->Release();
        m_GlowTexture[i]->Release();
        }

    SAFE_RELEASE(m_BackBuffer);
    m_GlowQuad.Release();

    SAFE_RELEASE(m_GlowState);
    SAFE_RELEASE(m_Sprite);

    Cube.Release();
    Quad.Release();
    }

HRESULT Effects::SetDevice(LPDIRECT3DDEVICE9 device,TextureCache * cache)
    {
    m_Device = device;
    m_TextureCache = cache;
    HRESULT m_HR;

    m_FlashSpeed = 0.002f;

    // SkyCube Init Start
    if ( FAILED( m_HR =  Cube.LoadFromResource(m_Device,MESH_SPHERE) ) )
        return m_HR;

    // SkyCube Init End

    // Quad Init Start

    if ( FAILED( m_HR = Quad.SetDevice(device) ) )
        return m_HR;

    // Quad Init End


    // Glow Init Start

    if ( FAILED ( m_HR = m_GlowQuad.SetDevice(m_Device) ) )
        return m_HR;

    if ( FAILED( m_HR = D3DXCreateEffectFromResource(
        m_Device,
        NULL,
        MAKEINTRESOURCE(SHADER_BLUR),
        NULL,
        NULL,
        0,
        NULL,
        &m_Effect,
        NULL ) ) )
        return m_HR;

    for(int face=0; face < 2; face++)
        {
        if  ( FAILED( 
            m_HR = CreateRenderTargetTexture(
            256,
            256,
            &m_GlowTexture[face] ) ) )
            return m_HR;

        m_GlowTexture[face]->GetSurfaceLevel(0,&m_GlowSurface[face]);
        }

    m_Device->CreateStateBlock(D3DSBT_ALL,&m_GlowState);

    // Glow Init End

    if ( FAILED( m_HR = D3DXCreateSprite(m_Device,&m_Sprite) ) )
        return m_HR;

    return S_OK;
    }

void Effects::RenderSprite(LPDIRECT3DTEXTURE9 texture, D3DXMATRIX * transform , D3DCOLOR color)
    {
    m_Sprite->SetTransform(transform);
    m_Sprite->Begin(0);
    m_Sprite->Draw(texture, NULL, NULL, NULL, color );
    m_Sprite->End();

    //m_Sprite->Begin();
    //m_Sprite->DrawTransform(texture,NULL,transform,color);
    //m_Sprite->End();
    }

void Effects::StartGlow(D3DCOLOR backColor,BOOL clear)
    {
    m_Device->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_BackBuffer);
    m_Device->SetRenderTarget(0,m_GlowSurface[0]);

    if ( clear == TRUE )
        m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,backColor,1.0f,0 );
    }

void Effects::EndGlow(float glowSize,BOOL zoomHack)
    {
    D3DXMATRIX matMVP = m_GlowQuad.m_World * m_GlowQuad.m_View * m_GlowQuad.m_Proj;

    float scaleVal = 0.985f;
    D3DXMATRIX scale;
    D3DXMatrixScaling(&scale,scaleVal,scaleVal,scaleVal);

    int face = 0;

    D3DXHANDLE m_TechniqueHandle = m_Effect->GetTechnique(0);

    if ( SUCCEEDED( m_Effect->SetTechnique( m_TechniqueHandle ) ) )
        {
        UINT m_Passes;

        m_Effect->Begin( &m_Passes,0 );

        m_Effect->SetFloat("pSize",glowSize); 

        for(UINT p = 0; p < m_Passes - 1; p++)
            {
            D3DXPASS_DESC desc;
            D3DXHANDLE m_PassHandle = m_Effect->GetPass(m_TechniqueHandle,p);
            m_Effect->GetPassDesc(m_PassHandle,&desc);

            if ( zoomHack )
                {
                // little zoom out for zooming blur effect
                matMVP *= scale;
                }

            m_Effect->SetTexture("glowTexture",m_GlowTexture[face]);
            m_Effect->SetMatrix("texTrans",&matMVP);

            m_Device->SetRenderTarget(0,m_GlowSurface[(face + 1) % 2]);

            m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_COLORVALUE(0,0,0,0),1.0f,0 );

            //m_Device->BeginScene();

            //m_Effect->Pass(p);
            m_Effect->BeginPass(p);

            m_GlowQuad.Render(NULL);

            m_Effect->EndPass();

            //m_Device->EndScene();

            face = (++face) % 2;
            }

        m_Effect->End();
        }

    m_Device->SetRenderTarget(0,m_BackBuffer);
    }

void Effects::RenderGlow()
    {
    m_GlowState->Capture();

    m_Device->SetRenderState(D3DRS_ZENABLE,FALSE);
    m_Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
    m_Device->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

    D3DXHANDLE m_TechniqueHandle = m_Effect->GetTechnique(0);
    D3DXMATRIX matMVP = m_GlowQuad.m_World * m_GlowQuad.m_View * m_GlowQuad.m_Proj;

    float scaleVal = 1.015f;
    D3DXMATRIX scale;
    D3DXMatrixScaling(&scale,scaleVal,scaleVal,scaleVal);

    matMVP *= scale;

	if ( SUCCEEDED( m_Effect->SetTechnique( m_TechniqueHandle ) ) )
	{
		UINT m_Passes;

		if ( SUCCEEDED( m_Effect->Begin( &m_Passes,0 ) ) )
		{
			if ( SUCCEEDED( m_Effect->BeginPass(m_Passes-1) ) )
			{
				V(m_Effect->SetFloat("pSize",0.02f)); 

				V(m_Effect->SetTexture("glowTexture",m_GlowTexture[0]));
				V(m_Effect->SetMatrix("texTrans",&matMVP));

				if ( SUCCEEDED( m_Effect->CommitChanges() ) )
				{
					//m_Effect->Pass(m_Passes-1);

					m_GlowQuad.Render(NULL);

					V(m_Effect->EndPass());
				}

				m_Effect->End();
			}
		}

	}

    m_GlowState->Apply();
    }

void Effects::DrawSkyCube(LPDIRECT3DTEXTURE9 texture, float fov, float z, float aspect)
    {
    m_GlowState->Capture();

    m_Device->SetRenderState(           D3DRS_LIGHTING,                 FALSE);
    m_Device->SetRenderState(           D3DRS_ZFUNC,                    D3DCMP_LESS);
    m_Device->SetRenderState(           D3DRS_ZWRITEENABLE,             FALSE);
    m_Device->SetRenderState(           D3DRS_ALPHABLENDENABLE,         TRUE);
    m_Device->SetRenderState(           D3DRS_BLENDOP,                  D3DBLENDOP_ADD );
    m_Device->SetRenderState(           D3DRS_SRCBLEND,                 D3DBLEND_SRCALPHA);
    m_Device->SetRenderState(           D3DRS_DESTBLEND,                D3DBLEND_INVSRCALPHA);
    m_Device->SetRenderState(           D3DRS_CLIPPING,                 TRUE);
    m_Device->SetRenderState(           D3DRS_LIGHTING,                 TRUE);
    m_Device->SetRenderState(           D3DRS_CULLMODE,                 D3DCULL_NONE);
    m_Device->SetRenderState(           D3DRS_FILLMODE,                 D3DFILL_SOLID);
    m_Device->SetRenderState(           D3DRS_CULLMODE,                 D3DCULL_NONE);
    m_Device->SetRenderState(           D3DRS_VERTEXBLEND,              D3DVBF_DISABLE );

    m_Device->SetTextureStageState(0,   D3DTSS_COLOROP,                 D3DTOP_SELECTARG1 );
    m_Device->SetTextureStageState(0,   D3DTSS_COLORARG1,               D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,   D3DTSS_ALPHAOP,                 D3DTOP_SELECTARG1);
    m_Device->SetTextureStageState(0,   D3DTSS_ALPHAARG1,               D3DTA_TEXTURE);
    m_Device->SetTextureStageState(0,   D3DTSS_TEXCOORDINDEX,           D3DTSS_TCI_PASSTHRU);
    m_Device->SetTextureStageState(0,   D3DTSS_TEXTURETRANSFORMFLAGS,   D3DTTFF_DISABLE);

    D3DXVECTOR3 m_Eye = D3DXVECTOR3(0,0,0.1f);
    D3DXVECTOR3 m_LookAt = D3DXVECTOR3(0,0,0);
    D3DXVECTOR3 m_UpVect = D3DXVECTOR3(0,1,0);
    D3DXMATRIX m_View;
    D3DXMATRIX m_Proj;

    D3DXMatrixLookAtLH(&m_View,&m_Eye,&m_LookAt,&m_UpVect);
    D3DXMatrixPerspectiveFovLH(&m_Proj, fov,aspect, 1.0f, 1000.0f);

    m_Device->SetTransform(D3DTS_VIEW,&m_View);
    m_Device->SetTransform(D3DTS_PROJECTION,&m_Proj);
    m_Device->SetTransform(D3DTS_WORLD,&Cube.Matrix);

    m_Device->SetTexture( 0, texture );

    Cube.Render();

    m_GlowState->Apply();
    }

void Effects::RenderFilledTexture(LPDIRECT3DTEXTURE9 texture, D3DCOLOR color)
    {
    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);

    D3DXMATRIX scaleMatrix;
    D3DXMatrixScaling(&scaleMatrix,0.8f,0.6f,1);

	m_Sprite->SetTransform(&scaleMatrix);

	V(m_Sprite->Begin(0));
    V(m_Sprite->Draw( texture, NULL, NULL, NULL, color));
	V(m_Sprite->End());
    }

void Effects::RenderFilledTexture(LPDIRECT3DTEXTURE9 texture)
    {
    D3DXMATRIX scaleMatrix;
    D3DXMatrixScaling(&scaleMatrix,0.8f,0.6f,1);

	m_Sprite->SetTransform(&scaleMatrix);

	V(m_Sprite->Begin(0));
    V(m_Sprite->Draw( texture, NULL, NULL, NULL, D3DCOLOR_COLORVALUE(1,1,1,1)));
	V(m_Sprite->End());
    }

void Effects::RenderFilledTexture(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR2 * center, float rotation)
    {
    D3DXMATRIX scaleMatrix;
    D3DXMatrixScaling(&scaleMatrix,0.8f,0.6f,1);

	m_Sprite->SetTransform(&scaleMatrix);

	V(m_Sprite->Begin(0));
    V(m_Sprite->Draw( texture, NULL, NULL, NULL, D3DCOLOR_COLORVALUE(1,1,1,1)));
	V(m_Sprite->End());
    }

void Effects::RenderFilledTexture(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR2 * center, float rotation, D3DCOLOR color)
    {
    D3DXMATRIX scaleMatrix;
    D3DXMatrixScaling(&scaleMatrix,0.8f,0.6f,1);

	m_Sprite->SetTransform(&scaleMatrix);

	V(m_Sprite->Begin(0));
    V(m_Sprite->Draw( texture, NULL, NULL, NULL, color));
	V(m_Sprite->End());
    }

HRESULT Effects::CreateRenderTargetTexture(DWORD width,DWORD height, LPDIRECT3DTEXTURE9 * texture)
    {
    return m_Device->CreateTexture(
        width,
        height,
        0,
        D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT ,
        texture,
        NULL );
    }

void Effects::StartRenderToTexture(LPDIRECT3DTEXTURE9 texture)
    {
    V(m_Device->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_BackBuffer));

    LPDIRECT3DSURFACE9 surface = NULL;

    V(texture->GetSurfaceLevel(0,&surface));

    V(m_Device->SetRenderTarget(0,surface));
    }

void Effects::EndRenderToTexture()
    {
    V(m_Device->SetRenderTarget(0,m_BackBuffer));
    }
