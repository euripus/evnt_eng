#include <iostream>
#include <memory>

template<typename Derived>
class Converter
{
    Derived &       self() { return static_cast<Derived &>(*this); }
    Derived const & self() const { return static_cast<Derived const &>(*this); }

public:
    void convert() { self().convert(); }

    void exportToInternal() const { self().exportToInternal(); }
};

class TestConverter : public Converter<TestConverter>
{
public:
    void convert() { std::cout << "Test convert" << std::endl; }

    void exportToInternal() const { std::cout << "Test export" << std::endl; }
};

class TestConverter2 : public Converter<TestConverter2>
{
public:
    void convert() { std::cout << "Test2 convert" << std::endl; }

    void exportToInternal() const { std::cout << "Test2 export" << std::endl; }
};

template<typename Derived>
std::unique_ptr<Converter<Derived>> create_test()
{
    return std::make_unique<Derived>();
}

int main()
{
    TestConverter tc;
    auto          ptr  = create_test<TestConverter>();
    auto          ptr2 = create_test<TestConverter2>();

    tc.convert();
    ptr->exportToInternal();
    ptr2->exportToInternal();

    return 0;
}
