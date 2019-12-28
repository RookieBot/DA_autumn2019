#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int val[360000];

int readNum() {   // øÏÀŸ∂¡»Î
	char ch = getchar();
	while (ch < '0' or ch > '9') ch = getchar();
	int v = 0;
	while (ch >= '0' and ch <= '9') {
		v = v * 10 + ch - '0';
		ch = getchar();
	}
	return v;
}

int getMaxim(int N)
{
	// int* maxim = new int[N*N]
	// set the first row and first column of maxim
	int* maxim = new int[N*N];
	maxim[0] = val[0];
	for (int i = 1; i < N; ++i)
	{
		maxim[i] = val[i] + maxim[i - 1]; // first row
		maxim[i * N] = val[i * N] + maxim[(i - 1) * N]; // first column
	}

	// get maxims
	int tmp1, tmp2;
	for (int i = 1; i < N; ++i) // loop over rows 
	{
		for (int j = 1; j < N; ++j) // loop over columns
		{
			// maxim[i * N + j] = val[i * N + j];
			tmp1 = maxim[(i - 1) * N + j];
			tmp2 = maxim[i * N + j - 1];
			// maxim[i * N + j] = val[i * N + j] + (tmp1 > tmp2) ? tmp1 : tmp2;
			maxim[i * N + j] = ((tmp1 > tmp2) ? tmp1 : tmp2) + val[i * N + j];
		}
	}

	int tmp = maxim[(N - 1) * N + N - 1];
	delete[] maxim;
	maxim = NULL;
	return tmp;
	// printf("%d", result);
}

void stateUpdate2(int* prev_maxim, int stateNum, int i, int N, int* maxim)
{
	if (i <= N - 1)
	{
		for (int j1 = 0; j1 <= stateNum; ++j1)
		{
			for (int j2 = 0; j2 <= stateNum; ++j2)
			{
				// different situations according to whether 2 miners reach the same spot
				int addition;
				if (j1 == j2) addition = val[j1 * N + (stateNum - j1)];
				else addition = val[j1 * N + (stateNum - j1)] + val[j2 * N + (stateNum - j2)];
			
				// find the best previous state
				int prev_max;
				if (j1 == stateNum)
				{
					if (j2 == stateNum) prev_max = prev_maxim[(j1 - 1) * N + j2 - 1];
					else if (j2 == 0) prev_max = prev_maxim[(j1 - 1) * N + j2];
					else
					{
						if (prev_maxim[(j1 - 1) * N + j2 - 1] > prev_maxim[(j1 - 1) * N + j2])
							prev_max = prev_maxim[(j1 - 1) * N + j2 - 1];
						else prev_max = prev_maxim[(j1 - 1) * N + j2];
					}
				}
				else if (j1 == 0)
				{
					if (j2 == stateNum) prev_max = prev_maxim[j1 * N + j2 - 1];
					else if (j2 == 0) prev_max = prev_maxim[j1 * N + j2];
					else
					{
						if (prev_maxim[j1 * N + j2 - 1] > prev_maxim[j1 * N + j2])
							prev_max = prev_maxim[j1 * N + j2 - 1];
						else prev_max = prev_maxim[j1 * N + j2];
					}
				}
				else if (j2 == 0) // j1 is "normal"
				{
					if (prev_maxim[j1 * N + j2] > prev_maxim[(j1 - 1) * N + j2])
						prev_max = prev_maxim[j1 * N + j2];
					else prev_max = prev_maxim[(j1 - 1) * N + j2];
				}
				else if (j2 == stateNum)
				{
					if (prev_maxim[j1 * N + j2 - 1] > prev_maxim[(j1 - 1) * N + j2 - 1])
						prev_max = prev_maxim[j1 * N + j2 - 1];
					else prev_max = prev_maxim[(j1 - 1) * N + j2 - 1];
				}
				else // both j1 and j2 are "normal"
				{
					// compare the 4 possibilites one by one
					prev_max = prev_maxim[(j1 - 1) * N + j2 - 1];
					if (prev_maxim[(j1 - 1) * N + j2] > prev_max)
						prev_max = prev_maxim[(j1 - 1) * N + j2];
					if (prev_maxim[j1 * N + j2 - 1] > prev_max)
						prev_max = prev_maxim[j1 * N + j2 - 1];
					if (prev_maxim[j1 * N + j2] > prev_max)
						prev_max = prev_maxim[j1 * N + j2];
				}
				
				maxim[j1 * N + j2] = addition + prev_max;
			}
		}
	}
	else // i > N - 1
	{
		for (int j1 = 0; j1 <= stateNum; ++j1)
		{
			for (int j2 = 0; j2 <= stateNum; ++j2)
			{
				// different situations according to whether 2 miners reach the same spot
				int addition;
				if (j1 == j2) addition = val[(i - N + 1 + j1) * N + N - 1 - j1];
				else addition = val[(i - N + 1 + j1) * N + N - 1 - j1] + val[(i - N + 1 + j2) * N + N - 1 - j2];

				// find the best previous state
				int prev_max = prev_maxim[j1 * N + j2];
				if (prev_maxim[(j1 + 1) * N + j2] > prev_max)
					prev_max = prev_maxim[(j1 + 1) * N + j2];
				if (prev_maxim[j1 * N + j2 + 1] > prev_max)
					prev_max = prev_maxim[j1 * N + j2 + 1];
				if (prev_maxim[(j1 + 1) * N + j2 + 1] > prev_max)
					prev_max = prev_maxim[(j1 + 1) * N + j2 + 1];

				maxim[j1 * N + j2] = addition + prev_max;
			}
		}
	}
}

int getMaxim2(int N)
{
	int* maxim = new int[N * N];
	int* prev_maxim = new int[N * N];
	// set initial state
	maxim[0] = val[0];
	for (int k = 1; k < N * N; ++k) maxim[k] = 0;
	int stateNum = 0; // this is actually the greatest index among current states 
	for (int i = 1; i <= 2 * N - 2; ++i)
	{
		// swap current states for prev_states
		int* tmp0 = prev_maxim;
		prev_maxim = maxim;
		maxim = tmp0;

		// update number of states which is given by the equation below
		stateNum = ((i <= N - 1) ? i : (2 * N - 2 - i));

		// update current states
		stateUpdate2(prev_maxim, stateNum, i, N, maxim);

	}
	int tmp = maxim[0];
	delete[] maxim;
	delete[] prev_maxim;
	maxim = NULL;
	prev_maxim = NULL;
	return tmp;
}

int main()
{
	int M, N;
	M = readNum();
	N = readNum();
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			val[i * N + j] = readNum();
		}
	}

	if (M == 1)
	{
		int result = getMaxim(N);
		printf("%d ", result);
	}
	else // M == 2
	{
		int result = getMaxim2(N);
		printf("%d ", result);
	}
	return 0;
}