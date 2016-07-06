#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Opcode               //This node is used for Hashing using Chaining
{
        char name[10];
        char code[35];
        char format[5];
        /**
        rri = reg reg imm. addr.
        rrr = reg reg reg
        ri  = reg imm.
        rr  = reg reg
        a   = add
        z   = zero
        */
        struct Opcode *next;
};
struct Symbol               //Symbol Table is made using Linked List to save space
{
    char name[50];
    int add;
    struct Symbol *next;
};
typedef struct Opcode Opcode;
typedef struct Symbol Symbol;

Symbol *head=NULL;

Opcode* hash_table[13] = {NULL};
void reverseArray(int arr[], int start, int end)
{
    int temp;
    while (start < end)
    {
        temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}
int* conBin(int num)
{
    int t;
    int i, j;
    int *bin;
    bin=(int*)malloc(10*sizeof(int));
    for(i=0; i<10; i++)
    {
        bin[i]=0;
    }
    i=9;
    t = num;
    while(t!=0)
    {
         bin[i--]= t % 2;
         t = t / 2;
    }
    return bin;
}

char* convertTo5BitBinaryString(int decimal)    //This decimal is between 0 and 31
{
     printf("bitbinary function receives %d\n",decimal);
     char *str = (char *)malloc(5*sizeof(char));
     int d[5]={0};
     int i=0,j=0;
     while(decimal>0)
    {
       d[i]=decimal%2;
       i++;
       decimal=decimal/2;
    }
     int size = i;
     int k=0;
   /*   for(j=i-1;j>=0;j--,k++)
      {
            mac[k] = d[j];
      }*/
      int s=0;

      reverseArray(d,0,4);
      for(s=0;s<5;s++)
      {
        printf("%d",d[s]);
        str[s] = d[s] + '0';
      }
      printf("\n");

     printf("%s",str);
     return str;

}
/*******************************************************************
    HASH TABLE IS USED TO STORE THE OPCODES BEING READ
*******************************************************************/
int getHashIndex(char name[])
{
    int sum=0,i=0;
    while(name[i]!='\0')
    {
        sum+=name[i++];
    }
    return sum%13;
}
void insertAtIndex(Opcode *Node,int index)
{
    if(hash_table[index] == NULL)
    {
        hash_table[index] = Node;
        Node->next = NULL;
    }
    else
    {
        Opcode* temp = hash_table[index];
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = Node;
        Node->next=NULL;
    }
}
void insertIntoHashMap(Opcode *Node)
{
    int index = getHashIndex(Node->name);
    insertAtIndex(Node,index);
}
int *getAddressCode(char* temp)
{
    Symbol * t = head;
    int * val;
    int num;
    while(t != NULL)
    {
        if(!strcmp(temp,t->name))
        {
            num = t->add;
            break;
        }
        t = t->next;
    }
    val = conBin(num);
    return val;
}
char * getRegisterCode(char* temp)
{
    char *s;
    if (strcmp(temp,"R0") == 0)
    {
        s = "00000";
    }
    else if (strcmp(temp,"R1") == 0)
        s = "00001";
    else if (strcmp(temp,"R2") == 0)
        s = "00010";
    else if (strcmp(temp,"R3") == 0)
        s = "00011";
    else if (strcmp(temp,"R4") == 0)
        s = "00100";
    else if (strcmp(temp,"R5") == 0)
        s = "00101";
    else if (strcmp(temp,"R6") == 0)
        s = "00110";
    else if (strcmp(temp,"R7") == 0)
        s = "00111";
    else if (strcmp(temp,"R8") == 0)
        s = "01000";
    else if (strcmp(temp,"R9") == 0)
        s = "01001";
    else if (strcmp(temp,"R10") == 0)
        s = "01010";
    else if (strcmp(temp,"R11") == 0)
        s = "01011";
    else if (strcmp(temp,"R12") == 0)
        s = "01100";
    else if (strcmp(temp,"R13") == 0)
        s = "01101";
    else if (strcmp(temp,"R14") == 0)
        s = "01110";
    else if (strcmp(temp,"R15") == 0)
        s = "01111";
    else if (strcmp(temp,"A1") == 0)
        s = "10000";
    else if (strcmp(temp,"A2") == 0)
        s = "10001";
    else if (strcmp(temp,"A3") == 0)
        s = "10010";
    else if (strcmp(temp,"A4") == 0)
        s = "10011";
    else if (strcmp(temp,"port0") == 0)
        s = "10100";
    else if (strcmp(temp,"port1") == 0)
        s = "10101";
/*******************************************************************
    EXTRA REGISTER ADDRESSES WHICH COULD BE USED IN THE FUTURE
*******************************************************************/
/*    else if (strcmp(temp,"R22") == 0)
        s = "10110";
    else if (strcmp(temp,"R23") == 0)
        s = "10111";
    else if (strcmp(temp,"R24") == 0)
        s = "11000";
    else if (strcmp(temp,"R25") == 0)
        s = "11001";
    else if (strcmp(temp,"R26") == 0)
        s = "1010";
    else if (strcmp(temp,"R27") == 0)
        s = "11011";
    else if (strcmp(temp,"R28") == 0)
        s = "11100";
    else if (strcmp(temp,"R29") == 0)
        s = "11101";
    else if (strcmp(temp,"R30") == 0)
        s = "11110";
    else if (strcmp(temp,"R31") == 0)
        s = "11111";
  */
    return s;
}
char *getConstantCode(int temp)
{
    return convertTo5BitBinaryString(temp);
}
Opcode* getOpcodeNode(char *op)
{
    Opcode* temp = NULL;
    int index = getHashIndex(op);
    if(hash_table[index] == NULL)
    {
        printf("Wrong Opcode");
        return NULL;
    }

    else
    {
        temp = hash_table[index];
        while(strcmp(temp->name,op)!=0 && temp!=NULL)
        {
            temp = temp->next;
        }
        if(temp == NULL)
        {
            printf("Opcode not found!");
            return NULL;
        }
        else
        {
            return temp;
        }
    }

}
char * getOpcodeFormat(Opcode* temp)
{
    return temp->format;
}


int main()
{
    FILE *input_opcode;
    FILE *output_machine_code;
    FILE *input_instructions;
    int ilc=0;  //Instruction Location Counter
    int base = 0;
    char c,c2,c3,temp;
    char opcode[100];
    char machine_code[100];
    char format[5];
  //  printf("Give the base address of the program");
    base = 0;
    input_opcode = fopen("input_opcode.txt","r+");  //input_opcode contains a list of opcodes followed by their format and mac.code
    if (input_opcode == NULL)
        printf("FILE OPENING PROBLEM");
    do
    {
        c = fscanf(input_opcode,"%s",opcode);//Assuming to get opcode as a string in opcode array
        c2= fscanf(input_opcode,"%s",machine_code);//Assuming to get a the integer as a string in machine_array array
        c3= fscanf(input_opcode,"%s",format);
        //now we will create node of each string
        Opcode* Node = malloc(sizeof(Opcode));

        strcpy(Node->name,opcode);
        //Name of the opcode is fed
        strcpy(Node->code,machine_code);
        //Machine code of the opcode is fed
        strcpy(Node->format,format);
        //Format of the opcode is fed
      //  printf("BEFORE INSERTING NAME:: %s ,CODE:: %s and format",Node->name,Node->code,Node->format);
        insertIntoHashMap(Node);


    }while(c!=EOF && c2!=EOF && c3!=EOF);
/*******************************************************************
        At this point we have a hash-map of Opcodes
*******************************************************************/

    printf("Hash-map Created Successfully!\n");
    /*TEST:: PRINTING HASHTABLE with hashcode*/
    int i=0;
    for(i =0;i<13;i++)
    {

        if(hash_table[i]!=NULL)
        {

            Opcode* temp = hash_table[i];

            while(temp!=NULL)
            {
               // printf("%d\n",g++); //Just to check whether the chaining is printed or not
                printf("NAME:: %s and CODE:: %s and format:: %s \n",temp->name,temp->code,temp->format);
                temp = temp->next;
            }

        }
    }
    printf("Now reading Opcodes and Converting them to machine codes\n");

    /*************************************************************************
    STRATEGY FOR CONVERTING Assembly to Machine Code
    1) Read 1st word
    2) Decide whether its an opcode or a label by checking the use of ':' operator
    3) If its an opcode, then use the hash table to get the corresponding format and machine code.
        We now know how many operands to expect and whether they are registers, immediate constants or addresses.
    4) The operands go to a specific functions designed to convert them to machine code and get printed in the file.

    **************************************************************************/
    input_instructions = fopen("input_instructions.txt","r+");
    output_machine_code = fopen("output_machine_code.txt","w+");
    char k;
    char op[100];


    /***********************************************************************************************/





    while ( fgets ( op, sizeof op, input_instructions ) != NULL ) /* read a line */
    {
         //printf("%d\n",count );
         //count++;
         //printf("%s\n",opc );
       //  fputs ( line, stdout ); /* write the line */

       int l=0;

       while(op[l+1]!='\0')
       {
        //  printf("%c",opc[l] );
        if(op[l]==':')   //Its a label
        {
          // printf("Label");
           printf("Label Found!");
           Symbol *t;
           Symbol *temp = malloc(sizeof(Symbol));
           int i=0;
           for(;i<l;i++)
                temp->name[i] = op[i];
            temp->name[i] = '\0';
            temp->add = ilc + 1 + base;
            temp->next = NULL;
           if(head == NULL)
                head = temp;
           else
           {
                t= head;
                while(t->next!=NULL)
                    t= t->next;
                t->next = temp;
           }
           //handle label
       }
       l++;
     }
     ilc++;
 }



 /*   do
    {
       k=fscanf(input_instructions,"%s",op);
       printf("WORD SCANNED IS %s  ",op);
            //check if opcode or label
       int l=0;
       while(op[l+1]!='\0')
       {
           l++;
       }
       if(op[l]==':')   //Its a label
       {
           printf("Label Found!");
           Symbol *t;
           Symbol *temp = malloc(sizeof(Symbol));
           int i=0;
           for(;i<l;i++)
                temp->name[i] = op[i];
            temp->name[i] = '\0';
            temp->add = ilc + 1 + base;
            temp->next = NULL;
           if(head == NULL)
                head = temp;
           else
           {
                t= head;
                while(t->next!=NULL)
                    t= t->next;
                t->next = temp;
           }
           //handle label

       }
       ilc++;
   }while(k!=EOF);*/
   fclose(input_instructions);
    /***********************************************************************************************/
   input_instructions = fopen("input_instructions.txt","r+");
   int * binary;
   int count;
    do
    {
       k=fscanf(input_instructions,"%s",op);
       printf("WORD SCANNED IS %s  ",op);
            /*check if opcode or label*/
       int l=0;
       while(op[l+1]!='\0')
       {
           l++;
       }
       if(op[l]==':')   //Its a label
       {
           //printf("Label Found!");
            fprintf(output_machine_code,"\n");
           //handle label
       }
       else
       {
           printf("Inside else\n");
           char temp[100];
           char temp2[100];
           char temp3[100];
           int temp4;
            //handle opcode and print corresponding machine code
            Opcode* current_node = getOpcodeNode(op);
            fprintf(output_machine_code,"%s",current_node->code);//print machine code of the opcode

            if (strcmp("z",getOpcodeFormat(current_node))==0)   //ZERO OPERAND INSTRUCTION
            {
                fprintf(output_machine_code,"\n");//Do nothing
            }
            else if(strcmp("r",getOpcodeFormat(current_node))==0)   //ONE OPERAND REGISTER OPERAND INSTRUCTION
            {
                k = fscanf(input_instructions,"%s",temp);

                fprintf(output_machine_code,"%s",getRegisterCode(temp));
                fprintf(output_machine_code,"\n");
            }
            else if(strcmp("a",getOpcodeFormat(current_node))==0)   //ONE OPERAND ADDRESS OPERAND INSTRUCTION
            {
                k = fscanf(input_instructions,"%s",temp);
                binary = getAddressCode(temp);
                for(count=0;count<10;count++)
                {
                    fprintf(output_machine_code,"%d",binary[count]);
                }
                fprintf(output_machine_code,"\n");
            }
            else if(strcmp("rr",getOpcodeFormat(current_node))==0)   //TWO OPERAND REGISTER REGISTER OPERAND INSTRUCTION
            {
                //printf("inside two");
                k = fscanf(input_instructions,"%s",temp);
                k = fscanf(input_instructions,"%s",temp2);
                fprintf(output_machine_code,"%s",getRegisterCode(temp));
                fprintf(output_machine_code,"%s",getRegisterCode(temp2));
                fprintf(output_machine_code,"\n");
            }
            else if(strcmp("ri",getOpcodeFormat(current_node))==0)   //TWO OPERAND REGISTER CONSTANT INSTRUCTION
            {
                k = fscanf(input_instructions,"%s",temp);
                k = fscanf(input_instructions,"%d",&temp4);
                fprintf(output_machine_code,"%s",getRegisterCode(temp));
              //  fprintf(output_machine_code,"%s",getConstantCode(temp4));
                binary = conBin(temp4);
                for(count=0;count<10;count++)
                {
                    fprintf(output_machine_code,"%d",binary[count]);
                }
                fprintf(output_machine_code,"\n");
            }
            else if(strcmp("rrr",getOpcodeFormat(current_node))==0)   //THREE OPERAND REGISTER-REGISTER-REGISTER INSTRUCTION
            {
                k = fscanf(input_instructions,"%s",temp);
                k = fscanf(input_instructions,"%s",temp2);
                k = fscanf(input_instructions,"%s",temp3);
                fprintf(output_machine_code,"%s",getRegisterCode(temp));
                fprintf(output_machine_code,"%s",getRegisterCode(temp2));
                fprintf(output_machine_code,"%s",getRegisterCode(temp3));
                fprintf(output_machine_code,"\n");
            }
            else if(strcmp("rri",getOpcodeFormat(current_node))==0)   //THREE OPERAND REGISTER-REGISTER-INTERMEDIATE INSTRUCTION
            {
                k = fscanf(input_instructions,"%s",temp);
                k = fscanf(input_instructions,"%s",temp2);
                k = fscanf(input_instructions,"%d",&temp4);
                fprintf(output_machine_code,"%s",getRegisterCode(temp));
                fprintf(output_machine_code,"%s",getRegisterCode(temp2));
                binary = conBin(temp4);
                for(count=0;count<10;count++)
                {
                    fprintf(output_machine_code,"%d",binary[count]);
                }
                fprintf(output_machine_code,"\n");
            }
       }

    }while(k!=EOF);
    printf("out\n");
    fclose(input_instructions);
    fclose(output_machine_code);
    fclose(input_opcode);

    /*PRINT SYMBOL TABLE*/
    Symbol *p;
    p=head;
    FILE *f = fopen("symbol_table.txt","w+");
    while(p!=NULL)
    {
        printf("%s :: ",p->name);
        fprintf(f,"%s :: ",p->name);
        printf("%d\n",p->add);
        fprintf(f,"%d\n",p->add);
        p = p->next;
    }
    return 0;
}
