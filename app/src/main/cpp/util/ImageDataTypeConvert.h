#ifndef IMAGEDATATYPECONVERT_H
#define IMAGEDATATYPECONVERT_H

#include "Assert.h"
#include "WebGLGraphicsContext.h"

namespace DCanvas
{

uint8_t convertColor16LittleTo8(uint16_t value)
{
    return value >> 8;
}

uint8_t convertColor16BigTo8(uint16_t value)
{
    return static_cast<uint8_t>(value & 0x00FF);
}

void unpackOneRowOfRGBA16LittleToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = convertColor16LittleTo8(source[0]);
        destination[1] = convertColor16LittleTo8(source[1]);
        destination[2] = convertColor16LittleTo8(source[2]);
        destination[3] = convertColor16LittleTo8(source[3]);
        source += 4;
        destination += 4;
    }
}

void unpackOneRowOfRGBA16BigToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = convertColor16BigTo8(source[0]);
        destination[1] = convertColor16BigTo8(source[1]);
        destination[2] = convertColor16BigTo8(source[2]);
        destination[3] = convertColor16BigTo8(source[3]);
        source += 4;
        destination += 4;
    }
}

void unpackOneRowOfRGB8ToRGBA8(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[0];
        destination[1] = source[1];
        destination[2] = source[2];
        destination[3] = 0xFF;
        source += 3;
        destination += 4;
    }
}

void unpackOneRowOfRGB16LittleToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = convertColor16LittleTo8(source[0]);
        destination[1] = convertColor16LittleTo8(source[1]);
        destination[2] = convertColor16LittleTo8(source[2]);
        destination[3] = 0xFF;
        source += 3;
        destination += 4;
    }
}

void unpackOneRowOfRGB16BigToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = convertColor16BigTo8(source[0]);
        destination[1] = convertColor16BigTo8(source[1]);
        destination[2] = convertColor16BigTo8(source[2]);
        destination[3] = 0xFF;
        source += 3;
        destination += 4;
    }
}

void unpackOneRowOfBGR8ToRGBA8(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[2];
        destination[1] = source[1];
        destination[2] = source[0];
        destination[3] = 0xFF;
        source += 3;
        destination += 4;
    }
}

void unpackOneRowOfARGB8ToRGBA8(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[1];
        destination[1] = source[2];
        destination[2] = source[3];
        destination[3] = source[0];
        source += 4;
        destination += 4;
    }
}

void unpackOneRowOfARGB16LittleToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = convertColor16LittleTo8(source[1]);
        destination[1] = convertColor16LittleTo8(source[2]);
        destination[2] = convertColor16LittleTo8(source[3]);
        destination[3] = convertColor16LittleTo8(source[0]);
        source += 4;
        destination += 4;
    }
}

void unpackOneRowOfARGB16BigToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = convertColor16BigTo8(source[1]);
        destination[1] = convertColor16BigTo8(source[2]);
        destination[2] = convertColor16BigTo8(source[3]);
        destination[3] = convertColor16BigTo8(source[0]);
        source += 4;
        destination += 4;
    }
}

void unpackOneRowOfABGR8ToRGBA8(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[3];
        destination[1] = source[2];
        destination[2] = source[1];
        destination[3] = source[0];
        source += 4;
        destination += 4;
    }
}

void unpackOneRowOfBGRA8ToRGBA8(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    const uint32_t* source32 = reinterpret_cast<const uint32_t*>(source);
    uint32_t* destination32 = reinterpret_cast<uint32_t*>(destination);
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        uint32_t bgra = source32[i];
//#if CPU(BIG_ENDIAN) /////////////////////////right?
//      uint32_t brMask = 0xff00ff00;
//      uint32_t gaMask = 0x00ff00ff;
//#else
        uint32_t brMask = 0x00ff00ff;
        uint32_t gaMask = 0xff00ff00;
//#endif
        uint32_t rgba = (((bgra >> 16) | (bgra << 16)) & brMask) | (bgra & gaMask);
        destination32[i] = rgba;
    }
}

