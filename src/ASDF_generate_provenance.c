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
 * @file ASDF_write.h
 * @brief 
 * @author Matthieu Lefebvre
 */

#include <stdio.h>

#include "gen_sf_parfile_provenance.h"
#include "ASDF_generate_provenance.h"

#ifdef __cplusplus
extern "C" {
#endif

void ASDF_generate_par_file_provenance(const char *filename,
                                       const char *prov_label,
                                       const char *prov_type,
                                       const char *prov_id,
                                       char **prov) {
  *prov = generate_sf_parfile_provenance(filename, prov_label, prov_type, prov_id);
}

void ASDF_clean_par_file_provenance(char *sub_provenance) {
  clean_sf_parfile_provenance(sub_provenance);
}

#ifdef __cplusplus
}
#endif
