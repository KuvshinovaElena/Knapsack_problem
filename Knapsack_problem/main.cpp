#include "Knapsack_task.h"
int main()
{
	vector<long> key({ 2, 3, 6, 13, 27, 52, 104, 208 });
	long n = 101;
	long mod = 420;
	bool test = Knapsack_task::encrypt("input.txt", key, "output.txt", n, mod);
	test = Knapsack_task::decrypt("output.txt", key, "decrypt.txt", n, mod);
	return 0;
}