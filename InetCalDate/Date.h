#pragma once

#include <iostream>
using namespace std;

// 函数重载：可以让函数名相同，参数不同的函数存在
// 运算符重载：让自定义类型可以使用运算符，控制运算符的行为，增强可读性
// 多个同一运算符重载可以构成函数重载

class Date {
public:
	int getMonthDay(int year, int month) {
		static int MonthDay[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		if (month == 2) {
			if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
				return 29;
			}
			else {
				return 28;
			}
		}

		return MonthDay[month];
	}

	Date(const int year = 1, const int month = 1, const int day = 1);

	Date(const Date& origin);

	~Date();

	void Print() const;

	Date operator=(const Date& D);

	// ==
	bool operator==(const Date& D) const;

	// >
	bool operator>(const Date& D) const;

	// >= 
	bool operator>=(const Date& D) const;

	// <
	bool operator<(const Date& D) const;

	// <=
	bool operator<=(const Date& D) const;

	// != 
	bool operator!=(const Date& D) const;

	// += / +
	Date& operator+=(const int day);
	Date operator+(const int day);

	// -= / -
	Date& operator-=(const int day);
	Date operator-(const int day);

	// Date++ / ++Date
	Date operator++(int);
	Date& operator++();

	// Date-- / --Date
	Date operator--(int);
	Date& operator--();

	// d1 - d2;
	int operator-(const Date& right) const;

private:
	int _year;
	int _month;
	int _day;
};