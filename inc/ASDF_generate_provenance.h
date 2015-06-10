#ifdef _ASDF_GENERATE_PROVENANCE_H_
#define _ASDF_GENERATE_PROVENANCE_H_

#ifdef __cplusplus
extern "C" {
#endif

void ASDF_generate_par_file_provenance(char *filename,
                                       char *prov_label,
                                       char *prov_id,
                                       char *prov);

void ASDF_clean_par_file_provenance(char *sub_provenance);

#ifdef __cplusplus
}
#endif

#endif  // _ASDF_GENERATE_PROVENANCE_H_
