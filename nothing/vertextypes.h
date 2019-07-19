
#if !defined(VertexTypes__INCLUDED_)
#define VertexTypes__INCLUDED_

#include <d3dx9.h>

struct Vertex_PositionedTextured 
    {
    float x;
    float y;
    float z;
    float u;
    float v;
    static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
    };

//const DWORD Vertex_PositionedTextured::FVF = D3DFVF_XYZ | D3DFVF_TEX1 ;

struct Vertex_PosColText
    {
    float x;
    float y;
    float z;
    int color;
    float u;
    float v;
    static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE;
    };

struct Vertex_PosColTextNorm
    {
    float x;
    float y;
    float z;
    float n1;
    float n2;
    float n3;
    int color;
    float u;
    float v;
    static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_NORMAL;
    };

struct Vertex_PosTextNorm
    {
    float x;
    float y;
    float z;
    float n1;
    float n2;
    float n3;
    float u;
    float v;
    static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL;
    };

struct Vertex_PosTextNormBlend
    {
    float x;
    float y;
    float z;
    float blend;
    float n1;
    float n2;
    float n3;
    float u;
    float v;
    static const DWORD FVF = D3DFVF_XYZB1 | D3DFVF_TEX1 | D3DFVF_NORMAL;
    };

struct Vertex_PosColSize
    {
    float x;
    float y;
    float z;
    float size;
    DWORD color;
    static const DWORD FVF = D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE;
    };

//const DWORD Vertex_PositionedColoredTextured::FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE;

extern void UpdateVertex( Vertex_PositionedTextured* data, int index, float x, float y, float z, float u, float v );

extern void UpdateVertex( Vertex_PosColText* data, int index, float x, float y, float z, int color, float u, float v );

extern void UpdateVertex( Vertex_PosColTextNorm* data, int index, float x, float y, float z, int color, float u, float v );

extern void UpdateVertex( Vertex_PosColSize* data, int index, float x, float y, float z, int color, float size );



#endif