void unpackOneRowOfBGRA16LittleToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = convertColor16LittleTo8(source[2]);
        destination[1] = convertColor16LittleTo8(source[1]);
        destination[2] = convertColor16LittleTo8(source[0]);
        destination[3] = convertColor16LittleTo8(source[3]);
        source += 4;
        destination += 4;
    }
}

void unpackOneRowOfBGRA16BigToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = convertColor16BigTo8(source[2]);
        destination[1] = convertColor16BigTo8(source[1]);
        destination[2] = convertColor16BigTo8(source[0]);
        destination[3] = convertColor16BigTo8(source[3]);
        source += 4;
        destination += 4;
    }
}

void unpackOneRowOfRGBA5551ToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        uint16_t packedValue = source[0];
        uint8_t r = packedValue >> 11;
        uint8_t g = (packedValue >> 6) & 0x1F;
        uint8_t b = (packedValue >> 1) & 0x1F;
        destination[0] = (r << 3) | (r & 0x7);
        destination[1] = (g << 3) | (g & 0x7);
        destination[2] = (b << 3) | (b & 0x7);
        destination[3] = (packedValue & 0x1) ? 0xFF : 0x0;
        source += 1;
        destination += 4;
    }
}

void unpackOneRowOfRGBA4444ToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        uint16_t packedValue = source[0];
        uint8_t r = packedValue >> 12;
        uint8_t g = (packedValue >> 8) & 0x0F;
        uint8_t b = (packedValue >> 4) & 0x0F;
        uint8_t a = packedValue & 0x0F;
        destination[0] = r << 4 | r;
        destination[1] = g << 4 | g;
        destination[2] = b << 4 | b;
        destination[3] = a << 4 | a;
        source += 1;
        destination += 4;
    }
}

void unpackOneRowOfRGB565ToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        uint16_t packedValue = source[0];
        uint8_t r = packedValue >> 11;
        uint8_t g = (packedValue >> 5) & 0x3F;
        uint8_t b = packedValue & 0x1F;
        destination[0] = (r << 3) | (r & 0x7);
        destination[1] = (g << 2) | (g & 0x3);
        destination[2] = (b << 3) | (b & 0x7);
        destination[3] = 0xFF;
        source += 1;
        destination += 4;
    }
}

void unpackOneRowOfR8ToRGBA8(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[0];
        destination[1] = source[0];
        destination[2] = source[0];
        destination[3] = 0xFF;
        source += 1;
        destination += 4;
    }
}

void unpackOneRowOfR16LittleToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = convertColor16LittleTo8(source[0]);
        destination[1] = convertColor16LittleTo8(source[0]);
        destination[2] = convertColor16LittleTo8(source[0]);
        destination[3] = 0xFF;
        source += 1;
        destination += 4;
    }
}

void unpackOneRowOfR16BigToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = convertColor16BigTo8(source[0]);
        destination[1] = convertColor16BigTo8(source[0]);
        destination[2] = convertColor16BigTo8(source[0]);
        destination[3] = 0xFF;
        source += 1;
        destination += 4;
    }
}

void unpackOneRowOfRA8ToRGBA8(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[0];
        destination[1] = source[0];
        destination[2] = source[0];
        destination[3] = source[1];
        source += 2;
        destination += 4;
    }
}

void unpackOneRowOfRA16LittleToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = convertColor16LittleTo8(source[0]);
        destination[1] = convertColor16LittleTo8(source[0]);
        destination[2] = convertColor16LittleTo8(source[0]);
        destination[3] = convertColor16LittleTo8(source[1]);
        source += 2;
        destination += 4;
    }
}

void unpackOneRowOfRA16BigToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = convertColor16BigTo8(source[0]);
        destination[1] = convertColor16BigTo8(source[0]);
        destination[2] = convertColor16BigTo8(source[0]);
        destination[3] = convertColor16BigTo8(source[1]);
        source += 2;
        destination += 4;
    }
}

