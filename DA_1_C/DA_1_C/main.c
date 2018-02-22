/*
 * DA_1_C.c
 *
 * Created: 2/21/2018 6:38:11 PM
 * Author : Lopez
 */ 

#include <avr/io.h>
#include <stdio.h>


int main(void)
{
    int nums[300];
	int startAddLw = 34;       // Lower half of STARTADD, (0x22 = 34)
	int startAddHi = 2;        // upper half of STARTADD
	
	int div5[300];                // will keep numbers that are divisible by 5
	int evr[300];                 // will keep every other number
	int fvCnt = 0;             // index count for numbers divisible by 5
	int evrCnt = 0;            // index count for the rest of the numbers
	
	int fvSum = 0;
	int evrSum = 0;
	
	for (int i = 0; i < 300; i++)
	{
		if ((startAddHi + startAddLw) >= 256)
		{
			nums[i] = (startAddHi + startAddLw) - 256;    // If the sum is >255, subtract 256 since 
		}                                                 // asm equiv will not have any overflow
		else
			nums[i] = startAddHi + startAddLw;
			
		startAddLw++;
		if (startAddLw == 256)
		{
			startAddHi++;
			startAddLw = 0;
		}
	}
	
	for (int i = 0; i < 300; i++)        // parse through the numbers
	{
		if ((nums[i] % 5) == 0)          // number is divisible by 5
		{
			fvSum += nums[i];
			div5[fvCnt] = nums[i];
			fvCnt++;
		}
		else                             // everything else
		{
			evrSum += nums[i];
			evr[evrCnt] = nums[i];
			evrCnt++;
		}
	}
}

