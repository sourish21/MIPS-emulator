
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<math.h>

FILE *mycode;

void bintodec(int *icount, char count[], int size);
void negbintodec(int *icount, char count[], int size);
void dectobin(char *acount, int count, int size);



//program counter
char pc[33];
int progc=0;
int hmm=0;


//Memory & register blocks
char memory[8][500]={NULL};
char reg_file[32][33]={NULL};

//intermediate registers
char instruction_reg[33]={NULL};
char mem_data_reg[33]={NULL};
char value_a[33]={NULL};
char value_b[33]={NULL};
char alu_output[33]={NULL};


//read in machine code
void readtxt(char (*input)[500]);


//the 5 stages
void fetch(char *reg, int *counter, char *pcount); // Get the instruction from memory and increment pc by 4.
void decode(char *a, char *b, char *alu, char reg[],char progcount[], int *control);  // Get values of rs and rt and store it in register file and then  a,b and compute branch adress
void execute(char *alu, char *counter,char imm[], int instuc_type, int insert); //calculates funct and i/r instructions
void memory_access(char (*reg)[33], char (*mem)[500], char *mdr, int type); //R type completion  & storing mdr value into register  
void alu_operation(int *type);



void main(){
   int q=0;
   int w=0;
   for(q=0;q<32;q++){
      value_a[q] ='0';
      value_b[q]='0';
      pc[q]='0';
      instruction_reg[q] ='0';
      mem_data_reg[q]='0';
      alu_output[q]='0';

      for(w=0;w<32;w++){
         reg_file[w][q]='0';
      }
   }
   readtxt(memory);
   int opVal=0;
   char immediate[17]={NULL};
   int funct=0;

   while(opVal != 63){
      opVal =0;
      printf("\n----------------------------\n");
      //printf("FETCH:\n");
      fetch(instruction_reg,&progc,pc);
      if(progc>32){
        break;
      }
      //printf("1-%s |%d\n",instruction_reg,strlen(instruction_reg));
      //printf("DECODE\n");
      decode(value_a,value_b,alu_output,instruction_reg,pc,&opVal);
      //printf("opVal is %d\n",opVal);
      int g=0;
      //printf("IMMEDIATE:\n");
      for(g=0;g<16;g++){
        immediate[g] = instruction_reg[g+16];
        //printf("%c",immediate[g]);
      }
      //printf("\ncheck\n");

      alu_operation(&funct);
      execute(alu_output,pc,immediate,opVal,funct);
      memory_access(reg_file,memory,mem_data_reg,opVal);
   } 
   printf("\n");

   int v=0;
   int z=0;
   for(v=0;v<500;v++){
      for(z=0;z<8;z++){
         printf("%c",memory[z][v]);
      }
      printf("--->%d\n",v);
   }

}

// Read file and save incd memory array
void readtxt(char (*input)[500]){
   int i =0;
   int j =0;
   mycode =fopen("mcode.txt","r");
   while((input[i][j]=fgetc(mycode)) != EOF){
   //printf("%c",*(input+i));
   i++;
   if(i==8){
       j++;
       i=0;
   }
   }
   fclose(mycode);
}

void bintodec(int *icount, char count[], int size){
   *(icount) =0;
   int i=0;
   int k =size-1;
   //printf("size: %d\n",size);
   int w=0;
   
   for(i=k;i>=0;i--){
      if (count[i]== '1')
      {
        //printf("enters-1 : %c -%d",count[i],i);
        *(icount)=*(icount)+pow(2,(k-i));
         w= pow(2,(k-i));
        //printf("count: %d\n",*(icount));

      }
      else{
        //printf("enters-2 : %c- %d",count[i],i);
        *(icount) = *(icount) + 0;
        //printf("count: %d\n",*(icount));
      }
   }
   //printf("\n");

}

void negbintodec(int *icount, char count[], int size){
    int i=0; 
    for(i=size-1;i>=0;i--){
        if(i==0){
            //printf("char: %c-->",count[0]);
            *(icount) = - *(icount) ;
            //printf("count: %d\n",*(icount));

        }
        else if (count[i]== '1' && i!=0)
        {
            //printf("char: %c-->",count[i]);
            //printf("enters-1 : %c -%d",count[i],i);
            *(icount)=*(icount)+pow(2,(size-1-i));
            //printf("count: %d\n",*(icount));

        }
        else{
            //printf("char: %c-->",count[i]);
            //printf("enters-2 : %c- %d",count[i],i);
            *(icount) = *(icount) + 0;
            //printf("count: %d\n",*(icount));
        }
   }    
    
}
void dectobin(char *acount, int count, int size){

   int k = count;
   int i=0;
   int w;
   int f =size;
   char temp[32] ={NULL};
   int arrsize=0;

   while(i<size){
       
       //printf("%d %d\n",i,k);
       if((k%2)==0){
        temp[i] = '0';
        //printf("char:%c--> %d|| ",temp[i],i);
       
      }
      else{
        temp[i] = '1';
        //printf("cgar:%c--> %d|| \n",temp[i],i);
      }
       k= k/2;
       i++;

    }
   //printf(("\n"));
   for(i=0;i<size;i++){
       *(acount+i)=temp[size-i-1];
      // printf("%c-size: %d\n",*(acount+i),i);
   }
   //printf("\n%s\n",*(acount));
   //printf("\n");
}