void unpackOneRowOfAR8ToRGBA8(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[1];
        destination[1] = source[1];
        destination[2] = source[1];
        destination[3] = source[0];
        source += 2;
        destination += 4;
    }
}

void unpackOneRowOfAR16LittleToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = convertColor16LittleTo8(source[1]);
        destination[1] = convertColor16LittleTo8(source[1]);
        destination[2] = convertColor16LittleTo8(source[1]);
        destination[3] = convertColor16LittleTo8(source[0]);
        source += 2;
        destination += 4;
    }
}

void unpackOneRowOfAR16BigToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = convertColor16BigTo8(source[1]);
        destination[1] = convertColor16BigTo8(source[1]);
        destination[2] = convertColor16BigTo8(source[1]);
        destination[3] = convertColor16BigTo8(source[0]);
        source += 2;
        destination += 4;
    }
}

void unpackOneRowOfA8ToRGBA8(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = 0x0;
        destination[1] = 0x0;
        destination[2] = 0x0;
        destination[3] = source[0];
        source += 1;
        destination += 4;
    }
}

void unpackOneRowOfA16LittleToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = 0x0;
        destination[1] = 0x0;
        destination[2] = 0x0;
        destination[3] = convertColor16LittleTo8(source[0]);
        source += 1;
        destination += 4;
    }
}

void unpackOneRowOfA16BigToRGBA8(const uint16_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = 0x0;
        destination[1] = 0x0;
        destination[2] = 0x0;
        destination[3] = convertColor16BigTo8(source[0]);
        source += 1;
        destination += 4;
    }
}

void unpackOneRowOfRGB32FToRGBA32F(const float* source, float* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[0];
        destination[1] = source[1];
        destination[2] = source[2];
        destination[3] = 1;
        source += 3;
        destination += 4;
    }
}

void unpackOneRowOfR32FToRGBA32F(const float* source, float* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[0];
        destination[1] = source[0];
        destination[2] = source[0];
        destination[3] = 1;
        source += 1;
        destination += 4;
    }
}

void unpackOneRowOfRA32FToRGBA32F(const float* source, float* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[0];
        destination[1] = source[0];
        destination[2] = source[0];
        destination[3] = source[1];
        source += 2;
        destination += 4;
    }
}

void unpackOneRowOfA32FToRGBA32F(const float* source, float* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = 0;
        destination[1] = 0;
        destination[2] = 0;
        destination[3] = source[0];
        source += 1;
        destination += 4;
    }
}

//----------------------------------------------------------------------
// Pixel packing routines.
//

void packOneRowOfRGBA8ToA8(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[3];
        source += 4;
        destination += 1;
    }
}

void packOneRowOfRGBA8ToR8(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[0];
        source += 4;
        destination += 1;
    }
}

void packOneRowOfRGBA8ToR8Premultiply(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = source[3] / 255.0f;
        uint8_t sourceR = static_cast<uint8_t>(static_cast<float>(source[0]) * scaleFactor);
        destination[0] = sourceR;
        source += 4;
        destination += 1;
    }
}

// FIXME: this routine is lossy and must be removed.
void packOneRowOfRGBA8ToR8Unmultiply(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = 1.0f / (source[3] ? source[3] / 255.0f : 1.0f);
        uint8_t sourceR = static_cast<uint8_t>(static_cast<float>(source[0]) * scaleFactor);
        destination[0] = sourceR;
        source += 4;
        destination += 1;
    }
}

void packOneRowOfRGBA8ToRA8(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[0];
        destination[1] = source[3];
        source += 4;
        destination += 2;
    }
}

void packOneRowOfRGBA8ToRA8Premultiply(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = source[3] / 255.0f;
        uint8_t sourceR = static_cast<uint8_t>(static_cast<float>(source[0]) * scaleFactor);
        destination[0] = sourceR;
        destination[1] = source[3];
        source += 4;
        destination += 2;
    }
}

