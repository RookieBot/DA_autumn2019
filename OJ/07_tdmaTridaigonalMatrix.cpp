#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	int N, M;
	scanf("%d", &N);
	scanf("%d", &M);

	// allocate space for matrix A and R.T
	double* U = new double[N];
	double* L = new double[N];
	double* D = new double[N];
	double* R = new double[N];
	double* Ans = new double[N];

	// get the diagonal elements of A
	for (int i = 0; i < N; ++i)
	{
		scanf("%lf", &D[i]);
	}

	// get the upper diagonal of A
	for (int i = 0; i < N - 1; ++i)
	{
		scanf("%lf", &U[i]);
	}

	// get the lower diagonal of A
	for (int i = 0; i < N - 1; ++i)
	{
		scanf("%lf", &L[i]);
	}

	// TDMA

	// transform U
	U[0] = U[0] / D[0];
	for (int i = 1; i < N - 1; ++i)
	{
		U[i] = U[i] / (D[i] - U[i - 1] * L[i - 1]);
	}

	for (int j = 0; j < M; ++j)
	{
		// read R[:,j]
		for (int i = 0; i < N; ++i) scanf("%lf", &R[i]);

		// transform R
		R[0] = R[0] / D[0];
		for (int i = 1; i < N ; ++i)
		{
			R[i] = (R[i] - L[i - 1] * R[i - 1]) / (D[i] - L[i - 1] * U[i - 1]);
		}

		Ans[N - 1] = R[N - 1];
		for (int i = N - 2; i >= 0; --i)
		{
			Ans[i] = R[i] - U[i] * Ans[i + 1];
		}

		for (int i = 0; i < N; ++i) printf("%lf ", Ans[i]);
		printf("\n");
	}



	

	// delete the newed variables from memory. 


	delete[] U;
	delete[] L;
	delete[] D;
	delete[] R;
	delete[] Ans;
	U = NULL;
	L = NULL;
	D = NULL;
	R = NULL;
	Ans = NULL;

	return 0;
}