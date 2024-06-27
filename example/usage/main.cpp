#include "argument_parser.hpp"

int main(int _argc, char* _argv[]) {
    icy::argument_parser _parser;
    _parser.add_optional_argument("--std").store_as<unsigned>().set_default("3").help("standard");
    _parser.parse(_argc, _argv);
    auto _c = _parser.get_option("--std").help();
    auto _r = _parser.get_option("--std").value<unsigned>();
    return 0;
}