// FIXME: this routine is lossy and must be removed.
void packOneRowOfRGBA8ToRA8Unmultiply(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = 1.0f / (source[3] ? source[3] / 255.0f : 1.0f);
        uint8_t sourceR = static_cast<uint8_t>(static_cast<float>(source[0]) * scaleFactor);
        destination[0] = sourceR;
        destination[1] = source[3];
        source += 4;
        destination += 2;
    }
}

void packOneRowOfRGBA8ToRGB8(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[0];
        destination[1] = source[1];
        destination[2] = source[2];
        source += 4;
        destination += 3;
    }
}

void packOneRowOfRGBA8ToRGB8Premultiply(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = source[3] / 255.0f;
        uint8_t sourceR = static_cast<uint8_t>(static_cast<float>(source[0]) * scaleFactor);
        uint8_t sourceG = static_cast<uint8_t>(static_cast<float>(source[1]) * scaleFactor);
        uint8_t sourceB = static_cast<uint8_t>(static_cast<float>(source[2]) * scaleFactor);

        destination[0] = sourceR;
        destination[1] = sourceG;
        destination[2] = sourceB;
        source += 4;
        destination += 3;
    }
}

// FIXME: this routine is lossy and must be removed.
void packOneRowOfRGBA8ToRGB8Unmultiply(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = 1.0f / (source[3] ? source[3] / 255.0f : 1.0f);
        uint8_t sourceR = static_cast<uint8_t>(static_cast<float>(source[0]) * scaleFactor);
        uint8_t sourceG = static_cast<uint8_t>(static_cast<float>(source[1]) * scaleFactor);
        uint8_t sourceB = static_cast<uint8_t>(static_cast<float>(source[2]) * scaleFactor);

        destination[0] = sourceR;
        destination[1] = sourceG;
        destination[2] = sourceB;
        source += 4;
        destination += 3;
    }
}

void packOneRowOfRGBA8ToRGBA8Premultiply(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = source[3] / 255.0f;
        uint8_t sourceR = static_cast<uint8_t>(static_cast<float>(source[0]) * scaleFactor);
        uint8_t sourceG = static_cast<uint8_t>(static_cast<float>(source[1]) * scaleFactor);
        uint8_t sourceB = static_cast<uint8_t>(static_cast<float>(source[2]) * scaleFactor);

        destination[0] = sourceR;
        destination[1] = sourceG;
        destination[2] = sourceB;
        destination[3] = source[3];
        source += 4;
        destination += 4;
    }
}

// FIXME: this routine is lossy and must be removed.
void packOneRowOfRGBA8ToRGBA8Unmultiply(const uint8_t* source, uint8_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = 1.0f / (source[3] ? source[3] / 255.0f : 1.0f);
        uint8_t sourceR = static_cast<uint8_t>(static_cast<float>(source[0]) * scaleFactor);
        uint8_t sourceG = static_cast<uint8_t>(static_cast<float>(source[1]) * scaleFactor);
        uint8_t sourceB = static_cast<uint8_t>(static_cast<float>(source[2]) * scaleFactor);

        destination[0] = sourceR;
        destination[1] = sourceG;
        destination[2] = sourceB;
        destination[3] = source[3];
        source += 4;
        destination += 4;
    }
}

void packOneRowOfRGBA8ToUnsignedShort4444(const uint8_t* source, uint16_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        *destination = (((source[0] & 0xF0) << 8)
                        | ((source[1] & 0xF0) << 4)
                        | (source[2] & 0xF0)
                        | (source[3] >> 4));
        source += 4;
        destination += 1;
    }
}

void packOneRowOfRGBA8ToUnsignedShort4444Premultiply(const uint8_t* source, uint16_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = source[3] / 255.0f;
        uint8_t sourceR = static_cast<uint8_t>(static_cast<float>(source[0]) * scaleFactor);
        uint8_t sourceG = static_cast<uint8_t>(static_cast<float>(source[1]) * scaleFactor);
        uint8_t sourceB = static_cast<uint8_t>(static_cast<float>(source[2]) * scaleFactor);

        *destination = (((sourceR & 0xF0) << 8)
                        | ((sourceG & 0xF0) << 4)
                        | (sourceB & 0xF0)
                        | (source[3] >> 4));
        source += 4;
        destination += 1;
    }
}

