#ifndef _ICY_ARGUMENT_HPP_
#define _ICY_ARGUMENT_HPP_

#include <any>
#include <optional>
#include <functional>

#include <string>
#include <vector>
#include <unordered_set>

#include <sstream>

namespace icy {

class argument;
class positional_argument;
class optional_argument;
class flag_argument;

struct virtual_store_handler;
template <typename _Tp> struct store_handler;

struct virtual_store_handler {
    virtual std::any save(const std::string&) = 0;
};
template <typename _Tp> struct store_handler : public virtual_store_handler {
    typedef _Tp value_type;
    std::stringstream _ss;
    std::any save(const std::string& _s) override {
        _ss.clear(); _ss << _s;
        value_type _v;  _ss >> _v; return _v;
    }
};

class argument_parser;

class argument {
protected:
    argument(argument_parser* const);
public:
    typedef argument self;
    argument(const self&) = delete;
    argument(self&&) = default;
    self& operator=(const self&) = delete;

    friend class argument_parser;
protected:
    argument_parser* _parser = nullptr; // 解析器指针
    std::string _comment;               // 参数说明
    bool _required = false;             // 参数是否必需
};

class optional_argument : public argument {
    typedef optional_argument self;
    friend class argument_parser;
public:
    optional_argument(argument_parser* const);
    virtual ~optional_argument();
    auto set_default(const std::string&) -> self&;
    auto has_value() const -> bool;
    auto size() const -> size_t;
    template <typename _Tp = std::string> auto value(size_t _i = 0) const -> _Tp;
public:
    template <typename... _Args> auto choices(_Args&&... _args) -> self&;
    template <typename _Tp> auto store_as() -> self&;
    /**
     * @brief store duplicated values by appending instead of replacing
     */
    auto append() -> self&;
    /**
     * @brief help message for argument
     */
    auto help(const std::string&) -> self&;
    auto help() const -> const std::string&;
    /**
     * @brief mark this argument required
     */
    auto required() -> self&;
    // action
private:
    auto set_value(const std::string&) -> void;
    auto required_verify() const -> bool;
    template <typename... _Args> auto _M_choices(const std::string&, _Args&&... _args) -> void;
    auto _M_choices() -> void;
    auto _M_set_value(const std::any&) -> void;
private:
    std::vector<std::any> _data;
    bool _append = false;
    std::unordered_set<std::string> _choices;
    virtual_store_handler* _store_handler = nullptr;
};
class flag_argument : public argument {
    typedef flag_argument self;
    friend class argument_parser;
public:
    flag_argument(argument_parser* const);
    auto has_value() const -> bool;
    auto value() const -> bool;
public:
    auto set_default(bool) -> self&;
    /**
     * @brief help message for argument
     */
    auto help(const std::string&) -> self&;
    auto help() const -> const std::string&;
    /**
     * @brief mark this argument required
     */
    auto required() -> self&;
private:
    auto set_value(bool) -> void;
    auto required_verify() const -> bool;
private:
    std::optional<bool> _data;
};
class positional_argument : public argument {
    typedef positional_argument self;
    friend class argument_parser;
public:
    positional_argument(argument_parser* const);
    auto has_value() const -> bool;
    template <typename _Tp = std::string> auto value() const -> _Tp;
public:
    template <typename... _Args> auto choices(_Args&&... _args) -> self&;
    template <typename _Tp> auto store_as() -> self&;
    /**
     * @brief help message for argument
     */
    auto help(const std::string&) -> self&;
    auto help() const -> const std::string&;
    /**
     * @brief mark this argument required
     */
    auto required() -> self&;
private:
    auto set_value(const std::string&) -> void;
    auto required_verify() const -> bool;
    template <typename... _Args> auto _M_choices(const std::string&, _Args&&... _args) -> void;
    auto _M_choices() -> void;
private:
    std::optional<std::any> _data = std::nullopt;
    std::unordered_set<std::string> _choices;
    virtual_store_handler* _store_handler = nullptr;
};


/**
 * @brief get argument value in specified type
 * @param _i index of value (default = 0), do not use unless value's stored by appending
 * @tparam _Tp specified type
 * @throw std::out_of_range if %_i is an invalid index
 * @throw std::bad_any_cast if fail to cast
 */
template <typename _Tp> auto optional_argument::value(size_t _i) const -> _Tp {
    return std::any_cast<_Tp>(_data.at(_i));
}
/**
 * @brief argument value must be in choices if set
 * @param _args value choices
 */
template <typename... _Args> auto optional_argument::choices(_Args&&... _args) -> self& {
    _M_choices(std::forward<_Args>(_args)...);
    return *this;
}
/**
 * @brief store value in specified type
 * @tparam _Tp specified type
 */
template <typename _Tp> auto optional_argument::store_as() -> self& {
    delete _store_handler; _store_handler = nullptr;
    _store_handler = new store_handler<_Tp>();
    return *this;
}
template <typename... _Args> auto optional_argument::_M_choices(const std::string& _s, _Args&&... _args) -> void {
    _choices.insert(_s);
    _M_choices(std::forward<_Args>(_args)...);
}


/**
 * @brief get argument value in specified type
 * @tparam _Tp specified type
 * @throw std::bad_any_cast if fail to cast
 */
template <typename _Tp> auto positional_argument::value() const -> _Tp {
    return std::any_cast<_Tp>(_data.value());
}
/**
 * @brief argument value must be in choices if set
 * @param _args value choices
 */
template <typename... _Args> auto positional_argument::choices(_Args&&... _args) -> self& {
    _M_choices(std::forward<_Args>(_args)...);
    return *this;
}
/**
 * @brief store value in specified type
 * @tparam _Tp specified type
 */
template <typename _Tp> auto positional_argument::store_as() -> self& {
    delete _store_handler; _store_handler = nullptr;
    _store_handler = new store_handler<_Tp>();
    return *this;
}
template <typename... _Args> auto positional_argument::_M_choices(const std::string& _s, _Args&&... _args) -> void {
    _choices.insert(_s);
    _M_choices(std::forward<_Args>(_args)...);
}

}

#endif // _ICY_ARGUMENT_HPP_