//FETCH PHASE
void fetch(char *reg, int *counter, char *pcount){
   printf("INSTRUCTION:FETCH\n");
   int p = *counter;
   bintodec(&p, pcount,32);
   //printf("program counter: %d\n",p);
   int i=0;
   int j=0;
   int k=0;
   int b=0;

   for(i=p;i<p+4;i++){
      for (j=0; j < 8; j++)
      { 
        k=(b*8)+j;
        *(reg+k) = memory[j][i];
         
      }
      b++;
   }

   p=p +4;
   hmm++;
   //printf("\n converting pc into char val\n");
   dectobin(pcount,p,32);
   //printf("counter: %s with size %d\n",pcount,strlen(pcount));
   printf("\n\ncount: %d\n\n",hmm);
}

//DECODE PHASE 

void decode(char *a, char *b, char *alu, char reg[],char progcount[], int *control){
    printf("INSTRUCTION: DECODE\n");
   // storing in a and b the values
   // calculating branch address 

   //  get addrees value fromIR--> REGISTER FILE & transfer stored value----> A & B BUFFER

   char address1[6]={NULL};
   int addr1=0;
   char address2[6]={NULL};
   int addr2=0;

   int i=0;
   for(i=0;i<5;i++){
      address1[i]= reg[i+6];
      address2[i] = reg[i+11];
     // printf("%c %c\n",reg[i+6],reg[i+11]);
      //printf(" addr1:%c & addr2:%c\n",address1[i],address2[i]);
   }
   //printf("address1: %s----- address2: %s\n",address1,address2);
   //printf("Converting addresses into integer:\n\n");
   bintodec(&addr1,address1,5);
   //printf("value of read address 1: %d\n",addr1);
   bintodec(&addr2,address2,5);
   //printf("value of read address 2: %d\n",addr2);
    
   i=0;
   //printf("\nSaving values into alu\n\n");
   for(i=0;i<32;i++){
      *(a+i) = reg_file[i][addr1];
      *(b+i) = reg_file[i][addr2];
      //printf("values saved in a:%c & b:%c -----%d\n ",*(a+i), *(b+i),i);
   }
   //printf("REG A: %s  && REG B: %s\n",a,b);

   //printf("--------------------------------\n Testing branch immediate:\n");

 // alu=PC + immediate (sign-extend(IR[15-0]<<2)) to do FOR BEQ
    
   int c=0;
   char imm[16]= {NULL};
   int imm1=0;
   int pec=0;
   for(c=0;c<16;c++){
    imm[c] = reg[c+16];
    printf("%c",imm[c]);
   }
   //printf("\n");
   //printf("length of progcount:%d\n",32);
   bintodec(&pec,progcount,32);
   //printf("length of immm:%d\n",strlen(imm));
   //printf("\n");
   //change to negative values
   negbintodec(&imm1,imm,16);
   printf("\n%d+%d\n",pec,imm1);
   pec = pec + imm1;
   printf("\nprogram counter value for branch is: %d\n",pec);
   dectobin(alu,pec,32);
   printf("alu is now: %s\n",alu);

   //Get alu opcode
    char op[7]={NULL};
    int f=0;
    for(f=0;f<6;f++){
        op[f] = instruction_reg[f];
        //printf("%c",op[f]);   
    }
    bintodec(control,op,6);
    //printf("value of control is : %d\n",*control);

}
//EXECUTE PHASE

