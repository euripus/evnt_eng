#include <iostream>
#include <memory>
#include <vector>

struct Base
{
    Base() {}
    virtual ~Base() {}

    virtual void get() const {}
};

struct A : public Base
{
    A() { std::cout << "A created" << std::endl; }
    ~A() { std::cout << "A destroyed" << std::endl; }
};

struct B : public Base
{
    B() { std::cout << "B created" << std::endl; }
    ~B() { std::cout << "B destroyed" << std::endl; }

    void get() const override { std::cout << "B::get() called" << std::endl; }
};

void TestFunc(std::shared_ptr<Base> & ptr)
{
    ptr = std::make_shared<B>();
}

int main()
{
    std::shared_ptr<Base> base_ptr = std::make_shared<A>();

    {
        std::vector<std::shared_ptr<Base>> vec_ptrs;
        for(uint32_t i = 0; i < 5; ++i)
            vec_ptrs.push_back(base_ptr);

        TestFunc(vec_ptrs[3]);

        base_ptr->get();
        vec_ptrs[3]->get();
    }

    std::cout << "vec_ptrs destroyed" << std::endl;
    return 0;
}

/*
A created
B created
B::get() called
B destroyed
vec_ptrs destroyed
A destroyed
*/
