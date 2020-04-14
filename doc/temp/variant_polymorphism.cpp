// Combining C++17 features
// https://www.youtube.com/watch?v=6HoxXeEBtW0

#include <iostream>
#include <variant>
#include <vector>

class Circle
{
private:
    int rad;

public:
    Circle(int r) : rad{r} {};
    void draw() const { std::cout << "Circle draw" << std::endl; }
};

class Line
{
private:
    int from;
    int to;

public:
    Line(int f, int t) : from{f}, to{t} {};
    void draw() const { std::cout << "Line draw" << std::endl; }
    int  getDistance() const { return to - from; }
};

using GeoObjVar = std::variant<Circle, Line>;

std::vector<GeoObjVar> CreateFig()
{
    std::vector<GeoObjVar> f;
    f.push_back(Line{1, 2});
    f.push_back(Circle{3});

    return f;
}

void DrawElem(const std::vector<GeoObjVar> & v)
{
    for(auto const & geo : v)
    {
        std::visit(
            [](const auto & obj) {
                obj.draw();

                if constexpr(std::is_same_v<decltype(obj), const Line &>)
                {
                    std::cout << "Line distance: " << obj.getDistance() << std::endl;
                }
            },
            geo);
    }
}

int main()
{
    auto fig = CreateFig();
    DrawElem(fig);

    return 0;
}
