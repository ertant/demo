#include "Noise.h"
#include "resource.h"

Noise::Noise(void)
    {

    }

Noise::~Noise(void)
    {
    }

void Noise::FrameMove( DWORD time , DWORD delta)
    {
    image += delta * 0.01f;

    if ( image > TextureCache::NOISE3 )
        image = TextureCache::NOISE1;

    if ( rotation = 0 )
        rotation = DEGREE(180);
    else
        rotation = 0;

    alpha -= delta * 0.001f;
    
    if ( alpha < 0 )
        alpha = 0;

    if ( (time / 1000) == 225 )
        Finished = TRUE;
    }

void Noise::Release()
    {
    }

HRESULT Noise::ResetDevice()
    {
    Finished = FALSE;

    image = TextureCache::NOISE1;
    rotation = DEGREE(0);
    alpha = 1.0f;

    center = D3DXVECTOR2(400,300);

    return S_OK;
    }

void Noise::Render()
    {

    m_Device->BeginScene();

    m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_COLORVALUE(0,0,0,0),1.0f,0 );

    m_Effects->RenderFilledTexture(m_TextureCache->Textures[(int)image],&center,rotation,D3DCOLOR_COLORVALUE(1,1,1,alpha));

    m_Effects->RenderFilledTexture( m_TextureCache->Textures[TextureCache::FILTER1]);

    m_Device->EndScene(); 
    }