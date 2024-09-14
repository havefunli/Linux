#include "Date.h"

int main()
{
    std::cout << "Please Enter# " << std::endl;
    int year, month, day;
    
    cin >> year >> month >> day;
    Date d1(year, month, day);

    cin >> year >> month >> day;
    Date d2(year, month, day);

    std::cout << d1 - d2 << std::endl;

    return 0;
}