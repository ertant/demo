#pragma once

#include <d3dx9.h>
#include "TextureCache.h"

class TextureCache
{
    LPDIRECT3DDEVICE9 m_Device;

    static const int Texture_Count = 30;

public:

    static const int CHROME = 1;
    //static const int FLARE = 2;
    static const int GREETSBACK = 3;
    static const int YILDIZBACK = 4;
    static const int GIRIS1BACK = 5;
    static const int GIRIS2BACK = 6;
    static const int GIRIS3BACK = 7;
    static const int GALAXY = 8;
    //static const int TEX1 = 9;
    static const int TEX2 = 10;
    static const int YAPRAK = 11;

    static const int TEXT1 = 12;
    static const int TEXT2 = 13;
    static const int TEXT3 = 14;
    static const int TEXT4 = 15;
    static const int TUNNEL = 16;
    static const int RENKLI = 17;
    //static const int RENKLI2 = 18;
    //static const int DARKCLOUD = 19;
    static const int GREETSTEXT0 = 20;
    static const int GREETSTEXT1 = 21;
    static const int GREETSTEXT2 = 22;
    static const int GREETSTEXT3 = 23;
    static const int GREETSTEXT4 = 24;
    static const int GREETSTEXT5 = 25;
    static const int GREETSTEXT6 = 26;
    static const int GREETSTEXT7 = 27;
    //static const int RING = 28;
    //static const int COLORPASS = 29;


    LPDIRECT3DTEXTURE9* Textures;

    TextureCache(void);
    ~TextureCache(void);

    void SetDevice(LPDIRECT3DDEVICE9 device);
    HRESULT ResetDevice();
};

