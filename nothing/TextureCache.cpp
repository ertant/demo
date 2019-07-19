#include "TextureCache.h"
#include <d3dx9.h>
#include "resource.h"
#include "macros.h"

TextureCache::TextureCache(void)
    {

    }

TextureCache::~TextureCache(void)
    {
    delete Textures;
    }

void TextureCache::SetDevice(LPDIRECT3DDEVICE9 device)
    {
    m_Device = device;
    }

void TextureCache::Release()
    {
    for(int i= 1; i < Texture_Count; i++)
        {
            SAFE_RELEASE(Textures[i]);
        }

    delete Textures;
    }

HRESULT TextureCache::ResetDevice()
    {
    HRESULT m_HR;
    Textures = new LPDIRECT3DTEXTURE9[Texture_Count];

    for(int i= 1; i < Texture_Count; i++)
        {
        Textures[i] = NULL;
        }

    if ( FAILED( m_HR = D3DXCreateTextureFromResource(m_Device,NULL, MAKEINTRESOURCE(TEXTURE_WHITE),&Textures[TextureCache::WHITE] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource(m_Device,NULL, MAKEINTRESOURCE(TEXTURE_LINES),&Textures[TextureCache::LINES] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource(m_Device,NULL, MAKEINTRESOURCE(TEXTURE_COLORS),&Textures[TextureCache::TEX1] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource(m_Device,NULL, MAKEINTRESOURCE(TEXTURE_LOGO),&Textures[TextureCache::LOGO] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_FILTER1) , &Textures[TextureCache::FILTER1] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_FILTER3) , &Textures[TextureCache::FILTER3] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_FILTER4) , &Textures[TextureCache::FILTER4] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_FILTER5) , &Textures[TextureCache::FILTER5] ) ) )
        return m_HR;

    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_STARS) , &Textures[TextureCache::STARS] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_CHROME) , &Textures[TextureCache::CHROME] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_HALU) , &Textures[TextureCache::HALU] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_WEST) , &Textures[TextureCache::WEST] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_MAVI) , &Textures[TextureCache::MAVI] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_LIGHTSQ) , &Textures[TextureCache::LIGHTSQ] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_SCALEBAR) , &Textures[TextureCache::SCALEBAR] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_SCALEBAR) , &Textures[TextureCache::SCALEBAR] ) ) )
        return m_HR;

    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_NOISE1) , &Textures[TextureCache::NOISE1] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_NOISE2) , &Textures[TextureCache::NOISE2] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_NOISE3) , &Textures[TextureCache::NOISE3] ) ) )
        return m_HR;

    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_TEXT1) , &Textures[TextureCache::TEXT1] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_TEXT2) , &Textures[TextureCache::TEXT2] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_TEXT3) , &Textures[TextureCache::TEXT3] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_TEXT4) , &Textures[TextureCache::TEXT4] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_TEXT5) , &Textures[TextureCache::TEXT5] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_TEXT6) , &Textures[TextureCache::TEXT6] ) ) )
        return m_HR;

    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_GREETS1) , &Textures[TextureCache::GREETS1] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_GREETS2) , &Textures[TextureCache::GREETS2] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_GREETS3) , &Textures[TextureCache::GREETS3] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_GREETS4) , &Textures[TextureCache::GREETS4] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_GREETS5) , &Textures[TextureCache::GREETS5] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_GREETS6) , &Textures[TextureCache::GREETS6] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_GREETS7) , &Textures[TextureCache::GREETS7] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_GREETS8) , &Textures[TextureCache::GREETS8] ) ) )
        return m_HR;


    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_ENVMAP01) , &Textures[TextureCache::ENVMAP01] ) ) )
        return m_HR;

    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_ROBOT1) , &Textures[TextureCache::ROBOT1] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_ROBOT2) , &Textures[TextureCache::ROBOT2] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_ROBOT3) , &Textures[TextureCache::ROBOT3] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromResource( m_Device,NULL, MAKEINTRESOURCE(TEXTURE_METAL) , &Textures[TextureCache::METAL] ) ) )
        return m_HR;

#if(!NDEBUG)
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "tmpres\\firebase.jpg" , &Textures[TextureCache::FIRE] ) ) )
        return m_HR;

    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "tmpres\\renkli2.png" , &Textures[TextureCache::RENKLI] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "tmpres\\renkli21.png" , &Textures[TextureCache::RENKLI21] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "tmpres\\flash.png" , &Textures[TextureCache::FLASH] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "tmpres\\COLORGRADE.png" , &Textures[TextureCache::COLORGRADE] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "tmpres\\grade.jpg" , &Textures[TextureCache::GRADE] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "tmpres\\lines2.png" , &Textures[TextureCache::LINES2] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "tmpres\\redblack.jpg" , &Textures[TextureCache::REDBLACK] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "tmpres\\grid.png" , &Textures[TextureCache::GRID] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "tmpres\\flare.png" , &Textures[TextureCache::FLARE] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "tmpres\\bok.jpg" , &Textures[TextureCache::BOK] ) ) )
        return m_HR;
    if ( FAILED( m_HR = D3DXCreateTextureFromFile( m_Device, "tmpres\\envmap05.jpg" , &Textures[TextureCache::ENVMAP05] ) ) )
        return m_HR;

#endif

    return S_OK;
    }
