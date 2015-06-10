#ifndef _ASDF_GEN_PARAM_PROVENANCE_
#define _ASDF_GEN_PARAM_PROVENANCE_

/*
std::string generate_provenance_entity(std::string label, 
                                       std::string id,
                                       std::vector<parameter> params);
*/

#ifdef __cplusplus
extern "C" {
#endif

char *generate_par_file_provenance(char *filename,
                                   char *prov_label,
                                   char *prov_id);

void clean_par_file_provenance(char *sub_provenance);

#ifdef __cplusplus
}
#endif


#endif  // _ASDF_GEN_PARAM_PROVENANCE_
