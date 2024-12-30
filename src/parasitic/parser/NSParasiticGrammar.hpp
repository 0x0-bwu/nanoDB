#pragma once
#include "NSParasiticDescription.hpp"
#include "generic/tools/Parser.hpp"

namespace nano::parasitic::grammar {

template <typename Iterator>
using ErrorHandler = generic::parser::ErrorHandler<Iterator>;

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;
namespace spirit = boost::spirit;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
struct SpefSkipper : qi::grammar<Iterator>
{
    qi::rule<Iterator> skip;
    SpefSkipper() : SpefSkipper::base_type(skip)
    {
        using qi::lexeme;
        using qi::char_;
        using qi::lit;
        using qi::eol;
        skip = ascii::space | (lit("//") >> lexeme[*(char_ - eol) >> eol]);
    }
};

template <typename Iterator, typename Skipper>
struct SpefGrammar : qi::grammar<Iterator, ast::SpefDescription(), Skipper>
{
    ErrorHandler<Iterator> & errHandler;
    qi::symbols<char, ast::ConnectType> connectType;
    qi::symbols<char, IOType> ioType;
    qi::symbols<char, char> escaped;
    qi::rule<Iterator, std::string()> name;
    qi::rule<Iterator, std::string()> quotedString;
    qi::rule<Iterator, std::string(), Skipper> string2eol;
    qi::rule<Iterator, ast::SpefHeader(), Skipper> spefHeader;
    qi::rule<Iterator, ast::SpefNamePair(), Skipper> spefNamePair;
    qi::rule<Iterator, ast::SpefNameMap(), Skipper> spefNameMap;
    qi::rule<Iterator, ast::SpefName(), Skipper> spefName;
    qi::rule<Iterator, ast::SpefNode(), Skipper> spefNode;
    qi::rule<Iterator, ast::SpefCoord(), Skipper> spefCoord;
    qi::rule<Iterator, ast::SpefPort(), Skipper> spefPort;
    qi::rule<Iterator, ast::SpefPorts(), Skipper> spefPorts;
    qi::rule<Iterator, ast::SpefConn(), Skipper> spefConn;
    qi::rule<Iterator, ast::SpefConns(), Skipper> spefConns;
    qi::rule<Iterator, ast::SpefValue(), Skipper> spefValue;
    qi::rule<Iterator, ast::SpefValues(), Skipper> spefRess;
    qi::rule<Iterator, ast::SpefValues(), Skipper> spefCaps;
    qi::rule<Iterator, ast::SpefNet(), Skipper> spefNet;
    qi::rule<Iterator, ast::SpefNets(), Skipper> spefNets;
    qi::rule<Iterator, ast::SpefDescription(), Skipper> spefDescription;

