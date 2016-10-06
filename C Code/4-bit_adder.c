/*
 *   4-bit Adder/Subtractor
 *   Author: Johannes Cavalcante
 *   Created in: 09/25/2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "genpat.h"

struct aluRes ALU(char code, int A[], int B[]);
struct result1 Sum_1bit(int A, int B, int Cin);
struct result4 Sum_4bit(int A[], int B[], int Cin);

static int one[] = {0,0,0,1};
static int zero[] = {0,0,0,0};

static int* ACC; 

char IntArrToChar(int input[]);
char *inttostr(int entier);


/***  Structs  ***/

struct aluRes
{
   char S;
   int Cout;
};


struct result4
{
   int S[4];
   int Cout;
};


struct result1
{
   int S;
   int Cout;
};


/***  Main  ***/

int main(void)
{
   char code[] = {0b0010, 0b0011};
   int a[4], b[4];
   struct aluRes res;

   ACC = (int*) malloc(4);

   DEF_GENPAT("somador_4bits_genpat_sem_atraso");
   DECLAR("a", ":2", "B", IN, "3 down to 0","");
   DECLAR("b", ":2", "B", IN, "3 down to 0","");
   DECLAR("cin", ":2", "B", IN, "","");
   DECLAR("s", ":2", "B", OUT, "3 down to 0","");
   DECLAR("cout", ":2", "B", OUT, "","");
   DECLAR("vdd", ":2", "B", IN, "","");
   DECLAR("vss", ":2", "B", IN, "","");

   LABEL ("somador");
   AFFECT ("0","vdd","0b1");
   AFFECT ("0","vss","0b0");

   //int i=0, j=0, k=0;

   for(int i = 0; i < 16; i++){
      for(int j = 0; j < 16; j++){
         for (int k = 0; k < 2; k++){
            AFFECT (inttostr(cur_vect),"a", inttostr(i));
            AFFECT (inttostr(cur_vect),"b", inttostr(j));
            AFFECT (inttostr(cur_vect),"cin", inttostr(k));
            AFFECT (inttostr(cur_vect),"cout", inttostr(Cout(i,j,k)));
            AFFECT (inttostr(cur_vect), "s", inttostr(S(i,j,k)));
            cur_vect++;
         }
      }
   }








   printf("code\ta\tb\tS\tCout\n\n");

   for (int i = 0; i < 2; ++i){
      for(int ai = 0; ai < 2; ++ai){
         a[0] = ai;
         for(int aj = 0; aj < 2; ++aj){
            a[1] = aj;
            for(int ak = 0; ak < 2; ++ak){
               a[2] = ak;
               for(int al = 0; al < 2; ++al){                
                  a[3] = al;
                  

                  for(int bi = 0; bi < 2; ++bi){
                     b[0] = bi;
                     //printf("\nb[0] >>>> %i\n\n", b[0]);
                     for(int bj = 0; bj < 2; ++bj){
                        b[1] = bj;
                        //printf("\nb[1] >>>> %i\n\n", b[1]);
                        for(int bk = 0; bk < 2; ++bk){
                           b[2] = bk;
                           //printf("\nb[2] >>>> %i\n\n", b[2]);
                           for(int bl = 0; bl < 2; ++bl){
                              b[3] = bl;
                              //printf("\nb[3] >>>> %i\n\n", b[3]);
                              printf("%x\t%i%i%i%i\t%i%i%i%i\t", code[i], a[0], a[1], a[2], a[3], b[0], b[1], b[2], b[3]);
                              res = ALU(code[i], a, b);
                              printf("%i\t%i\n", res.S <= 7? res.S : res.S-16, res.Cout);
                           }
                        }
                     }
                  }

               }
            }
         }
      }
   }
   //res = ALU(3, a, b);

   //printf("\nRES:%i\t%i\n", res.S, res.Cout);

   free(ACC);
   return 0;
}


/***  Functions  ***/

char IntArrToChar(int input[]) // Convert an int array to char
{
   char ret = 0;
   for (int i = 0; i < 4; ++i){
      ret += input[i] * (pow(2,(3-i)));
      //printf("%i\n", ret);
   }
   return ret;
}


struct aluRes ALU(char code, int A[], int B[])
{
   /*
   printf("Somando A e B:\n\n"
          "A = [%i,%i,%i,%i]\n"
          "B = [%i,%i,%i,%i]\n", A[0], A[1], A[2], A[3], B[0], B[1], B[2], B[3]);
   */
   int* MemA;
   int* MemB;
   
   MemA = (int*) malloc(4);
   MemB = (int*) malloc(4);

   for (int i = 0; i < 4; ++i){
      MemA[i] = A[i];
      MemB[i] = B[i];
   }
      
   struct aluRes res;
   struct result4 aux;

   switch(code){
   case 3: // Subtraction

      //printf("From: (%i, %i, %i, %i)\n", MemB[0], MemB[1], MemB[2], MemB[3]);
      for (int i = 0; i < 4; ++i) // Invert B
         MemB[i] = !MemB[i];

      //printf("To: (%i, %i, %i, %i)\n", MemB[0], MemB[1], MemB[2], MemB[3]);
      aux = Sum_4bit(MemB, zero, 1);
      MemB = aux.S;

      
      
   case 2: // Addition
      //printf("Somando: A = [%i,%i,%i,%i] B = [%i,%i,%i,%i]\n", MemA[0], MemA[1], MemA[2], MemA[3], MemB[0], MemB[1], MemB[2], MemB[3]);

      aux = Sum_4bit(MemA, MemB, 0);
      res.S = IntArrToChar(aux.S);
      res.Cout = aux.Cout;

      //printf("Resultado: 0b[ %i%i%i%i ] 0x[ %x ] ui[ %i ] si[ %i ]\n\n", aux.S[0], aux.S[1], aux.S[2], aux.S[3], res.S, res.S, res.S < 7? res.S : res.S-16);

      //free(MemA);
      //free(MemB);
      
      return res;
   }
}


struct result4 Sum_4bit(int A[], int B[], int Cin)
{
   struct result1 aux[4];
   struct result4 ret;

   for (int i = 0; i < 4; ++i)
      aux[3-i] = Sum_1bit(A[3-i], B[3-i], i == 0? Cin : aux[4-i].Cout);

   for (int i = 0; i < 4; ++i)
      ret.S[i] = aux[i].S;

   ret.Cout = aux[0].Cout;
   return ret;
}


struct result1 Sum_1bit(int A, int B, int Cin) // 1-bit sum
{
   struct result1 ret;

   ret.S = A^B^Cin;
   ret.Cout = (A&B)|(A&Cin)|(B&Cin);
   return ret;
}

/*
int Pow(int a, int p)
{
   int aux = 1;
   for(int i = 0; i < p; ++i)
      aux *= a;
   return a;
}


char *inttostr(int entier)
{
   char *str;
   str = (char *) mbkalloc (32* sizeof (char));
   sprintf (str, "%d",entier);
   
   return str;
}
*/
