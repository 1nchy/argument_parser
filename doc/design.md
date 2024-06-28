# DESIGN


## 参数定义

我们可以将在程序启动中遇到的参数分为三类：
选项参数（optional argument）、
标志参数（flag argument）、
位置参数（positional argument）。

同时，对于各种参数，我们期望他们拥有的如下的能力：

||optional|flag|positional|
|:-:|:-:|:-:|:-:|
|key|strings|strings|size_t|
|type|any|bool|any|
|default|+|+|-|
|required|+|+|+|
|help|+|+|+|
|action(p)|+|-|+|
|choices|+|-|+|
|nargs(p)|+|-|-|
|replace/append(p)|+|-|-|

`key` 项是标识参数对象的键的类型。

`type` 项是数据的存储类型。以 `any` 为例，解析器将使用 `set_value` 接口为参数赋值，`store_as` 约定把 `string` 类型的值按指定类型 `_Tp` 进行存储，`value` 接口获取参数值时，需要显式指定转换类型。

~~~cpp
class optional_argument {
public:
    template <typename _Tp = std::string> auto value() const -> _Tp;
    template <typename _Tp> auto store_as() -> self&;
private:
    auto set_value(const std::string&) -> void;
    std::optional<std::any> _data = std::nullopt;
};
~~~

`default` 项是缺省值；`required` 项用于表明该参数是必须被声明的。

`help` 项是参数提示信息。

`choices` 项是参数值的选项列表。

由于三类参数的可用功能各不相同，且差异较大，不建议设计成派生虚类的形式。

## 解析器定义

由于 `optional` 和 `flag` 类参数的定义相近，不便于用同一套接口构造参数对象。
因此在解析器中，我们使用三个接口构造三种参数：

~~~cpp
class argument_parser {
public:
    template <typename... _Args> auto
        add_optional_argument(_Args&&... _args) -> optional_argument&;
    template <typename... _Args> auto
        add_flag_argument(_Args&&... _args) -> flag_argument&;
    auto add_positional_argument(size_t _k) -> positional_argument&;
};
~~~

`add_optional_argument` 的传入参数是该参数对应的键，可以同时传入多个字符串作为参数对象的键。

~~~cpp
class argument_parser {
public:
    auto parse(int _argc, const char* const* _argv) -> bool {
        return parse({_argv, _argc + _argv});
    }
    auto parse(const std::vector<std::string>&) -> bool;
};
~~~

`parse` 将作为解析器最重要的接口，负责参数的解析。

我们预期的解析器使用方式如下：

~~~cpp
int main(int _argc, char* _argv[]) {
    typename argument_parser _parser;
    _parser.add_optional_argument("--std").store_as<unsigned>().set_default("3").help("standard");
    _parser.parse(_argc, _argv);
    auto _r = _parser.get_option("--std").value<unsigned>();
    return 0;
}
~~~

## 参数实现

### 参数基类

这里的参数基类并没有定义什么待实现的虚接口函数，仅把各参数共有的成员提炼到这里存储，减少冗余代码。

~~~cpp
class argument {
protected:
    argument(argument_parser* const);
    typename argument_parser* _parser = nullptr; // 解析器指针
    std::string _comment; // 参数说明
    bool _required = false; // 参数是否必需
};
~~~

我们希望参数方法均返回自身的引用，这样便于参数的属性设置，因此这些方法哪怕可能拥有相同的函数名和参数列表，但由于拥有不同的返回类型，也不能在基类中定义为虚函数。

不过这些签名相近的函数基本上拥有相同的实现，我们将在独立章节中叙述设计方案。

### optional 参数

### flag 参数

### positional 参数

### store_as 的实现

`store_as` 用于指定数据的存储方式（类型），即在每次赋值的时候，字符串格式的数据都将被转换为指定的数据类型。函数签名如下：

~~~cpp
auto save(const std::string&) -> std::any;
~~~

参数对象将存储一个用于类型转换的函数或对象，在每次赋值的时候调用。
鉴于我们需要存储类型参数，类是更合适的选择——父类暴露 `save` 接口，子类存储类型参数。

~~~cpp
struct virtual_store_handler {
    virtual std::any save(const std::string&) = 0;
};
template <typename _Tp> struct store_handler : public virtual_store_handler {
    typedef _Tp value_type;
    std::any save(const std::string&) override {
        // string -> value_type, return any
    }
};
~~~

## 解析器实现