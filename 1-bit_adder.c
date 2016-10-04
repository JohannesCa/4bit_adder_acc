/*
 *   1-bit Carry Lookaheaded Adder
 *   Author: Johannes Cavalcante
 *   Created in: 09/25/2016
 */

#include <stdio.h>
#include <stdlib.h>
#include "genpat.h"

//struct result ALU(char code, int A, int B, int Cin);
struct result Sum(int A, int B, int Cin);
char *inttostr(int entier);

/***  Structs  ***/

struct result
{
   int S;
   int Cout;
};


/***  Main  ***/

int main(void)
{
   //char code[] = {0b0010, 0b0011};
   struct result rr;

   int cur_vect = 0;	

   DEF_GENPAT("1-bit Carry Lookaheaded Adder");
   //DECLAR("Code", ":2", "B", IN, "3downto0", "");
   DECLAR("A", ":2", "B", IN, "", "");
   DECLAR("B", ":2", "B", IN, "", "");
   DECLAR("Cin", ":2", "B", IN, "", "");
   DECLAR("S", ":2", "B", OUT, "", "");
   DECLAR("Cout", ":2", "B", OUT, "", "");
   DECLAR("Vdd", ":2", "B", IN, "", "");
   DECLAR("Vss", ":2", "B", IN, "", "");	

   LABEL("Begin");
   AFFECT("0", "Vdd", "0b1");
   AFFECT("0", "Vss", "0b0");

   //for(int i = 0; i < 2; i++){
      //AFFECT(inttostr(cur_vect), "Code", inttostr(code[i]));
      for(int j = 0; j < 2; j++){
         AFFECT(inttostr(cur_vect), "A", inttostr(j));
         for(int k = 0; k < 2; k++){
            AFFECT(inttostr(cur_vect), "B", inttostr(k));
            for(int l = 0; l < 2; l++){
               AFFECT(inttostr(cur_vect), "Cin", inttostr(l));

               rr = Sum(j, k, l); //rr = ALU(code[i], j, k, l);

               AFFECT(inttostr(cur_vect), "S", inttostr(rr.S));
               AFFECT(inttostr(cur_vect), "Cout", inttostr(rr.Cout));
               cur_vect++;
            }
         }
      }
   //}
   SAV_GENPAT();
}


/***  Functions  ***/
/*
struct result ALU(char code, int A, int B, int Cin)
{
   struct result ret;
   int G; // Generate Cout signal
   int P; // Propagate Cout signal

   switch(code){
   case 3:
      ret = ALU(2, !B, 1, 0); // (!B + 1)
      B = ret.S;
   case 2:
      G = A&B;
      P = A|B;

      ret.S = A^B^Cin;
      ret.Cout = G|(P&Cin);
      return ret;
   }
}
*/

struct result Sum(int A, int B, int Cin)
{
   struct result ret;
   int G; // Generate Cout signal
   int P; // Propagate Cout signal

   G = A&B;
   P = A|B;

   ret.S = A^B^Cin;
   ret.Cout = G|(P&Cin);
   return ret;
}


char *inttostr(int entier)
{
   char *str;
   str = (char *) mbkalloc (32* sizeof (char));
   sprintf (str, "%d",entier);
   
   return str;
}


