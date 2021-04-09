#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define INT_MAX 2147483647
#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 26
#define INF 2147483647

typedef struct node{
    char name;
    int x;
    int y;
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
                path[w][path_num[w]+1]=u;
                path_num[w]++;
            }
            distance[w] = distance[u] + weight[u][w];
            /*for(int j=0;j<path_num[u];j++)
                path[w][j]=path[u][j];
            path_num[w]=path_num[u];
            path[w][path_num[w]]=w;
            path_num[w]++;
          distance[w] = distance[u] + weight[u][w];*/
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
    }
    printf("------------------------------------------------------------\n");
    printf("--------------------contury info-------------------------\n");
    printf("------------------------------------------------------------\n");
    for(int i=0;i<MAX_VERTICES;i++)
        printf("%c : (x,y) = (%d,%d)\n",contury[i].name,contury[i].x,contury[i].y);
    
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
}
int main()
{
    contury = (node*) malloc(sizeof(node)*MAX_VERTICES);
    init_contury();
    init_path();
    shortest_path(0, MAX_VERTICES);
    
    for(int i=0;i<MAX_VERTICES;i++)
    {
        for(int j=0;j<MAX_VERTICES;j++)
         if(path[i][j]!=INF) printf("%d->",path[i][j]);
        printf("%d\n",i);
    }
    printf("\n\n");
    for(int i=0;i<MAX_VERTICES;i++)
        printf("%f ",distance[i]);
    printf("\n");
    return 0;
}
