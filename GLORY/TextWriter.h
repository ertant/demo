#pragma once

#include <d3dx9.h>
#include "texturecache.h"

class TextWriter
{
    LPDIRECT3DDEVICE9 m_Device;
    LPD3DXFONT    m_Font;
    LPD3DXSPRITE m_Sprite;
    TextureCache * m_TextureCache;

    HRESULT TextWriter::CreateD3DXFont(LPD3DXFONT* ppd3dxFont, TCHAR* pstrFont, DWORD dwSize );

public:
    TextWriter(void);
    ~TextWriter(void);


    void SetDevice(LPDIRECT3DDEVICE9 device,TextureCache * cache);
    virtual void ResetDevice();
    void Render( LPCSTR string , LPRECT rect, DWORD format, D3DCOLOR color);

    void Render( int Texture ,  D3DXVECTOR2 * translation, D3DXVECTOR2 * scale );
};
