#include "NSKiCadParser.h"
#include "NSKiCadObject.h"
namespace nano::package::extension::kicad {

bool KiCadParser::operator() (std::string_view filename, Tree & tree)
{
    std::ifstream in(filename.data());
    if (not in.good()) return false;
    tree = ReadTree(in);
    return true;
}

Tree KiCadParser::ReadTree(std::istream & in)
{
    char c;
    ReadUntil(in, '(');
    ReadWhitespace(in);
    auto t = Tree{ReadNodeName(in), {}};
    for(;;) {
        ReadWhitespace(in);
        auto b = in.peek();
        if (b == EOF) break;
        if (b == ')') {
            in.get(c);
            break;
        }
        if (b == '(') {
            t.branches.emplace_back(ReadTree(in));
            continue;
        }
        if (b == '"') {
            in.get(c);
            t.branches.emplace_back(ReadQuotedString(in));
            in.get(c);
            continue;
        }
        t.branches.emplace_back(ReadString(in));
    }
    return t;
}

Tree KiCadParser::ReadString(std::istream & in)
{
    std::string s;
    for (;;) {
        auto b = in.peek();
        if (b == '\t' || b == '\n' || b == '\r' || b == ')' || b == ' ')
            break;
        char c;
        in.get(c);
        s.push_back(c);
    }
    return Tree{s, {}};    
}

Tree KiCadParser::ReadQuotedString(std::istream & in)
{
    char c;
    std::string s;
    // s.push_back('"');
    auto a = in.peek();
    for (;;) {
        auto b = in.peek();
        if (b == '"' && a != '\\')
            break;
        in.get(c);
        s.push_back(c);
        a = b;
    }
    // s.push_back('"');
    return Tree{s, {}};
}

std::string KiCadParser::ReadNodeName(std::istream & in)
{
    char c;
    std::string s;
    for(;;) {
        auto b = in.peek();
        if (b == '\t' || b == '\n' || b == '\r' || b == ' ' || b == ')') break;
        in.get(c);
        s.push_back(c);
    }
    return s;
}

bool KiCadParser::ReadUntil(std::istream & in, char c)
{
    char input;
    while(in.get(input)) {
        if (input == c)
            return false;
    }
    return true;
}

void KiCadParser::ReadWhitespace(std::istream & in)
{
    char c;
    for (;;) {
        auto b = in.peek();
        if (b != '\t' && b != '\n' && b != '\r' && b != ' ')
            break;
        in.get(c);
    }
}

} // namespace nano::package::extension::kicad