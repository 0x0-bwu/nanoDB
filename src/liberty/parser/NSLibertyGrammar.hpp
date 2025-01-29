#pragma once
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/phoenix.hpp>
#include <boost/fusion/adapted.hpp>
#include "NSLibertyDescription.hpp"
///@brief modified from https://github.com/csguth/Liberty.git

BOOST_FUSION_ADAPT_STRUCT(
    nano::liberty::ast::SimpleAttribute,
    (std::string, name)
    (nano::liberty::ast::SimpleAttribute::Value, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::liberty::ast::ComplexAttribute,
    (std::string, name)
    (nano::Vec<nano::liberty::ast::SimpleAttribute::Value>, values)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::liberty::ast::DefineStatement,
    (std::string, attrName)
    (std::string, groupName)
    (nano::liberty::ast::AttributeType, attrType)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::liberty::ast::GroupStatement,
    (std::string, groupName)
    (std::string, name)
    (nano::Vec<nano::liberty::ast::AttributeStatement>, statements)
)

namespace nano::liberty::grammar {

using namespace ast;
namespace phx = boost::phoenix;
namespace x3 = boost::spirit::x3;
namespace ascii = x3::ascii;

using x3::lit;
using x3::eps;
using x3::eol;
using x3::int_;
using x3::_val;
using x3::char_;
using x3::float_;
using x3::lexeme;
using x3::symbols;
using phx::at_c;
using phx::push_back;
struct AttrSymbols : x3::symbols<AttributeType>
{
    AttrSymbols()
    {
        add("Boolean"       , AttributeType::BOOLEAN)
           ("string"        , AttributeType::STRING)
           ("integer"       , AttributeType::INTEGER)
           ("floating point", AttributeType::FLOATING_POINT);
    }
} inline const attrSymbols;

inline x3::real_parser<float, x3::strict_real_policies<float> > strictFloat;
inline x3::rule<class name, std::string> name = "name";
inline x3::rule<class quotedString, std::string> quotedString = "quotedString";
inline x3::rule<class nonQuotedString, std::string> nonQuotedString = "nonQuotedString";
inline x3::rule<class parenthesedString, std::string> parenthesedString = "parenthesedString";
inline x3::rule<class stringValue, std::string> stringValue = "stringValue";
inline x3::rule<class value, SimpleAttribute::Value> value = "value";
inline x3::rule<class simpleAttribute, SimpleAttribute> simpleAttribute = "simpleAttribute";
inline x3::rule<class complexAttribute, ComplexAttribute> complexAttribute = "complexAttribute";
inline x3::rule<class defineStatement, DefineStatement> defineStatement = "defineStatement";
inline x3::rule<class attributeStatement, AttributeStatement> attributeStatement = "attributeStatement";
inline x3::rule<class groupStatement, GroupStatement> groupStatement = "groupStatement";
inline x3::rule<class skip> skip = "skip";

inline const auto name_def = char_("A-Za-z_") >> *(char_("A-Za-z-0-9_\\-"));
inline const auto quotedString_def = lexeme['"' >> *(char_ - '"') >> '"'];
inline const auto nonQuotedString_def = *char_("\\-0-9") >> char_("A-Za-z") >> *char_("A-Za-z0-9_\\-");
inline const auto parenthesedString_def = lexeme['(' >> *(char_ - ')') >> ')'];
inline const auto stringValue_def = quotedString | nonQuotedString;
inline const auto value_def = strictFloat | int_ | stringValue;
inline const auto simpleAttribute_def = name >> ':' >> value >> ';';
inline const auto complexAttribute_def = name >> '(' >> (value % ',')  >> ')' >> lit(";");
inline const auto defineStatement_def = lit("define") >> '(' >> name >> ',' >> name >> ',' >> attrSymbols >> ')' >> lit(";");
inline const auto attributeStatement_def = defineStatement | simpleAttribute | complexAttribute | groupStatement;
inline const auto groupStatement_def = name >> parenthesedString >> '{' >> *attributeStatement >> '}';
inline const auto skip_def = ascii::space | '\\' | lexeme[lit("/*") >> *(char_ - x3::string("*/")) >> lit("*/")];

BOOST_SPIRIT_DEFINE(
    name, 
    quotedString, 
    nonQuotedString, 
    parenthesedString,
    stringValue,
    value,
    simpleAttribute,
    complexAttribute, 
    defineStatement, 
    attributeStatement, 
    groupStatement,
    skip
)
} // namespace nano::liberty::grammar