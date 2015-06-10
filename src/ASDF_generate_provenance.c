#include <stdio.h>

#include "gen_param_provenance.h"
#include "ASDF_generate_provenance.h"

void ASDF_generate_par_file_provenance(char *filename,
                                       char *prov_label,
                                       char *prov_id,
                                       char **prov) {
  *prov = generate_par_file_provenance(filename, prov_label, prov_id);
  /*fprintf(stderr, "\n\n%s\n\n", *prov);*/
}

void ASDF_clean_par_file_provenance(char *sub_provenance) {
  clean_par_file_provenance(sub_provenance);
}
