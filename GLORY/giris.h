#if !defined(GIRIS__INCLUDED_)
#define GIRIS__INCLUDED_

#include "BaseSection.h"

class Giris : BaseSection
{
private:
    float op;
    float op_vel;
    int time;

public:
    virtual void FrameMove();
    virtual void Render();
    virtual HRESULT ResetDevice();
    virtual void Cleanup();
    virtual void SyncMeta( DWORD data );
};

#endif