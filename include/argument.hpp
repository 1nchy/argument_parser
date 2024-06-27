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

// typedef enum {
//     single,
//     multiple,
// } optional_argument_subtype;

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
    auto help(const std::string&) -> self&;
    auto help() const -> const std::string&;

    friend class argument_parser;
protected:
    argument_parser* _parser = nullptr;
private:
    std::string _comment;
};

// class positional_argument : public argument {
//     typedef positional_argument self;
// public:
//     template <typename _Tp = std::string> auto value() const -> _Tp;
//     template <typename... _Args> auto choices(_Args&&... _args) -> self&;
// private:
//     template <typename... _Args> auto _M_choices(const std::string&, _Args&&... _args) -> void;
//     template <typename... _Args> auto _M_choices() -> void;
// private:
//     std::optional<std::any> _data;
//     std::vector<const std::string> _choices;
// };
class optional_argument : public argument {
    typedef optional_argument self;
public:
    optional_argument(argument_parser* const);
    virtual ~optional_argument();
    auto set_default(const std::string&) -> self&;
    template <typename _Tp = std::string> auto value() const -> _Tp;
    template <typename... _Args> auto choices(_Args&&... _args) -> self&;
    template <typename _Tp> auto store_as() -> self&;
    // auto required() -> self&;
    // action
private:
    auto set_value(const std::string&) -> void;
    template <typename... _Args> auto _M_choices(const std::string&, _Args&&... _args) -> void;
    auto _M_choices() -> void;
private:
    std::optional<std::any> _data = std::nullopt;
    std::unordered_set<std::string> _choices;
    virtual_store_handler* _store_handler = nullptr;
};
// class flag_argument : public argument {
//     typedef flag_argument self;
// public:
//     auto set_default(bool) -> self&;
//     auto value() const -> bool;
// private:
//     std::optional<bool> _data;
// };


template <typename _Tp> auto optional_argument::value() const -> _Tp {
    return std::any_cast<_Tp>(_data.value());
}
template <typename... _Args> auto optional_argument::choices(_Args&&... _args) -> self& {
    _M_choices(std::forward<_Args>(_args)...);
    return *this;
}
template <typename _Tp> auto optional_argument::store_as() -> self& {
    _store_handler = new store_handler<_Tp>();
    return *this;
}
template <typename... _Args> auto optional_argument::_M_choices(const std::string& _s, _Args&&... _args) -> void {
    _choices.insert(_s);
    _M_choices(std::forward<_Args>(_args)...);
}

}

#endif // _ICY_ARGUMENT_HPP_