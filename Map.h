/***************************************************************************************/
/*Map Parameters And Data                                                              */
/***************************************************************************************/

/*File names*/
#define FileVertices            "MapDataVertices.txt"      /*Map vertex data.*/
#define FileEdges               "MapDataEdges.txt"         /*Map edge data.*/
#define FilePathEdges           "RouteEdges.txt"           /*Route edges text file.*/
#define FilePath                "Route.txt"                /*Route edges pixel coords.*/
#define FilePathCropped         "RouteCropped.txt"         /*Route edges cropped pixel coords.*/
#define FilePathVertices        "RouteVertices.txt"        /*Route vertices pixel coords.*/
#define FilePathVerticesCropped "RouteVerticesCropped.txt" /*Route vertices cropped pixel coords.*/

/*Limits*/
#define MaxString 100      /*Maximum length of any input string.*/
#define MaxLabel 5         /*Maximum length of a location label (includes ending \0).*/
#define MaxVertex 175      /*Maximum number of vertices.*/
#define MinVertex 5        /*Smallest number of a vertex that is real (not 0 or a map corner).*/
#define MaxEdge 600        /*Maximum number of edges.*/
#define MinEdge 20         /*Smallest number of an edge that is real (not to 0 or a map corner).*/
#define MaxArray 600       /*The maximum of MaxVertex and MaxEdge.*/
#define UndefinedIndex -1  /*All array indices start at 0.*/
#define InfiniteCost 10000 /*Cost of an edge that does not exist.*/

/*Vertex data.*/
int  nV=0;                        /*Number of vertices.*/
int  Vindex[MaxVertex];           /*Vertex index.*/
char Vlabel[MaxVertex][MaxLabel]; /*Vertex label - each at most 3 chars plus the \0.*/
int  VX[MaxVertex];               /*Vertex X-coord. in feet from upper left corner.*/
int  VY[MaxVertex];               /*Vertex Y-coord. in feet from upper left corner.*/
char *Vname[MaxVertex];           /*Vertex name - pointer to an allocated string.*/

/*Edge data.*/
int  nE=0;             /*Number of edges.*/
int  Eindex[MaxEdge];  /*Edge index.*/
int  Estart[MaxEdge];  /*Edge start (a vertex number).*/
int  Eend[MaxEdge];    /*Edge end (a vertex number).*/
int  Elength[MaxEdge]; /*Edge length in feet.*/
int  Eangle[MaxEdge];  /*Edge angle in degrees clockwise from north.*/
char Edir[MaxEdge][6]; /*Edge direction - one of North, NE, East, ..., NW*/
char Ecode[MaxEdge];   /*Edge code - f/F, u/U, d/D, s/t (steps up/down), b (bridge)*/
char *Ename[MaxEdge];  /*Edge name - pointer to an allocated string.*/

/*Variable that are set during user input.*/
int TourFlag;      /*1 if computing a tour instead of a route.*/
int BoardFlag;     /*1 if have skateboard*/
int TimeFlag;      /*1 to minimize time instead of distance*/
int Begin, Finish; /*Vertex indices of begin and finish locations.*/
