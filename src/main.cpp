#include "Expr.h"
#include "LargeNumber.h"

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
using std::getline;

#include <string>
using std::string;

#include <list>
using std::list;

#include <vector>
using std::vector;


string read_command();

void split_string(vector<string>& target, const string& str, char delim = ' ', bool append = false);

void echo(const vector<string>& tokens);

void build_tree(Expr& ans, const vector<string>& tokens);


int main()
{
    Expr ans("0");
    vector<string> tokens;
    cout << "ans = 0" << endl;

    while(true)
    {
        split_string(tokens, read_command());
        if(tokens.size() == 0) continue;

        if(tokens[0] == "quit") break;
        else if(tokens[0] == "echo") echo(tokens);
        else
        {
            build_tree(ans, tokens);
            cout << "ans = " << ans.getString() << endl;
        }
    }

    return 0;
}


string read_command()
{
    string result;
    cout << ">>> ";
    getline(cin, result);
    return result;
}

void split_string(vector<string>& target, const string& str, char delim, bool append)
{
    if(!append) target.clear();
    int begin{0};
    for(int i{0}; i<=str.length(); ++i) if(i == str.length() || str.at(i) == delim)
    {
        if(begin != i) target.push_back(str.substr(begin, i-begin));
        begin = i+1;
    }
}

void build_tree(Expr& ans, const vector<string>& tokens)
{
    list<Expr> stack;
    for(vector<string>::const_reverse_iterator iter{tokens.rbegin()}; iter!=tokens.rend(); ++iter)
    {
        if(*iter == "+" || *iter == "-" || *iter == "*" || *iter == "/" || *iter == "%")
        {
            Expr lvalue = stack.back();
            stack.pop_back();
            Expr rvalue = stack.back();
            stack.pop_back();

            stack.push_back(Expr(*iter, lvalue, rvalue));
        }
        else if(*iter == "subst")
        {
            Expr target = stack.back();
            stack.pop_back();
            string from = stack.back().getString();
            stack.pop_back();
            string to = stack.back().getString();
            stack.pop_back();

            target.subst(from, to);
            stack.push_back(target);
        }
        else if(*iter == "eval")
        {
            Expr target = stack.back();
            stack.pop_back();

            stack.push_back(Expr(target.eval()));
        }
        else if(*iter == "ans")
            stack.push_back(ans);
        else
            stack.push_back(Expr(*iter));
    }

    ans = stack.front();
}

void echo(const vector<string>& tokens)
{
    for(int i{1}; i<tokens.size(); ++i) cout << tokens[i] << ' ';
    cout << endl;
}
