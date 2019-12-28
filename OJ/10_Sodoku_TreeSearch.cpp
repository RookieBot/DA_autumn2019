# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int sodoku[81];

// for each of the following 3 arrays: 
// each element uses a 9-bit, 
// each bit representing if corresponding number has been filled
// 1 means number is occupied, 0 means number is available
int colRecord[9][10];
int rowRecord[9][10];
int blkRecord[9][10];
int rowcnt[9];
int colcnt[9];

int coeff[] = {
	1,2,3,4,5,4,3,2,1,
	2,2,3,4,5,4,3,2,2,
	3,3,3,4,5,4,3,3,3,
	4,4,4,4,5,4,4,4,4,
	5,5,5,5,5,5,5,5,5,
	4,4,4,4,5,4,4,4,4,
	3,3,3,4,5,4,3,3,3,
	2,2,3,4,5,4,3,2,2,
	1,2,3,4,5,4,3,2,1
};

int blank = 81; // number of cells unfilled yet
int max_score = 0; // defaults to 0

inline int score()
{
	int s = 0;
	for (int ii = 0; ii < 9; ++ii)
	{
		for (int jj = 0; jj < 9; ++jj)
		{
			s += sodoku[ii * 9 + jj] * coeff[ii * 9 + jj];
		}
	}
	return s;
}

inline void backTrack(int lasti, int lastj, int lastk)
{
	// back tracking
	sodoku[lasti * 9 + lastj] = 0; // restore sodoku
	rowRecord[lasti][lastk] = 0; // restore row number availability state
	colRecord[lastj][lastk] = 0; // restore column number availability state
	blkRecord[(lasti / 3) * 3 + lastj / 3][lastk] = 0; // restore block number availability state
	++blank;
	--rowcnt[lasti];
	--colcnt[lastj];
}

void highScore(int i, int j, int lasti, int lastj, int lastk, int r_, int c_)
{
	// if there are no blanks left to fill, record score and return
	// printf("%d ", blank);
	if (blank == 0)
	{
		//debug_print(1);
		int cur_score = score();
		if (cur_score > max_score) max_score = cur_score; // should always do this on leaf nodes
		// back tracking
		backTrack(lasti, lastj, lastk);
		return;
	}

	// locate the next unfilled cell
	//while (sodoku[i * 9 + j] != 0)
	//{
	//	if (j < 8) ++j;
	//	else if (i < 8)
	//	{
	//		++i;
	//		j = 0;
	//	}
	//	else // all cells are filled
	//	{
	//		// set for returning
	//		int cur_score = score();
	//		if (cur_score > max_score) max_score = cur_score; // should always do this on leaf nodes
	//		//debug_print(2);
	//		backTrack(lasti, lastj, lastk);
	//		return;
	//	}
	//}

	// for each number in [1,9] that can be filled in cell(i,j), try it out
	for (int k = 1; k <= 9; ++k)
	{
		if (rowRecord[i][k] == 0 && colRecord[j][k] == 0 && blkRecord[(i / 3) * 3 + j / 3][k] == 0)
		{
			// everything in this code block needs backtracking later
			sodoku[i * 9 + j] = k;
			// update number availability state 
			rowRecord[i][k] = 1;
			colRecord[j][k] = 1;
			blkRecord[(i / 3) * 3 + j / 3][k] = 1;
			--blank;
			++rowcnt[i];
			++colcnt[j];

			if (blank == 0) // if the last blank was just filled
			{
				//debug_print(1);
				int cur_score = score();
				if (cur_score > max_score) max_score = cur_score; // should always do this on leaf nodes
				// back tracking
				backTrack(i, j, k);
				continue;
			}

			// select the next blank to fill
			int r = -1, c = -1;
			int rowcnt_max = -1, colcnt_max = -1;
			for (int t1 = 0; t1 < 9; ++t1)
			{
				if (rowcnt[t1] > rowcnt_max && rowcnt[t1] < 9)
				{
					r = t1;
					rowcnt_max = rowcnt[t1];
				}
			}
			for (int t2 = 0; t2 < 9; ++t2)
			{
				if (colcnt[t2] > colcnt_max && colcnt[t2] < 9 && sodoku[r * 9 + t2] == 0)
				{
					c = t2;
					colcnt_max = colcnt[t2];
				}
			}

			// go deeper
			highScore(r, c, i, j, k, r_, c_);
		}
	}

	if (i == r_ && j == c_) return; // search finishes upon arriving back at the root
	else backTrack(lasti, lastj, lastk);
}

int main()
{
	int tmp;
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			scanf("%d", &tmp);
			sodoku[i * 9 + j] = tmp;
			if (tmp != 0)
			{
				--blank;
				// update number availability state
				rowRecord[i][tmp] = 1;
				colRecord[j][tmp] = 1;
				blkRecord[(i / 3) * 3 + j / 3][tmp] = 1;
				++rowcnt[i];
				++colcnt[j];
			}
		}
	}
	int r = -1, c = -1;
	int rowcnt_max = -1, colcnt_max = -1;
	for (int t1 = 0; t1 < 9; ++t1)
	{
		if (rowcnt[t1] > rowcnt_max && rowcnt[t1] < 9)
		{
			r = t1;
			rowcnt_max = rowcnt[t1];
		}
	}
	for (int t2 = 0; t2 < 9; ++t2)
	{
		if (colcnt[t2] > colcnt_max && colcnt[t2] < 9 && sodoku[r * 9 + t2] == 0)
		{
			c = t2;
			colcnt_max = colcnt[t2];
		}
	}
	highScore(r, c, 0, 0, 0, r, c);
	/*printf("final result\n");
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
			printf("%d ", sodoku[i * 9 + j]);
		printf("\n");
	}*/
	printf("%d ", max_score);
	return 0;
}