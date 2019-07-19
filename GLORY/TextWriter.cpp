#include "TextWriter.h"


TextWriter::TextWriter(void)
{
}

TextWriter::~TextWriter(void)
{
}

HRESULT TextWriter::CreateD3DXFont( LPD3DXFONT* ppd3dxFont, 
                                           TCHAR* pstrFont, DWORD dwSize )
{
    HRESULT hr;
    LPD3DXFONT pd3dxFontNew = NULL;
    HDC hDC;
    INT nHeight;
    HFONT hFont;

    hDC = GetDC( NULL );
    nHeight = -MulDiv( dwSize, GetDeviceCaps(hDC, LOGPIXELSY), 72 );
    ReleaseDC( NULL, hDC );
    hFont = CreateFont( nHeight, 0, 0, 0, FW_DONTCARE, false, false, false, 
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, pstrFont );
    if( hFont == NULL )
        return E_FAIL;
    hr = D3DXCreateFont( m_Device, hFont, &pd3dxFontNew );
    DeleteObject( hFont );

    if( SUCCEEDED( hr ) )
        *ppd3dxFont = pd3dxFontNew;

    return hr;
}

void TextWriter::SetDevice(LPDIRECT3DDEVICE9 device, TextureCache * cache)
{
    m_Device = device;
    m_TextureCache = cache;
}

void TextWriter::ResetDevice()
{
    if ( FAILED(CreateD3DXFont( &m_Font, TEXT("Arial"), 20 ) ) )
        DebugBreak();

    D3DXCreateSprite(m_Device,&m_Sprite);
}

void TextWriter::Render( LPCSTR string , LPRECT rect, DWORD format, D3DCOLOR color)
{
    m_Font->Begin();

    if ( m_Font->DrawText( string, -1, rect, format , color ) == 0 )
        DebugBreak();

    m_Font->End();
}

void TextWriter::Render( int texture, D3DXVECTOR2 * translation, D3DXVECTOR2 * scale )
{
    m_Sprite->Begin();
    m_Sprite->Draw(m_TextureCache->Textures[texture], NULL, scale, NULL, 0 ,translation,0xFFFFFFFF );
    m_Sprite->End();
}