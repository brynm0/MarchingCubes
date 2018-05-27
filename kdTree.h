#if !defined(KDTREE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Bryn Murrell $
   $Notice: (C) Copyright 2018 All Rights Reserved. $
   ======================================================================== */

struct Node
{
    int depth;
    Node* leftChild;
    Node* rightChild;
    v3 value;
};

#define KDTREE_H
#endif
