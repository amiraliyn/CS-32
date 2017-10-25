#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;


void spaces(string &infix);
string infixtoPostfix(string infix, string& postfix);
bool valid(string infix);
int prec(char operators);
int evaluate(string infix, const bool values[], string& postfix, bool& result);

int main()
{
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false, true,  false
	};
	string pf;
	bool answer;
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
		&& pf == "43!03&&|" && !answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);
	cout << "Passed all tests" << endl;
}

void spaces(string &infix)
{
	string newString = "";
	for (int pos = 0; pos < infix.size(); pos++)
	{
		if (infix[pos] != ' ')
			newString += infix[pos];
	}
	infix = newString;
}

string infixtoPostfix(string infix, string& postfix)
{
	postfix = "";
	stack<char> op;
	for (int pos = 0; pos < infix.size(); pos++)
	{
		char ch = infix[pos];
		switch (ch)
		{
		case('0'):case('1'):case('2'):case('3'):case('4'):case('5'):case('6'):case('7'):case('8'):case('9'):
			postfix += ch;
			break;
		case ('('):
			op.push(ch);
			break;
		case (')'):
			while (op.top() != '(')
			{
				postfix += op.top();
				op.pop();
			}
			op.pop();
			break;
		case('!'):case('&'):case('|'):
			while (!op.empty() && op.top() != '(' && prec(ch) <= prec(op.top()))
			{
				postfix += op.top();
				op.pop();
			}
			op.push(ch);
			break;
		default:
			break;
		}
	}
	while (!op.empty())
	{
		postfix += op.top();
		op.pop();
	}
	return postfix;
}

bool valid(string infix)
{
	spaces(infix);
	if (infix.size() == 0)
		return false;
	stack<char> paran;
	for (int i = 0; i < infix.size(); i++) 
	{
		char ch = infix[i];
		if (ch == '(') 
			paran.push(ch);
		if (ch == ')')
		{
			if (paran.top() != '(') 
				return false;
			paran.pop();
		}
	}
	if (!paran.empty())
		return false;
	for (int pos = 0; pos < infix.size(); pos++)
	{
		if (infix[0] == '&' || infix[0] == '|')
			return false;
		if (infix[infix.size() - 1] == '&' || infix[infix.size() - 1] == '|')
			return false;
		if (infix[pos] >= '0' && infix[pos] <= '9')
		{
			if ((infix[pos + 1] >= '0' && infix[pos + 1] <= '9') || infix[pos + 1] == '!' || infix[pos + 1] == '(')
				return false;
		}
	}
	return true;
}


int prec(char operators)
{
	if (operators == '!')
		return 3;
	else if (operators == '&')
		return 2;
	else if (operators == '|')
		return 1;
	else
		return 0;
}

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	if (!valid(infix))
		return 1;
	postfix = infixtoPostfix(infix, postfix);
	if (postfix.size() == 0)
		return 1;
	stack <bool> eval;
	for (int pos = 0; pos < postfix.size(); pos++)
	{
		if (postfix[pos] == '!')
		{
			bool temp = eval.top();
			eval.pop();
			eval.push(!temp);
		}
		else if (postfix[pos] == '&' || postfix[pos] == '|')
		{
			bool operand2 = eval.top();
			eval.pop();
			if (eval.empty())
				return 1;
			bool operand1 = eval.top();
			eval.pop();
			if (postfix[pos] == '&')
			{
				if (operand1 && operand2)
					eval.push(true);
				else
					eval.push(false);
			}
			if (postfix[pos] == '|')
			{
				if (operand1 || operand2)
					eval.push(true);
				else
					eval.push(false);
			}
		}
		else
		{
			int index = postfix[pos] - '0';
			if (values[index])
				eval.push(true);	
			if (!values[index])
				eval.push(false);
		}
	}
	result = eval.top();
	return 0;
}