#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/**
        Abstract Idea ::
                1)Using a Binary Trie to store all the opcodes and their corresponding Operations.
                We don't know the length of opcode since it could have any number of operands. Hence, doing
                try and error is not a good idea (Inefficient). We observe that decisions are made on reading each bit (depending
                on whether they are 0 or 1 so Binary Trie is the most efficient way possible(Sort of like decision tree)
                2)If we read a 0 => go to left node
                3)If we read a 1 => go to right node
                4)In the end(that is at leaf node), we get a string which is put into switch-case which executes the required command using
                        functions.
 */
int registers[16], fstatus=-2, fzero=0;

int *a1, *a2, *a3, *a4;

struct node {
    int bit; //bit is 0 if its a left child and 1 if its a right child
    int type; //type is 0 if node
    /*Node is capable to pointing to another node or another instruction*/
    struct instruction* i;
    struct node* left;
    struct node* right;
};
typedef struct node Node;

struct instruction {
    /*Instruction is always the leaf node in our data structure of a Binary Trie.
    It is used to get a string (name) that will be used to identify the opcode*/
    int type; //type is 1 if its an instruction
    char name[10];
    char machine_code[100];
    char format[5];
};
typedef struct instruction Ins;

Node* getNewNode(int bit) {
    Node* temp = (Node *) malloc(sizeof (Node));
    temp->type = 0; //To show that its an Inner Node and not a leaf Node
    temp->bit = bit; // Shows whether its a left(0) child OR a right(1) child of its parent node
    temp->i = NULL;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}
Node* head = NULL;

void insertIntoTrie(Ins* temp) //Build a Trie branch the bits after the 8th bit and put the Instruction in it
{
    if (head == NULL)
        head = getNewNode(0); //8th bit is always 0 (becomes head)
    char *name = (char *) malloc(100 * sizeof (char));
    char *mac = (char *) malloc(100 * sizeof (char));

    strcpy(name, temp->name);
    strcpy(mac, temp->machine_code);

    printf("Instruction name is = %s\n", name);
    printf("Machine code is = %s\n", mac);
    /*Now mac contains the machine code which will be used to create node branches for inserting Ins*/
    Node* t = head;
    int i = 0;

    printf("Inserting::       ");
    while (mac[i] != '\0') {
        //int value = mac[i] - '0';  // Convert character into number (0 or 1)

        printf("%c", mac[i]);
        if (mac[i] == '0' && t->left == NULL) {
            t->left = getNewNode(0);
            printf("New left Node Created");
            t = t->left;
        } else if (mac[i] == '0' && t->left != NULL) {
            t = t->left; // Just go left
        } else if (mac[i] == '1' && t->right == NULL) {
            t->right = getNewNode(1);
            printf("New right Node Created");
            t = t->right;
        } else if (mac[i] == '1' && t->right != NULL) {
            t = t->right; // Just go right
        } else
            printf("Conversion Problem, value found is = %c\n", mac[i]);

        i++;
    }
    printf("\n\n");
    t->i = temp; // Store the Instruction @ the root
}

Ins* getInstruction(char* mc) {
    Node* temp = head;
    int i = 0;

    while (mc[i] != '\0' && temp->i == NULL) {
        // printf("testing %s",mc);

        if (mc[i] == '0') //go left
        {
            //        printf("Go left\n");
            temp = temp->left;
        } else if (mc[i] == '1') //go right
        {
            //        printf("Go right\n");
            temp = temp ->right;
        } else
            printf("Wrong value in mc = %c\n", mc[i]);

        i++;
    }
    if (temp->i == NULL) {
        printf("Instruction not found!\n");
        return NULL;
    } else {
        return temp->i;
    }
}

char *getOpcodeName(char* mc) //Using the machine code provided, we find the opcode
{
    Ins* temp = getInstruction(mc);
    return temp->name;
}

char *getOpcodeFormat(char* mc) //Using the machine code provided, we find the opcode
{
    Ins* temp = getInstruction(mc);
    return temp->format;
}

