#pragma once

#include "BaseSection.h"

class Noise : BaseSection
{
private:
    D3DXMATRIX noiseMatrix;
    D3DXVECTOR2 center;

    float image;
    float rotation;
    float alpha;

public:
    Noise(void);
    ~Noise(void);

    virtual void Release();
    virtual void FrameMove( DWORD time , DWORD delta);
    virtual void Render();
    virtual HRESULT ResetDevice();
};
