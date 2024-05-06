#include <iostream>
#include <cstring>
#include <fstream>
#include "node.h"

using namespace std;

/*
 * Author | Sophia You
 * Date | May 6 2024
 * Description | This is a red-black binary search tree. It is a way of
 * creating a more balanced binary search tree. Red-black tree conditions
 * and cases are explained in more detail below.
 * Sources | 
 */


// RED BLACK TREE CONDITIONS
/*
 * Node is either red or black
 * root is black
 * all "leaves" (null children) are black
 * every red node has 2 black children
 * every path from root to leaf has the same number of black nodes
 */

// FUNCTION PROTOTYPES
// insertion
void insert(Node* &root, Node* current,  Node* newnode);
void fixInsert(Node* &root, Node* newnode);

// general operations
void rightRotation(Node* current, Node* &root);
void leftRotation(Node* current, Node* &root);
void print(Node* current, int numTabs);
int childStatus(Node* node);
Node* getUncle(Node* node);

// remove
void remove(Node* &root, Node* current, Node* parent, int searchkey);

int main()
{
  int max = 50;
  char input[max];
  bool running = true;
  Node* root = NULL;

  // the program will loop until the user wants to quit
  while (running)
    {
      // instructions
      cout << endl;
      cout << "Please only use lowercase letters." << endl;
      cout << "This is a red-black binary search tree." << endl;
      cout << endl;
      cout << "To quit the program, type 'quit.'" << endl;
      cout << "To insert nodes, type 'insert.'" << endl;
      cout << "To remove nodes, type 'remove.'" << endl;
      cout << "To visualize your tree, type 'print'" << endl;
      cout << "To find a value in the tree, type 'search.'" << endl;

      cin.getline(input, max);

      if (strcmp(input, "quit") == 0)
	{
	  running = false;
	}
      // insert nodes into the tree via console or file
      else if (strcmp(input, "insert") == 0)
	{
	  cout << "to insert a single number, type 'add.'" << endl;
	  cout << "to read in a file, type 'read.'" << endl;
	  cin.getline(input, max);
	  if (strcmp(input, "add") == 0)
	    {
	      cout << "Enter the number you want to add." << endl;
	      int newnum = 0;
	      cin >> newnum;
	      cin.ignore(max, '\n');
	      Node* newnode = new Node(newnum);
	      insert(root, root, newnode);
	      print(root, 0);
	    }
	  else if (strcmp(input, "read") == 0)
	    {
	      // read in the file
	      cout << "What is the name of the file you want to read in?" << endl;
	      cin.getline(input, max);
	      ifstream inFile;
	      inFile.open(input);
	      int newnum = 0; // temporarily keeps track of values
	      while (inFile >> newnum)
		{
		  Node* newnode	= new Node(newnum);
                  insert(root, root, newnode);
		}
	      print(root, 0); // print out the tree after insertion
	      inFile.close();
	    }
	  else
	    {
	      cout << "Command not recognized." << endl;
	    }
	}
      else if (strcmp(input, "remove") == 0)
        {
	  cout << "What number are you trying to remove?" << endl;
	  int searchkey = 0; // this is the number we're trying to remove
	  cin >> searchkey;
	  cin.ignore(max, '\n');
        }
      else if (strcmp(input, "print") == 0) // visual display of tree
        {
	  print(root, 0);
        }
    }
  return 0;
}

/**
 * This function takes in a new node (created above in main) which contains
 * a new value. It compares the new node's value to the root of the binary
 * search tree. If the new value is greater, it goes down to the right child,
 * and if it is lesser, it goes down the left. This process is repeated
 * with each node until the new value reaches a left and establishes
 * its position there. It then readjusts the tree based on the red black tree
 * cases.
 *
 * @param root | the root of the btree in question
 * &param current | the current node in the btree we're evaluating
 * @param newnode | the node we want to put in
 */
void insert(Node* &root, Node* current, Node* newnode)
{
  if (root == NULL) // empty tree
    {
      root = newnode;
      // since root was inserted as red, we must fix the violations
      fixInsert(root, newnode);
      return;
    }
  
  // the new node is smaller than the parent; add to left branch
  else if (newnode->getValue() < current->getValue())
    {
      if (current->getLeft() == NULL) // reached a leaf
	{
	  current->setLeft(newnode);
	  current->getLeft()->setParent(current); // establish the parent
	  fixInsert(root, newnode);
	}
      else // keep moving down the tree
	{
	  insert(root, current->getLeft(), newnode);
	}
    }

  // the new node is larger than the parent; add to right branch
  else if (newnode->getValue() > current->getValue())
    {
      if (current->getRight() == NULL) // reached a leaf
	{
	  current->setRight(newnode);
	  current->getRight()->setParent(current); // establish the parent
	  fixInsert(root, newnode);
	}
      else // keep moving down the tree
	{
	  insert(root, current->getRight(), newnode);
	}
    }

  // we cannot have two nodes of the same value
  else if (newnode->getValue() == current->getValue())
    {
      cout << "Two nodes of the same value cannot be added." << endl;
      cout << "Therefore the node " << newnode->getValue() << " cannot be added more than once." << endl;
    }
}

