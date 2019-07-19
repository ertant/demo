#define _CRT_SECURE_NO_DEPRECATE
#if(NDEBUG)
#define MUSIC_ENABLED 1
#else
//#define MUSIC_ENABLED 1
#endif


#include <d3dx9.h>
#include <strsafe.h>
#include <mmsystem.h>
#include "bass.h"
#include "resource.h"
#include "baseSection.h"
#include "effects.h"

#include "topuz.h"
#include "titrek.h"
#include "shine.h"
#include "tunnel.h"
#include "arms.h"
#include "greets2.h"
#include "uzun.h"
#include "noise.h"
//#include "planes.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D       = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device

static const int        m_MAXSections      = 8;
BaseSection*            m_Sections[m_MAXSections];
int                     m_SectionIndex      = 0;
TextureCache*           m_TextureCache;
HWND                    m_HWND;
HSTREAM                 m_HStream;
DWORD                   m_FirstTime         = 0;
DWORD                   m_LastTime          = 0;
Effects*                m_Effects;
UINT                    m_TimerID;
char *                  m_Title = "N O T H I N G";

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
    {
    SAFE_RELEASE(m_TextureCache);
    SAFE_RELEASE(m_Effects);

#if(NDEBUG)
    for(int index = 0; index < m_MAXSections; index++)
        {
        SAFE_RELEASE(m_Sections[index]);
        }
#else
    SAFE_RELEASE(m_Sections[m_SectionIndex]);
#endif

    SAFE_RELEASE(g_pd3dDevice);
    SAFE_RELEASE(g_pD3D);

    BASS_Free();
    }

HRESULT LoadMusic()
    {
    m_HStream = NULL;

    HRSRC hResource = FindResource(NULL,MAKEINTRESOURCE(MUSIC_GLOBAL),RT_RCDATA);

    if ( hResource != NULL )
        {
        DWORD length = SizeofResource(NULL,hResource);
        HGLOBAL hResourceLoaded = LoadResource(NULL,hResource);

        if ( hResourceLoaded != NULL )
            {
            void * buffer = LockResource(hResourceLoaded);

            if ( buffer != NULL )
                {
                m_HStream = BASS_StreamCreateFile(TRUE,buffer,0,length,BASS_SAMPLE_3D );

                if ( m_HStream == 0 )
                    {
                    return E_FAIL;
                    }
                }
            }
        }

    if ( m_HStream == NULL )
        {
        return E_FAIL;
        }

    return S_OK;
    }


//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
    {
#if(NDEBUG)
    int m_Result = MessageBox(hWnd,"Fullscreen ?",m_Title,MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON1);
#else
    int m_Result = IDNO;

    m_Result = MessageBox(hWnd,"Fullscreen ?",m_Title,MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON1);
#endif

    

    if ( m_Result == IDCANCEL )
        return E_FAIL;

    // Create the D3D object, which is needed to create the D3DDevice.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        {
        MessageBox(hWnd,"Unable to access directx. Please check you have lastest version of directx.",m_Title,MB_OK | MB_ICONERROR);

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

    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, &g_pd3dDevice ) ) )
        {
        MessageBox(hWnd,"Unable to create direct3d device. Please check you have a nice screen card.",m_Title,MB_OK | MB_ICONERROR);
        return E_FAIL;
        }

    Sleep(500);

    if ( m_Result == IDYES )
        g_pd3dDevice->ShowCursor(FALSE);

    for(unsigned i = 0;i < 8;++i)
        {
        g_pd3dDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);
        g_pd3dDevice->SetSamplerState(i,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
        g_pd3dDevice->SetSamplerState(i,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
        }


    if ( !BASS_Init(1,44100,0,m_HWND,NULL) )
        BASS_Init(0,44100,0,m_HWND,NULL);

    if ( FAILED(LoadMusic() ) )
        {
        MessageBox(hWnd,"Unable play music.",m_Title,MB_OK | MB_ICONERROR);
        return E_FAIL;
        }

    //m_HStream = BASS_StreamCreateFile(FALSE,"res\\music.mp3",0,0,BASS_STREAM_AUTOFREE );

    m_TextureCache = new TextureCache();
    m_TextureCache->SetDevice(g_pd3dDevice);

    if ( FAILED(m_TextureCache->ResetDevice() ) )
        {
        MessageBox(hWnd,"Unable load textures.",m_Title,MB_OK | MB_ICONERROR);
        return E_FAIL;
        }

    m_Effects = new Effects();

    if ( FAILED( m_Effects->SetDevice(g_pd3dDevice,m_TextureCache)))
        {
        MessageBox(hWnd,"Effects failed.",m_Title,MB_OK | MB_ICONERROR);
        return E_FAIL;
        }

#if(_DEBUG)
    m_SectionIndex = 6;
#else
    m_SectionIndex = 0;
#endif

    m_Sections[0] = (BaseSection*)new topuz();
    m_Sections[1] = (BaseSection*)new Titrek();
    m_Sections[2] = (BaseSection*)new Shine();
    m_Sections[3] = (BaseSection*)new Tunnel();
    m_Sections[4] = (BaseSection*)new Arms();
    m_Sections[5] = (BaseSection*)new Greets();
    m_Sections[6] = (BaseSection*)new Uzun();
    m_Sections[7] = (BaseSection*)new Noise();

    HRESULT m_HR;

#if(NDEBUG)

    for(int index = 0; index < m_MAXSections; index++)
        {
        m_Sections[index]->SetDevice(g_pd3dDevice,m_TextureCache,m_Effects);

        if ( FAILED( m_HR = m_Sections[index]->ResetDevice()) )
            {
            Cleanup();

            char output[50];

            sprintf(output,"Unable reset section %d. Error Code : %x", index, m_HR);

            MessageBox(hWnd,output,m_Title,MB_OK | MB_ICONERROR);

            return E_FAIL;
            }
        }
#else

    m_Sections[m_SectionIndex]->SetDevice(g_pd3dDevice,m_TextureCache,m_Effects);

        if ( FAILED( m_HR = m_Sections[m_SectionIndex]->ResetDevice()) )
            {
            Cleanup();

            char output[50];

            sprintf(output,"Unable reset section %d. Error Code : %x", m_SectionIndex, m_HR);

            MessageBox(hWnd,output,m_Title,MB_OK | MB_ICONERROR);

            return E_FAIL;
            }

#endif


#if(MUSIC_ENABLED)
    if ( m_HStream != NULL )
        {
        BASS_ChannelPlay(m_HStream,false);
        }
#endif

    return S_OK;
    }


