/*NAME AND DATE GOES HERE.*/
/*Brandeis Map - Tour .h file*/



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
	print_heap(&heap);
	traverseTree(startNode);
}

void traverseTree(Node *node){
	int i;
	printf("here1");
	int size = node->childSize;
		printf("size: %d", size);
	for (i=0; i<=size; i++){
		PrintLeg(node->children[i]->prev);
		printf("\n %d \n", node->v);
		traverseTree((node->children[i]));
		printf("\n %d \n", node->v);
		PrintLeg((node->children[i]->prev));
	}
	printf("here3");
	return;
}
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
