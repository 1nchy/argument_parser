#include "argument.hpp"

namespace icy {

argument::argument(argument_parser* const _p) : _parser(_p) {}
auto argument::help(const std::string& _c) -> self& {
    _comment = _c;
    return *this;
}
auto argument::help() const -> const std::string& {
    return _comment;
}


optional_argument::optional_argument(argument_parser* const _p) : argument(_p) {}
optional_argument::~optional_argument() {
    delete _store_handler; _store_handler = nullptr;
}
auto optional_argument::set_default(const std::string& _s) -> self& {
    set_value(_s);
    return *this;
}
// auto optional_argument::required() -> self& {}
auto optional_argument::set_value(const std::string& _s) -> void {
    if (!_choices.empty() && !_choices.contains(_s)) {
        throw std::out_of_range("not in choices.");
    }
    if (_store_handler == nullptr) {
        _data = std::make_optional(_s);
    }
    else {
        _data = _store_handler->save(_s);
    }
}
auto optional_argument::_M_choices() -> void {}

}