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
#include <cstring>
#include <string>

internal inline f32 fDist(f32 A, f32 B)
{
    return (B-A)*(B-A);
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

internal Node* node(int depth,
                   Node* leftChild,
                   Node* rightChild,
                   v3 value)
{
    
    Node* out = (Node* )malloc(sizeof(Node));
    out->depth = depth;
    out->leftChild = leftChild;
    out->rightChild = rightChild;
    out->value = value;
    return out;
}

internal Node* KDTree(int numPts,
                      v3* points,
                      int depth = 0)
{
    if(numPts == 1)
    {
        return node(depth+1, NULL, NULL, points[0]);
    }
    switch (depth % 3)
    {
        case 0:
            std::sort(points, points+numPts, xComparator);
            break;
        case 1:
            std::sort(points, points+numPts, yComparator);
            break;
        case 2:
            std::sort(points, points+numPts, zComparator);
            break;
    }
    int median;
    int lNum;
    int rNum;
    if (numPts % 2 == 1)
    {
        median = (numPts / 2) + 1;
        lNum = numPts / 2;
        rNum = numPts / 2;
    }
    else
    {
        median = numPts / 2;
        lNum = (numPts / 2) - 1;
        rNum = numPts / 2;
    }
    
    if (!(numPts == 2))
    {
        v3* leftList = (v3* )malloc(sizeof(v3) * lNum);
        memcpy(leftList, points, sizeof(v3) * lNum);
        v3* rightList = (v3* )malloc(sizeof(v3) * rNum);;
        memcpy(rightList, &points[median], sizeof(v3) * rNum);
        
        return node(depth,
                    KDTree(lNum, leftList, depth+1),
                    KDTree(rNum, rightList, depth+1),
                    points[median - 1]);
    }
    else
    {
        return node(depth+1,
                    KDTree(1, &points[0], depth+1),
                    NULL,
                    points[1]);
    }
    
}

internal std::string print(Node* node)
{
    std::string out = "";
    if (node->rightChild != NULL)
    {
        out.append(print(node->rightChild));
    }
    std::string temp = "\n";
    for (int i = 0; i < node->depth; i++)
    {
        temp.append("    ");
    }
    temp.append(std::to_string(node->value.x) + " " +
                std::to_string(node->value.y) + " " +
                std::to_string(node->value.z));
    out.append(temp);

    if (node->leftChild != NULL)
    {
        out.append(print(node->leftChild));
    }
    return out;
}

struct Best
{
    Node* n;
    f32 d;
};

internal inline Best best(Node* node, f32 dist)
{
    Best out;
    out.n = node;
    out.d = dist;
    return out;
};


internal void nearestNeighbour(v3* query, Node* node, Best* best)
{
    f32 dist = distSq(node->value, *query);
    b32 left = false;
    b32 right = false;
                                            
    if (node->leftChild == NULL && node->rightChild == NULL && dist < best->d)
    {
        best->n = node;
        best->d = dist;
        return;
    }
    else
    {
        switch (node->depth % 3)
        {
            case 0:
                if (query->x < node->value.x && node->leftChild != NULL)
                {
                    nearestNeighbour(query, node->leftChild, best);
                    left = true;
                }
                else if (node->rightChild != NULL)
                {
                    nearestNeighbour(query, node->rightChild, best);
                    right = true;   
                }
                break;
            case 1:
                if (query->y < node->value.y && node->leftChild != NULL)
                {
                    nearestNeighbour(query, node->leftChild, best);
                    left = true;
                }
                else if (node->rightChild != NULL)
                {
                    nearestNeighbour(query, node->rightChild, best);
                    right = true;   
                }
                break;
            case 2:
                if (query->z < node->value.z && node->leftChild != NULL)
                {
                    nearestNeighbour(query, node->leftChild, best);
                    left = true;
                }
                else if (node->rightChild != NULL)
                {
                    nearestNeighbour(query, node->rightChild, best);
                    right = true;   
                }
                break;
        }
    }
    f32 tempDist = distSq(node->value, *query);
    if (tempDist < best->d)
    {
        best->n = node;
        best->d = tempDist;
    }    
    if (left)
    {
        switch (node->depth % 3)
        {
            case 0:
                if (fDist(query->x, node->value.x) < best->d && node->rightChild != NULL)
                {
                    nearestNeighbour(query, node->rightChild, best);
                }
                break;
            case 1:
                if (fDist(query->y, node->value.y) < best->d && node->rightChild != NULL)
                {
                    nearestNeighbour(query, node->rightChild, best);
                }
                break;
            case 2:
                if (fDist(query->z, node->value.z) < best->d && node->rightChild != NULL)
                {
                    nearestNeighbour(query, node->rightChild, best);
                }
                break;
        }
    }
    else if (right)
    {
        switch (node->depth % 3)
        {
            case 0:
                if (fDist(query->x, node->value.x) < best->d && node->leftChild != NULL)
                {
                    nearestNeighbour(query, node->rightChild, best);
                }
                break;
            case 1:
                if (fDist(query->y, node->value.y) < best->d && node->leftChild != NULL)
                {
                    nearestNeighbour(query, node->rightChild, best);
                }
                break;
            case 2:
                if (fDist(query->z, node->value.z) < best->d && node->leftChild != NULL)
                {
                    nearestNeighbour(query, node->rightChild, best);
                }
                break;
        }
    }
    return;
}
