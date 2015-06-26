/******************************************************************************
 * Copyright 2015 ASDF developers
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
/** 
 * @file parse_sf_parfile.h
 * @brief 
 * @author Matthieu Lefebvre
 */

#include <iostream>
#include <fstream>
#include <vector>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include "prov_parameter.h"
#include "parse_sf_parfile.h"

///////////////////////////////////////////////////////////
namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

///////////////////////////////////////////////////////////
// Make struct act as a tuple
BOOST_FUSION_ADAPT_STRUCT (
  parameter,
  (std::string, name)
  (std::string, type)
  (std::string, value)
)

///////////////////////////////////////////////////////////
// Fill a vector with the content of the Par_file
std::vector<std::string> read_par_file(const std::string &filename) {
  std::vector<std::string> lines;
  std::ifstream is (filename.c_str());

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

///////////////////////////////////////////////////////////
// Define the Grammar use to parse the Par_file
template <typename Iterator>
struct par_file_grammar : qi::grammar<Iterator, parameter()> {
  par_file_grammar() : par_file_grammar::base_type(expr) {
     using qi::lit;
     using namespace qi::labels;
     using phoenix::at_c;

    // We actually don't care about comments or blank lines.
    // A parse failure is good enough for us.

    // xsd:boolean representation is {true, bool}
    boolean = qi::no_case [ qi::string(".true.") [_val = "true" ]
                          | qi::string(".false.") [_val = "false"]
                          ];
    // xsd:double does not include 'd0' at the end
    real = -qi::char_("-") 
        >> +qi::digit 
        >> qi::char_(".") 
        >> *qi::digit 
        >> qi::omit[-qi::string("d0")];

    //integer = -qi::char_("-") >> +(qi::char_("0-9") - qi::char_("."));
    integer = -qi::char_("-") >> +qi::digit;

    identifier = +(qi::alnum | qi::char_("_"));

    path = +(qi::alnum | qi::char_("_./"));
    
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
        |           path     [at_c<1>(_val) = "xsd:string"]
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
  qi::rule<Iterator, std::string()> path;
};

///////////////////////////////////////////////////////////
// Actual parsing function
//
std::vector<parameter> parse_sf_parfile(const std::string filename) {
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