/********SET OF OPCODES*******/
void CMPI(char* mc) {
    //printf("Machine code received in CMPI function %s",mc );
    ;
}

void HLT(char* mc) {
    //printf("Machine code received in HLT function %s",mc );
    ;
}

int bin2dec(char* str) {
    int n = 0;
    int size = strlen(str) - 1;
    int count = 0;
    while (*str != '\0') {
        if (*str == '1')
            n = n + pow(2, size - count);
        count++;
        str++;
    }
    return n;
}

char * getstr(char * q, int start, int end) {
    char * p;
    p = (char *) malloc(25 * sizeof (char));
    int i, c = 0;
    for (i = start; i <= end; i++) {
        //printf("%c", q[i]);
        p[c] = q[i];
        c++;
    }
    //p[c] = '\0';
    //printf("%s\n",p );
    return p;
}

int main() {
    FILE *input_opcode;
    char c, c2, c3;
    char opcode[100];
    char machine_code[100];
    char *str1, *str2, *str3;
    int num1, num2, num3;
    char * ALU = "\0";
    int kk = 0;
    char **machine_code123 = (char **) malloc(50 * sizeof (char *));
    for (kk = 0; kk < 100; kk++) {
        machine_code123[kk] = (char *) malloc(100 * sizeof (char));
    }
    char format[5];
    input_opcode = fopen("input_opcode.txt", "r+"); //input_opcode contains a list of opcodes followed by their format and mac.code
    if (input_opcode == NULL) {
        printf("OPCODE FILE OPENING PROBLEM");
    }
    /**Observation:: In our ISA, the first 8 bits are unused (always zero), so we will start our analysis from the 9th bit-*/
    do {
        c = fscanf(input_opcode, "%s", opcode); //Assuming to get opcode as a string in opcode array
        c2 = fscanf(input_opcode, "%s", machine_code); //Assuming to get the integer as a string in machine_array
        c3 = fscanf(input_opcode, "%s", format);
        //now we will create Ins of each opcode
        Ins* temp = (Ins *) malloc(sizeof (Ins));

        strcpy(temp->name, opcode);
        //Name of the opcode is fed
        strcpy(temp->machine_code, machine_code);
        //Machine code of the opcode is fed
        strcpy(temp->format, format);
        //Format of the opcode is fed

        insertIntoTrie(temp);

    } while (c != EOF && c2 != EOF && c3 != EOF);
    fclose(input_opcode);
    printf("Trie is successfully Created! \n");
    /************************************At this point we have a Binary Trie***************************************/


    /*Now we will read the machine instructions --> decide which operation they are performing-->perform those operations virtually*/


    FILE *mc = fopen("output_machine_code.txt", "r+");
    if (mc == NULL)
        printf("Machine Code FILE OPENING PROBLEM");
    int m = 0;
    do {
        c = fscanf(mc, "%s", machine_code); //Assuming to get the integer as a string in machine_array

        //  printf("test");
        strcpy(machine_code123[m], machine_code);
      //  printf("%s\n", machine_code123[m]);
        //    printf("test1");
        //    char *fun = getOpcodeName(machine_code);        //searches for current opcode in the trie and gives the corresponding opcode
        //    char *fun = getOpcodeName(*(machine_code123+m));
        //    printf("test2");
        //  printf("%s :0: ",fun);

        //     char *form = getOpcodeFormat(*(machine_code123+m));
        //   char *form = getOpcodeFormat(machine_code);
        //    printf("%s\n",form);
        m++;

    } while (c != EOF);

    FILE *out_new = fopen("output_new.txt", "w");
    int n = 0;
    while (n < m) {
      //  printf("%s\n", machine_code123[n]);

        //    char *fun = getOpcodeName(machine_code);        //searches for current opcode in the trie and gives the corresponding opcode
        char *fun = getOpcodeName(*(machine_code123 + n));
        // printf("test2");
      //  printf("%s :0: ", fun);

        char *form = getOpcodeFormat(*(machine_code123 + n));
        //  char *form = getOpcodeFormat(machine_code);
      //  printf("%s\n", form);

        if (strcmp(fun, "ADDI") == 0) {
            //        ADDI(machine_code);
            str1 = getstr(*(machine_code123 + n), 12, 16);
            str2 = getstr(*(machine_code123 + n), 17, 21);
            str3 = getstr(*(machine_code123 + n), 22, 31);
            num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            registers[num1] = registers[num2] + num3;
            printf(" Instruction :: %d added to the register R%d\n", num3, num2);
            //   printf("Value of R%d = %d\n",num, registers[num2]);
            printf("New value of R%d is %d \n\n",num1 ,registers[num1]);
            if (registers[num1] == 0) {
                fzero = 1;
            }
            ALU = "0000";
            
        }
        if (strcmp(fun, "SUBI") == 0) {
            //        SUBI(machine_code);
            str1 = getstr(*(machine_code123 + n), 12, 16);
            str2 = getstr(*(machine_code123 + n), 17, 21);
            str3 = getstr(*(machine_code123 + n), 22, 31);
            num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            registers[num1] = registers[num2] - num3;
           // printf("%d\n", registers[num1]);
          //  printf(" Instruction :: %d added to the register R%d\n", num3, num2);
            
          //   printf(" Instruction :: %d subtracted from the register R%d\n", num3, num2);
            //   printf("Value of R%d = %d\n",num, registers[num2]);
          //  printf("New value of R%d is %d \n\n",num1 ,registers[num1]);
            if (registers[num1] == 0) {
                fzero = 1;
            }
           ALU = "0001"; 
        }
        if (strcmp(fun, "MULI") == 0) {
            //        MULI(machine_code);
            str1 = getstr(*(machine_code123 + n), 12, 16);
            str2 = getstr(*(machine_code123 + n), 17, 21);
            str3 = getstr(*(machine_code123 + n), 22, 31);
            num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            registers[num1] = registers[num2] * num3;
            printf("%d\n", registers[num1]);
            if (registers[num1] == 0) {
                fzero = 1;
            }
            ALU = "0010";
        }
        if (strcmp(fun, "DIVI") == 0) {
            //        DIVI(machine_code);
            str1 = getstr(*(machine_code123 + n), 12, 16);
            str2 = getstr(*(machine_code123 + n), 17, 21);
            str3 = getstr(*(machine_code123 + n), 22, 31);
            num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            if (num3 != 0) {
                registers[num1] = registers[num2] / num3;
                printf("%d\n", registers[num1]);
            }
            if (registers[num1] == 0) {
                fzero = 1;
            }
            ALU = "0011";
        }
        if (strcmp(fun, "MODI") == 0) {
            //        MODI(machine_code);
            str1 = getstr(*(machine_code123 + n), 12, 16);
            str2 = getstr(*(machine_code123 + n), 17, 21);
            str3 = getstr(*(machine_code123 + n), 22, 31);
            num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            if (num3 != 0) {
                registers[num1] = registers[num2] % num3;
                printf("%d\n", registers[num1]);
            }
            if (registers[num1] == 0) {
                fzero = 1;
            }
            ALU = "1010";
        }
        if (strcmp(fun, "AND") == 0) {
            //  AND
            str1 = getstr(*(machine_code123 + n), 17, 21);
            str2 = getstr(*(machine_code123 + n), 22, 26);
            str3 = getstr(*(machine_code123 + n), 27, 31);
            num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            registers[num1] = registers[num2] & registers[num3];
            printf("%d\n", registers[num1]);
            if (registers[num1] == 0) {
                fzero = 1;
            }
            ALU = "0110";
        }
        if (strcmp(fun, "OR") == 0) {
            //  OR
            str1 = getstr(*(machine_code123 + n), 17, 21);
            str2 = getstr(*(machine_code123 + n), 22, 26);
            str3 = getstr(*(machine_code123 + n), 27, 31);
            num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            registers[num1] = registers[num2] | registers[num3];
            printf("%d\n", registers[num1]);
            if (registers[num1] == 0) {
                fzero = 1;
            }
            ALU = "0111";
        }
        if (strcmp(fun, "NOT") == 0) {
            //  NOT
            //str1 = getstr(*(machine_code123 + n), 17, 21);
            str2 = getstr(*(machine_code123 + n), 22, 26);
            str3 = getstr(*(machine_code123 + n), 27, 31);
            //num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            registers[num2] = ~registers[num3];
            printf("%d\n", registers[num2]);
            if (registers[num2] == 0) {
                fzero = 1;
            }
            ALU = "1001";
        }
        if (strcmp(fun, "XOR") == 0) {
            //  XOR
            str1 = getstr(*(machine_code123 + n), 17, 21);
            str2 = getstr(*(machine_code123 + n), 22, 26);
            str3 = getstr(*(machine_code123 + n), 27, 31);
            num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            registers[num1] = registers[num2] ^ registers[num3];
            printf("%d\n", registers[num2]);
            if (registers[num1] == 0) {
                fzero = 1;
            }
            ALU = "1000";
        }
        if (strcmp(fun, "ADD") == 0) {
            //      ADD(machine_code);
            str1 = getstr(*(machine_code123 + n), 17, 21);
            str2 = getstr(*(machine_code123 + n), 22, 26);
            str3 = getstr(*(machine_code123 + n), 27, 31);
            num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            registers[num1] = registers[num2] + registers[num3];
            
             printf(" Instruction :: register%d added to the register R%d\n", num2, num3);
            //   printf("Value of R%d = %d\n",num, registers[num2]);
            printf("New value of R%d is %d \n\n",num1 ,registers[num1]);
            
            
            printf("%d\n", registers[num1]);
            if (registers[num1] == 0) {
                fzero = 1;
            }
            ALU = "0000";
        }
        if (strcmp(fun, "SUB") == 0) {
            //        SUB(machine_code);
            str1 = getstr(*(machine_code123 + n), 17, 21);
            str2 = getstr(*(machine_code123 + n), 22, 26);
            str3 = getstr(*(machine_code123 + n), 27, 31);
            num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            registers[num1] = registers[num2] - registers[num3];
            printf("%d\n", registers[num1]);
            if (registers[num1] == 0) {
                fzero = 1;
            }
            ALU = "0001";
        }
        if (strcmp(fun, "MUL") == 0) {
            //        MUL(machine_code);
            str1 = getstr(*(machine_code123 + n), 17, 21);
            str2 = getstr(*(machine_code123 + n), 22, 26);
            str3 = getstr(*(machine_code123 + n), 27, 31);
            num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            registers[num1] = registers[num2] * registers[num3];
            printf("%d\n", registers[num1]);
            if (registers[num1] == 0) {
                fzero = 1;
            }
            ALU = "0010";
        }
        if (strcmp(fun, "DIV") == 0) {
            //        DIV(machine_code);
            str1 = getstr(*(machine_code123 + n), 17, 21);
            str2 = getstr(*(machine_code123 + n), 22, 26);
            str3 = getstr(*(machine_code123 + n), 27, 31);
            num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            if (registers[num3] != 0) {
                registers[num1] = registers[num2] / registers[num3];
                printf("%d\n", registers[num1]);
            }
            if (registers[num1] == 0) {
                fzero = 1;
            }
            ALU = "0011";
        }
        if (strcmp(fun, "MOD") == 0) {
            //        MOD(machine_code);
            str1 = getstr(*(machine_code123 + n), 17, 21);
            str2 = getstr(*(machine_code123 + n), 22, 26);
            str3 = getstr(*(machine_code123 + n), 27, 31);
            num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            if (registers[num3] != 0) {
                registers[num1] = registers[num2] % registers[num3];
                printf("%d\n", registers[num1]);
            }
            if (registers[num1] == 0) {
                fzero = 1;
            }
            ALU = "1010";
        }
        if (strcmp(fun, "CMPI") == 0) {
            //  CMPI(machine_code);
            str2 = getstr(*(machine_code123 + n), 17, 21);
            str3 = getstr(*(machine_code123 + n), 22, 31);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            if (registers[num2] == num3) {
                fstatus = 0;
            } else if (registers[num2] > num3) {
                fstatus = 1;
            } else {
                fstatus = -1;
            }
        }
        if (strcmp(fun, "MOVI") == 0) {
            //  MOVI(machine_code);
            str2 = getstr(*(machine_code123 + n), 17, 21);
            str3 = getstr(*(machine_code123 + n), 22, 31);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            registers[num2] = num3;
            printf("%d\n", registers[num2]);
            if (registers[num2] == 0) {
                fzero = 1;
            }
        }
        if (strcmp(fun, "MOV") == 0) {
            //  MOV(machine_code);
            //str1 = getstr(*(machine_code123 + n), 17, 21);
            str2 = getstr(*(machine_code123 + n), 22, 26);
            str3 = getstr(*(machine_code123 + n), 27, 31);
            //num1 = bin2dec(str1);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            registers[num2] = registers[num3];
            printf("%d\n", registers[num2]);
            if (registers[num2] == 0) {
                fzero = 1;
            }
        }
        if (strcmp(fun, "CMP") == 0) {
            //  CMPI(machine_code);
            str2 = getstr(*(machine_code123 + n), 22, 26);
            str3 = getstr(*(machine_code123 + n), 27, 31);
            num2 = bin2dec(str2);
            num3 = bin2dec(str3);
            printf("Reg : %d %d",registers[num2],registers[num3]);
            if (registers[num2] == registers[num3])
                fstatus = 0;
            else if (registers[num2] > registers[num3])
                fstatus = 1;
            else
                fstatus = -1;
                
         //   printf("\nCMPSTAT:%d\n",fstatus);
        }
        if (strcmp(fun, "IN") == 0) {
            // IN
            str2 = getstr(*(machine_code123 + n), 27, 31);
            num2 = bin2dec(str2);
            scanf("%d", &registers[num2]);
            if (registers[num2] == 0) {
                fzero = 1;
            }
        }
        if (strcmp(fun, "OUT") == 0) {
            // OUT
            str2 = getstr(*(machine_code123 + n), 27, 31);
            num2 = bin2dec(str2);
            printf("=>OUTPUT VALUE ::: %d\n ", registers[num2]);

        }
        if (strcmp(fun, "INC") == 0) {
            //  INC
            str1 = getstr(*(machine_code123 + n), 27, 31);
            num1 = bin2dec(str1);
            registers[num1] += 1;
         //   printf("Increamented value of R%d is\n", num1);
            printf("%d\n\n", registers[num1]);
            if (registers[num1] == 0) {
                fzero = 1;
            }
            ALU = "0100";
        }
        if (strcmp(fun, "DEC") == 0) {
            //  DEC
            str1 = getstr(*(machine_code123 + n), 27, 31);
            num1 = bin2dec(str1);
            registers[num1] -= 1;
            printf("%d\n", num1);
            printf("%d\n", registers[num1]);
            if (registers[num1] == 0) {
                fzero = 1;
            }
            ALU = "0101";
        }
        if (strcmp(fun, "JMP") == 0) {
            //  JMP
            str1 = getstr(*(machine_code123 + n), 22, 31);
            n = bin2dec(str1) - 1;
         //   printf("JUMPED TO %d\n", n+1);
        }
        if (strcmp(fun, "JEQ") == 0) {
            //  JEQ
          //  printf("\nJEQSTAT:%d\n",fstatus);
            if (fstatus == 0) {
                str1 = getstr(*(machine_code123 + n), 22, 31);
                n = bin2dec(str1) - 2;
                //printf("JUMPED TO %d\n", n+1);
            }
           // fstatus = -2;
        }
        if (strcmp(fun, "JLT") == 0) {
            //  JLT
            if (fstatus == -1) {
                str1 = getstr(*(machine_code123 + n), 22, 31);
                n = bin2dec(str1) - 1;
                printf("JUMPED TO %d\n", n);
            }
        }
        if (strcmp(fun, "JGT") == 0) {
            //  JGT
            if (fstatus == 1) {
                str1 = getstr(*(machine_code123 + n), 22, 31);
                n = bin2dec(str1) - 1;
                printf("JUMPED TO %d\n", n+1);
            }
        }
        if (strcmp(fun, "HLT") == 0) {
            HLT(machine_code);
        }
        if (strcmp(fun, "ARRD") == 0) {
            str2 = getstr(*(machine_code123 + n), 22, 26); // Address of special array register
            str3 = getstr(*(machine_code123 + n), 27, 31); //Address of the register that contains the size

            num3 = bin2dec(str3); // Size of the array register

            if (strcmp(str2, "10000") == 0) // A1 register
            {
                a1 = (int *) malloc(num3 * sizeof (int)); //A1 declared
            }
            if (strcmp(str2, "10001") == 0) // A2 register
            {
                a2 = (int *) malloc(num3 * sizeof (int)); //A2 declared
            }
            if (strcmp(str2, "10010") == 0) // A3 register
            {
                a3 = (int *) malloc(num3 * sizeof (int)); //A3 declared
            }
            if (strcmp(str2, "10011") == 0) // A4 register
            {
                a4 = (int *) malloc(num3 * sizeof (int)); //A4 declared
            }
        }
        if (strcmp(fun, "ARRI") == 0) {
            str1 = getstr(*(machine_code123 + n), 17, 21); // Array register
            str2 = getstr(*(machine_code123 + n), 22, 26); //Index
            str3 = getstr(*(machine_code123 + n), 27, 31); //Value to Insert
            num2 = bin2dec(str2); //index
            num3 = bin2dec(str3); //value to Insert is in register with this address

            if (strcmp(str1, "10000") == 0) // A1 register
            {
                a1[num2] = registers[num3];
            }
            if (strcmp(str1, "10001") == 0) // A2 register
            {
                a2[num2] = registers[num3];
            }
            if (strcmp(str1, "10010") == 0) // A3 register
            {
                a3[num2] = registers[num3];
            }
            if (strcmp(str1, "10011") == 0) // A4 register
            {
                a4[num2] = registers[num3];
            }
        }
        if (strcmp(fun, "ARRC") == 0) {
            str1 = getstr(*(machine_code123 + n), 17, 21); // Array register
            str2 = getstr(*(machine_code123 + n), 22, 26); //Index
            str3 = getstr(*(machine_code123 + n), 27, 31); //Address of Register in which value to be compared is present
            num2 = bin2dec(str2); //index
            num3 = bin2dec(str3); //add. of reg. Can be accessed by registers[num3]

            /**********/
            if (strcmp(str1, "10000") == 0) // A1 register
            {
                if (a1[num2] == registers[num3])
                    fstatus = 0;
                else if (a1[num2] > registers[num3])
                    fstatus = 1; // if left operand(Array) is greater than right operand (Register)
                else
                    fstatus = -1;

            }
            if (strcmp(str1, "10001") == 0) // A2 register
            {
                if (a2[num2] == registers[num3])
                    fstatus = 0;
                else if (a2[num2] > registers[num3])
                    fstatus = 1; // if left operand(Array) is greater than right operand (Register)
                else
                    fstatus = -1;
            }
            if (strcmp(str1, "10010") == 0) // A3 register
            {
                if (a3[num2] == registers[num3])
                    fstatus = 0;
                else if (a3[num2] > registers[num3])
                    fstatus = 1; // if left operand(Array) is greater than right operand (Register)
                else
                    fstatus = -1;

            }
            if (strcmp(str1, "10011") == 0) // A4 register
            {
                if (a3[num2] == registers[num3])
                    fstatus = 0;
                else if (a3[num2] > registers[num3])
                    fstatus = 1; // if left operand(Array) is greater than right operand (Register)
                else
                    fstatus = -1;
            }
        }
        fprintf(out_new,"%s\n",machine_code123[n]);
        fprintf(out_new,"Status flag :%d\n",fstatus);
        fprintf(out_new,"Zero flag :%d\n",fzero);
        if(ALU != "\0")
            fprintf(out_new,"ALU Control Signal:%s\n",ALU);
        ALU = "\0";
        fzero = 0;
       // fstatus = 0;
        n++;
    }
    //printf("%d\n", registers[5]);
    return 0;
}