void execute(char *alu, char *counter,char imm[], int instuc_type, int insert){

   int intvalA=0;
   int intvalB=0;
   int z=0;
   char target[27];
   //printf("type is: %d\n",instuc_type);

   //R-type add & subtract
   if (instuc_type==0 )
   { 
      //printf("function value is: %d\n",insert);
      
      if(insert== 32){
      printf("INSTRUCTION: Execute->ADD\n");
      //add
      //printf("add\n");
      bintodec(&intvalA,value_a,32);
      //printf("value A is: %d\n",intvalA);
      bintodec(&intvalB,value_b,32);
      //printf("value B is : %d\n",intvalB);
      intvalA = intvalA +intvalB;
      //printf("addition value is: %d",intvalA);
      dectobin(alu,intvalA,32);
      }
      else if(insert = 34 ){
      //sub
      //printf("sub\n");
      bintodec(&intvalA,value_a,32);
      //printf("value A is: %d\n",intvalA);
      bintodec(&intvalB,value_b,32);
      //printf("value B is : %d\n",intvalB);
      intvalA = intvalA - intvalB;
      //printf("subtration value is: %d",intvalA);
      dectobin(alu,intvalA,32);
      }
      else{
         //printf("oof\n");
      }
    }

   else if(instuc_type == 2){
      //JUMP EXECUTION TO DO ::  PC=JumpAddr :: strcpy
      //printf("jump:\n");
      int i=0;
      for(i=0;i<26;i++){
         target[i] = instruction_reg[i+6];
         //printf("%c",target[i]);
      }
      //printf("target is : %s\n",target);
      bintodec(&progc,target,26);
      //printf("Instruction: Execute->JUMP to %d\n",progc);
      //printf("counter is now set to : %d\n",progc);
      dectobin(pc,progc,32);
   }

    else if (instuc_type == 5){
        //printf("Instruction: Execute->BNE\n");
        bintodec(&intvalA,value_a,32);
        //printf("value 1:%d\n",intvalA);
        bintodec(&intvalB,value_b,32);
        //printf("value 2:%d\n",intvalB);

        if(intvalA!=intvalB){
            strncpy(counter,alu,32);
            //printf("Instruction: BNE to %s\n",counter);
            //printf("counter is now: %s\n",pc);
        }
        else{
            //printf("equal hence no branching\n");
        }
    }

    //I type
   else  {
    printf("Instrcution: Execute: I type\n");
      char immediate[16]={NULL};
      //addi// lw//sw calculations
      //printf("I-type\n");
      for(z=0;z<16;z++){
        immediate[z] = instruction_reg[z+16];
      }
      //printf("register a: %s | size%d\n",value_a,strlen(value_a));
      //printf("register b: %s| size %d\n",value_b,strlen(value_b));
      bintodec(&intvalA,value_a,32);
      //printf("VALUE 1: %d\n",intvalA);
      bintodec(&intvalB,immediate,16);
      //printf("VALUE 2: %d\n",intvalB);
      intvalA =intvalA + intvalB;
      //printf("result-->%d\n",intvalA);
      dectobin(alu,intvalA,32);
 
    }
}

//MEMORY AND WRITE BACKS PHASES
void memory_access(char (*reg)[33], char (*mem)[500], char *mdr, int type){
   // REG[IR[15-11]]= ALUout  --R-TYPE COMPLETION
   int regval=0;
   int i=0;
   if(type == 0 || type == 8){
      //printf("R-TYPE COMPLETION\n");
      int b=0;
      char dest[6]={NULL};
      if(type == 0){
        printf("Instruction: Memory writeback -R type completion\n");
        for(b=0;b<5;b++){
        dest[b]=instruction_reg[b+16];
        } 
      }
      else{
        printf("Instruction: Memory writeback - addi completion\n");
        for(b=0;b<5;b++){
        dest[b]=instruction_reg[b+11];
        }

      }
      bintodec(&regval,dest,5);
      for(i=0;i<32;i++){
        reg[i][regval]=alu_output[i];
        //printf("%c", reg[i][regval]);
      }
      //printf("\nstored in: %d\n",regval);

   }
   
   else if(type == 43){
    //SW: MEMORY[ALUOUT] =B;
      printf("Instruction: Memory access->STORE WORD\n");
      int final_value =0;

      bintodec(&regval,alu_output,32);
      int h=0;
      int s=0;
      int o=0;
      for(h=0;h<4;h++){
         for(s=0;s<8;s++){
            o = (h*8)+s;
            mem[s][h+regval+40] = value_b[o];
            //printf("%c",mem[s][h+regval+40]);
         }
      }
      bintodec(&final_value,value_b,32); 
      printf("\nSquare val: %d\n",final_value);  
      printf("\n stored in %d\n",(regval/4));
    }

   else if(type == 35){
    //LW :MDR = MEMORY[ALUOUT] &//LW: REG[IR[20-16]]=MDR(this is write back phase)
      printf("Instruction: Memory access-> LOAD WORD\n");
      int addr=0;
      int x=0;
      int y=0;
      bintodec(&addr,alu_output,32);
      //printf("access address : %d\n",addr);
      for(x=0;x<8;x++){
         for(y=0;y<8;y++){         
            mdr[(x*8)+y] = mem[y][x+addr+40];
            //printf("%c",mdr[(x*8)+y]);
         }
      }
       //Write back phase || LW: REG[IR[20-16]]=MDR(this is write back phase)
      printf("Instrcution: Write back\n");
      int counter=0;
      int addr2=0;
      char addres[5];
      int xc=0;
      //printf("destination address: ");
      for(xc=0;xc<5;xc++){
         addres[xc] = instruction_reg[xc+11];
         //printf("%c",addres[xc]);
      }
      //printf("\n");
         //printf("\n");
      bintodec(&addr2,addres,5);
      //printf("stored in %d\n",addr2);
      for(counter=0;counter<32;counter++){
         reg[counter][addr2]= mdr[counter];
            //printf("%c",reg[counter][addr2]);
      }
   }
   else{}

}

void alu_operation(int *type){
    //100000--- add
    //100010---sub
   int k=0;
   char f[7];
   for(k=0;k<6;k++){
      f[k] = instruction_reg[k+26];
   }
   //printf("\n");
   bintodec(type,f,6);
}