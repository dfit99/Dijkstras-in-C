/***************************************************************************************/
/*Functions to input the map data and compute time.                                    */
/***************************************************************************************/

#define DefaultEdgeCode 'F'

/*File pointers.*/
FILE *vfile; /*Vertex file.*/
FILE *efile; /*Edges file.*/

/*Open a file (use fclose to close a file).*/
/*   Arg 1: file name (PATH will be prepended to this name)*/
/*   Arg 2: "r" for read or "w" for write.*/
/*   Returns a file pointer.*/
FILE *FileOpen(char *filename, char *rw) {
FILE *filepointer;
char s[MaxString];
strcpy(s,PATH);
strcat(s,filename);
if ((filepointer = fopen(s,rw)) == NULL) {
     printf("Unable to open file: %s\n", s);
     exit(1);
     }
return(filepointer);
}

/*Remove last character of a a string s if it matches the character c.*/
void RemoveLastChar(char *s, char c) {
int lastchar = strlen(s)-1;
if (s[lastchar]==c) s[lastchar] = '\0';
}

/*Get vertex list.*/
void GetVertices() {
char s[MaxString];
char *ptr;
vfile = FileOpen(FileVertices,"r");
while (fgets(s,MaxString,vfile) != NULL) if ( (s[0]!='\n') && (s[0]!='/') ) {
   if (nV>=MaxVertex) {printf("ERROR: Max number of vertices exceeded.\n"); exit(1);}
   sscanf(s,"%d %s %d %d",&Vindex[nV],Vlabel[nV],&VX[nV],&VY[nV]);
   if ((Vname[nV] = malloc(strlen(s)+1)) == NULL) {printf("\nMemory allocation failed.\n"); exit(1);}
   ptr = strchr(s,'"');
   strcpy(Vname[nV],++ptr); /*start one character past leading "*/
   RemoveLastChar(Vname[nV],'\n');
   RemoveLastChar(Vname[nV],'"');
   nV++;
   }
fclose(vfile);
}

/*Get edge list (edge labels are skipped)*/
void GetEdges() {
char s[MaxString];
char CodeString[MaxString];
char *ptr;
efile = FileOpen(FileEdges,"r");
while (fgets(s,MaxString,efile) != NULL)  if ( (s[0]!='\n') && (s[0]!='/') ) {
   if (nE>=MaxEdge) {printf("ERROR: Max number of edges exceeded.\n"); exit(1);}
   sscanf(s,"%d %*s %*s %d %d %d %d %s %s",
      &Eindex[nE],&Estart[nE],&Eend[nE],&Elength[nE],&Eangle[nE],Edir[nE],CodeString);
   Ecode[nE] = CodeString[1];
   if (Ecode[nE]=='x') Ecode[nE] = DefaultEdgeCode;
   if ((Ename[nE] = malloc(strlen(s)+1)) == NULL) {printf("\nMemory allocation failed.\n"); exit(1);}
   ptr = strchr(s,'"');
   strcpy(Ename[nE],++ptr); /*start one character past leading "*/
   RemoveLastChar(Ename[nE],'\n');
   RemoveLastChar(Ename[nE],'"');
   nE++;
   }
fclose(efile);
}

/*Return the edge in the reverse direction.*/
int ReverseEdge(int edge) {
if (edge<0) {printf("\nERROR: Attempt to reverse negative edge.\n"); exit(1);}
if (edge>=nE) {printf("\nERROR: Attempt to reverse undefined edge.\n"); exit(1);}
if (edge==0) return(1);
if (edge==(nE-1)) return(nE-2);
if ( (Eend[edge]==Estart[edge+1]) && (Eend[edge+1]==Estart[edge]) ) return(edge+1);
if ( (Eend[edge]==Estart[edge-1]) && (Eend[edge-1]==Estart[edge]) ) return(edge-1);
{printf("\nERROR: A reverse edge is missing.\n"); exit(1);}
}


/***************************************************************************************/
/*Speeds (based on 3.1 mph average human walking speed according to Wikipedia).        */
/***************************************************************************************/
#define WalkSpeed 272    /*ft/min = (3.1 miles/hr) * (5280 ft/mile) / (60 mins/hr)*/
#define WalkFactorU 0.9  /*Multiply walk speed by this for walk up.*/
#define WalkFactorD 1.1  /*Multiply walk speed by this for walk down.*/
#define SkateFactorU 1.1 /*Multiply walk speed by this for skateboard up.*/
#define SkateFactorF 2.0 /*Multiply walk speed by this for skateboard flat.*/
#define SkateFactorD 5.0 /*Multiply walk speed by this for skateboard down.*/
#define StepFactorU 0.5  /*Multiply walk speed by this for walk up steps.*/
#define StepFactorD 0.9  /*Multiply walk speed by this for walk down steps.*/
#define BridgeFactor 1.0 /*Multiply walk speed by this for walking on a bridge.*/


/***************************************************************************************/
/*TIME (IN SECONDS) TO TRAVERSE AN EDGE i                                              */
/*   Note: This gives a reference for how roundoff is done when computing the seconds  */
/*   to traverse and edge, and because seconds are stored as an integer,               */
/*   results will be repeatable from one implementation to another of Dijkstra code.   */
/***************************************************************************************/
int Time(int i) {
int t = (int)( (60 * ( (double)Elength[i] / (double)WalkSpeed) ) + 0.5 );
switch (Ecode[i]) {
   case 'f':                                                break;
   case 'F': t /= (BoardFlag) ? SkateFactorF : 1;           break;
   case 'u': t /= WalkFactorU;                              break;
   case 'U': t /= (BoardFlag) ? SkateFactorU : WalkFactorU; break;
   case 'd': t /= WalkFactorD;                              break;
   case 'D': t /= (BoardFlag) ? SkateFactorD : WalkFactorD; break;
   case 's': t /= StepFactorU;                              break;
   case 't': t /= StepFactorD;                              break;
   case 'b': t /= BridgeFactor;                             break;
   }
return(t);
}
