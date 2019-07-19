#pragma once

#include <d3dx9.h>

class CMesh
{

    ID3DXBuffer*            m_ObjectAdjacency;
    ID3DXBuffer*            m_ObjectMaterials;
    DWORD                   m_ObjectNumMaterial;
    ID3DXMesh*              m_Mesh;
    

public:

    LPDIRECT3DSTATEBLOCK9 * States;
    D3DXMATRIX              Matrix;

    HRESULT Load( LPDIRECT3DDEVICE9 device, char * meshFile );
    void Render();

    void RotateMeshX( float angle );
    void RotateMeshY( float angle );
    void RotateMeshZ( float angle );

    void RotateMeshXYZ( float x, float y, float z );

    void Translate( float x, float y, float z );

    void ScaleMesh( float x, float y, float z );
    void ScaleMesh( float range );

    CMesh(void);
    ~CMesh(void);
};
