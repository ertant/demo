#if !defined(GREETS__INCLUDED_)
#define GREETS__INCLUDED_

#include "BaseSection.h"
#include "Boru.h"

class Greets : BaseSection
{
private:
    int frame;

    Boru * m_Boru;

public:

    Greets(void);

    virtual void Cleanup();
    virtual void FrameMove();
    virtual void Render();
    virtual HRESULT ResetDevice();
    virtual void SyncMeta( DWORD data );
};

#endif
