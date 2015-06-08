#include <vector>
#include <string>
#include <cstring>

#include <prov_parameter.h>
#include <parse_sf_parfile.h>
#include <gen_param_provenance.h>

char *ASDF_generate_par_file_provenance(char *filename,
                                       char *prov_label,
                                       char *prov_id) {
  std::vector<parameter> params = specfem::parse(std::string(filename));

  std::string xml_str = generate_provenance_entity(std::string(prov_label),
                                                   std::string(prov_id),
                                                   params);
  char *sub_provenance = new char[xml_str.length() +1];
  std::strncpy(sub_provenance, xml_str.c_str(), xml_str.length());

  return sub_provenance;
}

void ASDF_clean_par_file_provenance(char *sub_provenance) {
  delete[] sub_provenance;
}
