#if !defined(EFFECTS__INCLUDED_)
#define EFFECTS__INCLUDED_

#include "texturecache.h"
#include "vertexTypes.h"
#include "cmesh.h"
#include "cquad.h"

class Effects
    {
    private:
        LPD3DXEFFECT            m_Effect;
        LPDIRECT3DTEXTURE9      m_GlowTexture[2];
        LPDIRECT3DSURFACE9      m_GlowSurface[2];
        LPDIRECT3DDEVICE9       m_Device;
        LPDIRECT3DSURFACE9		m_BackBuffer;
        CQuad                   m_GlowQuad;
        LPDIRECT3DSTATEBLOCK9   m_GlowState;
        LPD3DXSPRITE            m_Sprite;
        TextureCache*           m_TextureCache;            

        float m_FlashAlpha;
        float m_FlashSpeed;

    public:

        Effects();

        void Release();

        void FrameMove(DWORD time,DWORD delta);

        void Flash();

        void SetFlashSpeed( float speed );

        void RenderFlash();

        CMesh   Cube;
        CQuad    Quad;

        HRESULT SetDevice(LPDIRECT3DDEVICE9 device,TextureCache * cache);

        void RenderSprite( LPDIRECT3DTEXTURE9 texture, D3DXMATRIX * transform , D3DCOLOR color);

        void DrawSkyCube(LPDIRECT3DTEXTURE9 texture, float fov, float z, float aspect);

        void StartGlow(D3DCOLOR backColor,BOOL clear);
        void RenderGlow();

        void EndGlow(float glowSize,BOOL zoomHack);

        HRESULT CreateRenderTargetTexture(DWORD width,DWORD height, LPDIRECT3DTEXTURE9 * texture);

        void StartRenderToTexture(LPDIRECT3DTEXTURE9 texture);

        void EndRenderToTexture();

        void RenderFilledTexture(LPDIRECT3DTEXTURE9 texture,D3DCOLOR color);
        void RenderFilledTexture(LPDIRECT3DTEXTURE9 texture);
        void RenderFilledTexture(LPDIRECT3DTEXTURE9 texture,D3DXVECTOR2 * center,float rotation);
        void RenderFilledTexture(LPDIRECT3DTEXTURE9 texture,D3DXVECTOR2 * center,float rotation,D3DCOLOR color);
    };

#endif
