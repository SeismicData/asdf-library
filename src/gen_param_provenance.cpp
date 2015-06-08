#include <iostream>
#include <sstream>
#include <vector>

#include "prov_parameter.h"

std::string generate_provenance_entity(std::string label, 
                                       std::string id,
                                       std::vector<parameter> params) {

  std::ostringstream prov;

  prov << "<prov:entity prov:id=\"seis_prov:" << id << "\">"
       << "<prov:label>" << name << "</prov:label>"
       << "<prov:type xsi:type=\"xsd:string\">"
          "seis_prov:input_parameter</prov:type>";

  for (auto p : params) {
    prov << "<seis_prov:" << p.name << "xsi:type=\"" << p.type << "\">" 
         << p.value 
         << "</seis_prov:" << p.name << ">";
  }


  return prov.str();
}
/*
int main(int argc, char *argv[])
{
  parameter p;
  p.name = std::string("name");
  p.type = std::string("type");
  p.value = std::string("value");

  std::vector<parameter> ps;
  ps.push_back(p);

  std::string str = generate_provenance_entity("specfem input par", "id_1234", ps);
  std::cout << str << std::endl;

  return 0;
}*/
