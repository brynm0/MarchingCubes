#if !defined(MARCHINGCUBES_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Bryn Murrell $
   $Notice: (C) Copyright 2018 All Rights Reserved. $
   ======================================================================== */
#include <stdint.h>
#include "vec.h"

#define assert(expression)                      \
    if (!(expression)) {*(int*)0 = 0;}


#define KiloBytes(arg) ((arg)*1024)
#define MegaBytes(arg) (KiloBytes(arg)*1024)
#define GigaBytes(arg) (MegaBytes(arg)*1024)
#define TeraBytes(arg) (GigaBytes(arg)*1024)
#define internal static 
#define local_persist static 
#define global_variable static
#define PI 3.14159265359f
#define TWO_PI 2.0f*3.14159265359f

typedef int32_t b32;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

struct Mesh
{
    int vCount;
    int fCount;
    v3* vertices;
    int* faceVerts;
};

struct Triangle
{
    v3 verts[3];
};

struct GridCell
{
    v3 vertices[8];
    f32 val[8];
};

inline v3 vec3(f32 x, f32 y, f32 z)
{
    v3 out = {x, y, z};
    return out;
}

#define MARCHINGCUBES_H
#endif
