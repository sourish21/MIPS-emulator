#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//DEfinitions for conversion
/*#define char *lw = strstr(txt, "lw");
#define char *sw = strstr(txt, "sw");
#define char *beq = strstr(txt, "beq");
#define char *jmp = strstr(txt, "jmp");*/


// registers and flip-flops required for multicycle
int instruction_mem[];
int instruction_register[32][32];
int registers[];
int mid1[];
int mid2[];
int aluVals[];

char txt[];
FILE *mycode;

//program counter
int pc=0;

void readtxt();
void returnMcode();

//the 5 stages
int fetch(int counter, int memory[]);
int decode();
int execute();
int mem();
int write_back();


int main(){
    int i=0;
    readtxt();
     printf("%s -1\n", txt[i]);

   

}




// Read file and conversion section

void readtxt(){
    mycode =fopen("");
    while(fgetc(mycode) != '\0'){
        txt = fgetc(mycode);
    }
    fclose(mycode);
}


/*void returnMcode(){
     int k=0;
     while(fgets(txt) != '\0'){
         printf("%s\n",fgets(txt));

         if(lw != NULL){
            instruction_mem[k]= 
            printf("load word\n");
         }
         else if(sw != NULL){
            instruction_mem[k]= 
         }
         else if(beq != NULL){
            instruction_mem[k]= 
         }
         else if(jmp != NULL){
            instruction_mem[k]= 
         }
         else{
            instruction_mem[k]= txt;
         }
         k++;
     }

}*/
