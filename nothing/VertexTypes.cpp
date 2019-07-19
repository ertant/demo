#include "VertexTypes.h"

void UpdateVertex( Vertex_PositionedTextured* data, int index, float x, float y, float z, float u, float v )
    {
    data[index].x = x;
    data[index].y = y;
    data[index].z = z;
    data[index].u = u;
    data[index].v = v;
    }

void UpdateVertex( Vertex_PosColText* data, int index, float x, float y, float z, int color,float u, float v )
    {
    data[index].x = x;
    data[index].y = y;
    data[index].z = z;
    data[index].u = u;
    data[index].v = v;
    data[index].color = color;
    }

void UpdateVertex( Vertex_PosColTextNorm* data, int index, float x, float y, float z, int color,float u, float v )
    {
    data[index].x = x;
    data[index].y = y;
    data[index].z = z;
    data[index].u = u;
    data[index].v = v;
    data[index].color = color;
    }

void UpdateVertex( Vertex_PosColSize* data, int index, float x, float y, float z, int color, float size )
    {
    data[index].x = x;
    data[index].y = y;
    data[index].z = z;
    data[index].color = color;
    data[index].size = size;
    }