//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
    {
    if( NULL == g_pd3dDevice )
        return;

    if ( m_SectionIndex < m_MAXSections )
        {

        if ( m_Sections[m_SectionIndex]->Finished == TRUE )
            {
            m_Sections[m_SectionIndex]->Finished = FALSE;

            m_SectionIndex++;

            m_Effects->Flash();

            if ( m_SectionIndex > m_MAXSections )
                SendMessage(m_HWND,WM_QUIT,0,0);
            }

        if ( m_SectionIndex < m_MAXSections )
            {
            m_Sections[m_SectionIndex]->Render();

            m_Effects->RenderFlash();

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
            Cleanup();
            PostQuitMessage( 0 );
            return 0;

        case WM_PAINT:

            Render();

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

void CALLBACK OnTimer(UINT wTimerID, UINT msg, 
                      DWORD dwUser, DWORD dw1, DWORD dw2) 
    { 
    DWORD current = timeGetTime();

    if ( m_FirstTime == 0 )
        {
        m_FirstTime = current;
        }

    DWORD m_Time = current - m_FirstTime;
    DWORD m_Delta = m_Time - m_LastTime;

    if ( m_LastTime == 0 )
        m_LastTime = m_Time;

    m_Effects->FrameMove(m_Time,m_Delta);

    if ( m_SectionIndex < m_MAXSections )
        m_Sections[m_SectionIndex]->FrameMove(m_Time, m_Delta);

        m_LastTime = m_Time;
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
        "Demo", NULL };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( "Demo", m_Title, 
        //WS_OVERLAPPEDWINDOW,
        WS_EX_OVERLAPPEDWINDOW,
        100, 
        100, 
        800, 
        600,
        GetDesktopWindow(), NULL, wc.hInstance, NULL );

    m_HWND = hWnd;

    TIMECAPS tc;
    UINT     wTimerRes;

    if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
        {
        // Error; application can't continue.
        }

    wTimerRes = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);
    timeBeginPeriod(wTimerRes);

    // Initialize Direct3D
    if( SUCCEEDED( InitD3D( hWnd ) ) )
        { 
        // Show the window
        ShowWindow( hWnd, SW_SHOWDEFAULT );
        UpdateWindow( hWnd );

        m_TimerID = timeSetEvent(1,wTimerRes,OnTimer,0,TIME_PERIODIC);

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

    timeKillEvent(m_TimerID);

    DestroyWindow(hWnd);
    UnregisterClass( "Glory", wc.hInstance );
    return 0;
    }
