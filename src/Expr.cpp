//Expr.cpp
//implementation of functions declared in Expr.h

#include "Expr.h"


expr_node::expr_node(): m_use_count{1}
{
    // nothing to do
}

uni_node::uni_node(const string& value): m_value{value}
{
    // nothing to do
}

uni_node::~uni_node()
{
    // nothing to do
}

LargeNumber uni_node::eval() const
{
    return LargeNumber(m_value);
}

void uni_node::subst(const string& from, const string& to)
{
    if(m_value == from) m_value = to;
}

string uni_node::getString() const
{
    return m_value;
}


binary_node::binary_node(const string& op, const Expr& lchild, const Expr& rchild): m_op{op}, m_lchild{lchild}, m_rchild{rchild}
{
    // nothing to do
}

binary_node::~binary_node()
{
    // nothing to do
}

LargeNumber binary_node::eval() const
{
    if(m_op == "+") return m_lchild.eval() + m_rchild.eval();
    if(m_op == "-") return m_lchild.eval() - m_rchild.eval();
    if(m_op == "*") return m_lchild.eval() * m_rchild.eval();
    if(m_op == "/") return m_lchild.eval() / m_rchild.eval();
    if(m_op == "%") return m_lchild.eval() % m_rchild.eval();
    return 0;
}

void binary_node::subst(const string& from, const string& to)
{
    m_lchild.subst(from, to);
    m_rchild.subst(from, to);
}

string binary_node::getString() const
{
    return m_op + " " + m_lchild.getString() + " " + m_rchild.getString();
}


Expr::Expr(const string& operand)
{
    m_ptr = new uni_node(operand);
}

Expr::Expr(const string& op, Expr left, Expr right)
{
    m_ptr = new binary_node(op, left, right);
}

Expr::~Expr()
{
    unhook();
}

void Expr::unhook()
{
    --(m_ptr->m_use_count);
    if(m_ptr->m_use_count == 0) delete m_ptr;
}

Expr::Expr(const Expr& expr): m_ptr{expr.m_ptr}
{
    ++(m_ptr->m_use_count);
}

Expr& Expr::operator=(const Expr& expr)
{
    if(m_ptr == expr.m_ptr) return *this;

    unhook();

    m_ptr = expr.m_ptr;
    ++(m_ptr->m_use_count);

    return *this;
}

LargeNumber Expr::eval() const
{
    return m_ptr->eval();
}

void Expr::subst(const string& from, const string& to)
{
    m_ptr->subst(from, to);
}

string Expr::getString() const
{
    return m_ptr->getString();
}
