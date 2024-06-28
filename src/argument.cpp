#include "argument.hpp"

namespace icy {

argument::argument(argument_parser* const _p) : _parser(_p) {}


optional_argument::optional_argument(argument_parser* const _p) : argument(_p) {}
optional_argument::~optional_argument() {
    delete _store_handler; _store_handler = nullptr;
}
auto optional_argument::has_value() const -> bool {
    return _data.has_value();
}
auto optional_argument::set_default(const std::string& _s) -> self& {
    set_value(_s);
    return *this;
}
auto optional_argument::help(const std::string& _c) -> self& {
    _comment = _c;
    return *this;
}
auto optional_argument::help() const -> const std::string& {
    return _comment;
}
auto optional_argument::required() -> self& {
    _required = true;
    return *this;
}
auto optional_argument::set_value(const std::string& _s) -> void {
    if (!_choices.empty() && !_choices.contains(_s)) {
        throw std::out_of_range("not in choices.");
    }
    if (_store_handler == nullptr) {
        _data = _s;
    }
    else {
        _data = _store_handler->save(_s);
    }
}
auto optional_argument::required_verify() const -> bool {
    return !_required || _data.has_value();
}
auto optional_argument::_M_choices() -> void {}


flag_argument::flag_argument(argument_parser* const _p) : argument(_p) {}
auto flag_argument::set_default(bool _b) -> self& {
    set_value(_b);
    return *this;
}
auto flag_argument::has_value() const -> bool {
    return _data.has_value();
}
auto flag_argument::value() const -> bool {
    return _data.value();
}
auto flag_argument::help(const std::string& _c) -> self& {
    _comment = _c;
    return *this;
}
auto flag_argument::help() const -> const std::string& {
    return _comment;
}
auto flag_argument::required() -> self& {
    _required = true;
    return *this;
}
auto flag_argument::set_value(bool _b) -> void {
    _data = std::make_optional(_b);
}
auto flag_argument::required_verify() const -> bool {
    return !_required || _data.has_value();
}


positional_argument::positional_argument(argument_parser* const _p) : argument(_p) {}
auto positional_argument::has_value() const -> bool {
    return _data.has_value();
}
auto positional_argument::help(const std::string& _c) -> self& {
    _comment = _c;
    return *this;
}
auto positional_argument::help() const -> const std::string& {
    return _comment;
}
auto positional_argument::required() -> self& {
    _required = true;
    return *this;
}
auto positional_argument::set_value(const std::string& _s) -> void {
    if (!_choices.empty() && !_choices.contains(_s)) {
        throw std::out_of_range("not in choices.");
    }
    if (_store_handler == nullptr) {
        _data = _s;
    }
    else {
        _data = _store_handler->save(_s);
    }
}
auto positional_argument::required_verify() const -> bool {
    return !_required || _data.has_value();
}
auto positional_argument::_M_choices() -> void {}

}