#pragma once
#include <nano/common>
#include <istream>
namespace nano::package::extension::kicad {

struct Tree
{
    std::string value;
    Vec<Tree> branches;
};

class KiCadParser
{
public:
    bool operator() (std::string_view filename, Tree & tree);

private:
    Tree ReadTree(std::istream & in);
    Tree ReadString(std::istream & in);
    Tree ReadQuotedString(std::istream & in);
    std::string ReadNodeName(std::istream & in);
    bool ReadUntil(std::istream & in, char c);
    void ReadWhitespace(std::istream & in);
};

}//namespace nano::package::extension::kicad