// FIXME: this routine is lossy and must be removed.
void packOneRowOfRGBA8ToUnsignedShort4444Unmultiply(const uint8_t* source, uint16_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = 1.0f / (source[3] ? source[3] / 255.0f : 1.0f);
        uint8_t sourceR = static_cast<uint8_t>(static_cast<float>(source[0]) * scaleFactor);
        uint8_t sourceG = static_cast<uint8_t>(static_cast<float>(source[1]) * scaleFactor);
        uint8_t sourceB = static_cast<uint8_t>(static_cast<float>(source[2]) * scaleFactor);

        *destination = (((sourceR & 0xF0) << 8)
                        | ((sourceG & 0xF0) << 4)
                        | (sourceB & 0xF0)
                        | (source[3] >> 4));
        source += 4;
        destination += 1;
    }
}

void packOneRowOfRGBA8ToUnsignedShort5551(const uint8_t* source, uint16_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        *destination = (((source[0] & 0xF8) << 8)
                        | ((source[1] & 0xF8) << 3)
                        | ((source[2] & 0xF8) >> 2)
                        | (source[3] >> 7));
        source += 4;
        destination += 1;
    }
}

void packOneRowOfRGBA8ToUnsignedShort5551Premultiply(const uint8_t* source, uint16_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = source[3] / 255.0f;
        uint8_t sourceR = static_cast<uint8_t>(static_cast<float>(source[0]) * scaleFactor);
        uint8_t sourceG = static_cast<uint8_t>(static_cast<float>(source[1]) * scaleFactor);
        uint8_t sourceB = static_cast<uint8_t>(static_cast<float>(source[2]) * scaleFactor);

        *destination = (((sourceR & 0xF8) << 8)
                        | ((sourceG & 0xF8) << 3)
                        | ((sourceB & 0xF8) >> 2)
                        | (source[3] >> 7));
        source += 4;
        destination += 1;
    }
}

// FIXME: this routine is lossy and must be removed.
void packOneRowOfRGBA8ToUnsignedShort5551Unmultiply(const uint8_t* source, uint16_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = 1.0f / (source[3] ? source[3] / 255.0f : 1.0f);
        uint8_t sourceR = static_cast<uint8_t>(static_cast<float>(source[0]) * scaleFactor);
        uint8_t sourceG = static_cast<uint8_t>(static_cast<float>(source[1]) * scaleFactor);
        uint8_t sourceB = static_cast<uint8_t>(static_cast<float>(source[2]) * scaleFactor);

        *destination = (((sourceR & 0xF8) << 8)
                        | ((sourceG & 0xF8) << 3)
                        | ((sourceB & 0xF8) >> 2)
                        | (source[3] >> 7));
        source += 4;
        destination += 1;
    }
}

void packOneRowOfRGBA8ToUnsignedShort565(const uint8_t* source, uint16_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        *destination = (((source[0] & 0xF8) << 8)
                        | ((source[1] & 0xFC) << 3)
                        | ((source[2] & 0xF8) >> 3));
        source += 4;
        destination += 1;
    }
}

void packOneRowOfRGBA8ToUnsignedShort565Premultiply(const uint8_t* source, uint16_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = source[3] / 255.0f;
        uint8_t sourceR = static_cast<uint8_t>(static_cast<float>(source[0]) * scaleFactor);
        uint8_t sourceG = static_cast<uint8_t>(static_cast<float>(source[1]) * scaleFactor);
        uint8_t sourceB = static_cast<uint8_t>(static_cast<float>(source[2]) * scaleFactor);

        *destination = (((sourceR & 0xF8) << 8)
                        | ((sourceG & 0xFC) << 3)
                        | ((sourceB & 0xF8) >> 3));
        source += 4;
        destination += 1;
    }
}

