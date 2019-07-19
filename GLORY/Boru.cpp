#include "Boru.h"

Boru::Boru(void)
    {
    for(int index = 0; index < flarecount; index ++ )
        {
        m_Angles[index] = 0.0f;
        }

for(int index = 0; index < flarecount; index ++ )
    {
    m_AngleVelocity[index] = 0.0f;
    }

m_AngleVelocity[0] = 0.1f;

XOffset = 0;
YOffset = 0;

XOffsetAdd = 0;
YOffsetAdd = 10;

Texture = NULL;

    }

Boru::~Boru(void)
    {
    }

HRESULT Boru::ResetDevice()
    {
    HRESULT m_HR;

    if ( FAILED( m_HR = D3DXCreateSprite(m_Device,&m_Sprite) ) )
        return m_HR;

    return m_HR;
    }

void Boru::SetDevice(LPDIRECT3DDEVICE9 device,TextureCache * cache)
    {
    m_Device = device;
    m_TextureCache = cache;
    }

void Boru::MoveParts()
    {

    for(int index = 1; index < flarecount; index ++ )
        {
        m_Angles[index] += m_AngleVelocity[index];

        m_AngleVelocity[index] += (m_AngleVelocity[index - 1] - m_AngleVelocity[index]) / 10;
        }

    m_Angles[0] += m_AngleVelocity[0];

    if ( m_AngleVelocity[0] >= 2)
        {
        m_AngleVelocity[0] = -0.01f;
        }
    else if ( m_AngleVelocity[0] <= -2)
        {
        m_AngleVelocity[0] = 0.01f;
        }

    if ( m_AngleVelocity[0] > 0 )
        m_AngleVelocity[0] += 0.01f;
    else
        m_AngleVelocity[0] -= 0.01f;
    }


void Boru::Render()
    {

    if ( Texture != NULL )
        {
        m_Sprite->Begin();

        for(int index = 0; index < flarecount ; index ++) 
            {

            D3DXMATRIX m_TransformMatrix;
            D3DXMATRIX m_TempMatrix;

            D3DXMatrixIdentity(&m_TransformMatrix);

            D3DXMatrixTranslation(&m_TempMatrix,-127,-127,0);

            m_TransformMatrix *= m_TempMatrix;

            D3DXMatrixRotationZ(&m_TempMatrix, D3DXToRadian( m_Angles[index] ) );

            m_TransformMatrix *= m_TempMatrix;

            D3DXMatrixTranslation( &m_TempMatrix, 
                XOffset + ( index * XOffsetAdd ),
                YOffset + ( index * YOffsetAdd ), 
                0);

            m_TransformMatrix *= m_TempMatrix;

            D3DXVECTOR3 m_Center  ( -127, -127, 0 );

            m_Sprite->DrawTransform(Texture,NULL,&m_TransformMatrix,D3DCOLOR_XRGB(255,255,255));

            }

        m_Sprite->End();
        }

    }