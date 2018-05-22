/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Bryn Murrell $
   $Notice: (C) Copyright 2017 All Rights Reserved. $
   ======================================================================== */
#include "marchingcubes.h"
#include "kdTree.h"
#include <algorithm>
#include <float.h>
#include <math.h>

internal void kdTree(v3* point, int _depth, KDTree* out)
{
    *out = {};
    out->value = point;
    out->depth = _depth;
    if (out->depth % 3 == 0)
    {
        out->axisLocation = out->value->x;
    }
    else if (out->depth % 3 == 1)
    {
        out->axisLocation = out->value->y;
    }
    else if (out->depth % 3 == 2)
    {
        out->axisLocation = out->value->z;
    }
}

internal bool xComparator(v3 A, v3 B)
{
    return B.x > A.x;
}
internal bool yComparator(v3 A, v3 B)
{
    return B.y > A.y;
}
internal bool zComparator(v3 A, v3 B)
{
    return B.z > A.z;
}
internal void buildWholeTree(int numPts, v3 points[], int _depth, KDTree* out)
{
    out->depth = _depth;
    out->value = nullptr;
    int axis = out->depth % 3;
    //x == 0, y == 1, z == 2
    float median;
    if (axis == 0)
    {
        std::sort(points, points + numPts, xComparator);
        
        if (numPts > 2)
        {
            median = points[(numPts / 2)].x;

        }
        else
        {
            assert(numPts == 2);
            median = points[0].x + points[1].x;
            median = median / 2;

        }
        
        
    }
    else if (axis == 1)
    {
        std::sort(points, points + numPts, yComparator);
        if (numPts > 2)
        {
            
            median = points[(numPts / 2)].y;
        }
        else
        {
            assert(numPts == 2);
            median = points[0].y + points[1].y;
            median = median / 2;
            
        }
        
        
    }
    else
    {
        std::sort(points, points + numPts, zComparator);
        if (numPts > 2)
        {
            
            median = points[(numPts / 2)].z;
        }
        else
        {
            assert(numPts == 2);
            median = points[0].z + points[1].z;
            median = median / 2;
            
        }
    }
    out->axisLocation = median;
    if (numPts == 2)
    {
        out->leftChild = (KDTree*)malloc(sizeof(KDTree));
        out->rightChild = (KDTree*)malloc(sizeof(KDTree));
        v3* p1 = (v3* )malloc(sizeof(v3));
        *p1 = points[0];
        v3* p2 = (v3* )malloc(sizeof(v3));
        *p2 = points[1];
        
        kdTree(p1, out->depth + 1, out->leftChild);
        kdTree(p2, out->depth + 1, out->rightChild);
        
    }
    else if (numPts == 3)
    {
        v3 tempArray[] = {points[0], points[1]}; 
        KDTree* lChild = (KDTree*)malloc(sizeof(KDTree));
        KDTree* rChild = (KDTree*)malloc(sizeof(KDTree));
        out->leftChild = lChild;
        out->rightChild = rChild;
        v3* p1 = (v3* )malloc(sizeof(v3));
        *p1 = points[2];
        
        assert(!isnan(points[2].x && !isnan(points[2].y) && !isnan(points[2].z)));
        buildWholeTree(2, tempArray, out->depth + 1, out->leftChild);
        kdTree(p1, out->depth + 1, out->rightChild);
    }
    else
    {
        //use memcpy
        unsigned int lNum = (numPts/2);
        
        v3 leftList[lNum];
        unsigned int rNum;
        if (!(numPts % 2 == 0))
        {
           rNum = (numPts/2) + 1;
        }
        else
        {
            rNum = lNum;
        }
        v3 rightList[rNum];
        memcpy(leftList, points, sizeof(v3) * lNum);
        memcpy(rightList, &points[(numPts / 2)], sizeof(v3) * rNum);
        KDTree* lChild = (KDTree*)malloc(sizeof(KDTree));
        KDTree* rChild = (KDTree*)malloc(sizeof(KDTree));
        out->leftChild = lChild;
        out->rightChild = rChild;
        buildWholeTree(lNum, leftList,  out->depth + 1, out->leftChild);
        buildWholeTree(rNum, rightList, out->depth + 1, out->rightChild);
        
    }
}

