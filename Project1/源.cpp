#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <stack>
using namespace std;
#define TABLE_COLS 8
#define TABLE_RWO 15
#define GENERATIVE_NUM 9

enum Vt { add = -'+', sub = -'-', multi = -'*', divide = -'/', lp = -'(', rp = -')', num = 0, $ = -'$' } vt;

enum Vn { E = 'E', T = 'T', F = 'F' } vn;

vector<vector<string>> generative;
auto FOLLOW_E = { add, sub, $ };
auto FOLLOW_T = { add, sub, multi, divide, rp, $ };
auto FOLLOW_F = { add, sub, multi, divide, rp, $ };
typedef pair<int, string> cell;
typedef map<int, string> row;
typedef vector<string> expression;
typedef vector<int> arithmetic_exp;

stack<int> state;
stack<int> symble;
arithmetic_exp input = { 3, multi, lp, 5, add, 4, divide, 2, rp, $ }; //input 3*(5+4/2)
arithmetic_exp input_test = { 3, add, 2, $ }; // test input 3+2
arithmetic_exp arithmetic_expression;
template <typename T> // must before the func
void println(T a)
{
	// simple println function
	cout << a << endl;
}

void init();
void analysis(arithmetic_exp input);
void print_arithmetic_exp(arithmetic_exp input);
// SLR(1) TABLE
vector<row> table;
// row table[TABLE_COLS];

int main(int argc, char* argv[])
{
	arithmetic_expression = input_test;
	print_arithmetic_exp(arithmetic_expression);
	init();
	analysis(arithmetic_expression);
	system("pause");
	return 0;
	// row coc{cell(-1, "aa")};
	// println(coc[-1]);
	// println(generative[0][0]);
	// string s = "R15";
	// s.erase(s.begin());
	// auto i = stoi(s, nullptr, 0);
	// println(i+!);

	// char a = '5';
	// println(a - '0');
	// int a = 1;
	// vector<int>t;
	// t.push_back(a);
	// a++;
	// t.push_back(a);
	// t.erase(t.begin());
	// println(t.front());
	// string ss = "asd";
	// println(ss[0] == 'a');
	// println(table[1][$]);
	// table[0][lp] = "S6"; // insert cannot update map
	// cout << static_cast<int>('+') << endl;
	// col.insert(cell(lp, "S4"));
	// col.insert(cell(num, "S5"));
	// println(col[lp][0]); // S
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
	// init the symble
	symble.push(0);
	// init the generative
	expression e{ "S", "E" };
	generative.push_back(e); //1
	e[0] = "E", e[1] = "E+T";
	generative.push_back(e); //2
	e[0] = "E", e[1] = "E-T";
	generative.push_back(e); //3
	e[0] = "E", e[1] = "T";
	generative.push_back(e); //4
	e[0] = "T", e[1] = "T*F";
	generative.push_back(e); //5
	e[1] = "T/F";
	generative.push_back(e); //6
	e[1] = "F";
	generative.push_back(e); //7
	e[0] = "F", e[1] = "(E)";
	generative.push_back(e); //8
	e[1] = "num";
	generative.push_back(e); //9

	// init the table
	row col{ cell(lp, "S4"), cell(num, "S5"), cell(E, "1"), cell(T, "2"), cell(F, "3") }; // row 0
	table.push_back(col);
	col[add] = "S6", col[sub] = "S7", col[$] = "ACC";
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
	col[lp] = "S4", col[E] = "10", col[T] = "2", col[F] = "3";
	table.push_back(col); // row4
	add_reduce_via_follow(FOLLOW_F, "R9", col);
	table.push_back(col); // row5
	col[lp] = "S4", col[num] = "S5", col[T] = "11", col[F] = "3";
	table.push_back(col); // r6
	col[lp] = "S4", col[E] = "4", col[T] = "12", col[F] = "3"; //r7
	table.push_back(col);
	col[lp] = "S4", col[num] = "S5", col[F] = "13"; // r8
	table.push_back(col);
	col[F] = "14";
	table.push_back(col); // r9
	col[add] = "S6", col[sub] = "S7", col[rp] = "S15";
	table.push_back(col); //r10
	col[multi] = "S8", col[divide] = "S9";
	add_reduce_via_follow(FOLLOW_E, "R2", col);
	table.push_back(col); //r11
	table.push_back(col); //r12=r11
	add_reduce_via_follow(FOLLOW_T, "R5", col);
	table.push_back(col); // r13
	add_reduce_via_follow(FOLLOW_T, "R6", col);
	table.push_back(col); //r14
	add_reduce_via_follow(FOLLOW_F, "R8", col);
	table.push_back(col); //r15
}

int is_number(string s) // check string is a num
{
	stringstream sin(s);
	int t;
	if (sin >> t)
	{
		return t;
	}
	return -1;
}

void analysis(arithmetic_exp input)
{
	cout << "analysis procedure:" << endl;
	auto index_input = 0;
	while (true)
	{
		// auto s = state.top();
		if (input[index_input] > 0)
		{
			input[index_input] = 0;
		}
		if (table[state.top()][input[index_input]][0] == 'S')
		{
			auto shift_action = table[state.top()][input[index_input]]; // like: S15
			shift_action.erase(shift_action.begin()); // erase 'S'
			auto new_state_top = stoi(shift_action, nullptr, 0);
			// auto new_state_top = table[state.top()][input[index_input]][1] - int('0');
			state.push(new_state_top);
			symble.push(input[index_input]);
			index_input++;
			// output arithmetic_expression
			cout << "Shift " << new_state_top << endl;
			continue;
		}
		if (table[state.top()][input[index_input]][0] == 'R')
		{
			auto reduce_state = table[state.top()][input[index_input]][1];
			auto g = generative[reduce_state - '0' - 1]; // only 9 generative
			if (g[1][0] != 'n') // !=NUM
			{
				for (int i = 0; i < g[1].length(); ++i) // pop |¦Â| from state
				{
					state.pop();
					symble.pop();
				}
			}
			else
			{
				state.pop();
				symble.pop();
			}
			symble.push(g[0].data()[0]);
			auto new_state_top = table[state.top()][static_cast<int>(g[0].data()[0])]; //goto[S',A] convert 'E' to int
			// new_state_top is a string, like "13"
			int int_state_top = stoi(new_state_top, nullptr, 0); // convert 13 to int
			state.push(int_state_top);
			cout << "reduce by " << g[0] << "->" + g[1] << endl;
			continue;
		}
		if (table[state.top()][input[index_input]] == "ACC")
		{
			println("ACC");
			break;
		}
		println("error");
	}
}

void print_arithmetic_exp(arithmetic_exp input)
{
	cout << "expression input: ";
	for (auto element : input)
	{
		if (element > 0)
		{
			cout << element << " ";
		}
		else
		{
			cout << static_cast<char>(-element) << " ";
		}
	}
	cout << endl;
}