/*************************************************************************************/
/*PATH OUTPUT PARAMETERS AND VARIABLES                                               */
/*************************************************************************************/

FILE *efile;  /*Path edges output file pointer (text file, one line per edge).*/
FILE *pfile;  /*Path output file pointer (coordinates in pixels).*/
FILE *pcfile; /*Path cropped output file pointer (coordinates in pixels).*/
FILE *vfile;  /*Path vertices output file pointer (coordinates in pixels).*/
FILE *vcfile; /*Path cropped vertices output file pointer (coordinates in pixels).*/

#define MapWidthFeet    5521 /*Width in feet of map.*/
#define MapHeightFeet   4369 /*Height in feet of map.*/
#define MapWidthPixels  2528 /*Width in pixels of map.*/
#define MapHeightPixels 2000 /*Height in pixels of map.*/
#define CropLeft         150 /*Pixels cropped from left of map.*/
#define CropDown         125 /*Pixels cropped from top of map.*/

int  TotalLegs;     /*Total legs travelled on current path.*/
int  TotalDistance; /*Total distance travelled on current path.*/
int  TotalTime;     /*Total time travelled on current path.*/

int Vcheck[MaxVertex]; /*Keeps track of which vertices have been output.*/



/*************************************************************************************/
/*OUTPUT EDGE - useful for debugging                                                 */
/*************************************************************************************/
void OutputEdge(int x) {
fprintf(efile,"%d - (%d,%d) [%s -> %s] %s\n",
   x,Estart[x],Eend[x],Vlabel[Estart[x]],Vlabel[Eend[x]],Ename[x]);
}


/*************************************************************************************/
/*OUTPUT EDGE PIXEL COORDINATES FOR BOTH MAP AND CROPPED MAP                         */
/*************************************************************************************/
void OutputEdgePixelCoordinates(int edge) {
int HeadX = VX[Estart[edge]];
int HeadY = VY[Estart[edge]];
int TailX = VX[Eend[edge]];
int TailY = VY[Eend[edge]];
fprintf(pfile,"%d %d %d %d\n",
   (int)( HeadX * ((double)MapHeightPixels / (double)MapHeightFeet) ),
   (int)( HeadY * ((double)MapWidthPixels  / (double)MapWidthFeet ) ),
   (int)( TailX * ((double)MapHeightPixels / (double)MapHeightFeet) ),
   (int)( TailY * ((double)MapWidthPixels  / (double)MapWidthFeet ) )
   );
fprintf(pcfile,"%d %d %d %d\n",
   (int)( ( HeadX * ((double)MapHeightPixels / (double)MapHeightFeet) ) - CropLeft),
   (int)( ( HeadY * ((double)MapWidthPixels  / (double)MapWidthFeet ) ) - CropDown),
   (int)( ( TailX * ((double)MapHeightPixels / (double)MapHeightFeet) ) - CropLeft),
   (int)( ( TailY * ((double)MapWidthPixels  / (double)MapWidthFeet ) ) - CropDown)
   );
}


/*************************************************************************************/
/*Make a distance string in feet or in miles.                                        */
/*************************************************************************************/
void MakeDistanceString(int feet, char *s) {
if (feet<5280) sprintf(s, "%d feet",feet);
else sprintf(s, "%.1f miles",((double)feet)/5280);
}


/*************************************************************************************/
/*Make a time string in seconds or minutes.                                          */
/*************************************************************************************/
void MakeTimeString(int seconds, char *s) {
if (seconds<60) sprintf(s, "%d seconds",seconds);
else sprintf(s, "%.1f minutes",((double)seconds)/60);
}


/*************************************************************************************/
/*INITIALIZE ROUTE OUTPUT                                                            */
/*************************************************************************************/
void RouteOpen() {
int i;
TotalLegs = TotalDistance = TotalTime = 0;
efile  = FileOpen(FilePathEdges,"w");
pfile  = FileOpen(FilePath,"w");
pcfile = FileOpen(FilePathCropped,"w");
vfile  = FileOpen(FilePathVertices,"w");
vcfile = FileOpen(FilePathVerticesCropped,"w");
for (i=0; i<MaxVertex; i++) Vcheck[i]=0;
}


/*************************************************************************************/
/*PRINT ONE LEG OF A ROUTE                                                           */
/*************************************************************************************/
void PrintLeg(int edge) {
int seconds = Time(edge);
static char t[MaxString];

/*Print the FROM line.*/
printf("\nFROM: (%s) %s\n",Vlabel[Estart[edge]],Vname[Estart[edge]]);

/*Print the ON line if edge name if defined.*/
if (strlen(Ename[edge])>2) printf("ON: %s\n",Ename[edge]);

/*Print the method of travel along this edge.*/
switch (Ecode[edge]) {
   case 'f': printf("%s","Walk"); break;
   case 'F': if (BoardFlag) printf("%s","Glide"); else printf("%s","walk"); break;
   case 'u': printf("%s","Walk up"); break;
   case 'U': if (BoardFlag) printf("%s","Board up"); else printf("%s","walk up"); break;
   case 'd': printf("%s","Walk down"); break;
   case 'D': if (BoardFlag) printf("%s","Coast down"); else printf("%s","walk down"); break;
   case 's': printf("%s","Climb up"); break;
   case 't': printf("%s","Go down"); break;
   case 'b': printf("%s","Walk"); break;
   }

/*Complete travel info line with distance, degrees, direction.*/
printf(" %d feet in direction %d degrees %s.\n",Elength[edge],Eangle[edge],Edir[edge]);

/*Print the TO line.*/
printf("TO: (%s) %s\n",Vlabel[Eend[edge]],Vname[Eend[edge]]);

/*Print the time line.*/
MakeTimeString(seconds,t);
printf("(");
if ( (BoardFlag) && (strchr("fdustb",Ecode[edge])!=NULL) ) printf("no skateboards allowed, ");
printf("%s)\n", t);

/*Output edge.*/
OutputEdge(edge);

/*Output edge coordinates.*/
OutputEdgePixelCoordinates(edge);

/*Update Summary Data*/
TotalLegs++;
TotalDistance += Elength[edge];
TotalTime += seconds;

}


/*************************************************************************************/
/*FINISH ROUTE OUTPUT                                                                */
/*************************************************************************************/
void RouteClose() {
static char d[MaxString], t[MaxString];
MakeDistanceString(TotalDistance,d);
MakeTimeString(TotalTime,t);
if (TotalLegs==0) printf("\n*** Sorry, could not find a route!\n");
else printf("\nlegs = %d, distance = %s, time = %s\n", TotalLegs, d, t);
fclose(efile);
fclose(pfile);
fclose(pcfile);
fclose(vfile);
fclose(vcfile);
}
