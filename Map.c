/*NAME AND DATE GOES HERE.*/
/*Brandeis Map*/

/*Standard system stuff - these are the ONLY ones that may be used.*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/*Path to the map folder; may need to change this.*/
#include "MapPATH.h"

/*MAY NOT be modified, but should be viewed.*/
#include "Map.h" /*Map data parameters, structures, and functions.*/

/*MAY NOT be modified, and there is no need view them:*/
#include "MapData.h"   /*Functions to input the map data.*/
#include "MapInput.h"  /*Functions to get user input.*/
#include "MapOutput.h" /*Functions to produce output.*/

/*Use this to get the time to travel along an edge.*/
#define EdgeCost(X) ( (TimeFlag) ? Time(X) : Elength[X] )

/*Use this to print a leg of a route or tour.*/
void PrintLeg(int edge);



/***************************************************************************************/
/*GRAPH ADJACENCY LIST DATA STRUCTURE                                                  */
/***************************************************************************************/
typedef struct adjListNode {

int edge;
struct adjListNode *Next;

}	adjListNode;


typedef struct adjList {

adjListNode *Head;

} adjList;

typedef struct Graph {

adjList* array [MaxVertex];
} Graph;

Graph  init_graph() {
	int i;
	int j;
	//GetEdges();
	//GetVertices();

	static Graph graph; 

	//initialize all list for all V
	adjListNode *SENTINEL_VALUE = malloc(sizeof(adjListNode));
	SENTINEL_VALUE->edge = -1;
	for (i=0; i<MaxVertex-1;i++){
		adjList *list = malloc(sizeof(adjList));
		list->Head = SENTINEL_VALUE;
		graph.array[i] = list;
	}
	
	for (i=20; i<=595; i++){
		int startVertex = Estart[i];
		if(startVertex == 0){
			break;
		}
		adjList *list = graph.array[startVertex];
		adjListNode *node = malloc(sizeof(adjListNode));
		node->edge = i;
		node->Next = list->Head;
		list->Head = node;
	}
	return graph;
};

void print_graph(Graph *graph){
	int i = 0;
	for (i=MinVertex; i<MaxVertex-1; i++){
		adjListNode *current = graph->array[i]->Head;
		printf("vertex: %d ", i);
		while(current->edge!= -1){
			printf("Edge: %d " , current->edge);
			current = current->Next;
		}
		printf("\n");
	}

}

/***************************************************************************************/
/*HEAP DATA STRUCTURE                                                                  */
/***************************************************************************************/
int size; 


typedef struct Node {
	int v;
	int dist;
	int pos;
	int prev;
	struct Node *prevNode;
    struct Node *children[170];
	int childSize;
} Node;

typedef struct Heap {
	Node *data[MaxVertex+1];
	Node *pos[MaxVertex];
	int size;
} Heap;


void swap (Node **a,  Node **b){
	Node *temp = *a;
	*a = *b;
	*b = temp;

}

#define data heap->data


percolate_up(Heap *heap, int i){

	while(i>1 && data[i]->dist < data[i/2]->dist){
		data[i]->pos = i/2;
		data[i/2]->pos = i;
		swap(&data[i], &data[i/2]);
		
		i = i/2;
	}
}

percolate_down(Heap *heap, int i){
	size = heap->size;
	while(i<= size){
		int k =i ;
		Node **min = &data[i];
		Node *_min = data[i];
		if(i*2 <= size){
			Node **left_child = &data[i*2];
			Node *left__child = data[i*2];
			if (left__child->dist < _min->dist){
				_min = left__child;	
				min =  left_child;
				k = i*2;
			}
		}
		
		if(i*2 + 1 <= size ){
			Node **right_child = &data[i*2 + 1];
			Node *right__child = data[i*2 + 1];
			if (right__child->dist < _min->dist){
				_min = right__child;
				min = right_child;
				k = i*2 +1;
			}
		}
		if (i == k){ //break if children are larger
			break;
		}
		
		_min->pos = i;
		data[i]->pos =k ;
		
		swap(min, &data[i]);
	   
		
		i = k;
		
	}
}

Node *extractMin(Heap *heap){
		Node *min = data[1];
		swap(&data[1], &data[heap->size]);
		data[1]->pos = 1;
		data[heap->size]->pos = heap->size;
		heap->size = heap->size - 1;
		percolate_down(heap, 1);
		return min;
}

