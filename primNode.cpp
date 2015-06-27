#include "primNode.h"


PrimNode::PrimNode( PrimNode *_father, NodeType _nodeType, Matrix &_currTransform)
{
  father = _father;
  sonLeft = sonRight = brother = NULL;
  nodeType = _nodeType;
  currTransform = _currTransform;
  numOfChildren = 0;
}

void PrimNode::CreatePrimBlock( IDirect3DDevice9 *directDevice, unsigned FVFFlags )
{
  switch (nodeType)
  {
  case NODE_COLUMN:    
    jointDir = Vec(0.0f, 1.0f, 0.0f);
    jointLen = COLUMN_HEIGHT + JOINT_RADIUS - JOINT_SHIFT;
    if (father != NULL)      
      jointPoint = father->jointPoint + Vec(0.0f, COLUMN_HEIGHT + 2 * JOINT_RADIUS - 2 * JOINT_SHIFT, 0.0f);
    else
      jointPoint = Vec(0.0f, 0.0f, 0.0f);
    

    primBlock = new Prim[2];
    primBlock[0].Init(directDevice, FVFFlags);
    primBlock[0].CreateCylinder(Vec(0.0f, 0.0f, 0.0f), COLUMN_HEIGHT, COLUMN_RADIUS, 10, 10);
    primBlock[0].SetMonotoneColor(0, 0, 125, 0);
    primBlock[1].Init(directDevice, FVFFlags);
    primBlock[1].CreateSphere(Vec(0.0f, COLUMN_HEIGHT + JOINT_RADIUS - JOINT_SHIFT, 0.0f), JOINT_RADIUS, 20, 20);
    primBlock[1].SetMonotoneColor(0, 0, 125, 0);
    
    matrixBlock = new Matrix[2];    
    break;
  
  case NODE_LFINGER_BASE:
    jointDir = Vec(0.0f, 1.0f, 0.0f);
    jointLen = COLUMN_HEIGHT + JOINT_RADIUS - JOINT_SHIFT;
    jointPoint = father->jointPoint + Vec(0.0f, jointLen, 0.0f);

    primBlock = new Prim[2];
    primBlock[0].Init(directDevice, FVFFlags);
    primBlock[0].CreateCylinder(Vec(0.0f, 0.0f, 0.0f), COLUMN_HEIGHT, COLUMN_RADIUS, 10, 10);
    primBlock[0].SetMonotoneColor(0, 0, 124, 124);
    primBlock[1].Init(directDevice, FVFFlags);
    primBlock[1].CreateSphere(Vec(0.0f, COLUMN_HEIGHT + JOINT_RADIUS - JOINT_SHIFT, 0.0f), JOINT_RADIUS, 20, 20);
    primBlock[1].SetMonotoneColor(0, 0, 124, 124);
    
    matrixBlock = new Matrix[2];    
    matrixBlock[0].RotateX(-DEFAULT_ANGLE);
    matrixBlock[1].RotateX(-DEFAULT_ANGLE);
    jointDir = matrixBlock[1] * jointDir;
    break; 
  
  case NODE_LFINGER_END:
    jointDir = Vec(0.0f, 1.0f, 0.0f);
    jointLen = COLUMN_HEIGHT;
    jointPoint = father->jointPoint +      
      father->jointDir * father->jointLen +
      Vec(0.0f, JOINT_RADIUS - JOINT_SHIFT, 0.0f);

    primBlock = new Prim[1];
    primBlock[0].Init(directDevice, FVFFlags);
    primBlock[0].CreateCylinder(Vec(0.0f, 0.0f, 0.0f), COLUMN_HEIGHT, COLUMN_RADIUS, 10, 10);
    primBlock[0].SetMonotoneColor(0, 0, 124, 124);
    
    matrixBlock = new Matrix[1];    
    break; 
  
  case NODE_RFINGER_BASE:
    jointDir = Vec(0.0f, 1.0f, 0.0f);
    jointLen = COLUMN_HEIGHT + JOINT_RADIUS - JOINT_SHIFT;
    jointPoint = father->jointPoint + Vec(0.0f, jointLen, 0.0f);

    primBlock = new Prim[2];
    primBlock[0].Init(directDevice, FVFFlags);
    primBlock[0].CreateCylinder(Vec(0.0f, 0.0f, 0.0f), COLUMN_HEIGHT, COLUMN_RADIUS, 10, 10);
    primBlock[0].SetMonotoneColor(0, 0, 124, 124);
    primBlock[1].Init(directDevice, FVFFlags);
    primBlock[1].CreateSphere(Vec(0.0f, COLUMN_HEIGHT + JOINT_RADIUS - JOINT_SHIFT, 0.0f), JOINT_RADIUS, 20, 20);
    primBlock[1].SetMonotoneColor(0, 0, 124, 124);
    
    matrixBlock = new Matrix[2];    
    matrixBlock[0].RotateX(DEFAULT_ANGLE);
    matrixBlock[1].RotateX(DEFAULT_ANGLE);
    jointDir = matrixBlock[1] * jointDir;
    break; 
  
  case NODE_RFINGER_END:
    jointDir = Vec(0.0f, 1.0f, 0.0f);
    jointLen = COLUMN_HEIGHT;
    jointPoint = father->jointPoint +      
      father->jointDir * father->jointLen +
      Vec(0.0f, JOINT_RADIUS - JOINT_SHIFT, 0.0f);

    primBlock = new Prim[1];
    primBlock[0].Init(directDevice, FVFFlags);
    primBlock[0].CreateCylinder(Vec(0.0f, 0.0f, 0.0f), COLUMN_HEIGHT, COLUMN_RADIUS, 10, 10);
    primBlock[0].SetMonotoneColor(0, 0, 124, 124);

    matrixBlock = new Matrix[1];    
    break; 
  
  default:
    break;
  }
}


