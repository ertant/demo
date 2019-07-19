#include "Greets.h"
#include <d3dx9.h>

Greets::Greets(VOID)
{
    m_Boru = new Boru();
}

void Greets::FrameMove()
{
    frame++;

    m_Boru->MoveParts();
}


void Greets::SyncMeta( DWORD data )
{
    if ( data == 0xC0 )
        Finished = TRUE;
}

void Greets::Cleanup()
{
}

HRESULT Greets::ResetDevice()
{
    BaseSection::ResetDevice();

    m_Boru->SetDevice(m_Device,m_TextureCache);
    m_Boru->ResetDevice();

    m_Boru->Texture = m_TextureCache->Textures[TextureCache::YAPRAK];

    frame = 0;

    for(int index = 0; index < 400; index++)
        m_Boru->MoveParts();

    return S_OK;
}

void Greets::Render()
{
    if ( SUCCEEDED(this->m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_XRGB(255,255,255),1.0f,0 ) ) )
    {
        if( SUCCEEDED( m_Device->BeginScene() ) )
        {
            m_Background->Render(m_TextureCache->Textures[TextureCache::GREETSBACK], 1.0f);

            m_Boru->XOffset = ((float)frame * 2) - 200;

            if ( m_Boru->XOffset > 140 )
                m_Boru->XOffset = 140;

            m_Boru->YOffset = 30;

            m_Boru->Render();

            int yoffset = -(frame) + 600;

            for(int i= 0; i < 7; i++)
            {
                //RECT m_Rect;
                //SetRect(&m_Rect,0,0,0,0);

                D3DXVECTOR2 m_Pos(400,yoffset + (float)(i * 73));
                //D3DXVECTOR2 m_Scale(1,1);

                //m_Scale.y = 1 + sin((float)(frame ) / 60) * 0.09f;
                //m_Scale.x = 1 - sin((float)(frame ) / 60) * 0.09f;
                //m_Pos.y /= m_Scale.y;
                //m_Pos.x /= m_Scale.x;

                m_Writer->Render( TextureCache::GREETSTEXT0 + i, &m_Pos , NULL);
            }
        }

        m_Device->EndScene();
    }
}