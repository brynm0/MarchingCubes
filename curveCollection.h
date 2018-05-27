#if !defined(CURVECOLLECTION_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Bryn Murrell $
   $Notice: (C) Copyright 2018 All Rights Reserved. $
   ======================================================================== */
#include <unordered_map>
#include "marchingcubes.h"
#include "vec.h"
#include <functional>
#include <vector>

struct CurveCollection
{
    Node* curvePointTree;
    std::unordered_map<v3, int*, v3Hash>* map;
    std::vector<std::vector<v3>> curvePop;
};

#define CURVECOLLECTION_H
#endif
