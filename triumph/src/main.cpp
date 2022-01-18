#include <iostream>
#include <string>
#include <sstream>

#include "Tryte.h"

int main()
{
    Tryte a(10);
    Tryte b(22);
    Tryte c = a;

    std::cout << "a = " << a << ' ' << "b = " << b << ' ' << "c = " << c << '\n';
    std::cout << std::boolalpha;
    std::cout << "a == b: " << (a == b) << '\n';
    std::cout << "a == c: " << (a == c) << '\n';
    std::cout << "a != b: " << (a != b) << '\n';

    Tryte d("mmm");
    std::cout << "d = " << d << " = " << Tryte::get_int(d) << '\n';
    std::cout << "a < d: " << (a < d) << '\n';
    std::cout << "a > b: " << (a > b) << '\n';

    std::cout << "a & b = " << (a & b) << '\n';
    std::cout << "a | b = " << (a | b) << '\n';
    std::cout << "~a = " << ~a << '\n';

    std::cout << "a + b = " << (a + b) << '\n';
    int8_t carry = 0;
    std::cout << "mmm + mmm = " << Tryte::add_with_carry(Tryte("mmm"), Tryte("mmm"), carry) << '\n';
    std::cout << "Carry flag = " << static_cast<int64_t>(carry) << '\n';

    return 0;
}