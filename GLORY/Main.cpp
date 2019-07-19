#include <d3dx9.h>
#include <strsafe.h>
#include "bass.h"
#include "baseSection.h"
#include "giris.h"
#include "yildiz.h"
#include "tunnel.h"
#include "cubuklar.h"
#include "topuz.h"
#include "greets.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D       = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device

static const int        m_MAXSections      = 6;
BaseSection             *m_Sections[m_MAXSections];
int                     m_SectionIndex      = 0;
Background*             m_BackgroundDrawer;   
TextureCache*           m_TextureCache;
int                     m_FrameNumber       = 0;
HWND                    m_HWND;
HMUSIC                  m_HMusic;

#define TIMERPERIOD	50		// timer period (ms)

void CALLBACK SyncProc(HSYNC handle, DWORD channel, DWORD data, DWORD user)
{
#if(_DEBUG)
    char output[50];
    
    ZeroMemory(output,50);
#endif

#if(_DEBUG)
    sprintf(output,"sync value : %x\n",data);
    OutputDebugString( output );
#endif

    m_Sections[m_SectionIndex]->SyncMeta( data );
}

void FrameMove()
    {
        if ( m_SectionIndex < m_MAXSections )
            m_Sections[m_SectionIndex]->FrameMove();
    }

//void CALLBACK TimerCall(HWND win, UINT m, UINT i, DWORD t)
//    {
//        FrameMove();
//    }




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if( g_pd3dDevice != NULL) 
        g_pd3dDevice->Release();

    if( g_pD3D != NULL)
        g_pD3D->Release();
}

//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
    int m_Result = MessageBox(hWnd,"Fullscreen ?","G L O R Y",MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON1);
    
    if ( m_Result == IDCANCEL )
        return E_FAIL;

    

    // Create the D3D object, which is needed to create the D3DDevice.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
    {
        MessageBox(hWnd,"Unable to access directx. Please check you have lastest version of directx.","G L O R Y",MB_OK | MB_ICONERROR);

        return E_FAIL;
    }

    // Set up the structure used to create the D3DDevice. Most parameters are
    // zeroed out. We set Windowed to TRUE, since we want to do D3D in a
    // window, and then set the SwapEffect to "discard", which is the most
    // efficient method of presenting the back buffer to the display.  And 
    // we request a back buffer format that matches the current desktop display 
    // format.
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    
    if ( m_Result == IDNO )
        d3dpp.Windowed = TRUE;
    else
        d3dpp.Windowed = FALSE;

    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferCount = 1;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferWidth = 800;
    d3dpp.BackBufferHeight = 600;

    // Create the Direct3D device. Here we are using the default adapter (most
    // systems only have one, unless they have multiple graphics hardware cards
    // installed) and requesting the HAL (which is saying we want the hardware
    // device rather than a software one). Software vertex processing is 
    // specified since we know it will work on all cards. On cards that support 
    // hardware vertex processing, though, we would see a big performance gain 
    // by specifying hardware vertex processing.
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        MessageBox(hWnd,"Unable to create direct3d device. Please check you have a nice screen card.","G L O R Y",MB_OK | MB_ICONERROR);
        return E_FAIL;
    }

    //MessageBox(hWnd,"DEVICEOK","DEVICEOK",MB_OK);

    Sleep(200);

    if ( m_Result == IDYES )
        g_pd3dDevice->ShowCursor(FALSE);

    if ( !BASS_Init(1,44100,0,m_HWND,NULL) )
        BASS_Init(0,44100,0,m_HWND,NULL);

    m_HMusic = BASS_MusicLoad(FALSE,"res\\VISION.XM",0,0,BASS_MUSIC_SURROUND2,0);

    //MessageBox(hWnd,"MUSICOK","MUSICOK",MB_OK);
    
    BASS_ChannelSetSync(m_HMusic,BASS_SYNC_MUSICFX,1,&SyncProc,0);

    m_BackgroundDrawer = new Background();
    m_BackgroundDrawer->SetDevice(g_pd3dDevice);
    m_BackgroundDrawer->ResetDevice();

    m_TextureCache = new TextureCache();
    m_TextureCache->SetDevice(g_pd3dDevice);

    if ( FAILED(m_TextureCache->ResetDevice() ) )
    {
        MessageBox(hWnd,"Unable load textures.","G L O R Y",MB_OK | MB_ICONERROR);
        return E_FAIL;
    }

    m_Sections[0] = (BaseSection*)new Giris();
    m_Sections[1] = (BaseSection*)new Yildiz();
    m_Sections[2] = (BaseSection*)new Tunnel();
    m_Sections[3] = (BaseSection*)new cubuklar();
    m_Sections[4] = (BaseSection*)new topuz();
    m_Sections[5] = (BaseSection*)new Greets();

    HRESULT m_HR;

    for(int index = 0; index < m_MAXSections; index++)
        {
            m_Sections[index]->SetDevice(g_pd3dDevice,m_TextureCache);
            m_Sections[index]->SetBackground(m_BackgroundDrawer);
        
            if ( FAILED( m_HR = m_Sections[index]->ResetDevice()) )
            {
                Cleanup();

                char output[50];

                sprintf(output,"Unable reset section %d. Error Code : %x", index, m_HR);

                MessageBox(hWnd,output,"G L O R Y",MB_OK | MB_ICONERROR);

                return E_FAIL;
            }
        }

    //MessageBox(hWnd,"SECTIONSOK","SECTIONSOK",MB_OK);

#if(_DEBUG1)
    m_SectionIndex = 5;
#else
    m_SectionIndex = 0;

    //SetTimer(m_HWND,1,TIMERPERIOD,&TimerCall);

    BASS_ChannelPlay(m_HMusic,false);
#endif

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
    FrameMove();

    m_FrameNumber++;

    if( NULL == g_pd3dDevice )
        return;

    if ( m_SectionIndex < m_MAXSections )
    {
        if ( m_Sections[m_SectionIndex]->Finished == TRUE )
            {
                m_SectionIndex++;

                if ( m_SectionIndex > m_MAXSections )
                    SendMessage(m_HWND,WM_QUIT,0,0);
            }

        if ( m_SectionIndex < m_MAXSections )
        {
            m_Sections[m_SectionIndex]->Render();

            g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            //KillTimer(m_HWND,1);
            BASS_Free();
            Cleanup();
            PostQuitMessage( 0 );
            return 0;

        case WM_PAINT:
            
            Render();
            //ValidateRect( hWnd, NULL );
            return 0;
        case WM_KEYUP:

            if ( wParam == 27 )     // ESC
                PostMessage(m_HWND,WM_QUIT,0,0);
            return 0;

        case WM_SETCURSOR:
            SetCursor( NULL );
            g_pd3dDevice->ShowCursor(FALSE);
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
    // Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "Glory", NULL };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( "Glory", "[G L O R Y]", 
                              //WS_OVERLAPPEDWINDOW,
                              WS_EX_OVERLAPPEDWINDOW,
                              100, 
                              100, 
                              800, 
                              600,
                              GetDesktopWindow(), NULL, wc.hInstance, NULL );

    m_HWND = hWnd;

    // Initialize Direct3D
    if( SUCCEEDED( InitD3D( hWnd ) ) )
    { 
        // Show the window
        ShowWindow( hWnd, SW_SHOWDEFAULT );
        UpdateWindow( hWnd );

        // Enter the message loop
        MSG msg; 
        while( GetMessage( &msg, NULL, 0, 0 ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );

            if ( m_SectionIndex >= m_MAXSections )
                break;
        }
    }

    UnregisterClass( "Glory", wc.hInstance );
    return 0;
}
