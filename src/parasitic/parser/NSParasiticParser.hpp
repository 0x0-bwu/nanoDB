#pragma once
#include "NSParasiticGrammar.hpp"

namespace nano::parasitic::parser {

struct SpefParser
{
    using AST = ast::SpefDescription;
    
    AST & ast;
    std::string error;
    explicit SpefParser(AST & ast) : ast(ast) {}
    bool operator() (std::string_view filename)
    {
        std::ifstream in(filename.data());
        if (not in.is_open()) return false;

        in.unsetf(std::ios::skipws);
        std::string storage;
        std::copy(std::istream_iterator<char>(in), std::istream_iterator<char>(), std::back_inserter(storage));
        return Parse(storage.begin(), storage.end());
    }
private:
    template <typename Iterator>
    bool Parse(Iterator begin, Iterator end)
    {
        using namespace grammar;
        using Skipper = SpefSkipper<Iterator>;
        Skipper skipper;
        std::stringstream ss;
        ErrorHandler<Iterator> errHandler(begin, end, ss);
        SpefGrammar<Iterator, Skipper> grammar(errHandler);
        bool res = qi::phrase_parse(begin, end, grammar, skipper, ast);
        if (not res) error = ss.str();
        return res && (begin == end);
    }
};

} // nano::parasitic::parser