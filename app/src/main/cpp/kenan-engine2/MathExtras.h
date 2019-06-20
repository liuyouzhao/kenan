#ifndef MathExtras_h
#define MathExtras_h

#include <cmath>
#include <float.h>
#include <stdlib.h>

#ifndef M_PI
const double piDouble = 3.14159265358979323846;
const float piFloat = 3.14159265358979323846f;
#else
const double piDouble = M_PI;
const float piFloat = static_cast<float>(M_PI);
#endif

#ifndef M_PI_4
const double piOverFourDouble = 0.785398163397448309616;
const float piOverFourFloat = 0.785398163397448309616f;
#else
const double piOverFourDouble = M_PI_4;
const float piOverFourFloat = static_cast<float>(M_PI_4);
#endif

#define ROUND_VERTEX_NUMBER 72

const float sinTable[ROUND_VERTEX_NUMBER] =
{
    (float)0.000000000000000, (float)0.087155742747658, (float)0.173648177666930, (float)0.258819045102521,
    (float)0.342020143325668, (float)0.422618261740699, (float)0.500000000000000, (float)0.573576436351046,
    (float)0.642787609686539, (float)0.707106781186547, (float)0.766044443118977, (float)0.819152044288991,
    (float)0.866025403784438, (float)0.906307787036649, (float)0.939692620785908, (float)0.965925826289068,
    (float)0.984807753012208, (float)0.996194698091745, (float)1.000000000000000, (float)0.996194698091746,
    (float)0.984807753012208, (float)0.965925826289069, (float)0.939692620785909, (float)0.906307787036651,
    (float)0.866025403784440, (float)0.819152044288993, (float)0.766044443118979, (float)0.707106781186549,
    (float)0.642787609686541, (float)0.573576436351048, (float)0.500000000000002, (float)0.422618261740702,
    (float)0.342020143325671, (float)0.258819045102524, (float)0.173648177666933, (float)0.087155742747661,
    (float)0.000000000000003, (float)-0.087155742747655, (float)-0.173648177666927, (float)-0.258819045102517,
    (float)-0.342020143325665, (float)-0.422618261740696, (float)-0.499999999999997, (float)-0.573576436351043,
    (float)-0.642787609686536, (float)-0.707106781186545, (float)-0.766044443118976, (float)-0.819152044288990,
    (float)-0.866025403784437, (float)-0.906307787036648, (float)-0.939692620785907, (float)-0.965925826289067,
    (float)-0.984807753012207, (float)-0.996194698091745, (float)-1.000000000000000, (float)-0.996194698091746,
    (float)-0.984807753012209, (float)-0.965925826289070, (float)-0.939692620785910, (float)-0.906307787036652,
    (float)-0.866025403784441, (float)-0.819152044288995, (float)-0.766044443118982, (float)-0.707106781186551,
    (float)-0.642787609686544, (float)-0.573576436351051, (float)-0.500000000000005, (float)-0.422618261740705,
    (float)-0.342020143325674, (float)-0.258819045102527, (float)-0.173648177666937, (float)-0.087155742747665
};

const float cosTable[ROUND_VERTEX_NUMBER] =
{
    (float)1.000000000000000, (float)0.996194698091746, (float)0.984807753012208, (float)0.965925826289068,
    (float)0.939692620785909, (float)0.906307787036650, (float)0.866025403784439, (float)0.819152044288992,
    (float)0.766044443118978, (float)0.707106781186548, (float)0.642787609686540, (float)0.573576436351047,
    (float)0.500000000000001, (float)0.422618261740700, (float)0.342020143325670, (float)0.258819045102522,
    (float)0.173648177666932, (float)0.087155742747660, (float)0.000000000000002, (float)-0.087155742747656,
    (float)-0.173648177666929, (float)-0.258819045102519, (float)-0.342020143325667, (float)-0.422618261740698,
    (float)-0.499999999999998, (float)-0.573576436351044, (float)-0.642787609686538, (float)-0.707106781186546,
    (float)-0.766044443118976, (float)-0.819152044288990, (float)-0.866025403784437, (float)-0.906307787036649,
    (float)-0.939692620785907, (float)-0.965925826289068, (float)-0.984807753012208, (float)-0.996194698091745,
    (float)-1.000000000000000, (float)-0.996194698091746, (float)-0.984807753012209, (float)-0.965925826289069,
    (float)-0.939692620785910, (float)-0.906307787036651, (float)-0.866025403784441, (float)-0.819152044288994,
    (float)-0.766044443118981, (float)-0.707106781186550, (float)-0.642787609686542, (float)-0.573576436351049,
    (float)-0.500000000000003, (float)-0.422618261740703, (float)-0.342020143325673, (float)-0.258819045102525,
    (float)-0.173648177666935, (float)-0.087155742747663, (float)-0.000000000000005, (float)0.087155742747653,
    (float)0.173648177666926, (float)0.258819045102516, (float)0.342020143325664, (float)0.422618261740695,
    (float)0.499999999999995, (float)0.573576436351042, (float)0.642787609686535, (float)0.707106781186544,
    (float)0.766044443118974, (float)0.819152044288988, (float)0.866025403784436, (float)0.906307787036647,
    (float)0.939692620785906, (float)0.965925826289067, (float)0.984807753012207, (float)0.996194698091745
};

inline double deg2rad(double d)  { return d * piDouble / 180.0; }
inline double rad2deg(double r)  { return r * 180.0 / piDouble; }
inline double deg2grad(double d) { return d * 400.0 / 360.0; }
inline double grad2deg(double g) { return g * 360.0 / 400.0; }
inline double turn2deg(double t) { return t * 360.0; }
inline double deg2turn(double d) { return d / 360.0; }
inline double rad2grad(double r) { return r * 200.0 / piDouble; }
inline double grad2rad(double g) { return g * piDouble / 200.0; }

inline float deg2rad(float d)  { return d * piFloat / 180.0f; }
inline float rad2deg(float r)  { return r * 180.0f / piFloat; }
inline float deg2grad(float d) { return d * 400.0f / 360.0f; }
inline float grad2deg(float g) { return g * 360.0f / 400.0f; }
inline float turn2deg(float t) { return t * 360.0f; }
inline float deg2turn(float d) { return d / 360.0f; }
inline float rad2grad(float r) { return r * 200.0f / piFloat; }
inline float grad2rad(float g) { return g * piFloat / 200.0f; }

inline void wrapOriginal(float& x, float& y)
{
    y = -y;
    y = 2*y + 1;
    x = 2*x - 1;
}

#endif // MathExtras_h