#include <stdio.h>

#include "ASDF_generate_provenance.h"

int main(int argc, char *argv[]) {
  char *prov;
  ASDF_generate_par_file_provenance(
      "Par_file",
      "SPECFEM-style input parameters",
      "id_1234",
      &prov);

  FILE *fd = fopen("prov_par_file.txt", "w");

  if (fd) {
    /*fprintf(stderr, "\n\n%s\n\n", prov);*/
    fputs(prov, fd);
    fclose(fd);
  }

  ASDF_clean_par_file_provenance(prov);
  return 0;
}
