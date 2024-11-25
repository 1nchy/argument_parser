#include "argument.hpp"

namespace icy {

argument::argument(argument_parser* const _p) : _parser(_p) {}

argument::out_of_range::out_of_range(const std::string& _s) : std::logic_error(_s) {}
argument::out_of_range::out_of_range(const char* _s) : std::logic_error(_s) {}
argument::lack_of_requisite::lack_of_requisite(const std::string& _s) : std::logic_error(_s) {}
argument::lack_of_requisite::lack_of_requisite(const char* _s) : std::logic_error(_s) {}


optional_argument::~optional_argument() {
    delete _store_handler; _store_handler = nullptr;
}
auto optional_argument::has_value() const -> bool {
    return !_data.empty();
}
auto optional_argument::size() const -> size_t {
    return _data.size();
}
auto optional_argument::set_default(const std::string& _s) -> self& {
    set_value(_s);
    return *this;
}
auto optional_argument::append() -> self& {
    _append = true;
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
        throw argument::out_of_range(_keys.empty() ? "" : _keys.front());
    }
    if (_store_handler == nullptr) {
        _M_set_value(_s);
    }
    else {
        _M_set_value(_store_handler->save(_s));
    }
}
auto optional_argument::required_verify() const -> bool {
    return !_required || has_value();
}
auto optional_argument::_M_set_key() -> void {}
auto optional_argument::_M_choices() -> void {}
auto optional_argument::_M_set_value(const std::any& _v) -> void {
    if (!_append) { // single
        if (_data.empty()) {
            _data.emplace_back(_v);
        }
        else {
            _data[0] = _v;
        }
    }
    else { // multiple
        _data.emplace_back(_v);
    }
}


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
auto flag_argument::_M_set_key() -> void {}


positional_argument::positional_argument(argument_parser* const _p, size_t _k) : argument(_p), _key(_k) {}
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
        const std::string _msg = "position[" + std::to_string(_key) + "]";
        throw argument::out_of_range(_msg);
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