/**
 * This function is only called inside the insert function. This is because
 * a new node in the red black tree is automatically inserted as a red node
 * in a standard binary search tree. Doing this may violate properties of
 * a red-black tree, so this separate function is designed specifically to
 * fix the violations on a case-by case basis:
 * 
 * Case 1: if the new node is the root, change its color to black.
 * Case 2: the new node's parent is black. No violations.
 * Case 3: Both the new node's parent, p and uncle, u are RED. Change p and u
 * to BLACK, change grandparent g to RED, and recursively call fixInsert again.
 * Case 4: p is RED, u is BLACK or NULL, and new node is the inner grandchild.
 * Case 5: p is RED, u is BLACK or NULL, and new node is the outer grandchild.
 */
void fixInsert(Node* &root, Node* newnode)
{
  // CASE 1: new node is the root. Just set it to black
  if (newnode == root) // case 1
    {
      root->setColor('b');
      return;
    }

  // CASE 2: newnode's parent is black
  else if (newnode->getParent()->getColor() == 'b') // case 2
    {
      // no violations
    }

  // CASE 3: Parent and the uncle are RED
  else if (newnode->getParent()->getColor() == 'r' &&
	   getUncle(newnode) && getUncle(newnode)->getColor() == 'r') // case 3
    {
      Node* grandparent = NULL;
      if (newnode->getParent()->getParent())
	{
	  grandparent = newnode->getParent()->getParent();
	}
      Node* uncle = getUncle(newnode);

      // set the parent node to black to fix the red-black property violation
      newnode->getParent()->setColor('b');
      if (uncle) // if uncle exists
	{
	  uncle->setColor('b');
	}
      if (grandparent) // if grandparent exists
	{
	  grandparent->setColor('r');
	}
      fixInsert(root, grandparent); // fix any new violations
    }

  // CASE 4: Uncle is black, and newnode is the inner grandchild (triangle)
  // childstatus 1 is left child, childstatus 2 is right child
  // CASE 5: Uncle is black, and newnode is the outer grandchild (line)
  else if (newnode->getParent()->getColor() == 'r' &&
	   ((getUncle(newnode) && getUncle(newnode)->getColor() == 'b') ||
	    getUncle(newnode) == NULL)) // null children are black
    {
      Node* parent = newnode->getParent();
      Node* grandparent = newnode->getParent()->getParent();
      
      // CASE 4
      // right inner grandchild
      if (childStatus(newnode) == 2 &&
	  childStatus(parent) == 1)
	{
	  // tree rotation through the node's parent in the OPPOSITE direction
	  leftRotation(parent, root);
	  fixInsert(root, parent); // call case 5 on the parent node
	}
      // left inner grandchild
      else if (childStatus(newnode) == 1 &&
             childStatus(parent) == 2)
	{
	  // tree rotation in the opposite direction
	  rightRotation(parent, root);
	  fixInsert(root, parent);
	}

      // CASE 5
      // left outer grandchild
      else if (childStatus(newnode) == 1 &&
	  childStatus(parent) == 1)
	{
	  // tree rotation through the grandparent
	  if (grandparent) // if grandparent is not null
	    {
	      rightRotation(grandparent, root);

	      // switch parent and grandparent colors
	      char parentColor = parent->getColor();
	      parent->setColor(grandparent->getColor());
	      grandparent->setColor(parentColor);
	    }
	}
      // right outer grandchild
      else if (childStatus(newnode) == 2 &&
	       childStatus(parent) == 2)
	{
	  if (grandparent)
	    {
	      leftRotation(grandparent, root);
	      // switch parent and grandparent colors                          
	      char parentColor = parent->getColor();
              parent->setColor(grandparent->getColor());
              grandparent->setColor(parentColor);
	    }
	}
    }
  else
    {
      cout << "Something is wrong." << endl;
    }
}

/**
 * This function indicates whether the current node is a right or left child
 */
int childStatus(Node* node)
{
  if (node) // if the node is not null
    {
      if (node->getParent()->getLeft() != NULL &&
	  node->getParent()->getLeft() == node)
	{
	  // the current node is a left child
	  return 1; // 1 = left
	}
      else if (node->getParent()->getRight() != NULL &&
	       node->getParent()->getRight() == node)
	{
	  // the current node is a right child
	  return 2; // 2 = right child
	}
    }
  return 0; // if we have some other situation going on
}

/**
 * This function will return the node that is the current node's uncle, or 
 * the sibling to the parent node
 */

