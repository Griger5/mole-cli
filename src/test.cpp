#include "cli-brary/test.hpp"

#include "rttg/rttg.h"

#include <iostream>
#include <string>

void test() {
    std::cout << "Hello!\n";

    std::tuple<int, float, std::string> tuple{ 5, 3.0, "str"};
    
    for (std::size_t idx{}; idx < std::tuple_size_v<decltype(tuple)>; ++idx) {
        rttg::visit([](auto &&tupleValue) { std::cout << tupleValue << " "; }, rttg::get(tuple, idx));
    }

    std::cout << std::endl;
}