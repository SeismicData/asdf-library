#include <iostream>
#include <sstream>
#include <vector>

#include "prov_parameter.h"
#include "gen_param_provenance.h"

// TODO: blob, see next TODO
#include "parse_sf_parfile.h"

std::string generate_provenance_entity(std::string label, 
                                       std::string id,
                                       std::vector<parameter> params) {

  std::ostringstream prov;

  prov << "<prov:entity prov:id=\"seis_prov:" << id << "\">"
       << "<prov:label>" << label << "</prov:label>"
       << "<prov:type xsi:type=\"xsd:string\">"
          "seis_prov:input_parameter</prov:type>";

  for (auto p : params) {
    prov << "<seis_prov:" << p.name << " xsi:type=\"" << p.type << "\">" 
         << p.value 
         << "</seis_prov:" << p.name << ">";
  }


  return prov.str();
}

/////////////////////
// TODO: move what follows
//
char *generate_par_file_provenance(char *filename,
                                   char *prov_label,
                                   char *prov_id) {
  std::vector<parameter> params = parse(std::string(filename));

  std::string xml_str = generate_provenance_entity(std::string(prov_label),
                                                   std::string(prov_id),
                                                   params);
  char *sub_provenance = new char[xml_str.length() +1];
  std::strncpy(sub_provenance, xml_str.c_str(), xml_str.length());
  //std::cout << sub_provenance << std::endl;

  return sub_provenance;
}

void clean_par_file_provenance(char *sub_provenance) {
  if (sub_provenance)
    delete[] sub_provenance;
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
