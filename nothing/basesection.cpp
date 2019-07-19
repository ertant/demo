#include "BaseSection.h"

BaseSection::BaseSection()
{
    Finished = FALSE;
}

void BaseSection::SetDevice(LPDIRECT3DDEVICE9 device,TextureCache * cache,Effects * effects)
{
    m_Device = device;
    m_TextureCache = cache;
    m_Effects = effects;
}

void BaseSection::FrameMove(DWORD time, DWORD delta)
{
}

void BaseSection::Render()
{
}

HRESULT BaseSection::ResetDevice()
{
    return S_OK;
}

void BaseSection::Release()
{
}
