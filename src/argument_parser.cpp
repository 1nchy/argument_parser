#include "argument_parser.hpp"

namespace icy {

argument_parser::argument_parser(const std::string& _chars)
: _prefix_chars(_chars.cbegin(), _chars.cend()) {}

auto argument_parser::add_positional_argument(size_t _k)
-> positional_argument& {
    auto _ptr = std::make_shared<positional_argument>(this, _k);
    _M_attach_positional_argument(_ptr, _k);
    return *_ptr;
}

auto argument_parser::contain_option(const std::string& _k) const
-> bool {
    return _optional_arguments.contains(_k);
}
auto argument_parser::contain_flag(const std::string& _k) const
-> bool {
    return _flag_arguments.contains(_k);
}
auto argument_parser::contain_position(size_t _k) const
-> bool {
    return _positional_arguments.contains(_k);
}
auto argument_parser::get_option(const std::string& _k) const
-> const optional_argument& {
    return *(_optional_arguments.at(_k));
}
auto argument_parser::get_flag(const std::string& _k) const
-> const flag_argument& {
    return *(_flag_arguments.at(_k));
}
auto argument_parser::get_position(size_t _k) const
-> const positional_argument& {
    return *(_positional_arguments.at(_k));
}
auto argument_parser::get_position() const
-> size_t {
    return _positional_arguments.size();
}

auto argument_parser::parse(int _argc, const char* const* _argv) -> void {
    parse({_argv, _argc + _argv});
}
auto argument_parser::parse(const std::vector<std::string>& _args) -> void {
    auto _i = _args.cbegin();
    auto _M_try_optional_argument = [&](const std::string& _s) -> bool {
        if (_s.empty()) return false;
        if (!this->_prefix_chars.contains(_s.at(0))) return false;
        // _s start with character in %_prefix_chars
        auto _l = _s.cbegin() + 1;
        auto _r = _l + this->_optional_keys.longest_match(_l, _s.cend());
        if (_r == _l) return false;
        const std::string _k(_l, _r);
        const std::string& _v = (_r == _s.cend()) ? *(++_i) : _s.substr(_r - _s.cbegin());
        this->_M_update_optional_argument(_k, _v);
        return true;
    };
    auto _M_try_flag_argument = [&](const std::string& _s) -> bool {
        if (_s.empty()) return false;
        if (!this->_prefix_chars.contains(_s.at(0))) return false;
        const std::string _k = _s.substr(1);
        if (this->_M_is_flag_argument(_k)) {
            this->_M_update_flag_argument(_k);
            return true;
        }
        return false;
    };
    size_t _position = 0;
    auto _M_try_positional_argument = [&](const std::string& _s) -> bool {
        this->_M_update_positional_argument(_position++, _s);
        return true;
    };
    for (; _i != _args.cend(); ++_i) {
        if (_M_try_optional_argument(*_i)) {}
        else if (_M_try_flag_argument(*_i)) {}
        else if (_M_try_positional_argument(*_i)) {}
    }
    _M_required_verify();
}

auto argument_parser::_M_attach_optional_argument(std::shared_ptr<optional_argument>)
-> void {}
auto argument_parser::_M_attach_flag_argument(std::shared_ptr<flag_argument>)
-> void {}
auto argument_parser::_M_attach_positional_argument(std::shared_ptr<positional_argument> _ptr, size_t _k)
-> void {
    _positional_arguments[_k] = _ptr;
}
auto argument_parser::_M_required_verify() const
-> void {
    for (const auto& _p : _optional_arguments) {
        if (!_p.second->required_verify()) {
            throw argument::lack_of_requisite(_p.first);
        }
    }
    for (const auto& _p : _flag_arguments) {
        if (!_p.second->required_verify()) {
            throw argument::lack_of_requisite(_p.first);
        }
    }
    for (const auto& _p : _positional_arguments) {
        if (!_p.second->required_verify()) {
            const std::string _msg = "position[" + std::to_string(_p.first) + "]";
            throw argument::lack_of_requisite(_msg);
        }
    }
}
auto argument_parser::_M_is_optional_argument(const std::string& _s) const -> bool {
    return _optional_arguments.contains(_s);
}
auto argument_parser::_M_is_flag_argument(const std::string& _s) const -> bool {
    return _flag_arguments.contains(_s);
}
auto argument_parser::_M_update_optional_argument(const std::string& _k, const std::string& _v) -> void {
    auto _oa = _optional_arguments.at(_k);
    _oa->set_value(_v);
}
auto argument_parser::_M_update_flag_argument(const std::string& _k) -> void {
    auto _fa = _flag_arguments.at(_k);
    _fa->set_value(true);
}
auto argument_parser::_M_update_positional_argument(size_t _k, const std::string& _v) -> void {
    if (_positional_arguments.contains(_k)) {
        auto _pa = _positional_arguments.at(_k);
        _pa->set_value(_v);
    }
    else {
        auto _pa = std::make_shared<positional_argument>(this, _k);
        _M_attach_positional_argument(_pa, _k);
        _pa->set_value(_v);
    }
}

}