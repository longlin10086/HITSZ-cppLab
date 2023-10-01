#include <iostream>
#include <cmath>
#include <memory>
#include <tuple>
#include <variant>
#include <vector>


class Shape{
protected:
    virtual double area() const { return 0.0; }
    virtual void shapeName() const = 0;
};

class Point :virtual public Shape{
protected:
    double p_x, p_y;
public:
    explicit Point(double = 0, double = 0);
    void setPoint(double, double );
    double getX() const { return p_x; }
    double getY() const { return p_y; }
    void shapeName() const override { std::cout << "Point: " << std::endl;}
    friend std::ostream &operator<<(std::ostream &, const Point &);
};

Point::Point(double x, double y) : p_x(x), p_y(y){}

void Point::setPoint(double x, double y) {
    this->p_x = x;
    this->p_y = y;
}

std::ostream &operator<<(std::ostream& output, const Point & point){
    point.shapeName();
    output << "Point x: " << point.getX() << std::endl;
    output << "Point y: " << point.getY() << std::endl;
    return output;
}


class Circle : public Shape{
protected:
    std::shared_ptr<Point> center;
    double radius;
public:
    explicit Circle(double x = 0, double y = 0, double r = 0);
    void setRadius(double);
    double getRadius() const;
    double area() const override;
    void shapeName() const override { std::cout << "Circle: " << std::endl;}
    friend std::ostream &operator<<(std::ostream &, const Circle &);
};

class Triangle : public Shape{
protected:
    std::shared_ptr<Point> t_x, t_y, t_z;
    double xy, xz, yz; // side length
    void setSideLength();
public:
    explicit Triangle(Point& x, Point& y, Point& z);
    std::tuple<double, double, double> getSide() const;
    double area() const override;
    void shapeName() const override { std::cout << "Triangle: " << std::endl;}
    friend std::ostream &operator<<(std::ostream &, const Triangle &);
};

class Rectangle : public Shape{
protected:
    std::shared_ptr<Point> r_x, r_y;
    double length, width;
    void setSideLength();
public:
    explicit Rectangle(Point& x, Point& y);
    std::tuple<double, double> getSide() const;
    double area() const override;
    void shapeName() const override { std::cout << "Rectangle: " << std::endl;}
    friend std::ostream &operator<<(std::ostream &, const Rectangle &);
};

using CircleTriangleRectangle = std::variant<std::unique_ptr<Circle>, std::unique_ptr<Triangle>, std::unique_ptr<Rectangle>>;
std::vector<CircleTriangleRectangle> shapes;

Circle::Circle(double x, double y, double r){
    if(r>0) {
        Point point;
        point.setPoint(x, y);
        center = std::make_shared<Point>(point);
        setRadius(r);
        CircleTriangleRectangle circle = std::make_unique<Circle>(*this);
        shapes.emplace_back(std::move(circle));
    }
    else std::cout << "Something gets wrong!" << std::endl;
}


void Circle::setRadius(double r) {
    radius = r;
}

double Circle::getRadius() const {
    return radius;
}

double Circle::area() const {
    return 3.1415*radius*radius;
}

std::ostream &operator<<(std::ostream& output, const Circle & circle){
    circle.shapeName();
    output << "Circle x: " << circle.center->getX() << std::endl;
    output << "Circle y: " << circle.center->getY() << std::endl;
    output << "Circle r: " << circle.getRadius() << std::endl;
    output << "Circle area: " << circle.area() << std::endl;
    return output;
}


Triangle::Triangle(Point &x, Point &y, Point &z) : t_x(std::make_shared<Point>(x)), t_y(std::make_shared<Point>(y)), t_z(std::make_shared<Point>(z)){
    setSideLength();
    if (xy+xz>yz && xy+yz>xz && xz+yz>xy) {
        CircleTriangleRectangle triangle = std::make_unique<Triangle>(*this);
        shapes.emplace_back(std::move(triangle));
    }
    else {
        std::cout << "Something gets wrong!" << std::endl;
        t_x = nullptr;
        t_y = nullptr;
        t_z = nullptr;
    }
}

void Triangle::setSideLength() {
    xy = sqrt(pow((t_y->getX()-t_x->getX()), 2)+pow((t_y->getY()-t_x->getY()), 2));
    xz = sqrt(pow((t_z->getX()-t_x->getX()), 2)+pow((t_z->getY()-t_x->getY()), 2));
    yz = sqrt(pow((t_y->getX()-t_z->getX()), 2)+pow((t_y->getY()-t_z->getY()), 2));
}

std::tuple<double, double, double> Triangle::getSide() const {
    return std::make_tuple(xy, xz, yz);
}

double Triangle::area() const {
    double p = (xy+xz+yz)/2;
    return sqrt(p*(p-xy)*(p-xz)*(p-yz));
}

std::ostream &operator<<(std::ostream & output, const Triangle & triangle){
    triangle.shapeName();
    output << "Triangle x: (" << triangle.t_x->getX() << ", " << triangle.t_x->getY() << ")" << std::endl;
    output << "Triangle y: (" << triangle.t_y->getX() << ", " << triangle.t_y->getY() << ")" << std::endl;
    output << "Triangle z: (" << triangle.t_z->getX() << ", " << triangle.t_z->getY() << ")" << std::endl;
    output << "Triangle area: " << triangle.area() << std::endl;
    return output;
}

Rectangle::Rectangle(Point &x, Point &y) : r_x(std::make_shared<Point>(x)), r_y(std::make_shared<Point>(y)){
    if ((r_x->getX() != r_y->getY()) and (r_x->getY() != r_y->getY())){
        setSideLength();
        CircleTriangleRectangle rectangle = std::make_unique<Rectangle>(*this);
        shapes.emplace_back(std::move(rectangle));
    }
    else{
        std::cout << "Something gets wrong!" << std::endl;
        r_x = nullptr;
        r_y = nullptr;
    }
}

void Rectangle::setSideLength() {
    length = fabs(r_x->getX()-r_y->getX());
    width = fabs(r_x->getY()-r_y->getY());
}

std::tuple<double, double> Rectangle::getSide() const {
    return std::make_tuple(length, width);
}

double Rectangle::area() const {
    return length*width;
}

std::ostream &operator<<(std::ostream &output, const Rectangle& rectangle){
    rectangle.shapeName();
    output << "Rectangle x: (" << rectangle.r_x->getX() << ", " << rectangle.r_x->getY() << ")" << std::endl;
    output << "Rectangle y: (" << rectangle.r_y->getX() << ", " << rectangle.r_y->getY() << ")" << std::endl;
    output << "Rectangle area: " << rectangle.area() << std::endl;
    return output;
}


int main() {
    Point point1 = Point(0, 0);
    Point point2 = Point(1, 0);
    Point point3 = Point(0, 1);
    Point point4 = Point(2, 2);
    Point point5 = Point(3, 3);
    Point point6 = Point(2, 0);
    Circle circle = Circle(0, 0, 2);
    Circle test_circle = Circle(0, 0, -1);
    Triangle triangle = Triangle(point1, point2, point3);
    Triangle test_triangle = Triangle(point1, point2, point6);
    Rectangle rectangle = Rectangle(point4, point5);
    Rectangle test_rectangle = Rectangle(point1, point1);
    double area_sum = 0.0;
    auto PrintVisitor = [&](const auto& t) {
        area_sum += (*t).area();
        std::cout << *t << std::endl;
    };
    for (auto &shape : shapes) {
        std::visit(PrintVisitor, shape);
    }
    std::cout << std::endl << "Area sum: " << area_sum << std::endl;
}
