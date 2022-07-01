#include <stdio.h>
#include <string.h>

static void hash (char a[], int b){
int sum = 0;
	for (int i = 0; i < b; i++){
		if (a[i] == 'A')
			sum += 18;
		else if (a[i] == 'B')
			sum += 11;
		else if (a[i] == 'C')
			sum += 10;
		else if (a[i] == 'D')
			sum += 21;
		else if (a[i] == 'E')
			sum += 7;
		else if (a[i] == 'F')
			sum += 5;
		else if (a[i] == 'G')
			sum += 9;
		else if (a[i] == 'H')
			sum += 22;
		else if (a[i] == 'I')
			sum += 17;
		else if (a[i] == 'J')
			sum += 2;
		else if (a[i] == 'K')
			sum += 12;
		else if (a[i] == 'L')
			sum += 3;
		else if (a[i] == 'M')
			sum += 19;
		else if (a[i] == 'N')
			sum += 1;
		else if (a[i] == 'O')
			sum += 14;
		else if (a[i] == 'P')
			sum += 16;
		else if (a[i] == 'Q')
			sum += 20;
		else if (a[i] == 'R')
			sum += 8;
		else if (a[i] == 'S')
			sum += 23;
		else if (a[i] == 'T')
			sum += 4;
		else if (a[i] == 'U')
			sum += 26;
		else if (a[i] == 'V')
			sum += 15;
		else if (a[i] == 'W')
			sum += 6;
		else if (a[i] == 'X')
			sum += 24;
		else if (a[i] == 'Y')
			sum += 13;
		else if (a[i] == 'Z')
			sum += 25;
		else if (a[i] == '1')
			sum = sum - 1;
		else if (a[i] == '2')
			sum = sum - 2;
		else if (a[i] == '3')
			sum = sum - 3;
		else if (a[i] == '4')
			sum = sum - 4;
		else if (a[i] == '5')
			sum = sum - 5;
		else if (a[i] == '6')
			sum = sum - 6;
		else if (a[i] == '7')
			sum = sum - 7;
		else if (a[i] == '8')
			sum = sum - 8;
		else if (a[i] == '9')
				sum = sum - 9;
	}
	printf ("Hash of the string %s is equal to %d.\n", a, sum);
}

int main(void){
	char a[] = "AAABCC";
	int length = (int) strlen(a);
	hash(a, length);
	return 0;
}
