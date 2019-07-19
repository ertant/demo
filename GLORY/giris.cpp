#include "Giris.h"
#include <d3dx9.h>

void Giris::FrameMove()
{
    op += op_vel;
}

HRESULT Giris::ResetDevice()
{
    BaseSection::ResetDevice();

    op = 0.0f;
    time = 0;
    op_vel = 0.01f;

    return S_OK;
}

void Giris::Cleanup()
{

}

void Giris::SyncMeta( DWORD data )
{
    if ( data == 0x10 )
        {
        time++;
        op = 0.0f;
        }
    else if ( data == 0x20 )
        {
        time++;
        op = 0.0f;
        op_vel = 0.007f;
        }
    else if ( data == 0x30 )
        Finished = true;
}

void Giris::Render()
{
    if ( SUCCEEDED(this->m_Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,D3DCOLOR_XRGB(0,0,0),1.0f,0 ) ) )
    {
        m_Device->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_XRGB(0,0,0));

        if( SUCCEEDED( m_Device->BeginScene() ) )
        {
            if ( time == 2 )
                {
                m_Background->Render(m_TextureCache->Textures[TextureCache::GIRIS1BACK], op);
                }
            else if ( time == 3 )
                {
                m_Background->Render(m_TextureCache->Textures[TextureCache::GIRIS2BACK], op);
                }
            else if ( time == 4 )
                {
                m_Background->Render(m_TextureCache->Textures[TextureCache::GIRIS3BACK], op);
                }
        }

        m_Device->EndScene();
    }
}