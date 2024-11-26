#include "argument_parser.hpp"

#include <iostream>

using std::cout;
using std::endl;

int main(int _argc, char* _argv[]) {
    icy::argument_parser _parser;

    _parser.add_optional_argument<unsigned>("-std=c++").help("compilation standard")
        .choices("3","11","17","20").set_default("3");
    _parser.add_optional_argument("o").help("output")
        .set_default("a.out");
    _parser.add_optional_argument<unsigned>("O").help("optimization level")
        .choices("0", "1", "2", "3").set_default("1");
    _parser.add_optional_argument("l").help("library name")
        .append();
    _parser.add_optional_argument("L").help("library path")
        .append();
    _parser.add_optional_argument("I").help("include path")
        .append();
    _parser.add_optional_argument("D").help("macro definition")
        .append();
    _parser.add_optional_argument("U").help("macro undefinition")
        .append();

    _parser.add_flag_argument("c").help("output elf, without linkage")
        .set_default(false);
    _parser.add_flag_argument("g").help("debug information")
        .set_default(false);
    _parser.add_flag_argument("Wall").help("output all warnings")
        .set_default(false);
    _parser.add_flag_argument("Werror").help("break with any warning")
        .set_default(false);
    _parser.add_flag_argument("v").help("compilation steps")
        .set_default(false);

    _parser.add_positional_argument(0).help("compiler").required();
    _parser.add_positional_argument(1).help("file").required();

    const std::vector<std::string> _args = {
        "g++", "--std=c++11", "-Wall", "-oa.out", "-o", "main", "main.cpp", "-lmath", "src.cpp", "-lpthread"
    };
    _parser.parse(_args);

    cout << "compiler = { " << _parser.get_position(0).value() << " }" << endl;
    cout << "c++ standard = { " << _parser.get_option("-std=c++").value<unsigned>() << " }" << endl;
    cout << "input file = { ";
    for (size_t _i = 1; _i < _parser.get_position(); ++_i) {
        cout << _parser.get_position(_i).value() << " ";
    }
    cout << "}" << endl;
    cout << "output file = { " << _parser.get_option("o").value() << " }" << endl;
    cout << "include path = { ";
    for (size_t _i = 0; _i < _parser.get_option("I").size(); ++_i) {
        cout << _parser.get_option("I").value(_i) << " ";
    }
    cout << "}" << endl;
    cout << "library path = { ";
    for (size_t _i = 0; _i < _parser.get_option("L").size(); ++_i) {
        cout << _parser.get_option("L").value(_i) << " ";
    }
    cout << "}" << endl;
    cout << "library name = { ";
    for (size_t _i = 0; _i < _parser.get_option("l").size(); ++_i) {
        cout << _parser.get_option("l").value(_i) << " ";
    }
    cout << "}" << endl;

    if (_parser.get_flag("Wall").value()) {
        cout << _parser.get_flag("Wall").help() << endl;
    }
    if (_parser.get_flag("Werror").value()) {
        cout << _parser.get_flag("Werror").help() << endl;
    }
    return 0;
}