#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#define INT_MAX 2147483647
#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 26
#define INF 2147483647
typedef struct Node{
	int key; // reservation number
	char color; // 'R' => red, 'B' => black
	char name[20]; // client name
	char source; // source (can change data structure)
	char destination; // destination (can change data structure)
	int date; // ex) 1225 == 12/25
    int time_index;
	int time; // ex) 1035 == 10:35
    int seatX;
    int seatY;
    int password;
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

Node* RB_INSERT(Node* root, int num, char input_name[20], char input_source, char input_destination, int input_date, int input_time, int input_seatX, int input_seatY, int input_time_index, int input_password) {
    Node* z = createNode(num);
	// reservation info
	strcpy(z->name, input_name);
    z->source=input_source;
    z->destination=input_destination;
	//strcpy(z->source, input_source);
	//strcpy(z->destination, input_destination);
	z->date = input_date;
	z->time = input_time;
    z->seatX = input_seatX;
    z->seatY = input_seatY;
    z->time_index = input_time_index;
    z->password = input_password;
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
		printf("| %-4d | %-17d | %-15s | %-5c | %-5c | %d%d/%d%-11d | %d%d:%d%-11d | (%c,%d) \n", ++column, root->key, root->name, root->source, root->destination, root->date / 1000, root->date % 1000 / 100, root->date % 100 / 10, root->date % 10, root->time / 1000, root->time % 1000 / 100, root->time % 100/ 10, root->time % 10,root->seatX+'A',root->seatY);
		inorder(root->RNode);
	}
}

void print_reservation(Node* root) {
	printf("< Reservation record >\n");
	printf("----------------------------------------------------------------------------------------------------------------------\n");
	printf("| %-4s | %-17s | %-15s | %-5s | %-5s | %-15s | %-15s | %-15s\n", "#", "Reservation No.", "Client Name", "Source", "Destination", "Departure Date", "Departure Time","Seat Info");
	printf("----------------------------------------------------------------------------------------------------------------------\n");
	inorder(root);
	column = 0;
	printf("----------------------------------------------------------------------------------------------------------------------\n");
}
typedef struct airPlane{
    int seat[8][40];
    int departure;
}airPlane;
typedef struct node{
    char name;
    int x;
    int y;
    airPlane plane[31][26][4]; // 날짜 / 도착지 / 시간대 4개
    Node* T;
}node;
node* contury;
int connect[MAX_VERTICES][MAX_VERTICES];
float weight[MAX_VERTICES][MAX_VERTICES];
int path[MAX_VERTICES][MAX_VERTICES];
int path_num[MAX_VERTICES];
float distance[MAX_VERTICES];
int found[MAX_VERTICES];
int ck[MAX_VERTICES][MAX_VERTICES];
int choose(float distance[], int n, int found[])
{
  int i, min, minpos;
  min = INT_MAX;
  minpos = -1;
  for (i = 0; i < n; i++)
  {
    if (distance[i] < min && found[i] == FALSE)
    {
      min = distance[i];
      minpos = i;
    }
  }
  return minpos;
}

