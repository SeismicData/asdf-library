#ifdef _ASDF_GENERATE_PROVENANCE_H_
#define _ASDF_GENERATE_PROVENANCE_H_

char *ASDF_generate_par_file_provenance(char *filename,
                                       char *prov_label,
                                       char *prov_id);

void ASDF_clean_par_file_provenance(char *sub_provenance);

#endif  // _ASDF_GENERATE_PROVENANCE_H_
