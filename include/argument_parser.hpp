#ifndef _ICY_ARGUMENT_PARSER_HPP_
#define _ICY_ARGUMENT_PARSER_HPP_

#include "argument.hpp"

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "trie_tree.hpp"

namespace icy {

class argument_parser;

class argument_parser {
    typedef argument_parser self;
public:
    /**
     * @brief argument_parser construtor
     * @param _prefix_chars the prefix char of arguments ("-" by default)
     */
    argument_parser(const std::string& _prefix_chars = "-");
    argument_parser(const self&) = delete;
    argument_parser(self&&) = default;
    self& operator=(const self&) = delete;
    virtual ~argument_parser() = default;
    /**
     * @brief add optional argument
     * @param _Tp store type (std::string by default)
     * @param _args string key of argument
     * @return reference to argument object
     */
    template <typename _Tp, typename... _Args> auto
        add_optional_argument(_Args&&... _args) -> optional_argument&;
    template <typename... _Args> auto
        add_optional_argument(_Args&&... _args) -> optional_argument&;
    /**
     * @brief add flag argument
     * @param _args string key of argument
     * @return reference to argument object
     */
    template <typename... _Args> auto
        add_flag_argument(_Args&&... _args) -> flag_argument&;
    /**
     * @brief add positional argument
     * @param _k serial position of argument
     * @return reference to argument object
     */
    auto add_positional_argument(size_t _k) -> positional_argument&;
    /**
     * @return whether contains optional argument
     */
    auto contain_option(const std::string&) const -> bool;
    /**
     * @return whether contains flag argument
     */
    auto contain_flag(const std::string&) const -> bool;
    /**
     * @return whether contains positional argument
     */
    auto contain_position(size_t) const -> bool;
    /**
     * @brief get optional argument
     */
    auto get_option(const std::string&) const -> const optional_argument&;
    /**
     * @brief get flag argument
     */
    auto get_flag(const std::string&) const -> const flag_argument&;
    /**
     * @brief get positional argument
     */
    auto get_position(size_t) const -> const positional_argument&;
    /**
     * @brief get the size of positional arguments
     */
    auto get_position() const -> size_t;
    auto parse(int, const char* const*) -> void;
    /**
     * @brief parse arguments
     * @throw `argument::out_of_range` not in choices
     * @throw `argument::lack_of_requisite` lack of required arguments
     */
    auto parse(const std::vector<std::string>&) -> void;
private:
    template <typename... _Args> auto
        _M_attach_optional_argument(std::shared_ptr<optional_argument>, const std::string&, _Args&&... _args) -> void;
    auto _M_attach_optional_argument(std::shared_ptr<optional_argument>) -> void;
    template <typename... _Args> auto
        _M_attach_flag_argument(std::shared_ptr<flag_argument>, const std::string&, _Args&&... _args) -> void;
    auto _M_attach_flag_argument(std::shared_ptr<flag_argument>) -> void;
    auto _M_attach_positional_argument(std::shared_ptr<positional_argument>, size_t) -> void;
    auto _M_required_verify() const -> void;
    auto _M_is_optional_argument(const std::string&) const -> bool;
    auto _M_is_flag_argument(const std::string&) const -> bool;
    auto _M_update_optional_argument(const std::string&, const std::string&) -> void;
    auto _M_update_flag_argument(const std::string&) -> void;
    auto _M_update_positional_argument(size_t, const std::string&) -> void;
private:
    std::unordered_map<std::string, std::shared_ptr<optional_argument>> _optional_arguments;
    std::unordered_map<std::string, std::shared_ptr<flag_argument>> _flag_arguments;
    std::unordered_map<size_t, std::shared_ptr<positional_argument>> _positional_arguments;

    const std::unordered_set<char> _prefix_chars;
    trie_tree _optional_keys;
};

template <typename _Tp, typename... _Args> auto
argument_parser::add_optional_argument(_Args&&... _args)
-> optional_argument& {
    if (sizeof...(_Args) == 0) {
        throw std::length_error("zero argument.");
    }
    auto _ptr = std::make_shared<optional_argument>(this, std::forward<_Args>(_args)...);
    _ptr->template store_as<_Tp>();
    _M_attach_optional_argument(_ptr, std::forward<_Args>(_args)...);
    return *_ptr;
}
template <typename... _Args> auto
argument_parser::add_optional_argument(_Args&&... _args)
-> optional_argument& {
    if (sizeof...(_Args) == 0) {
        throw std::length_error("zero argument.");
    }
    auto _ptr = std::make_shared<optional_argument>(this, std::forward<_Args>(_args)...);
    _M_attach_optional_argument(_ptr, std::forward<_Args>(_args)...);
    return *_ptr;
}
template <typename... _Args> auto
argument_parser::add_flag_argument(_Args&&... _args)
-> flag_argument& {
    if (sizeof...(_Args) == 0) {
        throw std::length_error("zero argument.");
    }
    auto _ptr = std::make_shared<flag_argument>(this, std::forward<_Args>(_args)...);
    _M_attach_flag_argument(_ptr, std::forward<_Args>(_args)...);
    return *_ptr;
}


template <typename... _Args> auto
argument_parser::_M_attach_optional_argument(std::shared_ptr<optional_argument> _ptr, const std::string& _k, _Args&&... _args)
-> void {
    _optional_arguments[_k] = _ptr;
    _optional_keys.add(_k);
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