/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Bryn Murrell $
   $Notice: (C) Copyright 2017 All Rights Reserved. $
   ======================================================================== */
#include "vec.h"
#include "curveCollection.h"
#include <unordered_map>
#include <utility>
#include <float.h>
internal CurveCollection curveCollection(std::vector<std::vector<v3>> curvePop,
                                         int num,
                                         std::unordered_map<v3, int*, v3Hash>* map)
{
    CurveCollection out = {};
    out.curvePop = curvePop; 
    v3* pointsBuffer = (v3*)malloc(sizeof(v3) * num);
    int totalIndex = 0;
    for(int i = 0; i < curvePop.size(); i++)
    {
        for (int j = 0; j < curvePop[i].size(); j++)
        {
            pointsBuffer[totalIndex] = curvePop[i][j];
            totalIndex++;
        }
    }
    out.curvePointTree = KDTree(num, pointsBuffer);
    for(int i = 0; i < curvePop.size(); i++)
    {
        for (int j = 0; j < curvePop[i].size(); j++)
        {
            int* tempArray = (int* )malloc(sizeof(int)*2);
            tempArray[0] = i;
            tempArray[1] = j;
            auto p = std::make_pair(curvePop[i][j], tempArray);
            map->insert(p);
        }
    }
    out.map = map;
    return out;
}

internal void getBBox(CurveCollection* c, v3* out)
{
    v3 highRec = vec3(FLT_MIN, FLT_MIN, FLT_MIN);
    v3 lowRec  = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    for (int i = 0; i < c->curvePop.size(); i++)
    {
        for (int j = 0; j < c->curvePop[i].size(); j++)
        {
            if (c->curvePop[i][j].x > highRec.x)
            {
                highRec.x = c->curvePop[i][j].x;
            }
            else if (c->curvePop[i][j].x < lowRec.x)
            {
                lowRec.x = c->curvePop[i][j].x;
            }

            if (c->curvePop[i][j].y > highRec.y)
            {
                highRec.y = c->curvePop[i][j].y;
            }
            else if (c->curvePop[i][j].y < lowRec.y)
            {
                lowRec.y = c->curvePop[i][j].y;
            }
        
            if (c->curvePop[i][j].z > highRec.z)
            {
                highRec.z = c->curvePop[i][j].z;
            }
            else if (c->curvePop[i][j].z < lowRec.z)
            {
                lowRec.z = c->curvePop[i][j].z;
            }
        }
    }
    out[0] = highRec;
    out[1] = lowRec;
}

internal v3 getAveragePoint(CurveCollection* c)
{
    int count = 0;
    v3 average = {};
    for (int i = 0; i < c->curvePop.size(); i++)
    {
        for (int j = 0; j < c->curvePop[i].size(); j++)
        {
            count++;
            average = average + c->curvePop[i][j];
        }
    }
    average = average / (f32)count;
    return average;
}
v3 lineCP2(v3 A, v3 B, v3 P)
{
//        auto AB = B - A;
    v3 AB = B - A;
//        auto AP = P - A;
    v3 AP = P - A;
//        float lengthSqrAB = AB.x * AB.x + AB.y * AB.y;
    float lengthSqAB = sqLen(AB);
//        float t = (AP.x * AB.x + AP.y * AB.y) / lengthSqrAB;
    float t = dot(AP, AB) / lengthSqAB;
    if (t > 1)
    {
        t = 1;
    }
    if (t < 0)
    {
        t = 0;
    }
    if (t == 0)
    {
        return B;
    }
    if (t == 1)
    {
        return A;
    }
    else
    {
        assert(t < 1 && t > 0);
        return lerpVector(B, A, t);
    }
}


internal f32 getDistFromCurve(CurveCollection c, v3 p)
{
    Best b = best(NULL, FLT_MAX);
    nearestNeighbour(&p, c.curvePointTree, &b);
    v3 cp = b.n->value;
    int* indices = c.map->at(cp);
    int i = indices[0];
    int j = indices[1];
    if (j == c.curvePop[i].size() - 1)
    {
        //check only index before
        v3 cp2 = c.curvePop[i][j-1];
        return distSq(lineCP2(cp, cp2, p), p);
    }
    else if (j == 0)
    {
        //check only index after
        v3 cp2 = c.curvePop[i][j+1];
        return distSq(lineCP2(cp, cp2, p), p);
    }
    else
    {
        //check both indices
        v3 candidate1 = c.curvePop[i][j+1];
        v3 candidate2 = c.curvePop[i][j-1];
        f32 dist1 = distSq(candidate1, p);
        f32 dist2 = distSq(candidate2, p);
        v3 cp2;
        if (dist1 < dist2)
        {
            cp2 = candidate1;
        }
        else
        {
            cp2 = candidate2;
        }
        return distSq(lineCP2(cp, cp2, p), p);
    }
}