Node* getUncle(Node* node)
{
  if (childStatus(node->getParent()) == 1) // parent is the left child
    {
      // returns the RIGHT child of the grandparent
      return node->getParent()->getParent()->getRight();
    }
  else if (childStatus(node->getParent()) == 2) // parent is the right child
  {
    return node->getParent()->getParent()->getLeft(); // parent is left child
  }
  else
    {
      return NULL;
    }
}

/**
 * This function performs a right rotation around a given node "current."
 */
void rightRotation(Node* current, Node* &root)
{
  // if the right subtree of the left child exists
  if (current->getLeft())
    {
      Node* rightSubtree = NULL;
      Node* rotated = current->getLeft(); // this will take current's place
      if (rotated->getRight())
	{
	  rightSubtree = rotated->getRight();
	}
	  if (current->getParent()) // if the rotated node is NOT the root
	{
	  // set left child's parent as the grandparent
	  rotated->setParent(current->getParent());

	  // depending on whether current itself was a left or right child
	  // current's left child will take the place of current
	  if (childStatus(current) == 1) // left child
	    {
	      //cout << "current is a left child" << endl;
	      current->getParent()->setLeft(rotated);
	    }
	  else if (childStatus(current) == 2) // right child
	    {
	      //cout << "current is a right child" << endl;
	      current->getParent()->setRight(rotated);
	    }
	}
      else if (!current->getParent()) // the rotated node IS the root
	{
	  // in this case, there is no parent
	  // we have to redefine the root as the rotated node
	  //cout << "hello" << endl;
	  root = rotated;
	  root->setParent(NULL);
	  cout << current->getValue() << endl;
	}

      current->setParent(rotated); // current becomes the right subtree
      rotated->setRight(current);

      // the old right subtree becomes current's left subtree
      current->setLeft(rightSubtree);
    }
}

/**
 * This function performs a left rotation around a given node "current."
 */
void leftRotation(Node* current, Node* &root)
{
  // if the right subtree of the left child exists
  if (current->getRight())
    {
      Node* rotated = current->getRight(); // this will take current's place
      Node* leftSubtree = NULL;
      if (rotated->getLeft())
	{
	  leftSubtree = rotated->getLeft();
	}
	  if (current->getParent() != NULL) // if the rotated node is NOT the root
	    {
	      // set left child's parent as the grandparent
	      rotated->setParent(current->getParent());

	      // depending on whether current itself was a left or right child
	      // current's left child will take the place of current
	      if (childStatus(current) == 1) // left child
		{
		  current->getParent()->setLeft(rotated);
		}
	      else if (childStatus(current) == 2) // right child
		{
		  current->getParent()->setRight(rotated);
		}
	    }
	  else if (current == root) // rotated node IS the root
	    {
	      root = rotated;
	      root->setParent(NULL);
	      cout << rotated->getValue() << endl;
	    }

	  current->setParent(rotated); // current becomes the right subtree
	  rotated->setLeft(current);
	  
	  // the old right subtree becomes current's left subtree
	  current->setRight(leftSubtree);
    }
}

/**
 * This function, given a searchkey, removes the requested node from the 
 * binary tree.
 * If the node is question has no children, the node is simply deleted.
 * If the node has one child, the child is adopted by the grandparent.
 * If the node has two children, we must find the next largest node.
 * This means we go to the right child, then as left as possible. The
 * left descendant replaces the original node. The left descendant's child,
 * who should be a right child, is then adopted by its grandparent. 
 */

void remove(Node* &root, Node* current, Node* parent, int searchkey)
{
  // This returns if the searchkey isn't found
  // This shouldn't happen because we have built in a searchkey check
  // up in main
  if (current == NULL)
    {
      return;
    }
  
  // we have found the node to remove
  if (searchkey == current->getValue())
    {
      // deletion will go here
    }
  else if (searchkey < current->getValue())
    {
      remove(root, current->getLeft(), current, searchkey);
    }
  else if (searchkey > current->getValue())
    {
      remove(root, current->getRight(), current, searchkey);
    }
}

/**
 * This function displays the binary search tree in a visual manner,
 * sideways.
 * 
 * @param current | the current node we are print out
 * @param numTabs | the number of indentations required for this node
 */
void print(Node* current, int numTabs)
{

  // if we get to a leaf, we want to jump out of print
  if (current == NULL)
    {
      return; // nothing to print
    }

  // otherwise, we have not reached a leaf
  numTabs += 1; // indent one more in
  print(current->getRight(), numTabs); // recursively print right child
  cout << endl; // line break
  for (int i = 1; i < numTabs; i++)
    {
      // indent the number of times as stated by numTabs
      cout << "\t";
    }
  cout << current->getValue();
  cout << " (" << current->getColor() << ") ";
  if (current->getParent()) // if parent exists
    {
      //cout << "p = " << current->getParent()->getValue();
      //cout << " childstatus: " << childStatus(current);
    }

  cout << "\n"; // print the current value
print(current->getLeft(), numTabs); // recursively print left child
}
