#include "TextureCache.h"
#include <d3dx9.h>

TextureCache::TextureCache(void)
{

}

TextureCache::~TextureCache(void)
{

}

void TextureCache::SetDevice(LPDIRECT3DDEVICE9 device)
{
    m_Device = device;
}

HRESULT TextureCache::ResetDevice()
{
    HRESULT m_HR;
    Textures = new LPDIRECT3DTEXTURE9[Texture_Count];

    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\chr.png" , &Textures[TextureCache::CHROME] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\greets.jpg" , &Textures[TextureCache::GREETSBACK] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\karadul.jpg" , &Textures[TextureCache::YILDIZBACK] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\s1.jpg" , &Textures[TextureCache::GIRIS1BACK] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\s2.jpg" , &Textures[TextureCache::GIRIS2BACK] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\s3.jpg" , &Textures[TextureCache::GIRIS3BACK] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\gal.jpg" , &Textures[TextureCache::GALAXY] )) )
        return m_HR;
    //if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\tex1.jpg" , &Textures[TextureCache::TEX1] )) )
    //    return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\texture.jpg" , &Textures[TextureCache::TEX2] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\yaprak2.png" , &Textures[TextureCache::YAPRAK] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\tunnel.jpg" , &Textures[TextureCache::TUNNEL] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\text1.png" , &Textures[TextureCache::TEXT1] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\text2.png" , &Textures[TextureCache::TEXT2] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\text3.png" , &Textures[TextureCache::TEXT3] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\text4.png" , &Textures[TextureCache::TEXT4] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\renkli3.png" , &Textures[TextureCache::RENKLI] )) )
        return m_HR;
    //if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\renkli2.png" , &Textures[TextureCache::RENKLI2] )) )
    //    return m_HR;
    //if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\darkcloud.jpg" , &Textures[TextureCache::DARKCLOUD] )) )
    //    return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\greets_text0.png" , &Textures[TextureCache::GREETSTEXT0] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\greets_text1.png" , &Textures[TextureCache::GREETSTEXT1] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\greets_text2.png" , &Textures[TextureCache::GREETSTEXT2] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\greets_text3.png" , &Textures[TextureCache::GREETSTEXT3] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\greets_text4.png" , &Textures[TextureCache::GREETSTEXT4] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\greets_text5.png" , &Textures[TextureCache::GREETSTEXT5] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\greets_text6.png" , &Textures[TextureCache::GREETSTEXT6] )) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\greets_text7.png" , &Textures[TextureCache::GREETSTEXT7] )) )
        return m_HR;

    //if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\ring.png" , &Textures[TextureCache::RING] )) )
    //    return m_HR;
    //if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "res\\colorpass.png" , &Textures[TextureCache::COLORPASS] )) )
    //    return m_HR;

    return S_OK;
}