// FIXME: this routine is lossy and must be removed.
void packOneRowOfRGBA8ToUnsignedShort565Unmultiply(const uint8_t* source, uint16_t* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = 1.0f / (source[3] ? source[3] / 255.0f : 1.0f);
        uint8_t sourceR = static_cast<uint8_t>(static_cast<float>(source[0]) * scaleFactor);
        uint8_t sourceG = static_cast<uint8_t>(static_cast<float>(source[1]) * scaleFactor);
        uint8_t sourceB = static_cast<uint8_t>(static_cast<float>(source[2]) * scaleFactor);

        *destination = (((sourceR & 0xF8) << 8)
                        | ((sourceG & 0xFC) << 3)
                        | ((sourceB & 0xF8) >> 3));
        source += 4;
        destination += 1;
    }
}

void packOneRowOfRGBA32FToRGB32F(const float* source, float* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[0];
        destination[1] = source[1];
        destination[2] = source[2];
        source += 4;
        destination += 3;
    }
}

void packOneRowOfRGBA32FToRGB32FPremultiply(const float* source, float* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = source[3];
        destination[0] = source[0] * scaleFactor;
        destination[1] = source[1] * scaleFactor;
        destination[2] = source[2] * scaleFactor;
        source += 4;
        destination += 3;
    }
}

void packOneRowOfRGBA32FToRGBA32FPremultiply(const float* source, float* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = source[3];
        destination[0] = source[0] * scaleFactor;
        destination[1] = source[1] * scaleFactor;
        destination[2] = source[2] * scaleFactor;
        destination[3] = source[3];
        source += 4;
        destination += 4;
    }
}

void packOneRowOfRGBA32FToA32F(const float* source, float* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[3];
        source += 4;
        destination += 1;
    }
}

void packOneRowOfRGBA32FToR32F(const float* source, float* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[0];
        source += 4;
        destination += 1;
    }
}

void packOneRowOfRGBA32FToR32FPremultiply(const float* source, float* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = source[3];
        destination[0] = source[0] * scaleFactor;
        source += 4;
        destination += 1;
    }
}


void packOneRowOfRGBA32FToRA32F(const float* source, float* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        destination[0] = source[0];
        destination[1] = source[3];
        source += 4;
        destination += 2;
    }
}

void packOneRowOfRGBA32FToRA32FPremultiply(const float* source, float* destination, unsigned int pixelsPerRow)
{
    for (unsigned int i = 0; i < pixelsPerRow; ++i)
    {
        float scaleFactor = source[3];
        destination[0] = source[0] * scaleFactor;
        destination[1] = scaleFactor;
        source += 4;
        destination += 2;
    }
}

template<typename SourceType, typename IntermediateType, typename DestType>
static void doUnpackingAndPacking(const SourceType* sourceData,
                                  void (*rowUnpackingFunc)(const SourceType*, IntermediateType*, unsigned int),
                                  unsigned int width,
                                  unsigned int height,
                                  unsigned int sourceElementsPerRow,
                                  DestType* destinationData,
                                  void (*rowPackingFunc)(const IntermediateType*, DestType*, unsigned int),
                                  unsigned int destinationElementsPerPixel)
{
    if (!rowPackingFunc)
    {
        // The row packing is trivial, so don't bother with a temporary buffer.
        const SourceType* endPointer = sourceData + height * sourceElementsPerRow;
        unsigned int destinationElementsPerRow = width * destinationElementsPerPixel;
        while (sourceData < endPointer) {
            rowUnpackingFunc(sourceData, reinterpret_cast<IntermediateType*>(destinationData), width);
            sourceData += sourceElementsPerRow;
            destinationData += destinationElementsPerRow;
        }
    }
    else
    {
        IntermediateType* temporaryRGBAData = new IntermediateType[width * 4];
        const SourceType* endPointer = sourceData + height * sourceElementsPerRow;
        unsigned int destinationElementsPerRow = width * destinationElementsPerPixel;

        while (sourceData < endPointer)
        {
            rowUnpackingFunc(sourceData, temporaryRGBAData, width);
            rowPackingFunc(temporaryRGBAData, destinationData, width);
            sourceData += sourceElementsPerRow;
            destinationData += destinationElementsPerRow;
        }

        if(temporaryRGBAData)
        {
            delete[] temporaryRGBAData;
            temporaryRGBAData = NULL;
        }
    }
}

