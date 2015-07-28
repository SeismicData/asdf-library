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
 * @file ASDF_generate_provenance.c
 * @brief
 * @author Matthieu Lefebvre
 */

#include <stdio.h>

#include "ASDF_provenance.h"
#include "gen_sf_parfile_provenance.h"
#include "gen_sf_provenance.h"

void ASDF_generate_par_file_provenance(const char *filename,
                                       const char *prov_label,
                                       const char *prov_id,
                                       char **prov) {
  *prov = generate_sf_parfile_provenance(filename, prov_label, prov_id);
}

void ASDF_clean_par_file_provenance(char *sub_provenance) {
  clean_sf_parfile_provenance(sub_provenance);
}

void ASDF_generate_sf_provenance(const char *startTime,
                                const char *endTime,
                                char **prov,
                                int *length) {
  char *temp_prov = generate_sf_provenance(startTime, endTime);
  *length = strlen(temp_prov);
  *prov = malloc (*length);
  strncpy(*prov, temp_prov, *length);
}

void ASDF_clean_provenance(char **provenance) {
  free(*provenance);
}
