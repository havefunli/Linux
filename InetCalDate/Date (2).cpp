#include "Date.h"

std::ostream& operator<<(std::ostream& os, Date& date)
{
	os << date._year << "-" << date._month << "-" << date._day;

	return os;
}

std::istream& operator>>(std::istream& in, Date& date)
{
	in >> date._year >> date._month >> date._day;

	return in;
}

Date::Date(const int year, const int month, const int day) {
	_year = year;
	_month = month;
	_day = day;
}

Date::Date(const Date& origin) {
	_year = origin._year;
	_month = origin._month;
	_day = origin._day;
}

Date::~Date() {
	// cout << "~Date()" << endl;
}

void Date::Print() const{
	cout << _year << '-' << _month << '-' << _day << endl;
}

Date Date::operator=(const Date& D) {
	_year = D._year;
	_month = D._month;
	_day = D._day;

	return *this;
}

bool Date::operator==(const Date& D) const{
	if ((this->_year == D._year)
		&& (this->_month == D._month)
		&& (this->_day == D._day)) return true;
	else return false;
}

bool Date::operator>(const Date& D) const{
	if (_year != D._year) {
		return _year > D._year;
	}
	if (_month != D._month) {
		return _month > D._month;
	}
	return _day > D._day;
}

bool Date::operator>=(const Date& D) const{
	return *this == D || *this > D;
}

bool Date::operator<(const Date& D) const{
	return !(*this >= D);
}

bool Date::operator<=(const Date& D) const{
	return *this == D || *this < D;
}

bool Date::operator!=(const Date& D) const{
	return !(*this == D);
}

Date& Date::operator+=(const int day) {
	_day += day;

	while (_day >= getMonthDay(_year, _month)) {
		_month += 1;
		if (_month > 12) {
			_month = 1;
			_year += 1;
		}

		_day -= getMonthDay(_year, _month);
	}

	return *this;
}

Date Date::operator+(const int day) {
	Date temp = *this;
	temp += 1;
	
	return temp;
}

Date& Date::operator-=(const int day) {
	_day -= day;

	while (_day < 0) {
		_month -= 1;
		if (_month < 1) {
			_month = 12;
			_year -= 1;
		}

		_day += getMonthDay(_year, _month);
	}

	return *this;
}

Date Date::operator-(const int day) {
	Date temp = *this;
	temp -= day;

	return temp;
}

Date Date::operator++(int) {
	Date temp = *this;
	*this += 1;

	return temp;
}

Date& Date::operator++() {
	*this += 1;

	return *this;
}

Date Date::operator--(int) {
	Date temp = *this;
	*this -= 1;

	return temp;
}

Date& Date::operator--() {
	*this -= 1;

	return *this;
}

int Date::operator-(const Date& D) const{
	int flag = 1;
	int day = 0;

	Date left = *this;
	Date right = D;
	if (right > left) {
		Date temp = left;
		left = right;
		right = temp;
		flag = -1;
	}

	while (left != right) {
		right += 1;
		day += 1;
	}

	return flag * day;
}

Json::Value Date::toJosn() const
{
	Json::Value Obj;
	Obj["Year"] = _year;
	Obj["Month"] = _month;
	Obj["Day"] = _day;

	return Obj;
}
