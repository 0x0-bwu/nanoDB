
#pragma once
#include <boost/phoenix/fusion.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/variant.hpp>
#include <sstream>
#include <ostream>
#include <memory>
namespace nano::liberty::ast {

namespace io { inline static constexpr char sp(32), eol('\n'); }

struct SimpleAttribute
{
    using Value = boost::variant<int, float, std::string>;
    std::string name;
    Value value;
};

struct ComplexAttribute
{
    std::string name;
    std::vector<SimpleAttribute::Value> values;
};

enum class AttributeType : int8_t
{
    BOOLEAN,
    STRING,
    INTEGER,
    FLOATING_POINT
};

inline static std::string Enum2Str(const AttributeType & attrType)
{
    switch (attrType)
    {
    case AttributeType::BOOLEAN :
        return "Boolean";
    case AttributeType::STRING :
        return "string";
    case AttributeType::INTEGER :
        return "integer";
    case AttributeType::FLOATING_POINT :
        return "floating point";
    default:
        throw std::runtime_error("unknown attribute_type");
    }
};

inline static AttributeType Str2Enum(const std::string & str)
{
    if ("Boolean" == str) return AttributeType::BOOLEAN;
    else if ("string" == str) return AttributeType::STRING;
    else if ("integer" == str) return AttributeType::INTEGER;
    else if ("floating point" == str) return AttributeType::FLOATING_POINT;
    else throw std::runtime_error("unknown attribute_type: " + str);
}
struct DefineStatement
{
    std::string attrName;
    std::string groupName;
    AttributeType attrType;
};

struct GroupStatement;

using  AttributeStatement = boost::variant<DefineStatement, SimpleAttribute, ComplexAttribute, GroupStatement>;

struct GroupStatement
{
    std::string groupName;
    std::string name;
    std::vector<AttributeStatement> statements;
};

using LibertyDescription = GroupStatement;

} // namespace nano::liberty::ast

namespace {
//forward declaration
inline std::ostream & operator<< (std::ostream & os, const nano::liberty::ast::SimpleAttribute & simpleAttr);
inline std::ostream & operator<< (std::ostream & os, const nano::liberty::ast::ComplexAttribute & complexAttr);
inline std::ostream & operator<< (std::ostream & os, const nano::liberty::ast::DefineStatement & defineStatement);
inline std::ostream & operator<< (std::ostream & os, const nano::liberty::ast::AttributeStatement & attrStatement);
inline std::ostream & operator<< (std::ostream & os, const nano::liberty::ast::GroupStatement & groupStatement);

}

namespace nano::liberty::ast {

class AttrStatement2StringVisitor : public boost::static_visitor<std::string>
{
public:
    template <typename T>
    std::string operator() (const T & t) const
    {
        std::stringstream ss; ss << t;
        return ss.str();
    }
};

} // namespace nano::liberty::ast

namespace {

inline std::ostream & operator<< (std::ostream & os, const nano::liberty::ast::SimpleAttribute & simpleAttr)
{
    using namespace nano::liberty::ast::io;
    os << simpleAttr.name << ':' << sp << simpleAttr.value << ';';
    return os;
}

inline std::ostream & operator<< (std::ostream & os, const nano::liberty::ast::ComplexAttribute & complexAttr)
{
    using namespace nano::liberty::ast::io;
    os << complexAttr.name << sp << '(';
    for (size_t i = 0; i < complexAttr.values.size(); ++i) {
        os << sp << complexAttr.values.at(i);
        if (i != complexAttr.values.size() - 1)
            os << ',';
    }
    os << ");";
    return os;
}

inline std::ostream & operator<< (std::ostream & os, const nano::liberty::ast::DefineStatement & defineStatement)
{
    os << "define(" << defineStatement.attrName << ", " << defineStatement.groupName << ", " << Enum2Str(defineStatement.attrType) << ") ;";
    return os;
}

inline std::ostream & operator<< (std::ostream & os, const nano::liberty::ast::AttributeStatement & attrStatement)
{
    os << boost::apply_visitor(nano::liberty::ast::AttrStatement2StringVisitor(), attrStatement);
    return os;
}

inline std::ostream & operator<< (std::ostream & os, const nano::liberty::ast::GroupStatement & groupStatement)
{
    using namespace nano::liberty::ast::io;
    os << groupStatement.groupName << sp << "(" << groupStatement.name << ") {" << eol;
    for (const auto & statement : groupStatement.statements)
        os << statement << eol;
    os << '}' << eol;
    return os;
}

}