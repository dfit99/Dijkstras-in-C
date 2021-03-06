/*Daniel Fiterman 12/12/15*/
/*Brandeis Map - Tour .h file*/

//Utility function to get the reverse edge
int getBackEdge(Graph *graph, int front, int back){
	adjListNode *current = graph->array[front]->Head; 
	while(current->edge!= -1){
		int currentEdge = current->edge;
		int neighborVertex = Eend[currentEdge];
		if (neighborVertex == back){
			return currentEdge;
		}
		current = current->Next;
	}
	return -1;
}

//After creating MST, do a post order traversal
void traverseTree(Node *node, Graph *graph){
	int i;
	int size = node->childSize;
	
	for (i=0; i<=size; i++){
		PrintLeg(node->children[i]->prev);
		traverseTree((node->children[i]), graph);
		int backEdge = getBackEdge(graph, node->children[i]->v, node->v); //get backEdge to print after recursing back up
		PrintLeg(backEdge);
	}
}

// Uses prim's algorithm to generate the MST
void makeTree(Heap *heap, Graph *graph, Node* currentNode){
	int v = currentNode->v;
	adjListNode *current = graph->array[v]->Head; 
	
	while(current->edge!= -1){
		int currentEdge = current->edge;
		int neighborVertex = Eend[currentEdge];
		int edgeLength = EdgeCost(currentEdge);
		Node *neighborNode =  heap->pos[neighborVertex];
		int newDistance = edgeLength;
		if (neighborNode->pos <= (heap->size) && newDistance <= (neighborNode->dist)){
			decreaseKey(heap, neighborVertex, newDistance);
			neighborNode->prev = currentEdge;
			neighborNode->prevNode = currentNode;
		}
		current = current->Next;
	}
}
	
void Tour () {
	Heap heap = init_Heap();
	Graph graph = init_graph();
	Node *nextNode;
	decreaseKey(&heap, Begin, 0);
	nextNode = extractMin(&heap);
	Node *startNode = nextNode;
	
	while(heap.size!=1){
		if(nextNode != startNode && nextNode->prev != -1){
			nextNode->prevNode->childSize++;
			nextNode->prevNode->children[nextNode->prevNode->childSize] = nextNode;
			}
		
		if(nextNode != startNode && nextNode->prev == -1){
			break;
		}
	
		makeTree(&heap, &graph, nextNode);
	    nextNode = extractMin(&heap);
	}
	traverseTree(startNode, &graph);
}