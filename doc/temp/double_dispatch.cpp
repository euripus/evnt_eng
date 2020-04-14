// https://gieseanw.wordpress.com/2018/12/29/stop-reimplementing-the-virtual-table-and-start-using-double-dispatch/

#include <iostream>
#include <memory>
#include <vector>

struct Animal;
struct Cat;
struct Dog;
struct Llama;
struct AnimalVisitor
{
    virtual void Visit(Cat *)   = 0;
    virtual void Visit(Dog *)   = 0;
    virtual void Visit(Llama *) = 0;
    virtual ~AnimalVisitor()    = default;
};

struct Animal
{
    virtual ~Animal()                   = default;
    virtual void Visit(AnimalVisitor *) = 0;
};

template<class T>
struct VisitableAnimal : Animal
{
    void Visit(AnimalVisitor * _visitor) override { _visitor->Visit(static_cast<T *>(this)); }
};

struct Cat : VisitableAnimal<Cat>
{};

struct Dog : VisitableAnimal<Dog>
{};

struct Llama : VisitableAnimal<Llama>
{};

template<class T>
struct SingleDoNothingAnimalVisitor : virtual AnimalVisitor
{
    using AnimalVisitor::Visit;
    void Visit(T *) override {}
};

template<class... T>
struct MultipleDoNothingAnimalVisitor : public SingleDoNothingAnimalVisitor<T>...
{
    using SingleDoNothingAnimalVisitor<T>::Visit...;
};

// strong typedef
struct DoNothingAnimalVisitor : public MultipleDoNothingAnimalVisitor<Cat, Dog, Llama>
{};

struct CatFilter : DoNothingAnimalVisitor
{
    using DoNothingAnimalVisitor::Visit;
    void               Visit(Cat * _cat) override { cats_.push_back(_cat); }
    std::vector<Cat *> cats_;
};

int main()
{
    std::vector<std::unique_ptr<Animal>> animals;
    for(size_t i = 0; i < 10; ++i)
    {
        if(i < 8)
        {
            if(i % 2 == 0)
                animals.push_back(std::make_unique<Dog>());
            else
                animals.push_back(std::make_unique<Llama>());
        }
        else
            animals.push_back(std::make_unique<Cat>());
    }

    CatFilter visitor;
    for(auto & animalPtr : animals)
    {
        animalPtr->Visit(&visitor);
    }
    std::cout << "visitor encountered " << visitor.cats_.size() << " cats\n";
}
