#include "BaseSection.h"

BaseSection::BaseSection()
{
    m_Writer = new TextWriter();
}

void BaseSection::SetDevice(LPDIRECT3DDEVICE9 device,TextureCache * cache)
{
    m_Device = device;
    m_TextureCache = cache;
    m_Writer->SetDevice(device,cache);
}

void BaseSection::SetBackground(Background* background)
{
    m_Background = background;
}

void BaseSection::FrameMove()
{
}

void BaseSection::Render()
{
}

HRESULT BaseSection::ResetDevice()
{
    m_Writer->ResetDevice();
    Finished = FALSE;

    return S_OK;
}

void BaseSection::SyncMeta( DWORD data )
{
}

void BaseSection::Cleanup()
{
}
