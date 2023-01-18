#include <stdio.h>
int sum(int x, int y){
	return x+y;
}
int swap(int *x, int *y){
	int t = *x;
	*x = *y;
	*y = t;
}
int main() {
	//printf() displays the string inside quotation
	printf("Hello, World!\n");
	int t = sum(1,2);
	printf("%d\n", t);

	const int N = 10;
	int fibo[N];
	fibo[0] = fibo[1] = 1;
	for(int i=2; i<N; ++i){
		fibo[i] = fibo[i-1] + fibo[i-2];
	}
	printf("%d\n", fibo[N-1]);
	return 0;
}