PrimNode::~PrimNode()
{
  delete[] primBlock;
  delete[] matrixBlock;
}


PrimNode *PrimNode::AddSon(NodeType _nodeType, Matrix &_currTransform)
{
  if (sonLeft == NULL)
    sonLeft = sonRight = new PrimNode(this, _nodeType, _currTransform);
  else
  {
    sonRight->brother = new PrimNode(this, _nodeType, _currTransform);
    sonRight = sonRight->brother;
  }
  ++numOfChildren;

  return sonRight;
}

PrimNode *PrimNode::AddBrother(NodeType _nodeType, Matrix &_currTransform)
{
  if (father != NULL)
    return father->AddSon(_nodeType, _currTransform);
  
  return NULL;
}

void PrimNode::RenderNode()
{
  Matrix localTransform, lastTransform;
  int n = (nodeType > NODE_RFINGER_BASE ? 1 : 2);

  lastTransform.Translate(jointPoint.getX(), 
    jointPoint.getY(), 
    jointPoint.getZ());
  for (int i = 0; i < n; i++)
  {        
    localTransform = matrixBlock[i];
    localTransform *= comboTransform;
    localTransform *= lastTransform;
    primBlock[i].Render(&localTransform);
  }
}



PrimSkeleton::PrimSkeleton()
{
}
  
void PrimSkeleton::DestructPrimitives()
{
  PrimNode *currNode = coreNode->sonLeft, *deletedNode;
  int exitFlag = 0, nodeWasDeleted = 0,
    dir = 0; // 0 - go down, 1 - go up  

  while (currNode != coreNode)
  {    
    if (dir == 0) // go down
    {
      if (!nodeWasDeleted)
        nodeWasDeleted = 1;
      
      if (currNode->sonLeft != NULL)
      {
        currNode = currNode->sonLeft;
        nodeWasDeleted = 0;
      }
      else
        dir = 1;
    }
    else // go up 
    {
      deletedNode = currNode;
      if (currNode->brother != NULL)
      {
        currNode = currNode->brother;
        dir = 0;
      }
      else
        currNode = currNode->father;
      
      delete deletedNode;
    }
  }
  delete coreNode;
}


void PrimSkeleton::Update()
{  
  Matrix invTransform;
  PrimNode *currNode = coreNode->sonLeft, *tempNode;
  int exitFlag = 0, nodeWasRendered = 0,
    dir = 0; // 0 - go down, 1 - go up
  
  coreNode->comboTransform = coreNode->currTransform;

  while (currNode != coreNode)
  {    
    if (dir == 0) // go down
    {
      if (!nodeWasRendered)
      {
        tempNode = currNode;
        while (tempNode != NULL)
        {       
          invTransform = tempNode->comboTransform;
          invTransform.Transpose();
          tempNode->comboTransform = tempNode->father->comboTransform;
          tempNode->comboTransform *= tempNode->currTransform;  
          tempNode->jointDir = tempNode->comboTransform * (invTransform * tempNode->jointDir);
          tempNode->jointDir.Normalize();
          tempNode->jointPoint = tempNode->father->jointPoint + 
            tempNode->father->jointDir * tempNode->father->jointLen;
          tempNode = tempNode->brother;
        }
        nodeWasRendered = 1;
      }

      if (currNode->sonLeft != NULL)
      {
        currNode = currNode->sonLeft;
        nodeWasRendered = 0;
      }
      else
        dir = 1;
    }
    else // go up 
    {
      if (currNode->brother != NULL)
      {
        currNode = currNode->brother;
        dir = 0;
      }
      else
        currNode = currNode->father;
    }
  }
}

