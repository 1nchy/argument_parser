#include "argument_parser.hpp"

int main(int _argc, char* _argv[]) {
    icy::argument_parser _parser;
    _parser.add_optional_argument("--std").store_as<unsigned>().set_default("3").help("standard").required();
    _parser.add_optional_argument("-o").help("output");
    _parser.add_flag_argument("-Wall").set_default(false).help("all warning");
    _parser.add_positional_argument(0).required().help("compiler");
    const std::vector<std::string> _args = {
        "gcc", "--std=11", "-Wall", "-o", "main", "main.cpp"
    };
    auto _req = _parser.parse(_args);
    auto _o0 = _parser.get_option("--std").value<unsigned>();
    auto _o1 = _parser.get_option("-o").value();
    auto _p0 = _parser.get_position(0).value();
    auto _p1 = _parser.get_position(1).value();
    auto f0 = _parser.get_flag("-Wall").value();
    return 0;
}