    SpefGrammar(ErrorHandler<Iterator> & errHandler)
     : SpefGrammar::base_type(spefDescription), errHandler(errHandler)
    {
        using qi::_1;
        using qi::_2;
        using qi::_3;
        using qi::_4;
        using qi::eps;
        using qi::eol;
        using qi::lit;
        using qi::_val;
        using qi::int_;
        using phx::at_c;
        using qi::char_;
        using qi::float_;
        using qi::lexeme;
        using qi::symbols;
        using phx::push_back;
        using spirit::repeat;
        connectType.add("*I", ast::ConnectType::INTERNAL)("*P", ast::ConnectType::EXTERNAL);
        ioType.add("I", IOType::INPUT)("B", IOType::INOUT)("O", IOType::OUTPUT);
        escaped.add("\\/", '/')("\\[", '[')("\\]", ']');

        name = char_("A-Za-z_") >> *(escaped | (char_("A-Za-z-0-9_[]") - '\\'));
        quotedString = lit('"') >> *(char_ - '"') >> char_('"');
        string2eol = lexeme[*(char_ - eol) >> eol];

        spefHeader = lit("*SPEF") >> quotedString[at_c<0>(_val) = _1] >>
        (
            (lit("*DESIGN"       ) > quotedString[at_c<1 >(_val) = _1]) ^
            (lit("*DATE"         ) > quotedString[at_c<2 >(_val) = _1]) ^
            (lit("*VENDOR"       ) > quotedString[at_c<3 >(_val) = _1]) ^
            (lit("*PROGRAM"      ) > quotedString[at_c<4 >(_val) = _1]) ^
            (lit("*VERSION"      ) > quotedString[at_c<5 >(_val) = _1]) ^
            (lit("*DESIGN_FLOW"  ) > string2eol  [at_c<6 >(_val) = _1]) ^
            (lit("*DIVIDER"      ) > string2eol  [at_c<7 >(_val) = _1]) ^
            (lit("*DELIMITER"    ) > string2eol  [at_c<8 >(_val) = _1]) ^
            (lit("*BUS_DELIMITER") > string2eol  [at_c<9 >(_val) = _1]) ^
            (lit("*T_UNIT"       ) > string2eol  [at_c<10>(_val) = _1]) ^
            (lit("*C_UNIT"       ) > string2eol  [at_c<11>(_val) = _1]) ^
            (lit("*R_UNIT"       ) > string2eol  [at_c<12>(_val) = _1]) ^
            (lit("*L_UNIT"       ) > string2eol  [at_c<13>(_val) = _1])
        );
        spefNamePair = lit('*') > int_ > name;
        spefNameMap = lit("*NAME_MAP") >> *(spefNamePair - (lit("*PORTS") | lit("*D_NET")));

        spefName = lexeme[lit('*') > int_] | name;
        spefNode = spefName > -(lit(':') > (int_ | name));
        spefCoord = lit("*C") > float_ > float_;
        spefPort = spefName > ioType > -spefCoord;
        spefPorts = lit("*PORTS") > *(spefPort[push_back(_val, _1)] - lit("*D_NET"));
        
        spefConn = connectType > spefNode > ioType > -spefCoord > -(lit("*L") > float_) > -(lit("*D") > spefName);
        spefConns = lit("*CONN") > *(spefConn - (lit("*CAP") | lit("*RES") | lit("*END")));

        spefValue = int_ > spefNode > -spefNode > float_;
        spefRess = lit("*RES") > *(spefValue - (lit("*CONN") | lit("*CAP") | lit("*END")));
        spefCaps = lit("*CAP") > *(spefValue - (lit("*CONN") | lit("*RES") | lit("*END")));
        spefNet = lit("*D_NET") > spefName[at_c<0>(_val) = _1] > float_[at_c<1>(_val) = _1] > 
                    (
                        spefConns[at_c<2>(_val) = _1] ^
                        spefRess[at_c<3>(_val) = _1] ^
                        spefCaps[at_c<4>(_val) = _1]
                    ) > lit("*END");
        spefNets = *(spefNet);
        spefDescription = spefHeader > -spefNameMap > -spefPorts > spefNets;

        connectType.name("connect type");
        ioType.name("IO type");
        name.name("name");
        string2eol.name("string to EOL");
        quotedString.name("quoted string");
        spefHeader.name("spef header");
        spefNamePair.name("spef name pair");
        spefNameMap.name("spef name map");
        spefName.name("spef name");
        spefNode.name("spef node");
        spefCoord.name("spef coord");
        spefPort.name("spef port");
        spefPorts.name("spef ports");
        spefConn.name("spef conn");
        spefConns.name("spef conns");
        spefValue.name("spef value");
        spefRess.name("spef resistances");
        spefCaps.name("spef capacitances");
        spefNet.name("spef net");
        spefNets.name("spef nets");
        spefDescription.name("spef description");

        qi::on_error<qi::fail>(spefDescription, phx::function<ErrorHandler<Iterator> >(errHandler)("expecting ", _4, _3));
    }
};

} // namespace nano::parasitic::grammar
