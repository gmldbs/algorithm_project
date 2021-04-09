#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Node{
	int key; // reservation number
	char color; // 'R' => red, 'B' => black
	char name[20]; // client name
	char source[20]; // source (can change data structure)
	char destination[20]; // destination (can change data structure)
	int date; // ex) 1225 == 12/25
	int time; // ex) 1035 == 10:35
	struct Node* PNode;
	struct Node* LNode;
	struct Node* RNode;
} Node;

Node* createNode(int num) {
	Node* node = (Node*)malloc(sizeof(Node));
	node->key = num;
	node->color = 'R';
	node->PNode = NULL;
	node->LNode = NULL;
	node->RNode = NULL;
	return node;
}

Node* search_prime(Node* root, int num) {
	if (root != NULL) {
		if (root->key == num) {
			return root;
		}
		else if (num < root->key) {
			if (root->LNode == NULL) {
				return root;
			}
			else return search_prime(root->LNode, num);
		}
		else {
			if (root->RNode == NULL) return root;
			else return search_prime(root->RNode, num);
		}
	}
    return root;
}

Node* minimum(Node* root) {
	while (root->LNode != NULL) {
		root = root->LNode;
	}
	return root;
}

Node* maximum(Node* root) {
	while (root->RNode != NULL) {
		root = root->RNode;
	}
	return root;
}

Node* successor(Node* x) {
	if (x->RNode != NULL) {
		return minimum(x->RNode);
	}
	else {
		Node* y = x->PNode;
		while (y != NULL && x == y->RNode) {
			x = y;
			y = y->PNode;
		}
		if (y != NULL) {
			return y;
		}
		else {
			return maximum(x);
		}
	}
}

Node* predeccessor(Node* x) {
	if (x->LNode != NULL) {
		return maximum(x->LNode);
	}
	else {
		Node* y = x->PNode;
		while (y != NULL && x == y->LNode) {
			x = y;
			y = y->PNode;
		}
		if (y != NULL) {
			return y;
		}
		else {
			return minimum(x);
		}
	}
}

Node* LRotate(Node* root, Node* x) {
	Node* y = x->RNode;
	x->RNode = y->LNode;
	if (y->LNode != NULL) {
		Node* ly = y->LNode;
		ly->PNode = x;
	}
	y->PNode = x->PNode;
	Node* px = x->PNode;
	if (x->PNode == NULL) {
		root = y;
	}
	else if (x == px->LNode) {
		px->LNode = y;
	}
	else {
		px->RNode = y;
	}
	y->LNode = x;
	x->PNode = y;
	return root;
}

Node* RRotate(Node* root, Node* x) {
	Node* y = x->LNode;
	x->LNode = y->RNode;
	if (y->RNode != NULL) {
		Node* ry = y->RNode;
		ry->PNode = x;
	}
	y->PNode = x->PNode;
	Node* px = x->PNode;
	if (x->PNode == NULL) {
		root = y;
	}
	else if (x == px->LNode) {
		px->LNode = y;
	}
	else {
		px->RNode = y;
	}
	y->RNode = x;
	x->PNode = y;
	return root;
}

Node* RB_INSERT_FIXUP(Node* root, Node* z) {
	Node* pz = z->PNode;
	while (z != root && pz->color == 'R') {
		Node* ppz = pz->PNode;
		Node* y;
		while (pz->color == 'R') {
			if (pz == ppz->LNode) {
				y = ppz->RNode;
				if (y->color == 'R') {
					pz->color = 'B';
					y->color = 'B';
					ppz->color = 'R';
					z = ppz;
				}
				else {
					if (z == pz->RNode) {
						z = pz;
						root = LRotate(root, z);
					}
					pz = z->PNode;
					ppz = pz->PNode;
					pz->color = 'B';
					ppz->color = 'R';
					root = RRotate(root, ppz);
				}
			}
			else {
				y = ppz->LNode;
				if (y->color == 'R') {
					pz->color = 'B';
					y->color = 'B';
					ppz->color = 'R';
					z = ppz;
				}
				else {
					if (z == pz->LNode) {
						z = pz;
						root = RRotate(root, z);
					}
					pz = z->PNode;
					ppz = pz->PNode;
					pz->color = 'B';
					ppz->color = 'R';
					root = LRotate(root, ppz);
				}
			}
			if (z->PNode != NULL) {
				pz = z->PNode;
				ppz = pz->PNode;
			}
		}
	}
	root->color = 'B';
	return root;
}

