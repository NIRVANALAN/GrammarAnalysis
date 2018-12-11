#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <stack>
using namespace std;
#define TABLE_COLS 8
#define TABLE_RWO 15
#define GENERATIVE_NUM 9

enum Vt { add='+', sub='-', multi='*', divide='/', lp='(', rp='RP', num, $='$' } vt;

enum Vn { E='E', T='T', F='F' } vn;

vector<vector<string>> generative;
auto FOLLOW_E = {add, sub, $};
auto FOLLOW_T = {add, sub, multi, divide, rp, $};
auto FOLLOW_F = {add, sub, multi, divide, rp, $};
typedef pair<int, string> cell;
typedef map<int, string> row;
typedef vector<string> expression;

stack<int> state;
vector<int> symble{-1};
vector<int> input = {3, multi, lp, 5, add, 4, divide, 2, rp, $}; //input 3*(5+4/2)

template <typename T> // must before the func
void test_out(T a)
{
	// simple test_out function
	cout << a << endl;
}

void init();
void analysis();
// SLR(1) TABLE
vector<row> table;
// row table[TABLE_COLS];

int main(int argc, char* argv[])
{
	init();
	// analysis();
	test_out(generative[0][0]);
	system("pause");
	return 0;
	// char a = '5';
	// test_out(a - '0');
	// int a = 1;
	// vector<int>t;
	// t.push_back(a);
	// a++;
	// t.push_back(a);
	// t.erase(t.begin());
	// test_out(t.front());
	// string ss = "asd";
	// test_out(ss[0] == 'a');
	// test_out(table[1][$]);
	// table[0][lp] = "S6"; // insert cannot update map
	// cout << static_cast<int>('+') << endl; 
	// col.insert(cell(lp, "S4"));
	// col.insert(cell(num, "S5"));
	// test_out(col[lp][0]); // S
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
	// init the state
	state.push(0);
	// init the generative
	expression e{"S", "E"};
	generative.push_back(e);//1
	e[0] = "E", e[1] = "E+T";
	generative.push_back(e);//2
	e[0] = "E", e[1] = "E-T";
	generative.push_back(e);//3
	e[0] = "E", e[1] = "T";
	generative.push_back(e);//4
	e[0] = "T", e[1] = "T*F";
	generative.push_back(e);//5
	e[1] = "T/F";
	generative.push_back(e);//6
	e[1] = "F";
	generative.push_back(e);//7
	e[0] = "F", e[1] = "(E)";
	generative.push_back(e);//8
	e[1] = "num";
	generative.push_back(e);//9

	// init the table
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
	table.push_back(col); // row4
	add_reduce_via_follow(FOLLOW_F, "R9", col);
	table.push_back(col); // row5
	col[lp] = "S4", col[num] = "S5";
	table.push_back(col); // r6
	col[lp] = "S4"; //r7
	table.push_back(col);
	col[lp] = "S4", col[num] = "S5"; // r8
	table.push_back(col);
	table.push_back(col); // r9 == r8 push twice
	col[add] = "S6", col[sub] = "S7", col[rp] = "S15";
	table.push_back(col); //r10
	col[multi] = "S8", col[divide] = "S9";
	add_reduce_via_follow(FOLLOW_E, "S2", col);
	table.push_back(col); //r11
	table.push_back(col); //r12=r11
	add_reduce_via_follow(FOLLOW_T, "R5", col);
	table.push_back(col); // r13
	add_reduce_via_follow(FOLLOW_T, "R6", col);
	table.push_back(col); //r14
	add_reduce_via_follow(FOLLOW_F, "R8", col);
	table.push_back(col); //r15
}

void analysis()
{
	auto index_input = 0;
	while (true)
	{
		auto s = state.top();
		if (table[s][input[index_input]][0] == 'S')
		{
			auto new_state_top = table[s][input[index_input]][1] - int('0');
			state.push(new_state_top);
			symble.push_back(input.front());
			index_input++;
		}
		if (table[s][input[index_input]][0] == 'R')
		{
		}
	}
}
