//Expr.h
//Declaration of class expr_node, binary_node, uni_node, Expr
//Modify class expr_node, uni_node, and binary node when implementing Part.E 

#pragma once

#include <string>
using std::string;

#include "LargeNumber.h"


class Expr;

class expr_node
{
public:
    expr_node();
    virtual ~expr_node() {}
    virtual LargeNumber eval() const = 0;
    virtual void subst(const string&, const string&) = 0;
    virtual string getString() const = 0;

protected:
    int m_use_count;

friend class Expr;
};

class Expr
{
public:
    explicit Expr(const string& operand);
    Expr(const string& op, Expr left, Expr right);

    Expr(const Expr& expr);
    Expr& operator=(const Expr& expr);

    ~Expr();

    LargeNumber eval() const;
    string getString() const;
    void subst(const string& from, const string& to);
    
protected:
    expr_node *m_ptr;

    void unhook();
};

class uni_node: public expr_node
{
private:
    string m_value;
public:
    explicit uni_node(const string& value);
    ~uni_node();
    LargeNumber eval() const;
    void subst(const string& from, const string& to);
    string getString() const;
};

class binary_node: public expr_node
{
private:
    string m_op;
    Expr m_lchild;
    Expr m_rchild;
public:
    binary_node(const string& op, const Expr& lchild, const Expr& rchild);
    ~binary_node(); // should also delete its subtree recursively
    LargeNumber eval() const;
    void subst(const string& from, const string& to);
    string getString() const;
};
