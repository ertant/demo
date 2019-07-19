#pragma once

#include <d3dx9.h>
#include "TextureCache.h"

class Boru
    {
    private:
        static const int flarecount = 70;

        float m_Angles[flarecount];
        float m_AngleVelocity[flarecount];

        LPD3DXSPRITE m_Sprite;
        TextureCache* m_TextureCache;
        LPDIRECT3DDEVICE9 m_Device;

    public:

        LPDIRECT3DTEXTURE9 Texture;

        void SetDevice(LPDIRECT3DDEVICE9 device,TextureCache * cache);
        HRESULT ResetDevice();
        void MoveParts();

        float XOffset;
        float YOffset;

        float XOffsetAdd;
        float YOffsetAdd;

        Boru(void);
        ~Boru(void);

        virtual void Render();
    };
