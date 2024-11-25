#include "argument_parser.hpp"

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
    _parser.add_flag_argument("w").help("no warnings")
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
    auto _o0 = _parser.get_option("-std=c++").value<unsigned>();
    auto _o1 = _parser.get_option("o").value();
    auto _o2 = _parser.get_option("l").value(0);
    auto _o3 = _parser.get_option("l").value(1);
    auto _p0 = _parser.get_position(0).value();
    auto _p1 = _parser.get_position(1).value();
    auto f0 = _parser.get_flag("Wall").value();
    return 0;
}