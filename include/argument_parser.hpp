#ifndef _ICY_ARGUMENT_PARSER_HPP_
#define _ICY_ARGUMENT_PARSER_HPP_

#include "argument.hpp"

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>

namespace icy {

class argument_parser;

class argument_parser {
    typedef argument_parser self;
public:
    argument_parser() = default;
    argument_parser(const self&) = delete;
    argument_parser(self&&) = default;
    self& operator=(const self&) = delete;
    virtual ~argument_parser() = default;
    template <typename... _Args> auto
        add_optional_argument(_Args&&... _args) -> optional_argument&;
    template <typename... _Args> auto
        add_flag_argument(_Args&&... _args) -> flag_argument&;
    auto add_positional_argument(size_t) -> positional_argument&;
    auto get_option(const std::string&) const -> const optional_argument&;
    auto get_flag(const std::string&) const -> const flag_argument&;
    auto get_position(size_t) const -> const positional_argument&;
    auto parse(int, const char* const*) -> bool;
    auto parse(const std::vector<std::string>&) -> bool;
private:
    template <typename... _Args> auto
        _M_attach_optional_argument(std::shared_ptr<optional_argument>, const std::string&, _Args&&... _args) -> void;
    auto _M_attach_optional_argument(std::shared_ptr<optional_argument>) -> void;
    template <typename... _Args> auto
        _M_attach_flag_argument(std::shared_ptr<flag_argument>, const std::string&, _Args&&... _args) -> void;
    auto _M_attach_flag_argument(std::shared_ptr<flag_argument>) -> void;
    auto _M_attach_positional_argument(std::shared_ptr<positional_argument>, size_t) -> void;
    auto _M_required_verify() const -> bool;
private:
    std::unordered_map<std::string, std::shared_ptr<optional_argument>> _optional_arguments;
    std::unordered_map<std::string, std::shared_ptr<flag_argument>> _flag_arguments;
    std::unordered_map<size_t, std::shared_ptr<positional_argument>> _positional_arguments;

    // std::list<std::shared_ptr<optional_argument>> _required_optional_arguments_list;
    // std::list<std::shared_ptr<flag_argument>> _required_flag_arguments_list;
    // std::list<std::shared_ptr<positional_argument>> _required_positional_arguments_list;
};

template <typename... _Args> auto
argument_parser::add_optional_argument(_Args&&... _args)
-> optional_argument& {
    if (sizeof...(_Args) == 0) {
        throw std::length_error("zero argument.");
    }
    auto _ptr = std::make_shared<optional_argument>(this);
    _M_attach_optional_argument(_ptr, std::forward<_Args>(_args)...);
    return *_ptr;
}
template <typename... _Args> auto
argument_parser::add_flag_argument(_Args&&... _args)
-> flag_argument& {
    if (sizeof...(_Args) == 0) {
        throw std::length_error("zero argument.");
    }
    auto _ptr = std::make_shared<flag_argument>(this);
    _M_attach_flag_argument(_ptr, std::forward<_Args>(_args)...);
    return *_ptr;
}
auto argument_parser::add_positional_argument(size_t _k)
-> positional_argument& {
    auto _ptr = std::make_shared<positional_argument>(this);
    _M_attach_positional_argument(_ptr, _k);
    return *_ptr;
}


template <typename... _Args> auto
argument_parser::_M_attach_optional_argument(std::shared_ptr<optional_argument> _ptr, const std::string& _k, _Args&&... _args)
-> void {
    _optional_arguments[_k] = _ptr;
    _M_attach_optional_argument(_ptr, std::forward<_Args>(_args)...);
}
template <typename... _Args> auto
argument_parser::_M_attach_flag_argument(std::shared_ptr<flag_argument> _ptr, const std::string& _k, _Args&&... _args)
-> void {
    _flag_arguments[_k] = _ptr;
    _M_attach_flag_argument(_ptr, std::forward<_Args>(_args)...);
}

}

#endif // _ICY_ARGUMENT_PARSER_HPP_