#include "molecli/test.hpp"

#include "rttg/rttg.h"
#include "isocline.h"

#include <iostream>
#include <string>
#include <cstring>

void test() {
    std::cout << "Hello!\n";

    std::tuple<int, float, std::string> tuple{ 5, 3.0, "str"};
    
    for (std::size_t idx{}; idx < std::tuple_size_v<decltype(tuple)>; ++idx) {
        rttg::visit([](auto &&tupleValue) { std::cout << tupleValue << " "; }, rttg::get(tuple, idx));
    }

    std::cout << std::endl;

    char* input;
    while((input = ic_readline("isoclinε")) != NULL)    // ctrl-d returns NULL (as well as errors)
    {
        bool stop = (strcmp(input,"exit") == 0 || strcmp(input,"") == 0); 
        ic_printf("[gray]-----[/]\n"           // echo the input
                "%s\n"
                "[gray]-----[/]\n", input );    
        free(input);               // do not forget to free the returned input!
        if (stop) break;
    }
    ic_println("done");
}