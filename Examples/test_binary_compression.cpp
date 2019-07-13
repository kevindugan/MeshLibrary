#include <iostream>
#include <algorithm>
#include <string>
#include <bitset>
int main()
{
        std::bitset<8> v32 = 0x00;

        std::string v32_str = v32.to_string();
        std::cout << "string form: " << v32_str << '\n';

        char a[33] = {0}; 
        std::copy(v32_str.begin(), v32_str.end(), a);
        // or even strcpy(a, v32_str.c_str());
        std::cout << "array form:  " << a << '\n';
}