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
 * @file parse_parfile_example.c
 * @brief
 * @author Matthieu Lefebvre
 */

#include <stdio.h>

#include "ASDF_provenance.h"

int main(int argc, char *argv[]) {
  char *prov;
  FILE *fd;

  /*=====================================================*/
  /* Function call of interest:                          */
  ASDF_generate_par_file_provenance(
      "Par_file",                       /* Par_file path    */
      "SPECFEM-style input parameters", /* provenance label */
      "id_1234",                        /* provenance id    */
      &prov);                           /* result           */
  /*=====================================================*/

  /*fprintf(stderr, "\n\n%s\n\n", prov);*/
  fd = fopen("prov_par_file.txt", "w");

  if (fd) {
    fputs(prov, fd);
    fclose(fd);
  }

  /*=====================================================*/
  /* Do not forget to clean:                             */
  ASDF_clean_par_file_provenance(prov);
  /*=====================================================*/
  return 0;
}

