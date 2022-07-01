/*****************************************************************************/
/*                             		Hash Calculator                            */
/*****************************************************************************/

/*
 *  A brief string check_cap and hash calculator.
 *
 *  It checks a string initialized to be tested element-wise and to find only capital letters or numbers and calculates the hash.
 *
 *  Author : Iliadis-Apostolidis Dimosthenis 8811
 *
 */
#include <stdio.h>
#include <string.h>

__asm int hash(const char *a, int *b, int c, int d){

	  //When this function is called, string a[] (an array) is passed @r0, b[] is passed @r1, the lengthA of array a[] is passed @r2 and the result is passed @r3 so we don't have to load them again. Hash is calculated and returned @r4.
		//This happens automatically in Keil (we can see that in dissasembly), but if we had to write an entire code in assembly we would have to push and mov all those variables by ourselves.

	//MOVS          r3, #0   	   //Initialize an increment to access (int i = 0;) (It's already 0 because of passing the result @r3 when calling the function. Still we do know that the result was initialized @r4 in main.)
	//MOVS					r4, #0		 	 //If we didn't use the "result" variable passed in and out the subroutine, this is where we would initialize r4 in order to calculate the result. Again, we don't need to initialize it because we already did in main.
		MOVS					r8, #4 	 		 //Integers are 4 bytes, so we need to multiply r6 with 4 (r8) if we have a number as a character.
		B             limit    		 //Check that the string exists.

check_cap
		LDRB          r5,[r0,r3]	 //Load into r5 the element a[i].
		SUBS					r6, r5, #'A' //In r6 we store a second increment j to access b[] array. We subtract character A (0x41) from r5 to check if the element of the string we examine is in the range of 0x41 (A) to 0x5A (Z).
		CMP						r6, #0			 //Compare with 0.
		BLT					  check_num		 //If less, go to check if it's a number.
		CMP						r6, #25			 //Compare j with 25 which is the last element of the capitals of the alphabet (0->25 = 26 elements of b[25] array).
		BGT 				  next				 //If more, go to the next element of the string a[] we're testing.
		MUL					  r6, r6, r8	 //Multiply j increment with 4 to get the correct element of b[] array, because it's an integer array (4 bytes per element).
		LDRB 					r7, [r1,r6]  //Load the element of b[] array.
		ADDS					r4, r4, r7	 //Add the value for the capital letter to calculate hash.
		B 						next				 //Go to the next element of the string a[].

check_num
		SUBS					r6, r5, #'0' //Subtract character 0 from a[i], which is already checked as less than A, so it's either a number, or some other characters we don't care about.
		CMP						r6, #9			 //Compare with number 9.
		BGT 				  next				 //If it's more than 9 it means that it is some character which we don't care about, so check the next element of string a[].
		CMP						r6, #1			 //Compare with number 1.
		BLT 				  next				 //If it's less than 1 it means that it is some character which we don't care about, so check the next element of string a[].
		SUBS					r4, r4, r6	 //Else it means that this element a[i], is in the range of 1 to 9 and we have it ready to be subtracted from the hash, because of the first line of check_num.

next
    ADDS          r3,r3,#1     //This label increases the increment by 1 because we have a char array as the string we are checking which has 1 byte elements.

limit
    CMP           r3,r2				 //This label compares the increment with r2 where the lengthA of the string is stored. (lengthA = strlen(a))
    BLT       	  check_cap		 //If the increment is lower than the limit (the lengthA of the string), go on and start checking for the right characters.
	//MOVS					r0,r4			 	 //We could use this line if we initialized the result inside the assembly subroutine, but we don't need it now because we pass the "result" variable in and out instead.
		BX    				lr		 			 //If the increment reaches the lengthA of the string, leave from this subroutine and continue to main.

END
}

int main(void){
	const char a[] = "ABBCC1090"; //Initialize the string to be tested. Change this to test other strings.
  int b[26] = {18, 11, 10, 21, 7, 5, 9, 22, 17, 2, 12, 3, 19, 1, 14, 16, 20, 8, 23, 4, 26, 15, 6, 24, 13, 25}; //Initialize the array values for capital letters, given as an exercise.
	int lengthA = (int) strlen(a);	//Get the lengthA of the string we are testing.
	int result = 0;
	hash(a, b, lengthA, result); //Store the result to the variable result by calling the assembly function to do the checking, calculate and return the hash.
	printf("The hash for '%s' is %d.\n", a, result); //Print the correct result.
	return 0; //After running this code we can see here with a breakpoint that the correct hash is stored @r4, in hex, while in debug mode.
}
