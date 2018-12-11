#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;
#define TABLE_COLS 8
#define TABLE_RWO 15

enum Vt { add='+', sub='-', multi='*', divide='/', lp='(', rp='RP', num, $='$' } vt;

enum Vn { E='E', T='T', F='F' } vn;

typedef pair<int, string> cell;
typedef map<int, string> row;

vector<int> state{0};
vector<int> symble{-1};
vector<int> input = {3, multi, lp, 5, add, 4, divide, 2, rp}; //input 3*(5+4/2)

template <typename T> // must before the func
void test(T a)
{
	// simple test function
	cout << a << endl;
}

void init();
// SLR(1) TABLE
// row table[TABLE_COLS];
vector<row> table;

int main(int argc, char* argv[])
{
	init();
	test(table[1][$]);
	system("pause");
	return 0;
	// table[0][lp] = "S6"; // insert cannot update map
	// cout << static_cast<int>('+') << endl; 
	// col.insert(cell(lp, "S4"));
	// col.insert(cell(num, "S5"));
	// test(col[lp][0]); // S
	/*
	int a = 1;
	vector<int>t;
	t.push_back(a);
	a++;
	t.push_back(a);
	 */
}

void add_reduce_via_follow(initializer_list<Vt> v, string action, row& col)
{
	for (auto element : v)
	{
		col[element] = action;
	}
}

void init()
{
	auto FOLLOW_E = {add, sub, $};
	auto FOLLOW_T = {add, sub, multi, divide, rp, $};
	auto FOLLOW_F = {add, sub, multi, divide, rp, $};
	row col{cell(lp, "S4"), cell(num, "S5")}; // row 0
	table.push_back(col);
	col[add] = "S6", col[sub] = "S7", col[$] = "R1";
	table.push_back(col); // row1
	col[multi] = "S8", col[divide] = "S9";
	// for (auto element : FOLLOW_E)
	// {
	// 	col[element] = "R4";
	// }
	add_reduce_via_follow(FOLLOW_E, "R4", col);
	table.push_back(col); // row2
	// for (auto element : FOLLOW_T)
	// {
	// 	col[element] = "R7";
	// }
	add_reduce_via_follow(FOLLOW_T, "R7", col);
	table.push_back(col); //row 3
	col[lp] = "S4";
	table.push_back(col);// row4
	add_reduce_via_follow(FOLLOW_F, "R9", col);
	table.push_back(col); // row5
	col[lp] = "S4", col[num] = "S5";
	table.push_back(col);// r6
	col[lp] = "S4"; //r7
	table.push_back(col);
	col[lp] = "S4", col[num] = "S5"; // r8
	table.push_back(col);
	table.push_back(col); // r9 == r8 push twice
	col[add] = "S6", col[sub] = "S7", col[rp] = "S15"; 
	table.push_back(col);//r10
	col[multi] = "S8", col[divide] = "S9"; 
	add_reduce_via_follow(FOLLOW_E, "S2", col);
	table.push_back(col);//r11
	table.push_back(col); //r12=r11
	add_reduce_via_follow(FOLLOW_T, "R5", col);
	table.push_back(col); // r13
	add_reduce_via_follow(FOLLOW_T, "R6", col);
	table.push_back(col);//r14
	add_reduce_via_follow(FOLLOW_F, "R8", col);
	table.push_back(col);//r15

}
