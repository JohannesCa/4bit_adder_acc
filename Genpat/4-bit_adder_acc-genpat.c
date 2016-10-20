/*
 *   4-bit Adder/Subtractor w/ accumulator Genpat
 *   Author: Johannes Cavalcante
 *   Created in: 09/28/2016
 */

#include <stdio.h>
#include <stdlib.h>
#include "genpat.h"
#include "functions.h"


/***  Global Variables  ***/

static int ones[] = {1,1,1,1};
static int zero[] = {0,0,0,0};

/***  Main  ***/

int main(void)
{
   int selection[2];
   //int** asr;
   struct result4 a;
   struct aluRes res;

   ACC = (int*) malloc(4);
   //asr = (int**) malloc(2);

   Copy(a.S, zero);
   //a.Cout = 0;

   //asr[0] = zero;
   //asr[1] = ones;

   int clk = 0;
   int aclk = 0;
   int cur_vect = 0;

   DEF_GENPAT("4bit-adder-acc-genpat");
   
   DECLAR("clk", ":2", "B", IN, "","");

   DECLAR("Sel0", ":2", "B", IN, "","");
   DECLAR("Sel1", ":2", "B", IN, "","");
   DECLAR("A", ":2", "B", IN, "3 down to 0","");   
   
   DECLAR("S", ":2", "B", OUT, "3 down to 0","");
   DECLAR("Cout", ":2", "B", OUT, "","");
   DECLAR("ACC", ":2", "B", OUT, "3 down to 0","");
   
   DECLAR("vdd", ":2", "B", IN, "","");
   DECLAR("vss", ":2", "B", IN, "","");

   //LABEL ("Begin");
   AFFECT ("0","vdd","0b1");
   AFFECT ("0","vss","0b0");

   int i = 0, j = 0, k = 0;

   for(; i < 2; ++i){
      for(; j < 2; ++j){
                  
         for(; k < 16; ++k){
            for(; clk < 2; ++clk){
              	selection[1] = j;
              	selection[0] = i;
               
              	res = SumAcc(selection, a.S, clk, &aclk);

              	AFFECT(inttostr(cur_vect),"clk", inttostr(clk));
              	AFFECT(inttostr(cur_vect),"Sel0", inttostr(selection[0]));
              	AFFECT(inttostr(cur_vect),"Sel1", inttostr(selection[1]));
              	AFFECT(inttostr(cur_vect),"A", inttostr(BinToInt(a.S)));
              	AFFECT(inttostr(cur_vect),"S", inttostr(res.S));
              	AFFECT(inttostr(cur_vect),"Cout", inttostr(res.Cout));
              	AFFECT(inttostr(cur_vect),"ACC", inttostr(BinToInt(ACC)));
               
               if(selection[0] == 0 && selection[1] == 0)
               	LABEL("Copy_A");
               else if(selection[0] == 0 && selection[1] == 1)
                 	LABEL("Add_A");
               else if(selection[0] == 1 && selection[1] == 0)
                 	LABEL("Copy_not_A");
            	else if(selection[0] == 1 && selection[1] == 1)
            	  	LABEL("Sub_A");

            	aclk = clk;
              	++cur_vect;
            }
      		a = Sum_4bit(a.S, zero, 1);
   	   }
      }
   }

   SAV_GENPAT();
}
