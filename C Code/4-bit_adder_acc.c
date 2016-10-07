/*
 *   4-bit Adder/Subtractor w/ accumulator
 *   Author: Johannes Cavalcante
 *   Created in: 09/28/2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "genpat.h"


/***  Funtion Headers  ***/

struct aluRes SumAcc(int Code[], int A[], int clk, int* aclk);
struct result1 Sum_1bit(int A, int B, int Cin);
struct result4 Sum_4bit(int A[], int B[], int Cin);

char IntArrToChar(int input[]);
//char *inttostr(int entier);
int* Inv(int A[]);
void Copy(int A[], int Source[]);
int pPow(int a, int p);

/***  Global Variables  ***/

static int ones[] = {1,1,1,1};
static int zero[] = {0,0,0,0};

int InvRet[4];

static int* ACC;


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
   int selection[2];
   //int a[4], b[4];
   //int** asr;
   struct result4 a;
   struct aluRes res;

   ACC = (int*) malloc(4);
   //asr = (int**) malloc(2);

   printf("CurV\taclk\tclk\tCode\tA\tS\tCout\tACC\n\n");

   //Copy(a, zero);
   //Copy(b, ones);

   //asr[0] = zero;
   //asr[1] = ones;

   Copy(a.S, zero);

   int clk = 0;
   int aclk = 0;
   int cur_vect = 0;

   for(int i = 0; i < 2; ++i){
      selection[0] = i;
      for(int j = 0; j < 2; ++j){
         selection[1] = j;
         
         for(int k = 0; k < 16; ++k){
            for(clk = 0; clk < 2; ++clk){
               res = SumAcc(selection, a.S, clk, &aclk);
               printf("%i\t%i\t%i\t%i%i\t%i%i%i%i\t%i\t%i\t%i%i%i%i\n", cur_vect, aclk, clk, selection[0], selection[1], a.S[0], a.S[1], a.S[2], a.S[3], res.S < 8? res.S : res.S-16, res.Cout, ACC[0], ACC[1], ACC[2], ACC[3]);

               aclk = clk;
               //clk = !clk;
               ++cur_vect;
            }
            a = Sum_4bit(a.S, zero, 1);
         }
         
      }
   }

   return 0;
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
      Copy(ACC, Code[1] == 0? (Code[0] == 0? A : Inv(A)) : aux.S);
      aux = Sum_4bit(Code[0] == 0? A : Inv(A), ACC, Code[0]);
   }

   res.S = Code[1] == 0? IntArrToChar(Code[0] == 0? A : Inv(A)) : IntArrToChar(aux.S);
   res.Cout = aux.Cout;

   //*aclk = clk;

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


char IntArrToChar(int input[]) // Convert an int array to char
{
   char ret = 0;
   for (int i = 0; i < 4; ++i){
      //printf("\n>>>> %i += %i * 2^%i (%i)", ret, input[i], 3-i, pPow(2,(3-i)));
      ret += input[i] * (pPow(2,(3-i)));
   }

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


int pPow(int a, int p)
{
   int aux = 1;
   for(int i = 0; i < p; ++i)
      aux *= a;
   return aux;
}

/*
char *inttostr(int entier)
{
   char *str;
   str = (char *) mbkalloc (32* sizeof (char));
   sprintf (str, "%d",entier);
   
   return str;
}
*/
