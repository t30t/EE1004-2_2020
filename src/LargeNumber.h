//LargeNumber.h
//Declaration of class LargeNumber

#pragma once

#include <ostream>
using std::ostream;

#include <istream>
using std::istream;

#include <string>
using std::string;

#include <cmath>
using std::max;
using std::log10;


class LargeNumber
{
public:
	// null constructor, equivalent to LargeNumber(0)
	LargeNumber();
	// LargeNumber a("123456")
	explicit LargeNumber(const string &raw);
	// LargeNumber a(1)
	LargeNumber(int raw);  // declared non-explicit as intended

	// copy constructor
	LargeNumber(const LargeNumber &ref);
	LargeNumber& operator=(const LargeNumber&);

	~LargeNumber();

	LargeNumber operator+(const LargeNumber&) const;
  	LargeNumber operator-(const LargeNumber&) const;
	LargeNumber operator*(const LargeNumber&) const;
	LargeNumber operator/(const LargeNumber&) const;
	LargeNumber operator%(const LargeNumber&) const;

	LargeNumber& trim();

	operator string() const;

	friend ostream& operator<<(ostream&, const LargeNumber&);
	friend istream& operator>>(istream&, LargeNumber&);

private:
	bool m_negative;
	unsigned char* m_digits;
	int m_size;

	// second integer is just a placeholder
	LargeNumber(int size, int);

	int getDigit(int index) const;
	LargeNumber& setDigit(int index, int value);

	LargeNumber& lshift1();
	LargeNumber negative() const;
	LargeNumber abs() const;

	bool abs_larger_than(const LargeNumber&) const;
};
