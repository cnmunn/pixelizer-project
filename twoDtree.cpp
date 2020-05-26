
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"
#include "math.h"

// Node constructor, given.
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

// twoDtree destructor, given.
twoDtree::~twoDtree(){
	clear();
}

// twoDtree copy constructor, given.
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}


// twoDtree assignment operator, given.
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

twoDtree::twoDtree(PNG & imIn){ 
	height = imIn.height();
	width = imIn.width();

	s = new stats(imIn);

	pair<int, int> ul(0,0);
	pair<int, int> lr(width - 1, height - 1);

	root = buildTree(*s, ul, lr, 1);
}

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr, bool vert) {

// YOUR CODE HERE!!
	//so were going to do the whole tree
	//call recursively until the area is 1
	//make average s.getAvg(ul, lr)
	//bool vert changes at each level
	//vert = !vert
	//call on ul and lr, change each value
	//half it depending on the value of bool
	if (ul == lr) {
		return new Node(ul, lr, s.getAvg(ul, lr));
	}

	long minDiff = LONG_MAX;

	Node* retNode = new Node(ul, lr, s.getAvg(ul, lr));

	if (vert) {
		if (lr.first == ul.first) {
			retNode->left = buildTree(s, ul, lr, !vert);
			// retNode->right is null by default
			return retNode;
		}
		int optimalX = ul.first;
		for (int x = ul.first; x < lr.first; x++) {
			pair<int, int> splitUl(x + 1, ul.second);
			pair<int, int> splitLr(x, lr.second);
			
			long currDiff = s.getScore(ul, splitLr) + s.getScore(splitUl, lr);
			if (currDiff < minDiff) {
				minDiff = currDiff;
				optimalX = x;
			}
		}	
		pair<int, int> newUl(optimalX + 1, ul.second);
		pair<int, int> newLr(optimalX, lr.second);
		
		retNode->left = buildTree(s, ul, newLr, !vert);
		retNode->right = buildTree(s, newUl, lr, !vert);
		return retNode;

	} else {
		if (lr.second == ul.second) {
			retNode->left = buildTree(s, ul, lr, !vert);
			// retNode->right is null by default
			return retNode;
		}
		int optimalY = ul.second;
		for (int y = ul.second; y < lr.second; y++) {
			pair<int, int> splitLr(lr.first, y);
			pair<int, int> splitUl(ul.first, y+1);
			
			long currDiff = s.getScore(ul, splitLr) + s.getScore(splitUl, lr);
			if (currDiff < minDiff) {
				minDiff = currDiff;
				optimalY = y;
			}
		}	
		pair<int, int> newLr(lr.first, optimalY);
		pair<int, int> newUl(ul.first, optimalY + 1);
		retNode->left = buildTree(s, ul, newLr, !vert);
		retNode->right = buildTree(s, newUl, lr, !vert);
		return retNode;
	}
}

void twoDtree::renderHelper(PNG &im, Node* currNode) {
	if (currNode == NULL) {
		return;
	}

	if (currNode->left == NULL && currNode->right == NULL) {
		for (unsigned int y = currNode->upLeft.second; y <= ((unsigned int) currNode->lowRight.second); y++) {
			for (unsigned int x = currNode->upLeft.first; x <= ((unsigned int) currNode->lowRight.first); x++) {
				*(im.getPixel(x, y)) = currNode->avg;
			}
		}
		return;
	}

	renderHelper(im, currNode->left);
	renderHelper(im, currNode->right);
}

PNG twoDtree::render(){
	PNG im(width, height);
	renderHelper(im, root);
	return im;
}

//returns the tolerence needed to achieve a certain number of leaves
int twoDtree::idealPrune(int leaves){
	int currTol = 1;
	
	int lastTol;
	int currLeaves = 0;
	int count = 0;

	do {
		currTol *= 10;
		currLeaves = pruneSize(currTol);
	} while(currLeaves > leaves);

	int diff = currTol;

	while (count < 42) {
		currLeaves = pruneSize(currTol);
		if ( currLeaves > leaves && currLeaves - leaves < 50) {
			break;
		} else if ( currLeaves < leaves && leaves - currLeaves < 50) {
			break;
		} else if (currLeaves > leaves) {
			lastTol = currTol;
			currTol += diff/2;
			diff = currTol - lastTol;
		} else {
			lastTol = currTol;
			currTol -= diff/2;
			diff = lastTol - currTol;
		}
		count++;
	}

	// Increase tolerance linearly
	if (currLeaves > leaves) {
		while (currLeaves > leaves) {
			currTol++;
			currLeaves = pruneSize(currTol);
		}
	}

	if (currLeaves < leaves) {
		while (currLeaves < leaves) {
			currTol--;
			currLeaves = pruneSize(currTol);
		}

		if (currLeaves != leaves) {
			currTol++;
		}
	}

	return currTol;
}

