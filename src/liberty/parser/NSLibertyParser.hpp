#pragma once
#include "NSLibertyGrammar.hpp"
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

namespace nano::liberty::parser {

namespace x3 = boost::spirit::x3;

struct LibertyParser
{
    using AST = grammar::LibertyDescription;
    
    AST & ast;
    std::string error;
    explicit LibertyParser(AST & ast) : ast(ast) {}
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
        std::stringstream ss;
        x3::error_handler<Iterator> errHandler(begin, end, ss);
        const auto parser = x3::with<x3::error_handler_tag>(std::ref(errHandler))[groupStatement_def];
        bool res = x3::phrase_parse(begin, end, parser, skip_def, ast) && (begin == end);
        if (not res) error = ss.str();
        return res;
    }
};

} // nano::liberty::parser