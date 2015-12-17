/*************************************************************************************/
/*Convert a string to lower case.                                                    */
/*************************************************************************************/
void StringToLowerCase(char s[]) {
int i;
for (i=0; (s[i]!='\0'); i++) if ( (s[i]>='A') && (s[i]<='Z') ) s[i] = s[i] - 'A' + 'a';
}


/*************************************************************************************/
/*Return the index of a location, or -1 if can't identify, -2 if ambiguous.          */
/*Matching is not case sensitive, ending \n character is ignored.                    */
/*Uses simple linear search.                                                         */
/*Suceeds on a match to a vertex label or a match to a substring of a vertex name    */
/*   (in the later case, vertex name must be longer than MaxLabel becasue otherwise  */ 
/*   a string that short is more likely to be an undefined label).                    */
/*************************************************************************************/
int INDEX(char *s) {
int count=0, match=-1, i;
char x[MaxString], y[MaxString];
strcpy(x,s);
if (x[i=(strlen(x)-1)]=='\n') x[i] = '\0'; /*Remove \n.*/
StringToLowerCase(x);
if ( strcmp(x,"l37")==0 || strcmp(x,"l38")==0 || strcmp(x,"l39")==0 ) x[2]='6'; /*Residence halls off map = L36.*/
for (i=0; i<nV; i++) {
   strcpy(y,Vlabel[i]); StringToLowerCase(y); if (strcmp(x,y)==0) return(i);
   strcpy(y,Vname[i]); StringToLowerCase(y); if (strcmp(x,y)==0) return(i);
   }
for (i=0; i<nV; i++) {
   strcpy(y,Vname[i]);
   StringToLowerCase(y);
   if ( (strlen(y)>MaxLabel) && (strstr(y,x)!=NULL) ) {
      if (count++) {match = -2; return(match);}
      match = i;
      }
   }
return(match);
}



/*************************************************************************************/
/*Get the user request.                                                              */
/*************************************************************************************/
int GetRequest() {
char s[MaxString];

printf("\n\n************* WELCOME TO THE BRANDEIS MAP *************\n");

/*Get start location.*/
BEGIN: printf("Enter start (return to quit): "); fflush(stdout); fgets(s,MaxString,stdin);
   if (s[0]=='\n') return(0);
   Begin = INDEX(s);
   if (Begin == -1) {printf("*** Cannot identify location, please enter another description.\n"); goto BEGIN;}
   if (Begin == -2) {printf("*** Ambiguous, please enter a longer description.\n"); goto BEGIN;}

/*Get finish location.*/
FINISH: printf("Enter finish (or return to do a tour): "); fflush(stdout); fgets(s,MaxString,stdin);
if (s[0]=='\n') {Finish=UndefinedIndex; TourFlag=1;}
else {
   TourFlag=0;
   Finish = INDEX(s);
   if (Finish == -1) {printf("*** Cannot identify location, please enter another description.\n"); goto FINISH;}
   if (Finish == -2) {printf("*** Ambiguous, please enter a longer description.\n"); goto FINISH;}
   if (Begin == Finish) {printf("*** Finish same as begin; you are already there!\n"); goto FINISH;}
   }

/*Get BoardFlag.*/
printf("Have a skateboard (y/n - default=n)? "); fflush(stdout); fgets(s,MaxString,stdin);
BoardFlag = ( (s[0]=='y') || (s[0]=='Y') || (strncmp(s,"yes",3)==0) || (strncmp(s,"Yes",3)==0) ) ? 1 : 0;

/*Get TimeFlag.*/
printf("Minimize time (y/n - default=n)? "); fflush(stdout); fgets(s,MaxString,stdin);
TimeFlag = ( (s[0]=='y') || (s[0]=='Y') || (strncmp(s,"yes",3)==0) || (strncmp(s,"Yes",3)==0) ) ? 1 : 0;

/*Return 1 to indicate successful input.*/
return(1);
}
