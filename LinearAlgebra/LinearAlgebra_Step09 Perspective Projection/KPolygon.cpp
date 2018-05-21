#include "stdafx.h"
#include "KPolygon.h"
#include "KVectorUtil.h"

void DrawIndexedPrimitive( HDC hdc
    , int* m_indexBuffer            // index buffer
    , int primitiveCounter          // primitive counter
    , KVector3* m_vertexBuffer      // vertex buffer
    , COLORREF color )
{
    int   i0, i1, i2;
    int   counter = 0;

    for (int i = 0; i < primitiveCounter; ++i)
    {
        // get index
        i0 = m_indexBuffer[counter];
        i1 = m_indexBuffer[counter + 1];
        i2 = m_indexBuffer[counter + 2];

        KVector3 normal;
        normal = Cross(m_vertexBuffer[i0] - m_vertexBuffer[i1], m_vertexBuffer[i0] - m_vertexBuffer[i2]);
        KVector3 forward(0, 0, 1);
        int penStyle = PS_SOLID;
        int penWidth = 3;
        if (Dot(forward, normal) > 0)
        {
            penStyle = PS_DOT;
            penWidth = 1;
        }

        // draw triangle
        KVectorUtil::DrawLine(hdc, m_vertexBuffer[i0].x, m_vertexBuffer[i0].y
            , m_vertexBuffer[i1].x, m_vertexBuffer[i1].y, penWidth, penStyle, color );
        KVectorUtil::DrawLine(hdc, m_vertexBuffer[i1].x, m_vertexBuffer[i1].y
            , m_vertexBuffer[i2].x, m_vertexBuffer[i2].y, penWidth, penStyle, color );
        KVectorUtil::DrawLine(hdc, m_vertexBuffer[i2].x, m_vertexBuffer[i2].y
            , m_vertexBuffer[i0].x, m_vertexBuffer[i0].y, penWidth, penStyle, color );

		// advance to next primitive
        counter += 3;
    }//for
}//DrawIndexedPrimitive()


KPolygon::KPolygon()
{
    m_sizeIndex  = 0;
    m_sizeVertex = 0;
    m_color = RGB(0, 0, 255);
}//KPolygon::KPolygon()

KPolygon::~KPolygon()
{
}//KPolygon::~KPolygon()

void KPolygon::SetIndexBuffer()
{
    //{{ seojt: step3
    int buffer[] = {
        0,2,1,
        2,0,3,
        3,6,2,
        6,3,7,
        7,5,6,
        5,7,4,
        4,1,5,
        1,4,0,
        4,3,0,
        3,4,7,
        1,6,5,
        6,1,2
    };

    for (int i=0; i<_countof(buffer); ++i)
    {
        m_indexBuffer[i] = buffer[i];
    }//for
    m_sizeIndex = _countof(buffer);
    //}} seojt: step3
}//KPolygon::SetIndexBuffer()

void KPolygon::SetVertexBuffer()
{
    //{{ seojt: step3
    m_vertexBuffer[0] = KVector3( -5.f,  5.f, -5.f);
    m_vertexBuffer[1] = KVector3( -5.f, -5.f, -5.f);
    m_vertexBuffer[2] = KVector3(  5.f, -5.f, -5.f);
    m_vertexBuffer[3] = KVector3(  5.f,  5.f, -5.f);
    m_vertexBuffer[4] = KVector3( -5.f,  5.f,  5.f);
    m_vertexBuffer[5] = KVector3( -5.f, -5.f,  5.f);
    m_vertexBuffer[6] = KVector3(  5.f, -5.f,  5.f);
    m_vertexBuffer[7] = KVector3(  5.f,  5.f,  5.f);
    //}} seojt: step3
    m_sizeVertex = 8;
}//KPolygon::SetVertexBuffer()

void KPolygon::Render(HDC hdc)
{
    //{{ seojt: step3
    ::DrawIndexedPrimitive(
        hdc,
        m_indexBuffer,      // index buffer
        12,                 // primitive(triangle) counter
        m_vertexBuffer,     // vertex buffer
        m_color );
    //}} seojt: step3
}//KPolygon::Render()

//{{ seojt: step2
void KPolygon::RotateX(float theta)
{
    for (int i=0; i<m_sizeVertex; ++i)
    {
        m_vertexBuffer[i].RotateX(theta);
    }//for
}//KPolygon::RotateX(float theta)

void KPolygon::RotateY(float theta)
{
    for (int i=0; i<m_sizeVertex; ++i)
    {
        m_vertexBuffer[i].RotateY(theta);
    }//for
}//KPolygon::RotateY(float theta)

void KPolygon::Translate(float tx, float ty, float tz)
{
    for (int i=0; i<m_sizeVertex; ++i)
    {
        m_vertexBuffer[i].Translate(tx,ty,tz);
    }//for
}//KPolygon::Translate(float tx, float ty, float tz)
//}} seojt: step2


//{{ seojt: step3
void KPolygon::Transform(KMatrix4& mat)
{
    for (int i=0; i<m_sizeVertex; ++i)
    {
        m_vertexBuffer[i] = mat * m_vertexBuffer[i];
    }//for
}//KPolygon::Transform()
//}} seojt: step3

//{{ seojt: step3
void KPolygon::Projection(KMatrix4& mat)
{
    float d = mat(0,0);
    float z;

    for (int i=0; i<m_sizeVertex; ++i)
    {
        z = m_vertexBuffer[i].z;
        m_vertexBuffer[i] = mat * m_vertexBuffer[i];
        m_vertexBuffer[i].x /= (z+d);
        m_vertexBuffer[i].y /= (z+d);
    }//for
}//KPolygon::Projection()
//}} seojt: step3
