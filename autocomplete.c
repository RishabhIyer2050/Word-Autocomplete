#include "autocomplete.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

int compare (const void *arg1, const void *arg2)
{
       return(strcmp(((struct term *)arg1)->term, ((struct term *)arg2)->term));
}

void read_in_terms(struct term **terms, int *pnterms, char *filename){
    
    char line[200];
    FILE *fp; 
    fp = fopen(filename, "r");
    fgets(line, sizeof(line), fp);   
 
    *pnterms = atoi(line);
    *terms = (struct term *)malloc(sizeof(struct term)*((int)*pnterms));
    
    for(int i = 0; i<*pnterms; i++){
        fgets(line, sizeof(line), fp); 
        (*terms)[i].weight = atof(line);
        strcpy((*terms)[i].term, strchr(line, '\t')+1);

        if ((strchr((*terms)[i].term, '\n')) != NULL){
            *(strchr((*terms)[i].term, '\n')) = '\000';
        } 
    }   

    fclose(fp);

    qsort((*terms), *pnterms, sizeof(struct term), compare);
}
    
int lowest_match(struct term *terms, int nterms, char *substr){
    
    int start = 0;
    int stop = nterms;
    int index = nterms +1;

    while((stop-start)>=0){
        int mid = start+(stop-start)/2;

        if ((strstr((terms)[mid].term, substr) != NULL) && (strncmp(strstr((terms)[mid].term, substr), (terms)[mid].term, strlen(substr)) == 0)){
            if (mid<index){
                index = mid;
                stop = mid-1;}}
        else{
            if(strcmp(substr, (terms)[mid].term) < 0){
                stop = mid-1;}
            else{
                start = mid+1;}
        }
    }

    if (index==nterms+1){
        index = -1;}
    return index;   
}

int highest_match(struct term *terms, int nterms, char *substr){
    
    int start = 0;
    int stop = nterms;
    int index = -1;

    while((stop-start)>=0){
        int mid = start+(stop-start)/2;

        if ((strstr((terms)[mid].term, substr) != NULL) && (strncmp(strstr((terms)[mid].term, substr), (terms)[mid].term, strlen(substr)) == 0)){
            if (mid>index){
                index = mid;
                start = mid+1;}}
        else{
            if(strcmp(substr, (terms)[mid].term) < 0){
                stop = mid-1;}
                
            else{
                start = mid+1;}
        }
    }

    return index;   
}

int struct_cmp(const void *a, const void* b) {
    double l = ((struct term *)a)->weight; 
    double r = ((struct term *)b)->weight;  

    if (r == l)
        {return 0;}
    else if(r > l)
        {return 1;}
    else
        {return -1;}
    } 

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr){
    
    int low = lowest_match(terms, nterms, substr);
    int high = highest_match(terms, nterms, substr);

    if(high==-1 || low==-1){
        *n_answer = 0;
        *answer = (struct term *)malloc(sizeof(struct term)*((int)*n_answer));
    }
    else{
    *n_answer = (high-low)+1;

    *answer = (struct term *)malloc(sizeof(struct term)*((int)*n_answer));

    for(int i = 0; i<*n_answer; i++){
        strcpy((*answer)[i].term, (terms)[low + i].term);
        (*answer)[i].weight = (terms)[low + i].weight;
    }
    qsort((*answer), *n_answer, sizeof(struct term), struct_cmp);}
    }