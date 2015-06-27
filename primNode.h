/*  FILE: PrimNode.H 
 *  DATE: 01.12.2014
 *  INITIAL PROJECT: CG_4
 *  PURPOSE: Additional primitive definitions.
 */

#ifndef _PRIMNODE_H_
#define _PRIMNODE_H_

#include "prim.h"


typedef enum {NODE_NONE, NODE_TRIANGLE, NODE_PLANE, NODE_SPHERE, 
              NODE_CYLINDER, NODE_CONE, NODE_TORUS, NODE_COMBO} NodeType;

class PrimNode
{
public:
  int id;
  NodeType nodeType;
  int numOfChildren;
  Prim *primBlock;                  // array of base elements
  PrimNode *father, *brother,       // 'brother' - node from the right of us
    *sonLeft,                       // 'sonLeft' - left-most node from bottom level, 
    *sonRight;                      // 'sonRight' - right-most node from bottom level
                       
  Matrix currTransform, comboTransform;

  PrimNode( PrimNode *_father = NULL, NodeType _nodeType = NODE_NONE, 
            Matrix &_currTransform = Matrix());

  void SetPrimMatrix( Matrix &primMatrix );  
  PrimNode *AddSon( NodeType _nodeType = NODE_NONE, Matrix &_currTransform = Matrix()); // return son
  PrimNode *AddBrother( NodeType _nodeType = NODE_NONE, Matrix &_currTransform = Matrix()); // return brother
  
  ~PrimNode();
};


class PrimSkeleton
{
private:
  PrimNode *coreNode;

public:
  PrimSkeleton();
  
  void DestructPrimitives();
  
  void MoveArm( float time );

  ~PrimSkeleton();
};

#endif /* _PrimNode_H_ */