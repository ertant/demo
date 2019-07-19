#pragma once

#include <d3dx9.h>

#include "TextureCache.h"

class TextureCache
{
    LPDIRECT3DDEVICE9 m_Device;

    static const int Texture_Count = 100;

public:

    void Release();

    static const int CHROME             = 0;
    static const int TEX1               = 1;
    static const int LINES              = 2;
    static const int STARS              = 4;
    static const int HALU               = 5;
    static const int LOGO               = 6;
    static const int WHITE              = 7;
    static const int FILTER1            = 8;
    static const int FILTER2            = 9;
    static const int FILTER3            = 10;
    static const int FILTER4            = 11;
    static const int WEST               = 12;
    static const int MAVI               = 13;
    static const int LIGHTSQ            = 14;
    static const int FILTER5            = 15;
    static const int SCALEBAR           = 16;

    static const int TEXT1              = 17;
    static const int TEXT2              = 18;
    static const int TEXT3              = 19;
    static const int TEXT4              = 20;
    static const int TEXT5              = 21;
    static const int TEXT6              = 22;

    static const int GREETS1            = 23;
    static const int GREETS2            = 24;
    static const int GREETS3            = 25;
    static const int GREETS4            = 26;
    static const int GREETS5            = 27;
    static const int GREETS6            = 28;
    static const int GREETS7            = 29;
    static const int GREETS8            = 30;

    static const int NOISE1             = 31;
    static const int NOISE2             = 32;
    static const int NOISE3             = 33;

    static const int ENVMAP01            = 34;

    static const int ROBOT1             = 35;
    static const int ROBOT2             = 36;
    static const int ROBOT3             = 37;
    static const int METAL              = 38;


#if(!NDEBUG)
    static const int FLARE              = 44;
    static const int BOK                = 45;
    static const int FIRE               = 46;
    static const int GRID               = 47;
    static const int REDBLACK           = 48;
    static const int LINES2             = 49;
    static const int GRADE              = 40;
    static const int COLORGRADE         = 41;
    static const int FLASH              = 42;
    static const int RENKLI             = 43;
    static const int RENKLI21           = 44;
    static const int ENVMAP05           = 47;
    static const int TEST               = 48;

    
#endif


    LPDIRECT3DTEXTURE9* Textures;

    TextureCache(void);
    ~TextureCache(void);

    void SetDevice(LPDIRECT3DDEVICE9 device);
    HRESULT ResetDevice();
};

