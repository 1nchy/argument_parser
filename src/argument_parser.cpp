#include "argument_parser.hpp"

namespace icy {

auto argument_parser::add_positional_argument(size_t _k)
-> positional_argument& {
    auto _ptr = std::make_shared<positional_argument>(this);
    _M_attach_positional_argument(_ptr, _k);
    return *_ptr;
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

auto argument_parser::parse(int _argc, const char* const* _argv) -> bool {
    return parse({_argv, _argc + _argv});
}
auto argument_parser::parse(const std::vector<std::string>& _args) -> bool {
    auto _i = _args.cbegin();
    size_t _position = 0;
    auto _M_try_optional_argument = [&](const std::string& _s) -> bool {
        size_t _pivot = _s.find_first_of('=');
        if (_pivot == std::string::npos) { // '=' not exist
            if (this->_M_is_optional_argument(_s) && _i + 1 != _args.cend()) {
                ++_i;
                const std::string& _v = *_i;
                this->_M_update_optional_argument(_s, _v);
                return true;
            }
        }
        else {
            const std::string& _k = _s.substr(0, _pivot);
            if (this->_M_is_optional_argument(_k)) {
                const std::string& _v = _s.substr(_pivot + 1);
                this->_M_update_optional_argument(_k, _v);
                return true;
            }
        }
        return false;
    };
    auto _M_try_flag_argument = [&](const std::string& _s) -> bool {
        if (this->_M_is_flag_argument(_s)) {
            this->_M_update_flag_argument(_s);
            return true;
        }
        return false;
    };
    auto _M_try_positional_argument = [&](const std::string& _s) -> bool {
        this->_M_update_positional_argument(_position++, _s);
        return true;
    };
    for (; _i != _args.cend(); ++_i) {
        if (_M_try_optional_argument(*_i)) {}
        else if (_M_try_flag_argument(*_i)) {}
        else if (_M_try_positional_argument(*_i)) {}
    }
    return _M_required_verify();
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
-> bool {
    for (const auto& _p : _optional_arguments) {
        if (!_p.second->required_verify()) return false;
    }
    for (const auto& _p : _flag_arguments) {
        if (!_p.second->required_verify()) return false;
    }
    for (const auto& _p : _positional_arguments) {
        if (!_p.second->required_verify()) return false;
    }
    return true;
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
        auto _pa = std::make_shared<positional_argument>(this);
        _M_attach_positional_argument(_pa, _k);
        _pa->set_value(_v);
    }
}

}