Node* RB_INSERT(Node* root, int num, char input_name[20], char input_source[20], char input_destination[20], int input_date, int input_time) {
	Node* z = createNode(num);
	// reservation info
	strcpy(z->name, input_name);
	strcpy(z->source, input_source);
	strcpy(z->destination, input_destination);
	z->date = input_date;
	z->time = input_time;
	//
	if (root == NULL) {
		root = z;
	}
	else {
		Node* y = NULL;
		Node* x = root;
		while (x != NULL) {
			if (x->key == -1) {
				break;
			}
			y = x;
			if (num < x->key) {
				x = x->LNode;
			}
			else {
				x = x->RNode;
			}
		}
		z->PNode = y;
		if (num < y->key) {
			y->LNode = z;
		}
		else {
			y->RNode = z;
		}
	}
	Node* lz = createNode(-1);
	lz->color = 'B';
	z->LNode = lz;
	lz->PNode = z;
	Node * rz = createNode(-1);
	rz->color = 'B';
	z->RNode = rz;
	rz->PNode = z;
	root = RB_INSERT_FIXUP(root, z);
	return root;
}

Node* RB_DELETE_FIXUP(Node* root, Node* x) {
	Node* px = x->PNode;
	Node* ppx, * w, * lw, * rw;
	if (x == px->LNode) {
		w = px->RNode;
	}
	else {
		w = px->LNode;
	}
	while (x != root && x->color == 'B') {
		if (w->key == -1) {
			break;
		}
		if (x == px->LNode) {
			w = px->RNode;
			// case 1
			if (w->color == 'R') {
				w->color = 'B';
				px->color = 'R';
				root = LRotate(root, px);
				w = px->RNode;
			}
			// case 2
			lw = w->LNode;
			rw = w->RNode;
			if (lw->color == 'B' && rw->color == 'B') {
				w->color = 'R';
				x = px;
				px = x->PNode;
				ppx = px->PNode;
			}
			// case 3
			else if (rw->color == 'B') {
				lw->color = 'B';
				w->color = 'R';
				root = RRotate(root, w);
				w = px->RNode;
			}
			// case 4
			else if (lw->color == 'B') {
				w->color = px->color;
				px->color = 'B';
				rw->color = 'B';
				root = LRotate(root, px);
				x = root;
			}
		}
		else {
			w = px->LNode;
			// case 1
			if (w->color == 'R') {
				w->color = 'B';
				px->color = 'R';
				root = RRotate(root, px);
				w = px->LNode;
			}
			// case 2
			lw = w->RNode;
			rw = w->LNode;
			if (lw->color == 'B' && rw->color == 'B') {
				w->color = 'R';
				x = px;
				px = x->PNode;
				ppx = px->PNode;
			}
			// case 3
			else if (lw->color == 'B') {
				rw->color = 'B';
				w->color = 'R';
				root = LRotate(root, w);
				w = px->LNode;
			}
			// case 4
			else if (rw->color == 'B') {
				w->color = px->color;
				px->color = 'B';
				lw->color = 'B';
				root = RRotate(root, px);
				x = root;
			}
		}
	}
	x->color = 'B';
	return root;
}