void shortest_path(int start, int n)
{
  int i, u, w;
 
  for (i = 0; i < n; i++)
  {
    distance[i] = weight[start][i];
    found[i] = FALSE;
      path_num[i]=1;
      path[i][0]=start;
  }
  found[start] = TRUE;
  distance[start] = 0;
  for (i = 0; i < n - 2; i++)
  {
    u = choose(distance, n, found);
    found[u] = TRUE;
    for (w = 0; w < n; w++)
    {
      if (found[w]==FALSE)
      {
        if (distance[u] + weight[u][w] < distance[w])
        {
            if(i==0)
            {
                path[w][path_num[w]]=u;
                path_num[w]++;
            }
            else{
                for(int j=0;j<=path_num[u];j++)
                    path[w][j]=path[u][j];
                path_num[w]=path_num[u]+1;
                path[w][path_num[w]]=u;
            }
            distance[w] = distance[u] + weight[u][w];
        }
        
      }
    }
  }
}
void init_path(){
    for(int i=0;i<MAX_VERTICES;i++)
    {
        path_num[i]=0;
        for(int j=0;j<MAX_VERTICES;j++)
        {
            path[i][j]=INF;
            ck[i][j]=0;
        }
    }
    
}
void init_contury(){
    srand(time(NULL));
    for(int i=0;i<MAX_VERTICES;i++)
    {
        contury[i].x=rand()%6000-3000;
        contury[i].y=rand()%6000-3000;
        contury[i].name='A'+i;
        contury[i].T = NULL;
    }
    for(int i=0;i<100;i++)
    {
        int x=rand()%MAX_VERTICES,y;
        while(((y=rand()%MAX_VERTICES)==x)||connect[x][y]==1);
        connect[x][y]=1;
        connect[y][x]=1;
    }
    for(int i=0;i<MAX_VERTICES;i++)
    {
        for(int j=0;j<MAX_VERTICES;j++)
        {
            if(connect[i][j]==1) weight[i][j]=sqrt((contury[i].x-contury[j].x)*(contury[i].x-contury[j].x)+(contury[i].y-contury[j].y)*(contury[i].y-contury[j].y));
            else weight[i][j]=INF;
        }
    }
    for(int i=0;i<MAX_VERTICES;i++)
    {
        for(int j=0;j<31;j++)
        {
            for(int des=0;des<MAX_VERTICES;des++)
            {
                if(connect[i][des]==1)
                {
                    for(int k=0;k<4;k++)
                    {
                        contury[i].plane[j][des][k].departure=((rand()%6)+6*k)*100+rand()%60;
                        for(int p=0;p<8;p++)
                        {
                            for(int q=0;q<40;q++)
                            {
                                contury[i].plane[j][des][k].seat[p][q]=0;
                            }
                        }
                    }
                }
            }
        }
    }
    // 랜덤 인풋 500개 채우기 (나라별로 매일 500개씩)
    char names[3][20] = { "James", "Alisa", "HyunJi" };
    for(int i=0;i<MAX_VERTICES;i++)
    {
        for(int j=0;j<31;j++)
        {
            int cnt=1;
            while(cnt<=500)
            {
                int des=rand()%26;
                if(des==i||connect[i][des]!=1) continue;
                char des_name='A'+des;
                char source_name='A'+i;
                int pN=rand()%4;
                int row=rand()%8;
                int column=rand()%40;
                contury[i].plane[j][des][pN].seat[row][column]=1;
                contury[i].T=RB_INSERT(contury[i].T, rand()%1000000000, names[rand() % 3], source_name, des_name, j+1, contury[i].plane[j][des][pN].departure,row,column,pN,0);
                cnt++;
            }
        }
        //printf("나라 %c의 reservation\n",'A'+i);
        //print_reservation(contury[i].T);
    }
    /*printf("------------------------------------------------------------\n");
    printf("--------------------timeTable info-------------------------\n");
    printf("------------------------------------------------------------\n");
    for(int i=0;i<MAX_VERTICES;i++)
    {
        for(int j=0;j<31;j++)
        {
            for(int k=0;k<4;k++)
            {
                int time = contury[i].plane[j][k].departure;
                printf("%dth departure time of %c is %d%d:%d%d\n",k,contury[i].name,time/1000,(time/100)%10,(time/10)%10,time%10);
            }
        }
    }
    printf("------------------------------------------------------------\n");
    printf("--------------------contury info-------------------------\n");
    printf("------------------------------------------------------------\n");
    for(int i=0;i<MAX_VERTICES;i++)
        printf("%c : (x,y) = (%d,%d)\n",contury[i].name,contury[i].x,contury[i].y);
    
    
    printf("------------------------------------------------------------\n");
    printf("--------------------connected state-------------------------\n");
    printf("------------------------------------------------------------\n");
    for(int i=0;i<MAX_VERTICES;i++)
    {
        for(int j=0;j<MAX_VERTICES;j++)
        {
            printf("%d ",connect[i][j]);
        }
        printf("\n");
    }
    printf("------------------------------------------------------------\n");
    printf("--------------------weight state-------------------------\n");
    printf("------------------------------------------------------------\n");
    for(int i=0;i<MAX_VERTICES;i++)
    {
        for(int j=0;j<MAX_VERTICES;j++)
        {
            if(weight[i][j]==INF) printf("0 ");
            else printf("%.2f ",weight[i][j]);
        }
        printf("\n");
    }
    printf("------------------------------------------------------------\n");
    printf("--------------------init end-------------------------\n");
    printf("------------------------------------------------------------\n");
     */
}
void reservation(){
    char src_contury[20],des_contury[20];
    char c;
    int dep_date;
    int passing_path[26];
    int passing_cnt=0;
    printf("Select Source Country (A~Z) : ");
    scanf("%s",src_contury);
    printf("Select Destination Country (A~Z) : ");
    scanf("%s",des_contury);
    if(src_contury[0]==des_contury[0])
    {
        printf("Source Contury is same with Destination Contury\n");
        reservation();
        return;
    }
    shortest_path(src_contury[0]-'A',MAX_VERTICES);
    printf("\n\nshortest path is : ");
    for(int i=0;i<MAX_VERTICES;i++)
    {
        if(path[des_contury[0]-'A'][i]!=INF) {
            printf("%c->",path[des_contury[0]-'A'][i]+'A');
            passing_path[passing_cnt++]=path[des_contury[0]-'A'][i];
        }
    }
    passing_path[passing_cnt++]=des_contury[0]-'A';
    printf("%c\n",des_contury[0]);
    printf("distance : %.2f\n",distance[des_contury[0]-'A']);
    int hour=distance[des_contury[0]-'A']/500, minute=(distance[des_contury[0]-'A']-500*hour)/500*60;
    printf("Total Flight time : %d hour %d minutes\n",hour,minute);
    printf("\nNow, you can book each step\n\n\n");
    for(int con=1;con<passing_cnt;con++)
    {
        int time;
        printf("source contury : %c -> destination contury : %c\n",passing_path[con-1]+'A',passing_path[con]+'A');
        printf("Select Departure Date (1~31) : ");
        scanf("%d",&dep_date);
        printf("\nDeparture Table\n");
        for(int i=0;i<4;i++)
        {
            //int time = contury[src_contury[0]-'A'].plane[dep_date-1][des_contury[0]-'A'][i].departure;
            time = contury[passing_path[con-1]].plane[dep_date-1][passing_path[con]][i].departure;
            printf("%dth plane : %d%d:%d%d\n",i+1,time/1000,(time%1000)/100,(time%100)/10,time%10);
        }
        char planeNumber[50];
        int pN;
        printf("\nSelect plane number (1~4) : ");
        scanf("%s",planeNumber);
        pN=planeNumber[0]-'1';
        time=contury[passing_path[con-1]].plane[dep_date-1][passing_path[con]][pN].departure;

process :
        printf("\nPlane Seat Info\n\n");
        printf(" ");
        for(int i=1;i<41;i++)
            printf(" %d",i);
        printf("\n");
        for(int i=0;i<8;i++)
        {
            printf("%c ",'A'+i);
            for(int j=0;j<40;j++)
            {
                if(j<9)
                {
                    if(contury[passing_path[con-1]].plane[dep_date-1][passing_path[con]][pN].seat[i][j]==0) printf("T ");
                    else printf("F ");
                }
                else
                {
                    if(contury[passing_path[con-1]].plane[dep_date-1][passing_path[con]][pN].seat[i][j]==0) printf("T  ");
                    else printf("F  ");
                }
            }
            printf("\n");
        }
        int seat_row,seat_column;
        char seat_info[50],column[2],cnt=0;
        for(int i=0;i<50;i++)
            seat_info[i]=0;
        printf("\nChoose seat ex) A,20 : ");
        scanf("%s",seat_info);
        for(int i=0;i<sizeof(seat_info);i++)
        {
            if(seat_info[i]>='A'&&seat_info[i]<='Z') {
                seat_row=seat_info[i]-'A';
            }
            else if(seat_info[i]>=48&&seat_info[i]<=57) column[cnt++]=seat_info[i];
        }
        if(cnt==1) seat_column=column[0]-'0';
        else if(cnt==2) seat_column=(column[0]-'0')*10+column[1]-'0';
        if(contury[passing_path[con-1]].plane[dep_date-1][passing_path[con]][pN].seat[seat_row][seat_column-1]==1)
        {
            printf("You can select only T part\n");
            goto process;
        }
        else contury[passing_path[con-1]].plane[dep_date-1][passing_path[con]][pN].seat[seat_row][seat_column-1]=1;
        char name[20],pw[20];
        int password;
        printf("Input your name : ");
        scanf("%s",name);
        printf("Input your password to confirm or cancel your reservation (only number) : ");
        scanf("%d",&password);
        int reservation_num=rand()%1000000000;
        while(1)
        {
            Node* root=search_prime(contury[passing_path[con-1]].T, reservation_num);
            if(root->key!=reservation_num) break;
            else reservation_num=rand()%1000000000;
        }
        contury[passing_path[con-1]].T=RB_INSERT(contury[passing_path[con-1]].T,reservation_num, name, passing_path[con-1]+'A', passing_path[con]+'A', dep_date,
                                                contury[passing_path[con-1]].plane[dep_date-1][passing_path[con]][pN].departure,seat_row,seat_column,pN,password);
        hour=weight[passing_path[con-1]][passing_path[con]]/500;
        minute=(weight[passing_path[con-1]][passing_path[con]]-500*hour)/500*60;
        minute+=time%100;
        if(minute>=60) {
            hour++;
            minute-=60;
        }
        hour+=time/100;
        if(hour>=24)
        {
            dep_date++;
            hour-=24;
        }
        if(dep_date>=32)
        {
            dep_date-=31;
        }
        printf("\n\nArrived at %d(date) %d%d:%d%d\n",dep_date, hour/10,hour%10,minute/10,minute%10);
        printf("\nyour reservation ticket Number = %d\n\n",reservation_num);
        printf("Must remember your reservation ticket Number to confirm or cancel your reservation!!\n");
    }
    
}
void confirm_reservation(){
    int rev_num,src_contury;
    char source_contury[10];
    printf("input source contury : ");
    scanf("%s",source_contury);
    src_contury=source_contury[0]-'A';
    printf("input reservation number : ");
    scanf("%d",&rev_num);
    Node* root=search_prime(contury[src_contury].T, rev_num);
    if(root->key!=rev_num) printf("Cannot found data\n");
    else
    {
        int password;
        printf("input your password to confirm reservation : ");
        scanf("%d",&password);
        if(password==root->password)
        {
            printf("< Reservation record >\n");
            printf("---------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("| %-4s | %-17s | %-15s | %-15s | %-15s | %-15s | %-15s | %-15s\n", "#", "Reservation No.", "Client Name", "Source", "Destination", "Departure Date", "Departure Time","Seat Info");
            printf("---------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("| %-4d | %-17d | %-15s | %-15c | %-15c |    %d%-11d | %d%d:%d%-11d | (%c,%d) \n", 1, root->key, root->name, root->source, root->destination, root->date % 100 / 10, root->date % 10, root->time / 1000, root->time % 1000 / 100, root->time % 100/ 10, root->time % 10,root->seatX+'A',root->seatY);
            printf("---------------------------------------------------------------------------------------------------------------------------------------\n\n");
        }
        else
        {
            printf("Wrong password!\n");
        }
    }
};
void cancel_reservation(){
    int rev_num,src_contury;
    char source_contury[10];
    printf("input source contury : ");
    scanf("%s",source_contury);
    src_contury=source_contury[0]-'A';
    printf("input reservation number : ");
    scanf("%d",&rev_num);
    Node* root=search_prime(contury[src_contury].T, rev_num);
    if(root->key!=rev_num) printf("Cannot found data\n");
    else
    {
        int password;
        printf("input your password to cancel reservation : ");
        scanf("%d",&password);
        if(password==root->password)
        {
            contury[src_contury].T = RB_DELETE(contury[src_contury].T, rev_num);
            contury[src_contury].plane[root->date-1][root->destination-'A'][root->time_index].seat[root->seatX][root->seatY-1]=0;
            printf("Complete cancel!\n\n");
        }
        else{
            printf("Wrong password!\n");
        }
    }
};
void show_contury(){
    printf("------------------------------------------------------------\n");
    printf("--------------------connected state-------------------------\n");
    printf("------------------------------------------------------------\n");
    printf(" ");
    for(int i=0;i<MAX_VERTICES;i++)
    {
        printf(" %c",'A'+i);
    }
    printf("\n");
    for(int i=0;i<MAX_VERTICES;i++)
    {
        printf("%c ",'A'+i);
        for(int j=0;j<MAX_VERTICES;j++)
        {
            printf("%d ",connect[i][j]);
        }
        printf("\n");
    }
}
void show_path(){
    char src_contury[10], des_contury[10];
    int passing_path[26];
    int passing_cnt=0;
    printf("Input source contury (A~Z) : ");
    scanf("%s",src_contury);
    printf("Input destination contury (A~Z) : ");
    scanf("%s",des_contury);
    if(src_contury[0]==des_contury[0])
    {
        printf("Source Contury is same with Destination Contury\n\n\n");
        show_path();
        return;
    }
    shortest_path(src_contury[0]-'A',MAX_VERTICES);
    printf("shortest path is : ");
    for(int i=0;i<MAX_VERTICES;i++)
    {
        if(path[des_contury[0]-'A'][i]!=INF) {
            printf("%c->",path[des_contury[0]-'A'][i]+'A');
            passing_path[passing_cnt++]=path[des_contury[0]-'A'][i];
        }
    }
    passing_path[passing_cnt++]=des_contury[0]-'A';
    printf("%c\n",des_contury[0]);
    printf("distance : %.2f\n",distance[des_contury[0]-'A']);
    int hour=distance[des_contury[0]-'A']/500, minute=(distance[des_contury[0]-'A']-500*hour)/500*60;
    printf("Total Flight time : %d hour %d minutes\n",hour,minute);
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
    char input[100];
    for(int i=0;i<100;i++)
        input[i]=0;
    int type;
    int ck=0;
    // reservation tree - root node initialization
    contury = (node*) malloc(sizeof(node)*MAX_VERTICES);
    init_contury();
    init_path();
    while(1)
    {
        printf("\n\n------------------------------------------------------------\n");
        printf("--------------Welcome to reservation program----------------\n");
        printf("------------------------------------------------------------\n");
        printf("1: reservation\n");
        printf("2: confirm your reservation\n");
        printf("3: cancel your reservation\n");
        printf("4: Show contury connect state\n");
        printf("5: Show shortest path and total flight time\n");
        printf("6: exit\n");
        printf("\nSelect type of this program to use : ");
        scanf("%s",input);
        for(int i=0;i<100;i++)
        {
            if(input[i]>='1'&&input[i]<='9')
            {
                if(input[i+1]!=0) break;
                ck=1;
                type=input[i]-'0';
                break;
            }
            else if(input[i]!=' ') break;
        }
        if(ck==0) {
            printf("Wrong input!!\n");
            continue;
        }
        ck=0;
        switch (type) {
            case 1:
                reservation();
                break;
            case 2:
                confirm_reservation();
                break;
            case 3:
                cancel_reservation();
                break;
            case 4:
                show_contury();
                break;
            case 5:
                show_path();
                break;
            case 6:
                printf("Thank you for using our reservation program\n");
                return 0;
            default:
                break;
        }
    }
    return 0;
}
