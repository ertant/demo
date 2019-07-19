#pragma once


#include <d3dx9.h>

class CMesh
{


    
    
    
    LPDIRECT3DDEVICE9       m_Device;

    LPDIRECT3DVERTEXBUFFER9 vb;

public:

    LPD3DXMESH              Mesh;

    BOOL                    UseMaterial;
    BOOL                    UseMatrix;
    D3DXMATRIX              Matrix;
    DWORD                   numVertices;
    D3DMATERIAL9*           Materials;
    DWORD                   NumMaterials;

    HRESULT Load( LPDIRECT3DDEVICE9 device, char * meshFile );
    HRESULT LoadFromResource(LPDIRECT3DDEVICE9 device,WORD name);

    void Render();
    void Render(DWORD subSet);

    void RotateMeshX( float angle );
    void RotateMeshY( float angle );
    void RotateMeshZ( float angle );

    void RotateMeshXYZ( float x, float y, float z );

    void Translate( float x, float y, float z );

    void ScaleMesh( float x, float y, float z );
    void ScaleMesh( float range );

    void SetMatrix();

    void** LockVertices();
    void Unlock();

    void Release();

    HRESULT SetFVF( DWORD fvf );

    CMesh(void);
    ~CMesh(void);
};
