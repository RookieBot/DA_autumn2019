#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits>

double W[200]; // weights, initialized to zeros(200) as a global varaible 
double dW[200]; // gradient of weights W

//void gradient(double* X, double* W, double y, double* result, double loss, int M)
//{
//	// performs dot product <X, W>
//	double dot = 0.0;
//	for (int i = 0; i < M; ++i)
//		dot += X[i] * W[i];
//	loss = (y-1.0/(1.0+exp(-1.0*dot))) * (y - 1.0 / (1.0 + exp(-1.0 * dot)));
//	double tmp = 1.0 - y - 1.0 / (1.0 + exp(dot));
//	
//	// calculate gradient without L2 regularization
//	for (int i = 0; i < M; ++i)
//		result[i] = tmp * X[i]; // +0.01 * W[i];
//}

void gradient(double* X, double* W, double y, double* result, int M)
{
	// performs dot product <X, W>
	double dot = 0.0;
	for (int i = 0; i < M; ++i)
		dot += X[i] * W[i];
	// loss = (y - 1.0 / (1.0 + exp(-1.0 * dot))) * (y - 1.0 / (1.0 + exp(-1.0 * dot)));
	// log(std::numeric_limits<double>::max())
	//if (dot < 709.0) tmp = 1.0 - y - 1.0 / (1.0 + exp(dot));
	//else tmp = 0.999 - y;
	double tmp = 1.0 - y - 1.0 / (1.0 + exp(dot));

	// calculate gradient without L2 regularization
	for (int i = 0; i < M; ++i)
		result[i] = tmp * X[i]; // +0.001 * W[i];
}

// double LR[] = { 1.0, 0.1, 0.01, 0.001 };

int main()
{
	// printf("%lf\n", exp(710.0)); // inf
    // take inputs
	int M, N; // M: #demension, N: dataset size
	scanf("%d", &M);
	scanf("%d", &N);
	double* y = new double[N];
	double* X = new double[M * N];

	for (int i = 0; i < N; ++i)
	{
		scanf("%lf", y + i);
		for (int j = 0; j < M; ++j)
		{
			scanf("%lf", X + i * M +j);
		}
	}


	// SGD
	int batchsize = 50;
	int epochs = 170;
	int nbatches = epochs * N / batchsize;
	int k = 0; // tracks the number of iterations
	double alpha = 100; // learning rate
	// double loss = 0.0;
	double momentum = 0.1;
	double* moment = new double[M];
	for (int i = 0; i < M; ++i) moment[i] = 0.0;
	while (k < nbatches) 
	{
		// an iteration
		//if (k <= nbatches / 3) alpha = 0.5 / (k + 1.0); // diminishing step size
		//else alpha = 0.1 / (k + 1.0);
		//alpha = 600.0 / (k + 1.0);

		for (int j = 0; j < M; ++j)
		{
			dW[j] = 0;
		}
		// double loss_tmp0 = 0.0;
		for (int j = k * batchsize; j < (k + 1) * batchsize; ++j)
		{
			double* tmp = new double[M];
			// double loss_tmp1;
			int sampleIdx = j % N; // circle rule
			// int sampleIdx = rand() % N; // stochastic
			gradient(X + sampleIdx * M, W, y[sampleIdx], tmp, M);
			for (int m = 0; m < M; ++m)
			{
				dW[m] += tmp[m];
			}
			// loss_tmp0 += loss_tmp1;
			delete[] tmp;
		}
		// loss = (loss * k + loss_tmp0 / batchsize) / (k + 1);
		// if (loss <= 1e-10) break;
		++k;

		// update moments and weights
		for (int i = 0; i < M; ++i)
		{
			moment[i] = momentum * moment[i] - alpha * dW[i] / batchsize;
			// dW[i] is the sum of gradient of batchsize number of samples
			W[i] += moment[i];
		}

		
	}

	// normalize
	double norm_W = 0.0; // (square of) 2-norm of gradient
	for (int i = 0; i < M; ++i)
	{
		norm_W += W[i] * W[i];
	}
	norm_W = sqrt(norm_W);
	//if (norm_W <= 1e-8) norm_W = 1e-8;
	for (int i = 0; i < M; ++i)
	{
		W[i] /= norm_W;
	}

	// print result
	for (int i = 0; i < M; ++i)
	{
		printf("%lf ", W[i]);
	}

	return 0;
}