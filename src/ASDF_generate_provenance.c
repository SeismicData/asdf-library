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
 * @file ASDF_generate_provenance.h
 * @brief 
 * @author Matthieu Lefebvre
 */

#include <stdio.h>

#include "ASDF_generate_provenance.h"
#include "gen_software_provenance.h"
#include "gen_trace_provenance.h"
#include "gen_simulation_provenance.h"
#include "gen_association_provenance.h"
#include "gen_usage_provenance.h"
#include "gen_wasGeneratedby_provenance.h"
#include "gen_sf_parfile_provenance.h"
#include "gen_sf_provenance.h"
#include "parse_provfile.h"
#include "gen_sf_provfile.h"
#include "gen_provenance_id.h"

void ASDF_generate_software_agent(const char *software_name,
                                  const char *software_version,
                                  const char *software_website,
                                  const char *prov_label,
                                  const char *prov_id,
                                  char **prov) {
  *prov = generate_software_provenance(software_name, software_version, software_website, prov_label, prov_id);
}

void ASDF_generate_trace_entity(const char *prov_label,
                                const char *prov_id,
                                char **prov) {
  *prov = generate_trace_provenance(prov_label, prov_id);
}

void ASDF_generate_simulation_activity(const char *startTime,
                                       const char *endTime,
                                       const char *prov_label,
                                       const char *prov_id,
                                       char **prov) {
  *prov = generate_simulation_provenance(startTime, endTime, prov_label, prov_id);
}

void ASDF_association(const char *prov_id1,
                      const char *prov_id2,
                      char **prov) {
  *prov = generate_association_provenance(prov_id1, prov_id2);
}

void ASDF_usage(const char *prov_id1,
                const char *prov_id2,
                char **prov) {
  *prov = generate_usage_provenance(prov_id1, prov_id2);
}

void ASDF_wasGeneratedby(const char *prov_id1,
                         const char *prov_id2,
                         char **prov) {
  *prov = generate_wasGeneratedby_provenance(prov_id1, prov_id2);
}

void ASDF_generate_par_file_provenance(const char *filename,
                                       const char *prov_label,
                                       const char *prov_id,
                                       char **prov) {
  *prov = generate_sf_parfile_provenance(filename, prov_label, prov_id);
}

void ASDF_clean_par_file_provenance(char *sub_provenance) {
  clean_sf_parfile_provenance(sub_provenance);
}

void ASDF_generate_sf_provenance(const char *filename, const char *startTime, const char *endTime, char **provenance) {

  char *sf_prov, *pf_prov, *tr_prov, *ws_prov, *association_prov, *usage_prov, *generated_prov;
  char *software_name='\0', *software_version='\0', *software_website='\0', *software_label='\0', *software_id='\0';
  char *parfile='\0', *parfile_label='\0', *parfile_id='\0';
  char *trace_label='\0', *trace_id='\0';
  char *simulation_label='\0', *simulation_id='\0';
  char *prov_id='\0';

  generate_sf_provfile(filename, software_name, software_version, software_website, software_label, parfile, parfile_label, trace_label, simulation_label);

  prov_id = generate_provenance_ID();
  ASDF_generate_software_agent(software_name, software_version, software_website, software_label, software_id, &sf_prov);

  prov_id = generate_provenance_ID();
  ASDF_generate_par_file_provenance(parfile, parfile_label, parfile_id, &pf_prov);

  prov_id = generate_provenance_ID();
  ASDF_generate_trace_entity(trace_label, trace_id, &tr_prov);

  prov_id = generate_provenance_ID();
  ASDF_generate_simulation_activity(startTime, endTime, simulation_label, simulation_id, &ws_prov);

  ASDF_association(simulation_id, software_id, &association_prov);
  ASDF_usage(simulation_id, parfile_id, &usage_prov);
  ASDF_wasGeneratedby(trace_id, simulation_id, &generated_prov);

  *provenance = generate_sf_provenance(sf_prov, pf_prov, tr_prov, ws_prov, association_prov, usage_prov, generated_prov);
}
