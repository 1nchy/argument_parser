#include "argument_parser.hpp"

namespace icy {

auto argument_parser::get_option(const std::string& _k) const
-> const optional_argument& {
    return *(_optional_arguments.at(_k));
}
// auto argument_parser::get(size_t _k) const
// -> const argument& {
//     return *(_anonymous_arguments.at(_k));
// }

auto argument_parser::parse(int _argc, const char* const* _argv) -> bool {
    return parse({_argv, _argc + _argv});
}
auto argument_parser::parse(const std::vector<std::string>& _args) -> bool {
    for (auto _i = _args.cbegin(); _i != _args.cend(); ++_i) {
        // const std::string& _arg = *_i;
        // size_t _equal_pos = _arg.find_first_of('=');
        // const std::string _k = (_equal_pos == std::string::npos) ? _arg : _arg.substr(0, _equal_pos);
        // if (_optional_arguments.contains(_k)) {
        //     const std::string _v = (_equal_pos == std::string::npos) ? 
        //         (++_i == _args.cend() ? "" : *_i) :
        //         (++_equal_pos == _args.size() ? "" : _arg.substr(_equal_pos));
        // }
        // else {
        //     _anonymous_arguments
        // }
    }
    return _M_required_verify();
}

auto argument_parser::_M_attach_optional_argument(std::shared_ptr<optional_argument>)
-> void {}
auto argument_parser::_M_required_verify() const
-> bool {
    for (const auto& _p : _optional_arguments) {
        if (!_p.second->required_verify()) return false;
    }
    return true;
}
}