template<typename SourceType>
static unsigned int computeSourceElementsPerRow(unsigned int width,
                                                unsigned int bytesPerPixel,
                                                unsigned int unpackAlignment)
{
    unsigned int elementSizeInBytes = sizeof(SourceType);
    ASSERT(elementSizeInBytes <= bytesPerPixel);
    ASSERT(!(bytesPerPixel % elementSizeInBytes));
    unsigned int validRowBytes = width * bytesPerPixel;
    unsigned int totalRowBytes = validRowBytes;
    if (unpackAlignment)
    {
        unsigned int remainder = validRowBytes % unpackAlignment;
        if (remainder)
            totalRowBytes += (unpackAlignment - remainder);
    }
    return totalRowBytes / elementSizeInBytes;
}

template<typename DestType>
static void doPacking(const void* sourceData,
                      SourceDataFormat sourceDataFormat,
                      unsigned int width,
                      unsigned int height,
                      unsigned int sourceUnpackAlignment,
                      DestType* destinationData,
                      void (*rowPackingFunc)(const uint8_t*, DestType*, unsigned int),
                      unsigned int destinationElementsPerPixel)
{
    switch (sourceDataFormat)
    {
    case SourceFormatRGBA8:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint8_t>(width, 4, sourceUnpackAlignment);
        const uint8_t* source = static_cast<const uint8_t*>(sourceData);
        const uint8_t* endPointer = source + height * sourceElementsPerRow;
        unsigned int destinationElementsPerRow = width * destinationElementsPerPixel;
        while (source < endPointer)
        {
            if (rowPackingFunc)
                rowPackingFunc(source, destinationData, width);
            else
                memcpy(destinationData, source, width * 4);

            source += sourceElementsPerRow;
            destinationData += destinationElementsPerRow;
        }
        break;
    }
    case SourceFormatRGBA16Little:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 8, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfRGBA16LittleToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatRGBA16Big:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 8, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfRGBA16BigToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatRGB8:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint8_t>(width, 3, sourceUnpackAlignment);
        doUnpackingAndPacking<uint8_t, uint8_t, DestType>(static_cast<const uint8_t*>(sourceData), unpackOneRowOfRGB8ToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatRGB16Little:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 6, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfRGB16LittleToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatRGB16Big:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 6, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfRGB16BigToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatBGR8:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint8_t>(width, 3, sourceUnpackAlignment);
        doUnpackingAndPacking<uint8_t, uint8_t, DestType>(static_cast<const uint8_t*>(sourceData), unpackOneRowOfBGR8ToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatARGB8:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint8_t>(width, 4, sourceUnpackAlignment);
        doUnpackingAndPacking<uint8_t, uint8_t, DestType>(static_cast<const uint8_t*>(sourceData), unpackOneRowOfARGB8ToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatARGB16Little:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 8, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfARGB16LittleToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatARGB16Big:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 8, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfARGB16BigToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatABGR8:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint8_t>(width, 4, sourceUnpackAlignment);
        doUnpackingAndPacking<uint8_t, uint8_t, DestType>(static_cast<const uint8_t*>(sourceData), unpackOneRowOfABGR8ToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatBGRA8:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint8_t>(width, 4, sourceUnpackAlignment);
        doUnpackingAndPacking<uint8_t, uint8_t, DestType>(static_cast<const uint8_t*>(sourceData), unpackOneRowOfBGRA8ToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatBGRA16Little:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 8, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfBGRA16LittleToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatBGRA16Big:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 8, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfBGRA16BigToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatRGBA5551:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 2, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfRGBA5551ToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatRGBA4444:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 2, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfRGBA4444ToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatRGB565: {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 2, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfRGB565ToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatR8:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint8_t>(width, 1, sourceUnpackAlignment);
        doUnpackingAndPacking<uint8_t, uint8_t, DestType>(static_cast<const uint8_t*>(sourceData), unpackOneRowOfR8ToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatR16Little:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 2, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfR16LittleToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatR16Big:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 2, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfR16BigToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatRA8:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint8_t>(width, 2, sourceUnpackAlignment);
        doUnpackingAndPacking<uint8_t, uint8_t, DestType>(static_cast<const uint8_t*>(sourceData), unpackOneRowOfRA8ToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatRA16Little:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 4, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfRA16LittleToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatRA16Big:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 4, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfRA16BigToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatAR8:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint8_t>(width, 2, sourceUnpackAlignment);
        doUnpackingAndPacking<uint8_t, uint8_t, DestType>(static_cast<const uint8_t*>(sourceData), unpackOneRowOfAR8ToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatAR16Little:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 4, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfAR16LittleToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatAR16Big:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 4, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfAR16BigToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatA8:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint8_t>(width, 1, sourceUnpackAlignment);
        doUnpackingAndPacking<uint8_t, uint8_t, DestType>(static_cast<const uint8_t*>(sourceData), unpackOneRowOfA8ToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatA16Little:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 2, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfA16LittleToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatA16Big:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<uint16_t>(width, 2, sourceUnpackAlignment);
        doUnpackingAndPacking<uint16_t, uint8_t, DestType>(static_cast<const uint16_t*>(sourceData), unpackOneRowOfA16BigToRGBA8, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    default:
        ASSERT(false);
    }
}

static void doFloatingPointPacking(const void* sourceData,
                                   SourceDataFormat sourceDataFormat,
                                   unsigned int width,
                                   unsigned int height,
                                   unsigned int sourceUnpackAlignment,
                                   float* destinationData,
                                   void rowPackingFunc(const float*, float*, unsigned int),
                                   unsigned int destinationElementsPerPixel)
{
    switch (sourceDataFormat)
    {
    case SourceFormatRGBA8:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<float>(width, 16, sourceUnpackAlignment);
        const float* source = static_cast<const float*>(sourceData);
        const float* endPointer = source + height * sourceElementsPerRow;
        unsigned int destinationElementsPerRow = width * destinationElementsPerPixel;
        while (source < endPointer)
        {
            rowPackingFunc(source, destinationData, width);
            source += sourceElementsPerRow;
            destinationData += destinationElementsPerRow;
        }
        break;
    }
    case SourceFormatRGB32F:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<float>(width, 12, sourceUnpackAlignment);
        doUnpackingAndPacking<float, float, float>(static_cast<const float*>(sourceData), unpackOneRowOfRGB32FToRGBA32F, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatR32F:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<float>(width, 4, sourceUnpackAlignment);
        doUnpackingAndPacking<float, float, float>(static_cast<const float*>(sourceData), unpackOneRowOfR32FToRGBA32F, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatRA32F:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<float>(width, 8, sourceUnpackAlignment);
        doUnpackingAndPacking<float, float, float>(static_cast<const float*>(sourceData), unpackOneRowOfRA32FToRGBA32F, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    case SourceFormatA32F:
    {
        unsigned int sourceElementsPerRow = computeSourceElementsPerRow<float>(width, 4, sourceUnpackAlignment);
        doUnpackingAndPacking<float, float, float>(static_cast<const float*>(sourceData), unpackOneRowOfA32FToRGBA32F, width, height, sourceElementsPerRow, destinationData, rowPackingFunc, destinationElementsPerPixel);
        break;
    }
    default:
        //ASSERT_NOT_REACHED();
        break;
    }
}

} // namespace DCanvas

#endif // IMAGEDATATYPECONVERT_H