internal v3 nearestNeighbour(KDTree* tree, v3 point, v3 currentBest = vec3(FLT_MAX, FLT_MAX, FLT_MAX))
{
    b32 left = false;
    b32 right = false;
    if (tree->value != 0x0 && !(*tree->value == point) && currentBest == vec3(FLT_MAX, FLT_MAX, FLT_MAX))
    {
        currentBest = *tree->value;
    }
    else if (tree->value != 0x0 && !(*tree->value == point))
    {
        if (dist(currentBest, point) > dist(*tree->value, point))
        {
            currentBest = *tree->value;
        }
    }
    else
    {
        if (tree->depth % 3 == 0)
        {
            //x
            if (point.x <= tree->axisLocation && tree->leftChild != nullptr)
            {
                currentBest = nearestNeighbour(tree->leftChild, point, currentBest);
                left = true;
               
            }
            else if (tree->rightChild != nullptr)
            {
                currentBest = nearestNeighbour(tree->rightChild, point, currentBest);
                right = true; 
            }
        }
        else if (tree->depth % 3 == 1)
        {
            if (point.y <= tree->axisLocation && tree->leftChild != nullptr)
            {
                //check left
                currentBest = nearestNeighbour(tree->leftChild, point, currentBest);
                left = true;
            }
            else if (tree->rightChild != nullptr)
                
            {
                //check y
                currentBest = nearestNeighbour(tree->rightChild, point, currentBest);
                right = true;
            }
        }
        else
        {
            if (point.z <= tree->axisLocation && tree->leftChild != nullptr)
            {
                //check left
                currentBest = nearestNeighbour(tree->leftChild, point, currentBest);
                left = true;
            }
            else if (tree->rightChild != nullptr)
                
            {
                //check y
                currentBest = nearestNeighbour(tree->rightChild, point, currentBest);
                right = true;
            }

            
        }
    }
    if (!right && left)
    {
        if (tree->depth % 3 == 0)
        {
            if (dist(vec3(point.x, 0, 0), vec3(tree->axisLocation, 0, 0)) < dist(currentBest, point))
            {
                currentBest = nearestNeighbour(tree->rightChild, point, currentBest);
            }
        }
        else if (tree->depth % 3 == 1)
        {
            if (dist(vec3(0, point.y, 0), vec3(0, tree->axisLocation, 0)) < dist(currentBest, point))
            {
                currentBest = nearestNeighbour(tree->rightChild, point, currentBest);
            }
        }
        else if (tree->depth % 3 == 2)
        {
           if (dist(vec3(0, 0, point.z), vec3(0, 0, tree->axisLocation)) < dist(currentBest, point))
            {
                currentBest = nearestNeighbour(tree->rightChild, point, currentBest);
            }
        }
    }
    else if (right)
    {
    
        if (tree->depth % 3 == 0)
        {
            if (dist(vec3(point.x, 0, 0), vec3(tree->axisLocation, 0, 0)) < dist(currentBest, point))
            {
                currentBest = nearestNeighbour(tree->leftChild, point, currentBest);
            }
        }
        else if (tree->depth % 3 == 1)
        {
            if (dist(vec3(0, point.y, 0), vec3(0, tree->axisLocation, 0)) < dist(currentBest, point))
            {
                currentBest = nearestNeighbour(tree->leftChild, point, currentBest);
            }
        }
        else if (tree->depth % 3 == 2)
        {
           if (dist(vec3(0, 0, point.z), vec3(0, 0, tree->axisLocation)) < dist(currentBest, point))
            {
                currentBest = nearestNeighbour(tree->leftChild, point, currentBest);
            }
        }
        
    }
    return currentBest;
}