int twoDtree::pruneSizeHelper(Node* currNode, int tol) {

	// If the node is a leaf, check if it should be pruned
	if (currNode->left == NULL && currNode->right == NULL) {
		return 1;
	}

	if (shouldPrune(currNode, tol, currNode->avg)) {
		return 1;
	}

	// Otherwise, if the node is not a leaf
	return pruneSizeHelper(currNode->left, tol) + pruneSizeHelper(currNode->right, tol);
}

int twoDtree::pruneSize(int tol){
    
// YOUR CODE HERE!!
	//copy and call prune
	//do a traversal on copy to get the number of nodes
	//destroy the copy
	//return the number of nodes if the prune were to happen
	// twoDtree tcopy(*this);
	// tcopy.prune(tol);
	int retVal = pruneSizeHelper(root, tol);
	//tcopy.clear();

	return retVal;
}

int twoDtree::compareColors(int tol, RGBAPixel color, RGBAPixel avg) {
	int rDiff = (color.r - avg.r) * (color.r - avg.r);
	int gDiff = (color.g - avg.g) * (color.g - avg.g);
	int bDiff = (color.b - avg.b) * (color.b - avg.b);

	if (rDiff + gDiff + bDiff <= tol) {
		return true;
	} else {
		return false;
	}
}

int twoDtree::shouldPrune(Node* currNode, int tol, RGBAPixel avg) {
	if(currNode == NULL) {
		return true;
	}

	// If the node is a leaf, check if it should be pruned
	if (currNode->left == NULL && currNode->right == NULL) {
		if (compareColors(tol, currNode->avg, avg)) {
			return true;
		} else {
			return false;
		}
	}

	// Otherwise, if the node is not a leaf
	if (shouldPrune(currNode->left, tol, avg) && shouldPrune(currNode->right, tol, avg)) {
		return true;
	} else {
		// If even one leaf should not be pruned, we should return false
		return false;
	}
}

void twoDtree::pruneHelper(Node* currNode, int tol) {
	if (shouldPrune(currNode, tol, currNode->avg)) {
		currNode->left = NULL;
		currNode->right = NULL;
	}

	if (currNode->left != NULL) {
		pruneHelper(currNode->left, tol);
	}
	if (currNode->right != NULL) {
		pruneHelper(currNode->right, tol);
	}
}

void twoDtree::prune(int tol){

	pruneHelper(root, tol);

}

void twoDtree::clearHelper(Node* currNode) {
	if (currNode == NULL) {
		return;
	}

	clearHelper(currNode->left);
	clearHelper(currNode->right);

	free(currNode);
}


void twoDtree::clear() {
	clearHelper(root);
	root = NULL;
}

void twoDtree::copyHelper(Node * origNode, Node* currNode) {
	if (origNode == NULL) {
		return;
	}

	if (origNode->left != NULL) {
		Node* leftNode = new Node(origNode->left->upLeft, origNode->left->lowRight, origNode->left->avg);
		currNode->left = leftNode;
	} else {
		currNode->left = NULL;
	}

	if (origNode->right != NULL) {
		Node* rightNode = new Node(origNode->right->upLeft, origNode->right->lowRight, origNode->right->avg);
		currNode->right = rightNode;
	} else {
		currNode->right = NULL;
	}

	copyHelper(origNode->left, currNode->left);
	copyHelper(origNode->right, currNode->right);
}

void twoDtree::copy(const twoDtree & orig){
	s = orig.s;
	height = orig.height;
	width = orig.width;

	root = new Node(orig.root->upLeft, orig.root->lowRight, orig.root->avg);

	copyHelper(orig.root, root);
}



