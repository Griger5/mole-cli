#include <molecli/molecli.hpp>

#include <memory>

void get_status() {
    std::cout << "Status: On\n"; 
}

int main() {
    molecli::CLI main_cli{"device0"};

    std::shared_ptr<molecli::CLI> device1 = std::make_shared<molecli::CLI>("device1");
    std::shared_ptr<molecli::CLI_s<int>> device2 = std::make_shared<molecli::CLI_s<int>>("device2");

    device1->add_command("status", "Get status", get_status);
    device2->add_command("status", "Get status", get_status);
    
    device2->add_command_s("increment", "Increment static variable", [](MOLECLI_STATIC_VARS(device2)) {
        GetStaticVar(0) += 1;
    });
    device2->add_command_s("counter", "Display the value of static variable", [](MOLECLI_STATIC_VARS(device2)) {
        std::cout << GetStaticVar(0) << '\n';
    });

    main_cli.add_sub_cli("device1", "CLI for device1", device1);
    main_cli.add_sub_cli("device2", "CLI for device2", device2);

    main_cli.run_loop();
}