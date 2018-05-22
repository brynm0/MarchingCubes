#if !defined(VEC_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Bryn Murrell $
   $Notice: (C) Copyright 2018 All Rights Reserved. $
   ======================================================================== */

#include <functional>


struct v3
{
    float x;
    float y;
    float z;
};

struct v3Hash
{
    std::size_t operator()(const v3& vec) const
    {
        using std::size_t;
        using std::hash;
        int p1 = 73856093;
        int p2 = 19349663;
        int p3 = 83492791;
        int xInt = *(int*)&vec.x;
        int yInt = *(int*)&vec.y;
        int zInt = *(int*)&vec.z;
        return(xInt * p1 xor yInt * p2 xor zInt * p3);
        
    }
};

#define VEC_H
#endif
