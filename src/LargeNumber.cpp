//LargeNumber.cpp
//implementation of class LargeNumber

#include "LargeNumber.h"


LargeNumber::LargeNumber(): m_negative{false}, m_size{1}
{
    m_digits = new unsigned char[1];
    m_digits[0] = 0;
}

LargeNumber::LargeNumber(const string& raw): m_negative{false}, m_size{signed(raw.length())}
{
    m_digits = new unsigned char[m_size];
    for(int i{0}; i<m_size; ++i) m_digits[i] = (unsigned char)(raw.at(m_size - i - 1) - '0');
    if(raw.at(0) == '-')
    {
        m_digits[m_size-1] = 0;
        m_negative = true;
    }
}

LargeNumber::LargeNumber(int raw): m_negative{raw<0}
{
    if(raw < 0) raw *= -1;
    m_size = (raw == 0) ? 1 : int(log10(raw))+1;
    m_digits = new unsigned char[m_size];
    for(int i{0}; i<m_size; ++i)
    {
        m_digits[i] = (unsigned char)(raw % 10);
        raw /= 10;
    }
}

LargeNumber::LargeNumber(int size, int): m_negative{false}, m_size{size}
{
    m_digits = new unsigned char[m_size];
    for(int i{0}; i<m_size; ++i) m_digits[i] = 0;
}

LargeNumber::LargeNumber(const LargeNumber& other): m_negative{other.m_negative}, m_size{other.m_size}
{
    m_digits = new unsigned char[m_size];
    for(int i{0}; i<m_size; ++i) m_digits[i] = other.m_digits[i];
}

LargeNumber& LargeNumber::operator=(const LargeNumber& other)
{
    if(m_digits != nullptr) delete[] m_digits;

    m_negative = other.m_negative;
    m_size = other.m_size;

    m_digits = new unsigned char[m_size];
    for(int i{0}; i<m_size; ++i) m_digits[i] = other.m_digits[i];

    return *this;
}

LargeNumber::~LargeNumber()
{
    if(m_digits != nullptr) delete[] m_digits;
}

int LargeNumber::getDigit(int index) const
{
    if(0 <= index && index < m_size) return m_digits[index];
    else return 0;
}

LargeNumber& LargeNumber::setDigit(int index, int value)
{
    if(0 <= index && index < m_size) m_digits[index] = value;
    
    return *this;
}

LargeNumber& LargeNumber::lshift1()
{
    for(int i{m_size-1}; i>0; --i) m_digits[i] = m_digits[i-1];
    m_digits[0] = 0;

    return *this;
}

LargeNumber LargeNumber::negative() const
{
    LargeNumber result(*this);
    result.m_negative = !result.m_negative;

    return result;
}

LargeNumber LargeNumber::abs() const
{
    return m_negative ? this->negative() : *this;
}

bool LargeNumber::abs_larger_than(const LargeNumber& other) const
{
    for(int i{max(m_size, other.m_size)-1}; i>=0; --i) if(getDigit(i) != other.getDigit(i))
    {
        if(getDigit(i) > other.getDigit(i)) return true;
        else return false;
    }
    return false;
}

LargeNumber& LargeNumber::trim()
{
    int new_len{1};
    for(int i{m_size-1}; i>0; --i) if(m_digits[i] != 0)
    {
        new_len = i + 1;
        break;
    }
    if(m_size == new_len) return *this;  // no need to trim

    unsigned char* digits = new unsigned char[new_len];
    for(int i{0}; i<new_len; ++i) digits[i] = m_digits[i];

    delete[] m_digits;
    m_digits = digits;
    m_size = new_len;

    return *this;
}

LargeNumber LargeNumber::operator+(const LargeNumber& other) const
{
    if(!m_negative && other.m_negative) return *this - other.negative();  // A + (-B) = A - B
    if(m_negative && !other.m_negative) return other - this->negative();  // (-A) + B = B - A
    if(m_negative && other.m_negative) return (this->negative() + other.negative()).negative();  // (-A) + (-B) = -(A + B)

    LargeNumber result(max(m_size, other.m_size)+1, 0);
    int carry{0};
    for(int i{0}; i<result.m_size; ++i)
    {
        int m = getDigit(i) + other.getDigit(i) + carry;
        carry = m / 10;
        result.setDigit(i, m % 10);
    }

    return result;
}

LargeNumber LargeNumber::operator-(const LargeNumber& other) const
{
    if(!m_negative && other.m_negative) return *this + other.negative();  // A - (-B) = A + B
    if(m_negative && !other.m_negative) return (this->negative() + other).negative();  // (-A) - B = -(A + B)
    if(m_negative && other.m_negative) return other.negative() - this->negative();  // (-A) - (-B) = B - A

    if(other.abs_larger_than(*this)) return (other - *this).negative();  // A - B = -(B - A)

    LargeNumber result(max(m_size, other.m_size), 0);
    int carry{0};
    for(int i{0}; i<result.m_size; ++i)
    {
        int m = getDigit(i) - other.getDigit(i) + carry;
        if(m < 0)
        {
            m += 10;
            carry = -1;
        }
        else carry = 0;
        result.setDigit(i, m % 10);
    }

    return result;
}

LargeNumber LargeNumber::operator*(const LargeNumber& other) const
{
    LargeNumber result(m_size + other.m_size, 0);
    result.m_negative = m_negative ^ other.m_negative;
    int carry{0};
    for(int i{0}; i<result.m_size; ++i)
    {
        int m{carry};
        for(int j{0}; j<=i; ++j) m += getDigit(j) * other.getDigit(i-j);
        carry = m / 10;
        result.setDigit(i, m % 10);
    }

    return result;
}

LargeNumber LargeNumber::operator/(const LargeNumber& other) const
{
    LargeNumber result(m_size, 0);
    LargeNumber remainder(other.m_size + 1, 0);
    result.m_negative = m_negative ^ other.m_negative;
    for(int i{result.m_size-1}; i>=0; --i)
    {
        remainder.lshift1().setDigit(0, getDigit(i));
        for(int j{1}; j<=10; ++j)
        {
            LargeNumber guess(other * j);  // an example of implicit casting by single argument ctor
            if(guess.abs_larger_than(remainder))
            {
                result.setDigit(i, j-1);
                break;
            }
        }
        remainder = remainder - other.abs() * result.getDigit(i);
    }

    return result;
}

LargeNumber LargeNumber::operator%(const LargeNumber& other) const
{
    return *this - (*this / other) * other;
}

LargeNumber::operator string() const
{
    string result;
    int i;
    for(i=m_size-1; i>0; --i) if(m_digits[i] != 0) break;
    if(m_negative && !(i == 0 && m_digits[0] == 0)) result.push_back('-');
    for(; i>=0; --i) result.push_back(char(m_digits[i] + '0'));

    return result;
}

ostream& operator<<(ostream& os, const LargeNumber& num)
{
    os << string(num);

    return os;
}

istream& operator>>(istream& is, LargeNumber& num)
{
    string str;
    is >> str;
    num = LargeNumber(str);

    return is;
}