Node* RB_DELETE(Node* root, int num) {
	Node* z = search_prime(root, num);
    printf("ck : %d\n",z->key);
    Node* x, * y, * py;
    if (root == NULL) return root;
	if (z->key != num) {
		printf("\nERROR : Reservation No. '%d' doesn't exist.\n\n", num);
		return root;
	}
	else {
		Node* lz = z->LNode;
		Node* rz = z->RNode;
		Node* ly;
		if (lz->key == -1 || rz->key == -1) {
			y = z;
		}
		else {
			y = successor(z)->PNode;
		}
		ly = y->LNode;
		if (ly->key != -1) {
			x = y->LNode;
		}
		else {
			x = y->RNode;
		}

		// node 'y' is removed
		if (x->key != -1) {
			x->PNode = y->PNode;
		}
		if (y->PNode == NULL) {
			root = x;
		}
		else {
			py = y->PNode;
			if (y == py->LNode) {
				py->LNode = x;
			}
			else {
				py->RNode = x;
			}
		}

		// if y == z's successor
		if (y != z) {
			z->key = y->key;
			z->color = y->color;
		}
		if (y->color == 'B') {
			root = RB_DELETE_FIXUP(root, x);
		}
	}
	return root;
}

void PRINT_BST(Node* root, int space)
{
	int i;
	if (root == NULL)
		return;

	space += 5;
	PRINT_BST(root->RNode, space);
	
	printf("\n");
	for (i = 5; i < space; i++)
		printf(" ");
	if(root->key == -1)
		printf("(NIL)\n");
	else {
		if(root->color == 'B')
			printf("(%d)\n", root->key);
		else if(root->color == 'R')
			printf("%d\n", root->key);
	}

	PRINT_BST(root->LNode, space);
}

// fill the array with random integers (without duplicate values)
void randomize(int* arr) {
	int i, j;
	srand(time(NULL));
	for (i = 0; i < 20; i++) {
		arr[i] = rand() % 49 + 1;
		for (j = 0; j < i; j++) {
			if (arr[i] == arr[j]) {
				arr[i] = rand() % 49 + 1;
				j = 0;
			}
		}
	}
}

// print reservation info in-order
int column = 0;
void inorder(Node* root) {
	if (root->key != -1) {
		inorder(root->LNode);
		printf("| %-4d | %-17d | %-15s | %-15s | %-15s | %d%d/%d%-11d | %d%d:%d%-11d |\n", ++column, root->key, root->name, root->source, root->destination, root->date / 1000, root->date % 1000 / 100, root->date % 100 / 10, root->date % 10, root->time / 1000, root->time % 1000 / 100, root->time % 100/ 10, root->time % 10);
		inorder(root->RNode);
	}
}

void print_reservation(Node* root) {
	printf("< Reservation record >\n");
	printf("----------------------------------------------------------------------------------------------------------------------\n");
	printf("| %-4s | %-17s | %-15s | %-15s | %-15s | %-15s | %-15s |\n", "#", "Reservation No.", "Client Name", "Source", "Destination", "Departure Date", "Departure Time");
	printf("----------------------------------------------------------------------------------------------------------------------\n");
	inorder(root);
	column = 0;
	printf("----------------------------------------------------------------------------------------------------------------------\n");
}

int main() {
	char name[20];
	char source[20];
	char destination[20];
	int date = 1225;
	int time = 1035;
	char names[3][20] = { "James", "Alisa", "HyunJi" };
	char sources[3][20] = { "Korea", "China", "Japan" };
	char destinations[3][20] = { "United States", "Canada", "Australia" };

	// reservation tree - root node initialization
	Node* T = NULL;

	int A[20], i;
	randomize(A);
	for (i = 0; i < 10; i++) {
		T = RB_INSERT(T, A[i], names[i % 3], sources[i % 3], destinations[i % 3], date, time);
	}

	PRINT_BST(T, 0); printf("\n");
	print_reservation(T);

	// input test
	printf("input name : ");
	scanf("%s", &name);
	printf("input source : ");
	scanf("%s", &source);
	printf("input destination : ");
	scanf("%s", &destination);
	printf("input date : ");
	scanf("%d", &date);
	printf("input time : ");
	scanf("%d", &time);

	T = RB_INSERT(T, 500, name, source, destination, date, time);
	print_reservation(T);
    PRINT_BST(T, 0);
	// delete test
	T = RB_DELETE(T, 500);
	print_reservation(T);
	PRINT_BST(T, 0);
    
    return 0;
}
