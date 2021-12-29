#include<bits/stdc++.h>
#define NULL_VALUE -999999
#define INFINITY 99999999999
#define WHITE 1
#define GREY 2
#define BLACK 3
using namespace std;
typedef pair<int, int> inTPair;

class Graph
{
    int nVertices, nEdges,time,countForDij,countForBell ;
    bool directed ;
    int ** matrix ;
    int *parent;
    int *distance;
    int *color;
    int *arrayForDij;
    int *arrayForBell;
public:
    Graph(bool dir = false);
    ~Graph();
    void setnVertices(int n);
    bool addEdge(int u, int v, int weight);
    void removeEdge(int u, int v);
    void removeAllEdges();
    bool isEdge(int u, int v);
    void printShortestDistance_dijkstra(int source, int destination);
    void printGraph();
    void printShortestPath_dij(int parent[], int destination);
    void printShortestPath_Bell(int parent[], int destination);
    void printShortestDistance_BellMan(int source, int destination, int nEdges);
};


Graph::Graph(bool dir)
{
    nVertices = 0 ;
    nEdges = 0 ;
    matrix = 0 ;
    countForDij=0;
    countForBell=0;
    directed = dir ;
}

void Graph::setnVertices(int n)
{
    this->nVertices = n ;
    parent=new int[n];
    color=new int[n];
    distance=new int[n];
    arrayForBell=new int[n];
    arrayForDij=new int[n];
    //allocate space for the matrix
    matrix = new int*[nVertices];
    for(int i=0; i<nVertices; i++)
    {
        matrix[i] = new int[nVertices];
        for(int j=0; j<nVertices; j++)
            matrix[i][j] = 0; //initialize the matrix cells to 0
    }

}

bool Graph::addEdge(int u, int v, int weight)
{
    //write your code here
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return false;
    else if(isEdge(u,v)) return false;
    else
        matrix[u][v] = weight;
    nEdges++;
    return true;
}

void Graph::removeEdge(int u, int v)
{
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return;
    nEdges--;
    matrix[u][v]=0;
    if(!directed)matrix[v][u]=0;
    //write this function
}

void Graph::removeAllEdges()
{
    for(int i=0; i<nVertices; i++)
    {
        for(int j=0; j<nVertices; j++)
        {
            matrix[i][j]=0;
        }
    }
    nEdges=0;
}

bool Graph::isEdge(int u, int v)
{
    return matrix[u][v];
    //returns true if (u,v) is an edge, otherwise should return false
}

void Graph::printShortestPath_dij(int parent[], int destination)
{
    if(parent[destination]==-1){
        return;
    }
    printShortestPath_dij(parent,parent[destination]);
    arrayForDij[countForDij++]=destination;
}

void Graph::printShortestPath_Bell(int parent[], int destination)
{
    if(parent[destination]==-1){
        return;
    }
    printShortestPath_Bell(parent,parent[destination]);
    arrayForBell[countForBell++]=destination;
}

void Graph::printShortestDistance_dijkstra(int source, int destination)
{
    priority_queue< inTPair, vector <inTPair>, greater<inTPair>> p_que;
    for(int i=0; i<nVertices; i++)
    {
        distance[i]=INFINITY;
        parent[i]=-1;
    }
    distance[source]=0;
    p_que.push(make_pair(0,source));
    do
    {
        int u=p_que.top().second;
        p_que.pop();
        cout<<u<<endl;
        for(int i=0; i<nVertices; i++)
        {
            if(isEdge(u,i))
            {

                if(distance[i]>distance[u]+matrix[u][i])
                {
                    distance[i]=distance[u]+matrix[u][i];
                    p_que.push(make_pair(distance[i],i));
                    parent[i]=u;
                }
            }
        }
    }
    while(!p_que.empty()&&p_que.top().second!=destination);
    for(int i=0;i<nVertices;i++)
        cout<<"Node "<<i<<": "<<distance[i]<<endl;
    FILE *fptr;
    fptr = fopen("Out.txt", "a");
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }
    fprintf(fptr, "Dijkstra Algorithm:\n");
    fprintf(fptr,"%d\n",distance[destination]);
    fprintf(fptr,"%d ->",source);
    printShortestPath_dij(parent,destination);
    for(int i=0;i<countForDij-1;i++)
        fprintf(fptr,"%d ->",arrayForDij[i]);
    fprintf(fptr,"%d\n",destination);
    fclose(fptr);
}

void Graph::printShortestDistance_BellMan(int source, int destination, int nEdges)
{
    for(int i=0; i<nVertices; i++)
    {
        distance[i]=INFINITY;
        parent[i]=-1;
    }
    distance[source]=0;
    for(int k=1; k<nVertices; k++)
    {
        for(int i=0; i<nVertices; i++)
        {
            for(int j=0; j<nVertices; j++)
            {
                if(isEdge(i,j))
                {
                    if(distance[j]>distance[i]+matrix[i][j])
                    {
                        distance[j]=distance[i]+matrix[i][j];
                        parent[j]=i;
                    }
                }
            }
        }
    }
    FILE *fptr;
    fptr = fopen("Out.txt", "w");
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }
    for(int i=0; i<nVertices; i++)
    {
        for(int j=0; j<nVertices; j++)
        {
            if(isEdge(i,j))
            {
                if(distance[j]>distance[i]+matrix[i][j])
                {
                    fprintf(fptr,"Bellman Ford Algorithm:\n");
                    fprintf(fptr,"Negative cycle detected.\n");
                    cout<<"Negative cycle detected"<<endl;
                    return;
                }
            }
        }
    }
    fprintf(fptr, "Bellman Ford Algorithm:\n");
    fprintf(fptr,"%d\n",distance[destination]);
    fprintf(fptr,"%d ->",source);
    printShortestPath_Bell(parent,destination);
    for(int i=0;i<countForBell-1;i++)
        fprintf(fptr,"%d ->",arrayForBell[i]);
    fprintf(fptr,"%d\n",destination);
    fclose(fptr);
}
void Graph::printGraph()
{
    printf("\nNumber of vertices: %d, Number of edges: %d\n", nVertices, nEdges);
    for(int i=0; i<nVertices; i++)
    {
        printf("%d:", i);
        for(int j=0; j<nVertices; j++)
        {
            if(matrix[i][j]!=0)
                printf(" %d", j);
        }
        printf("\n");
    }
}
Graph::~Graph()
{
    delete[] parent;
    delete[] distance;
    delete[] color;
}

int main()
{
    Graph g(true);
    fstream file;
    int x,nVertices,nEdge,i=0,u,v,w;
    file.open("input4.txt");
    int array[100000];
    while(file>>x)
    {
        array[i++]=x;
    }
    nVertices=array[0];
    nEdge=array[1];
    g.setnVertices(nVertices);
    for(int j=2; j<i-2; j=j+3)
    {
        g.addEdge(array[j],array[j+1],array[j+2]);
    }
    g.printShortestDistance_BellMan(array[i-2],array[i-1],nEdge);
    g.removeAllEdges();
    cout<<endl;
    for(int j=2; j<i-2; j=j+3)
    {
        g.addEdge(array[j],array[j+1],(array[j+2]));
    }
    g.printShortestDistance_dijkstra(array[i-2],array[i-1]);
    cout<<endl;
    file.close();
    return 0;
}
