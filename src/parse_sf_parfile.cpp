#include <iostream>
#include <fstream>
#include <vector>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include "prov_parameter.h"

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

// Needs to be in the global namespace
BOOST_FUSION_ADAPT_STRUCT (
  parameter,
  (std::string, name)
  (std::string, type)
  (std::string, value)
)

namespace specfem {

std::vector<std::string> read_par_file(const std::string &filename) {
  std::vector<std::string> lines;
  std::ifstream is (filename);

  // TODO: determine what to do in case of failure.
  if (is.is_open()) {
    // TODO: move the hardcoded 512.
    constexpr std::streamsize max_length = 512;
    char *buf = new char[max_length];

    while(is.getline(buf, max_length)) {
      lines.push_back(buf);
    }
    delete[] buf;
  }
  return lines;
}

template <typename Iterator>
struct par_file_grammar : qi::grammar<Iterator, parameter()> {
  par_file_grammar() : par_file_grammar::base_type(expr) {
     using qi::lit;
     using namespace qi::labels;
     using phoenix::at_c;

    // We actually don't care about comments or blank lines.
    // A parse failure is good enough for us.

    boolean = qi::no_case [qi::string(".true.") 
                         | qi::string(".false.")];
    real = -qi::char_("-") 
        >> +qi::digit 
        >> qi::char_(".") 
        >> *qi::digit 
        >> -qi::string("d0");

    integer = -qi::char_("-") >> +qi::char_("0-9~.");

    identifier = +(qi::alnum | qi::char_("_"));
    
    // Identifier that can start with a number.
    // Forced by geophycists practice in model naming.
    gnl_id = *qi::digit 
          >> (qi::alnum | qi::char_("_")) 
          >> identifier;

    comment = qi::char_("#") >> *qi::char_;

    // An expression looks like:
    // VARIABLE_NAME = VALUE   # comment
    // with VALUE being a boolean, an integer, a real or a string.
    expr =
         identifier [at_c<0>(_val) = _1]
      >> qi::omit [*qi::space
      >>            qi::char_("=")
      >>           *qi::space]
      >> (qi::hold [boolean] [at_c<1>(_val) = "xsd:boolean"]
        | qi::hold [real]    [at_c<1>(_val) = "xsd:double"]
        | qi::hold [gnl_id]  [at_c<1>(_val) = "xsd:string"]
        | qi::hold [integer] [at_c<1>(_val) = "xsd:int"]
        ) [at_c<2>(_val) = _1]
      >> qi::omit [-(*qi::space >> comment)];
  }

  qi::rule<Iterator, parameter()> expr;
  qi::rule<Iterator, std::string()> comment;
  qi::rule<Iterator, std::string()> boolean;
  qi::rule<Iterator, std::string()> real;
  qi::rule<Iterator, std::string()> integer;
  qi::rule<Iterator, std::string()> identifier;
  qi::rule<Iterator, std::string()> gnl_id;
};

std::vector<parameter> parse(std::string filename) {
  std::vector<std::string> lines = read_par_file(filename);

  typedef std::string::const_iterator iterator_type;
  typedef par_file_grammar<iterator_type> par_file_grammar;

  par_file_grammar par_file_parser;
  
  std::vector<parameter> params;

  for (auto str : lines) {
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();

    parameter result;
    if (parse(iter, end, par_file_parser, result))
      params.push_back(result);
  }
  return params;
}

}  // namespace specfem
/*
int main(int argc, char *argv[]) {
  std::string filename = "Par_file";

  std::vector<parameter> params = specfem::parse("Par_file");

  for (auto p : params) {
    //std::cout << "-------------------------\n";
    std::cout << p.name << " / " << p.type << " / " << p.value;
    std::cout << std::endl;
  }

  return 0;
}
*/
