/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Bryn Murrell $
   $Notice: (C) Copyright 2017 All Rights Reserved. $
   ======================================================================== */
#include "marchingcubes.h"
#include "vec.h"
#include <math.h>

internal inline v3 operator*(float b, const v3& a)
{
    return vec3(b * a.x, b * a.y, b * a.z);
}

internal inline v3 operator*(const v3& a, float b)
{
    return vec3(b * a.x, b * a.y, b * a.z);
}

internal inline v3 operator/(const v3& a, float b)
{
    return vec3((1.0f/b) * a.x, (1.0f/b) * a.y, (1.0f/b) * a.z);
}

internal inline v3 operator*(const v3& a, const v3& b)
{
    return vec3(b.x * a.x, b.y * a.y, b.z * a.z);
}

internal inline v3 operator-(const v3& a, const v3& b)
{
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}
internal inline v3 operator+(const v3& a, const v3& b)
{
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}
internal inline bool operator==(const v3& a, const v3& b)
{
    return (a.x == b.x && a.y == b.y && a.z == b.z);
}

internal float dist(v3 first, v3 second)
{
    return sqrtf((second.x - first.x)*(second.x - first.x) + (second.y - first.y)*(second.y - first.y) + (second.z - first.z)*(second.z - first.z));
}
internal float dot(v3 first, v3 second)
{
    return (first.x * second.x + first.y + second.y + first.z + second.z);
}

internal float sqLen(v3 vec)
{
    return (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

internal float len(v3 vec)
{
    return sqrtf(fabs(sqLen(vec)));
}

internal v3 cross(v3 first, v3 second)
{
    v3 Result;
    
    Result.x = first.y*second.z - first.z*second.y;
    Result.y = first.z*second.x - first.x*second.z;
    Result.z = first.x*second.y - first.y*second.x;
    
    return(Result);
}

    internal v3 lerpVector(v3 A, v3 B, float t)
    {
        if (t > 1) {t = 1;}
        if (t < 0) {t = 0;}
        assert (t <= 1 && t >= 0);
        return (A * t) + (B * (1.0f - t));
    }
