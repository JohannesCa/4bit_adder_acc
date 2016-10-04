/*
 *   4-bit Adder/Subtractor w/ accumulator
 *   Author: Johannes Cavalcante
 *   Created in: 09/28/2016
 */

#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include "genpat.h"


/***  Funtion Headers  ***/

struct aluRes SumAcc(int Code[], int A[], int clk, int* aclk);
struct result1 Sum_1bit(int A, int B, int Cin);
struct result4 Sum_4bit(int A[], int B[], int Cin);

int IntArrToInt(int input[]);
char* inttostr(int entier);
int* Inv(int A[]);
void Copy(int A[], int Source[]);
int Pow(int a, int p);


/***  Global Variables  ***/

static int ones[] = {1,1,1,1};
static int zero[] = {0,0,0,0};

int InvRet[4];

static int* ACC;


/***  Structs  ***/

struct aluRes
{
   int S;
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
   int selection[2];
   int** asr;
   struct aluRes res;

   ACC = (int*) malloc(4);
   asr = (int**) malloc(2);

   asr[0] = zero;
   asr[1] = ones;

   int clk = 0;
   int aclk = 0;
   int cur_vect = 0;

   DEF_GENPAT("4bit-adder-acc-genpat");
   
   DECLAR("aclk", ":2", "B", OUT, "","");   
   DECLAR("clk", ":2", "B", IN, "","");

   DECLAR("Sel0", ":2", "B", IN, "","");
   DECLAR("Sel1", ":2", "B", IN, "","");
   DECLAR("A", ":2", "B", IN, "3 down to 0","");   
   
   DECLAR("S", ":2", "B", OUT, "3 down to 0","");
   DECLAR("Cout", ":2", "B", OUT, "","");
   DECLAR("ACC", ":2", "B", OUT, "3 down to 0","");
   
   DECLAR("vdd", ":2", "B", IN, "","");
   DECLAR("vss", ":2", "B", IN, "","");

   LABEL ("Begin");
   AFFECT ("0","vdd","0b1");
   AFFECT ("0","vss","0b0");

   for(int i = 0; i < 2; ++i){
      for(int j = 0; j < 2; ++j){
                  
         for(int k = 0; k < 2; ++k){
            for(clk = 0; clk < 2; ++clk){
               selection[1] = j;
               selection[0] = i;
               
               res = SumAcc(selection, asr[k], clk, &aclk);

               AFFECT(inttostr(cur_vect),"clk", inttostr(clk));
               AFFECT(inttostr(cur_vect),"aclk", inttostr(aclk));
               AFFECT(inttostr(cur_vect),"Sel0", inttostr(selection[0]));
               AFFECT(inttostr(cur_vect),"Sel1", inttostr(selection[1]));
               AFFECT(inttostr(cur_vect),"A", inttostr(IntArrToInt(asr[k])));
               AFFECT(inttostr(cur_vect),"S", inttostr(res.S));
               AFFECT(inttostr(cur_vect),"Cout", inttostr(res.Cout));
               AFFECT(inttostr(cur_vect),"ACC", inttostr(IntArrToInt(ACC)));
               
               aclk = clk;
               ++cur_vect;
            }
         }
         
      }
   }

   SAV_GENPAT();
}

/*

sel0 | sel1

  0  |  0   -> Copia a entrada A para ACC
  0  |  1   -> Soma a entrada com ACC
  1  |  0   -> Copia a entrada A invertida bit a bit para ACC
  1  |  1   -> Subtrai a entrada A de ACC

*/

/***  Functions  ***/

struct aluRes SumAcc(int Code[], int A[], int clk, int* aclk)
{
   struct aluRes res;
   struct result4 aux;

   aux = Sum_4bit(Code[0] == 0? A : Inv(A), ACC, Code[0]);
   
   if((clk == 1) && (*aclk == 0)){
      Copy(ACC, Code[1] == 0? A : aux.S);
      aux = Sum_4bit(Code[0] == 0? A : Inv(A), ACC, Code[0]);
   }

   res.S = Code[1] == 0? IntArrToInt(Code[0] == 0? A : Inv(A)) : IntArrToInt(aux.S);
   res.Cout = aux.Cout;

   return res;
}


struct result4 Sum_4bit(int A[], int B[], int Cin)
{
   struct result1 aux[4];
   struct result4 ret;

   //printf("\n>>>> Somando: A = [%i,%i,%i,%i] B = [%i,%i,%i,%i]\n", A[0], A[1], A[2], A[3], B[0], B[1], B[2], B[3]);

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
char IntArrToChar(int input[]) // Convert an int array to char
{
   char ret = 0;
   for (int i = 0; i < 4; ++i)
      ret += input[i] * (Pow(2,(3-i)));

   return ret;
}
*/

int IntArrToInt(int input[])
{
   int ret = 0;
   for (int i = 0; i < 4; ++i)
      ret += input[i] * (Pow(2,(3-i)));

   return ret;
}


int* Inv(int A[])
{
   for(int i = 0; i < 4; ++i)
      InvRet[i] = !A[i];
   return InvRet;
}


void Copy(int A[], int Source[]) // Copy the content of Source to A
{
   for (int i = 0; i < 4; ++i)
      A[i] = Source[i];
}


int Pow(int a, int p)
{
   int aux = 1;
   for(int i = 0; i < p; ++i)
      aux *= a;
   return aux;
}


char* inttostr(int entier)
{
   char* str;
   str = (char*) mbkalloc(32 * sizeof(char));
   sprintf(str, "%i", entier);
   
   return str;
}
