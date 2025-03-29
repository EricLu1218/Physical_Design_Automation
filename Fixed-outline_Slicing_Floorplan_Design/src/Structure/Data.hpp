#pragma once
#include <memory>
#include <string>
#include <vector>

struct Pin
{
    using ptr = std::unique_ptr<Pin>;

    std::string name;
    int x, y;

    Pin();
    Pin(const std::string &name_, int x_, int y_);
};

struct Hardblock
{
    using ptr = std::unique_ptr<Hardblock>;

    std::string name;
    int width, height;

    int x, y;
    bool isRotated;
    Pin::ptr pin;

    Hardblock();
    Hardblock(const std::string &name_, int width_, int height_);
    int currWidth() const;
    int currHeight() const;
    void update(int width_, int height_, int x_, int const y_);
};

struct Net
{
    using ptr = std::unique_ptr<Net>;

    std::vector<Pin *> pins;

    Net();
    int wirelength() const;
};

struct Input
{
    using ptr = std::unique_ptr<Input>;

    std::vector<Hardblock::ptr> hardblocks;
    std::vector<Pin::ptr> fixedPins;
    std::vector<Net::ptr> nets;
    double deadspaceRatio;

    Input();
};

struct Node
{
    using ptr = std::unique_ptr<Node>;

    struct Record
    {
        int width, height, leftChoice, rightChoice;

        Record();
        Record(int width_, int height_, int leftChoice_, int rightChoice_);
    };

    enum Type
    {
        HORIZONTAL_CUT = -2,
        VERTICAL_CUT = -1,
        HARDBLOCK = 0
    };

    int type;
    Hardblock *hardblock;
    Node *lchild, *rchild;
    std::vector<Record> records;

    Node();
    Node(int type_, Hardblock *hardblock_ = nullptr);
    void updateRecord();
};
