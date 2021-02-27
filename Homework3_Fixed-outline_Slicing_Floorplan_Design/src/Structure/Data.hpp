#pragma once
#include <string>
#include <vector>

struct Pin
{
    std::string name;
    int x, y;

    Pin(std::string const &name, int const &x, int const &y)
        : name(name), x(x), y(y) {}
};

struct Hardblock
{
    std::string name;
    int width, height, x, y;
    bool isRotated;
    Pin *pin;

    Hardblock(std::string const &name, int const &width, int const &height)
        : name(name), width(width), height(height), x(0), y(0), isRotated(false), pin(new Pin(name, x, y)) {}
    void update(int const &_width, int const &_height, int const &_x, int const &_y);
};

struct Net
{
    std::vector<Pin *> pins;

    int HPWL();
};

struct Node
{
    enum Type
    {
        VERTICAL_CUT = -2,
        HORIZONTAL_CUT = -1,
        HARDBLOCK = 0
    };

    int type;
    Hardblock *hardblock;
    Node *lchild, *rchild;
    std::vector<std::vector<int>> record; // {{width, height, left choice, right choice}}

    Node(int const &type, Hardblock *hardblock = nullptr)
        : type(type), hardblock(hardblock), lchild(nullptr), rchild(nullptr)
    {
        if (hardblock != nullptr)
            record = {{hardblock->width, hardblock->height, 0, 0},
                      {hardblock->height, hardblock->width, 1, 1}};
    }
    void updateRecord();
};

struct SAInput
{
    std::vector<Hardblock *> hardblocks;
    std::vector<Net *> nets;
    double deadSpaceRatio;

    SAInput(std::vector<Hardblock *> const &hardblocks, std::vector<Net *> const &nets, double const &deadSpaceRatio)
        : hardblocks(hardblocks), nets(nets), deadSpaceRatio(deadSpaceRatio) {}
};