void decreaseKey(Heap *heap, int v, int dist){
	Node *node = heap->pos[v]; 
	node->dist = dist;
	percolate_up(heap, node->pos);
}

//wraps a vertex around a node used in heap
Node *add_node (int v, int pos, int dist){
	Node *node = malloc(sizeof(Node));
	node->dist = dist;
	node->pos = pos;
	node->v = v;
	node->prev = -1;
	node->childSize = -1;
	return node;
}


Heap init_Heap (){
	static  Heap _heap;
	Heap *heap = &_heap;
	int i;
	for(i=0; i< MaxVertex; i++){ //ith vertex will initially be put in the i + 1th position in the heap
		Node *node = add_node(i, i+1, InfiniteCost);
		data[i+1] = node;
		heap->pos[i] = node;
	}
	
	heap->size = MaxVertex;
	return _heap;
};

Heap test_Heap(){
	static Heap test_heap;
	static  Heap *heap = &test_heap;
	int i;
	for(i=0; i<12; i++){ //ith vertex will initially be put in the i + 1th position in the heap
		Node *node = add_node(i, i+1, i);
		data[i+1] = node;
		heap->pos[i] = node;
	}
	
	heap->size = 12;
	return test_heap;
}

void decreaseKeys(Heap *heap, Graph *graph, Node *currentNode){
    int v = currentNode->v;
	adjListNode *current = graph->array[v]->Head; 
	while(current->edge!= -1){
		int currentEdge = current->edge;
		int edgeLength = EdgeCost(currentEdge);
		int neighborVertex = Eend[currentEdge];
		Node *neighborNode =  heap->pos[neighborVertex];
		int newDistance = edgeLength + (currentNode->dist);
		if (newDistance <= (neighborNode->dist)){
			decreaseKey(heap, neighborVertex, newDistance);
			neighborNode->prev = currentEdge;
			neighborNode->prevNode = currentNode;
		}
		current = current->Next;
	}
	
}

void print_heap(Heap *heap){
	int i;
	for (i=1; i<=heap->size; i++){
		printf("i: %d, Vertex: %d, pos: %d, dist: %d\n", i, data[i]->v, data[i]->pos, data[i]->dist);
	}
}

void backTrace (Node *node){
	Node *currentNode = node;
	int size = 0;
	int edges[MaxEdge];
	
	while(currentNode->prev != -1){
		edges[size] = (currentNode->prev);
		size++;	
		if(currentNode->prev != -1){
			currentNode = currentNode->prevNode;
		}
	}
	
	int i;
	for (i = size-1; i>=0; i--){
		PrintLeg(edges[i]);
	}
	
	
}



	


/***************************************************************************************/
/*Dijkstra Algorithm                                                                   */
/*DijkstraFlag=1 to supress output when Dijkstra is called from tour code.)            */
/***************************************************************************************/

void Dijkstra(int DijkstraFlag) {

	Heap heap = init_Heap();
	Graph graph = init_graph();
	Node *nextNode;
	decreaseKey(&heap, Begin, 0);
	nextNode = extractMin(&heap);
	
	if (!DijkstraFlag){
		while (nextNode->dist != InfiniteCost &&  nextNode->v != Finish) {
			decreaseKeys(&heap, &graph, nextNode);
			nextNode = extractMin(&heap);
		}
		if (nextNode->dist != InfiniteCost){
			backTrace(nextNode);
		}
	}
	
	
	
	
	
	
	
}


/***************************************************************************************/
/*CAMPUS TOUR                                                                          */
/***************************************************************************************/
#include "Tour.h"


/***************************************************************************************/
/*MAIN PROGRAM (don't modify)                                                          */
/***************************************************************************************/
int main(int argc, char *argv[]) {

int DEBUG = 0;
if (argc > 1) {
  DEBUG =  1;
}

if (DEBUG){
	Heap heap = init_Heap();
    Graph graph = init_graph();
	print_graph(&graph);
	print_heap(&heap);
	
	
}


else{ 
	GetVertices();
	GetEdges();
	while (GetRequest()) {RouteOpen(); TourFlag ? Tour() : Dijkstra(0); RouteClose();}
	return(0);
}
}