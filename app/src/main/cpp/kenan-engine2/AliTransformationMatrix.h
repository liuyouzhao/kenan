/*
 * Copyright (C) 2005, 2006 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef AliTransformationMatrix_h
#define AliTransformationMatrix_h

#include <string.h> //for memcpy

namespace DCanvas
{
class AliTransformationMatrix
{

public:
    typedef float Matrix4[4][4];

    AliTransformationMatrix() { makeIdentity(); }
    AliTransformationMatrix(const DCanvas::AliTransformationMatrix& t) { *this = t; }
    AliTransformationMatrix(float a, float b, float c, float d, float e, float f) { setMatrix(a, b, c, d, e, f); }
    AliTransformationMatrix(float m11, float m12, float m13, float m14,
                         float m21, float m22, float m23, float m24,
                         float m31, float m32, float m33, float m34,
                         float m41, float m42, float m43, float m44)
    {
        setMatrix(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
    }

    void setMatrix(float a, float b, float c, float d, float e, float f)
    {
        m_matrix[0][0] = a; m_matrix[0][1] = b; m_matrix[0][2] = 0; m_matrix[0][3] = 0;
        m_matrix[1][0] = c; m_matrix[1][1] = d; m_matrix[1][2] = 0; m_matrix[1][3] = 0;
        m_matrix[2][0] = 0; m_matrix[2][1] = 0; m_matrix[2][2] = 1; m_matrix[2][3] = 0;
        m_matrix[3][0] = e; m_matrix[3][1] = f; m_matrix[3][2] = 0; m_matrix[3][3] = 1;
    }

    void setMatrix(float m11, float m12, float m13, float m14,
                   float m21, float m22, float m23, float m24,
                   float m31, float m32, float m33, float m34,
                   float m41, float m42, float m43, float m44)
    {
        m_matrix[0][0] = m11; m_matrix[0][1] = m12; m_matrix[0][2] = m13; m_matrix[0][3] = m14;
        m_matrix[1][0] = m21; m_matrix[1][1] = m22; m_matrix[1][2] = m23; m_matrix[1][3] = m24;
        m_matrix[2][0] = m31; m_matrix[2][1] = m32; m_matrix[2][2] = m33; m_matrix[2][3] = m34;
        m_matrix[3][0] = m41; m_matrix[3][1] = m42; m_matrix[3][2] = m43; m_matrix[3][3] = m44;
    }

    AliTransformationMatrix& operator =(const AliTransformationMatrix &t)
    {
        setMatrix(t.m_matrix);
        return *this;
    }

    AliTransformationMatrix& makeIdentity()
    {
        setMatrix(1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1);
        return *this;
    }

    bool isIdentity() const
    {
        return m_matrix[0][0] == 1 && m_matrix[0][1] == 0 && m_matrix[0][2] == 0 && m_matrix[0][3] == 0 &&
               m_matrix[1][0] == 0 && m_matrix[1][1] == 1 && m_matrix[1][2] == 0 && m_matrix[1][3] == 0 &&
               m_matrix[2][0] == 0 && m_matrix[2][1] == 0 && m_matrix[2][2] == 1 && m_matrix[2][3] == 0 &&
               m_matrix[3][0] == 0 && m_matrix[3][1] == 0 && m_matrix[3][2] == 0 && m_matrix[3][3] == 1;
    }

    // This form preserves the float math from input to output
    void map(float x, float y, float& x2, float& y2) const { multVecMatrix(x, y, x2, y2); }
    // Map a 3D point
    void map(float x, float y, float z, float& x2, float& y2, float& z2) const { multVecMatrix(x, y, z, x2, y2, z2); }


    float m11() const { return m_matrix[0][0]; }
    void setM11(float f) { m_matrix[0][0] = f; }
    float m12() const { return m_matrix[0][1]; }
    void setM12(float f) { m_matrix[0][1] = f; }
    float m13() const { return m_matrix[0][2]; }
    void setM13(float f) { m_matrix[0][2] = f; }
    float m14() const { return m_matrix[0][3]; }
    void setM14(float f) { m_matrix[0][3] = f; }
    float m21() const { return m_matrix[1][0]; }
    void setM21(float f) { m_matrix[1][0] = f; }
    float m22() const { return m_matrix[1][1]; }
    void setM22(float f) { m_matrix[1][1] = f; }
    float m23() const { return m_matrix[1][2]; }
    void setM23(float f) { m_matrix[1][2] = f; }
    float m24() const { return m_matrix[1][3]; }
    void setM24(float f) { m_matrix[1][3] = f; }
    float m31() const { return m_matrix[2][0]; }
    void setM31(float f) { m_matrix[2][0] = f; }
    float m32() const { return m_matrix[2][1]; }
    void setM32(float f) { m_matrix[2][1] = f; }
    float m33() const { return m_matrix[2][2]; }
    void setM33(float f) { m_matrix[2][2] = f; }
    float m34() const { return m_matrix[2][3]; }
    void setM34(float f) { m_matrix[2][3] = f; }
    float m41() const { return m_matrix[3][0]; }
    void setM41(float f) { m_matrix[3][0] = f; }
    float m42() const { return m_matrix[3][1]; }
    void setM42(float f) { m_matrix[3][1] = f; }
    float m43() const { return m_matrix[3][2]; }
    void setM43(float f) { m_matrix[3][2] = f; }
    float m44() const { return m_matrix[3][3]; }
    void setM44(float f) { m_matrix[3][3] = f; }

    float a() const { return m_matrix[0][0]; }
    void setA(float a) { m_matrix[0][0] = a; }

    float b() const { return m_matrix[0][1]; }
    void setB(float b) { m_matrix[0][1] = b; }

    float c() const { return m_matrix[1][0]; }
    void setC(float c) { m_matrix[1][0] = c; }

    float d() const { return m_matrix[1][1]; }
    void setD(float d) { m_matrix[1][1] = d; }

    float e() const { return m_matrix[3][0]; }
    void setE(float e) { m_matrix[3][0] = e; }

    float f() const { return m_matrix[3][1]; }
    void setF(float f) { m_matrix[3][1] = f; }

    // this = this * mat
    AliTransformationMatrix& multiply(const AliTransformationMatrix&);
    AliTransformationMatrix& multiply(float m11, float m12, float m21, float m22, float dx, float dy);

    AliTransformationMatrix& scale(float s);
    AliTransformationMatrix& scaleNonUniform(float sx, float sy);
    AliTransformationMatrix& scale3d(float sx, float sy, float sz);

    AliTransformationMatrix& rotate(float d) { return rotate3d(0, 0, d); }
    AliTransformationMatrix& rotateFromVector(float x, float y);
    AliTransformationMatrix& rotate3d(float rx, float ry, float rz);

    // The vector (x,y,z) is normalized if it's not already. A vector of
    // (0,0,0) uses a vector of (0,0,1).
    AliTransformationMatrix& rotate3d(float x, float y, float z, float angle);

    AliTransformationMatrix& translate(float tx, float ty);
    AliTransformationMatrix& translate3d(float tx, float ty, float tz);

    // translation added with a post-multiply
    AliTransformationMatrix& translateRight(float tx, float ty);
    AliTransformationMatrix& translateRight3d(float tx, float ty, float tz);

    AliTransformationMatrix& flipX();
    AliTransformationMatrix& flipY();
    AliTransformationMatrix& skew(float angleX, float angleY);
    AliTransformationMatrix& skewX(float angle) { return skew(angle, 0); }
    AliTransformationMatrix& skewY(float angle) { return skew(0, angle); }

    AliTransformationMatrix& applyPerspective(float p);
    bool hasPerspective() const { return m_matrix[2][3] != 0.0f; }

    bool isInvertible() const;

    // This method returns the identity matrix if it is not invertible.
    // Use isInvertible() before calling this if you need to know.
    AliTransformationMatrix inverse() const;

    // decompose the matrix into its component parts
    typedef struct {
        float scaleX, scaleY, scaleZ;
        float skewXY, skewXZ, skewYZ;
        float quaternionX, quaternionY, quaternionZ, quaternionW;
        float translateX, translateY, translateZ;
        float perspectiveX, perspectiveY, perspectiveZ, perspectiveW;
    } DecomposedType;

    bool decompose(DecomposedType& decomp) const;
    void recompose(const DecomposedType& decomp);

    void blend(const AliTransformationMatrix& from, float progress);

    bool isAffine() const
    {
        return (m13() == 0 && m14() == 0 && m23() == 0 && m24() == 0 &&
                m31() == 0 && m32() == 0 && m33() == 1 && m34() == 0 && m43() == 0 && m44() == 1);
    }

    // Throw away the non-affine parts of the matrix (lossy!)
    void makeAffine();

    bool operator==(const AliTransformationMatrix& m2) const
    {
        return (m_matrix[0][0] == m2.m_matrix[0][0] &&
                m_matrix[0][1] == m2.m_matrix[0][1] &&
                m_matrix[0][2] == m2.m_matrix[0][2] &&
                m_matrix[0][3] == m2.m_matrix[0][3] &&
                m_matrix[1][0] == m2.m_matrix[1][0] &&
                m_matrix[1][1] == m2.m_matrix[1][1] &&
                m_matrix[1][2] == m2.m_matrix[1][2] &&
                m_matrix[1][3] == m2.m_matrix[1][3] &&
                m_matrix[2][0] == m2.m_matrix[2][0] &&
                m_matrix[2][1] == m2.m_matrix[2][1] &&
                m_matrix[2][2] == m2.m_matrix[2][2] &&
                m_matrix[2][3] == m2.m_matrix[2][3] &&
                m_matrix[3][0] == m2.m_matrix[3][0] &&
                m_matrix[3][1] == m2.m_matrix[3][1] &&
                m_matrix[3][2] == m2.m_matrix[3][2] &&
                m_matrix[3][3] == m2.m_matrix[3][3]);
    }

    bool operator!=(const AliTransformationMatrix& other) const { return !(*this == other); }

    // *this = *this * t
    AliTransformationMatrix& operator*=(const AliTransformationMatrix& t)
    {
        return multiply(t);
    }

    // result = *this * t
    AliTransformationMatrix operator*(const AliTransformationMatrix& t) const
    {
        AliTransformationMatrix result = *this;
        result.multiply(t);
        return result;
    }
/*
#if PLATFORM(CA)
    AliTransformationMatrix(const CATransform3D&);
    operator CATransform3D() const;
#endif
#if PLATFORM(CG)
    AliTransformationMatrix(const CGAffineTransform&);
    operator CGAffineTransform() const;
#elif PLATFORM(CAIRO)
    operator cairo_matrix_t() const;
#elif PLATFORM(OPENVG)
    operator VGMatrix() const;
#elif PLATFORM(QT)
    operator QTransform() const;
#elif PLATFORM(SKIA)
    operator SkMatrix() const;
#elif PLATFORM(WX) && USE(WXGC)
    operator wxGraphicsMatrix() const;
#endif

#if PLATFORM(WIN) || (PLATFORM(QT) && OS(WINDOWS)) || (PLATFORM(WX) && OS(WINDOWS))
    operator XFORM() const;
#endif
*/
    bool isIdentityOrTranslation() const
    {
        return m_matrix[0][0] == 1 && m_matrix[0][1] == 0 && m_matrix[0][2] == 0 && m_matrix[0][3] == 0
            && m_matrix[1][0] == 0 && m_matrix[1][1] == 1 && m_matrix[1][2] == 0 && m_matrix[1][3] == 0
            && m_matrix[2][0] == 0 && m_matrix[2][1] == 0 && m_matrix[2][2] == 1 && m_matrix[2][3] == 0
            && m_matrix[3][3] == 1;
    }

private:
    // multiply passed 2D point by matrix (assume z=0)
    void multVecMatrix(float x, float y, float& dstX, float& dstY) const;

    // multiply passed 3D point by matrix
    void multVecMatrix(float x, float y, float z, float& dstX, float& dstY, float& dstZ) const;

    void setMatrix(const Matrix4 m)
    {
        if (m && m != m_matrix)
            memcpy(m_matrix, m, sizeof(Matrix4));
    }

    Matrix4 m_matrix;
};
}
#endif // AliTransformationMatrix_h
