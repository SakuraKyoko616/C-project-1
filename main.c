#include "stdio.h"
#include "stdlib.h"
#include "math.h"
/*#include "buildTree.h"
#include"treeStructure.h"
#include "writeTree.h"*/

// make a node at given location (x,y) and level

struct qnode
{
  int level;
  double xy[2];
  struct qnode *child[4];
};
typedef struct qnode Node;

// function definitions

Node *makeNode(double x, double y, int level);
void makeChildren(Node *parent);
void writeTree(Node *head);
void writeNode(FILE *fp, Node *node);
void printOut(FILE *fp, Node *node);

// make a node at given location (x,y) and level
Node *makeNode(double x, double y, int level)
{

  int i;

  Node *node = (Node *)malloc(sizeof(Node));

  node->level = level;

  node->xy[0] = x;
  node->xy[1] = y;

  for (i = 0; i < 4; ++i)
    node->child[i] = NULL;

  return node;
}

// split a leaf nodes into 4 children

void makeChildren(Node *parent)
{

  double x = parent->xy[0];
  double y = parent->xy[1];

  int level = parent->level;

  double hChild = pow(2.0, -(level + 1));

  parent->child[0] = makeNode(x, y, level + 1);
  parent->child[1] = makeNode(x + hChild, y, level + 1);
  parent->child[2] = makeNode(x + hChild, y + hChild, level + 1);
  parent->child[3] = makeNode(x, y + hChild, level + 1);

  return;
}

// add children nodes to the last leaf node.

void growTree(Node *node)
{
  int i;
  if (node->child[0]== NULL)
  {
    makeChildren(node);
    return 0;
  }
  else
  {
    for (i = 0; i < 4; i++)
    {
      growTree(node->child[i]);
    }
  }
  return;
}
// free the whole tree from last node to head
void destroyTree(Node *node)
{
  int i;
  if (node->child[0]== NULL)
  {     
        free(node);
        return 0;
  }
  else
  {
    for (i = 0; i < 4; i++)
    {
      destroyTree(node->child[i]);
      free(node);
    }
  }
}

// write out the tree to file 'quad.out'

void writeTree(Node *head)
{

  FILE *fp = fopen("quad.out", "w");

  writeNode(fp, head);

  fclose(fp);

  return;
}

// recursively visit the leaf nodes

void writeNode(FILE *fp, Node *node)
{

  int i;

  if (node->child[0] == NULL)
    printOut(fp, node);
  else
  {
    for (i = 0; i < 4; ++i)
    {
      writeNode(fp, node->child[i]);
    }
  }
  return;
}

// write out the (x,y) corners of the node

void printOut(FILE *fp, Node *node)
{
  double x = node->xy[0];
  double y = node->xy[1];
  int level = node->level;
  double h = pow(2.0, -level);

  fprintf(fp, " %g %g\n", x, y);
  fprintf(fp, " %g %g\n", x + h, y);
  fprintf(fp, " %g %g\n", x + h, y + h);
  fprintf(fp, " %g %g\n", x, y + h);
  fprintf(fp, " %g %g\n\n", x, y);

  return;
}




int main(int argc, char **argv)
{

  Node *head;

  // make the head node
  head = makeNode(0.0, 0.0, 0);

  // make a tree
  makeChildren(head);
  makeChildren(head->child[1]);
  makeChildren((head->child[1])->child[1]);
  destroyTree(head);
  
  
  // print the tree for Gnuplot
  writeTree(head);

  return 0;
}
