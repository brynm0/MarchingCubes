#if !defined(KDTREE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Bryn Murrell $
   $Notice: (C) Copyright 2018 All Rights Reserved. $
   ======================================================================== */

struct KDTree
{
    int depth;
    f32 axisLocation;
    KDTree* leftChild;
    KDTree* rightChild;
    v3* value;
};

#define KDTREE_H
#endif
