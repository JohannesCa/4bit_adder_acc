/*
 *   4-bit Adder/Subtractor w/ accumulator Genpat
 *   Author: Johannes Cavalcante
 *   Created in: 10/06/2016
 */


/***  Funtion Headers  ***/

struct aluRes SumAcc(int Code[], int A[], int clk, int* aclk);
struct result1 Sum_1bit(int A, int B, int Cin);
struct result4 Sum_4bit(int A[], int B[], int Cin);

int BinToInt(int input[]);
char* inttostr(int entier);
int* Inv(int A[]);
void Copy(int A[], int Source[]);
int Pow(int a, int p);


/***  Global Variables  ***/

int InvRet[4];
static int* ACC;

/***  Struct Definitions  ***/

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

   res.S = Code[1] == 0? BinToInt(Code[0] == 0? A : Inv(A)) : BinToInt(aux.S);
   res.Cout = aux.Cout;

   return res;
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


int BinToInt(int input[])
{
   int ret = 0;
   for (int i = 0; i < 4; ++i)
      ret += input[i] * (Pow(2,(3-i)));

   return ret;
}


char* inttostr(int entier)
{
   char* str;
   str = (char*) mbkalloc(32 * sizeof(char));
   sprintf(str, "%i", entier);
   
   return str;
}