void PrimSkeleton::RenderSkeleton()
{
  PrimNode *currNode = coreNode->sonLeft, *tempNode;
  int exitFlag = 0, nodeWasRendered = 0,
    dir = 0; // 0 - go down, 1 - go up
  
  coreNode->RenderNode();

  while (currNode != coreNode)
  {    
    if (dir == 0) // go down
    {
      if (!nodeWasRendered)
      {
        tempNode = currNode;
        while (tempNode != NULL)
        {
          tempNode->RenderNode();
          tempNode = tempNode->brother;
        }
        nodeWasRendered = 1;
      }

      if (currNode->sonLeft != NULL)
      {
        currNode = currNode->sonLeft;
        nodeWasRendered = 0;
      }
      else
        dir = 1;
    }
    else // go up 
    {
      if (currNode->brother != NULL)
      {
        currNode = currNode->brother;
        dir = 0;
      }
      else
        currNode = currNode->father;
    }
  }
}


void PrimSkeleton::CreateArm(IDirect3DDevice9 *directDevice, unsigned FVFFlags)
{
  PrimNode *currNode;

  currNode = coreNode = new PrimNode(NULL, NODE_COLUMN);
  coreNode->CreatePrimBlock(directDevice, FVFFlags);
  currNode = currNode->AddSon(NODE_COLUMN);
  currNode->CreatePrimBlock(directDevice, FVFFlags);
  currNode = currNode->AddSon(NODE_COLUMN);
  currNode->CreatePrimBlock(directDevice, FVFFlags);
  currNode = currNode->AddSon(NODE_COLUMN);
  currNode->CreatePrimBlock(directDevice, FVFFlags);
  currNode = currNode->AddSon(NODE_RFINGER_BASE);
  currNode->CreatePrimBlock(directDevice, FVFFlags);
  currNode = currNode->AddSon(NODE_RFINGER_END);
  currNode->CreatePrimBlock(directDevice, FVFFlags);
  currNode = currNode->father->father->AddSon(NODE_LFINGER_BASE);
  currNode->CreatePrimBlock(directDevice, FVFFlags);
  currNode = currNode->AddSon(NODE_LFINGER_END);
  currNode->CreatePrimBlock(directDevice, FVFFlags);
}

void PrimSkeleton::MoveArm( float time )
{
  coreNode->sonLeft->sonLeft->currTransform.RotateX(cos(time / 2));
  coreNode->sonLeft->sonLeft->sonLeft->currTransform.RotateX(-cos(time / 2));
  coreNode->sonLeft->sonLeft->sonLeft->sonLeft->currTransform.RotateY(1);
  coreNode->sonLeft->sonLeft->sonLeft->sonLeft->currTransform.Rotate(- 2 + (((int)((time) * 2000.0f)) % 2000) / 500.f,
    Vec(-coreNode->sonLeft->sonLeft->sonLeft->sonLeft->jointDir.getZ(), 0.0f, 
    coreNode->sonLeft->sonLeft->sonLeft->sonLeft->jointDir.getX()));
  coreNode->sonLeft->sonLeft->sonLeft->sonLeft->brother->currTransform.RotateY(1);  
  coreNode->sonLeft->sonLeft->sonLeft->sonLeft->brother->currTransform.Rotate(+ 2 - (((int)((time) * 2000.0f)) % 2000) / 500.f,
    Vec(coreNode->sonLeft->sonLeft->sonLeft->sonLeft->brother->jointDir.getZ(), 0.0f,
    -coreNode->sonLeft->sonLeft->sonLeft->sonLeft->brother->jointDir.getX()));  
  
  Update();
}


PrimSkeleton::~PrimSkeleton()
{
  DestructPrimitives();
}
