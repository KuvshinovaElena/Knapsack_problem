#include "Knapsack_task.h"

int main()
{
	vector<long> key({ 2,3,6,13,27,52 });
	Knapsack_task::encrypt("gf",key,"k");
	Knapsack_task::decrypt("",key,"");
	return 0;
}