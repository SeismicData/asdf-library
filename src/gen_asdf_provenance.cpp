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
 * @file gen_asdf_provenance.h
 * @brief 
 * @author Matthieu Lefebvre
 */

#include <iostream>
#include <sstream>
#include <cstring>

#include "gen_provenance_id.h"
#include "gen_provenance_software.h"
#include "gen_provenance_trace.h"
#include "gen_provenance_simulation.h"
#include "gen_provenance_association.h"
#include "gen_provenance_usage.h"
#include "gen_provenance_generatedby.h"
#include "parse_provfile.h"
#include "gen_sf_provenance.h"

#include "gen_asdf_provenance.h"

/** 
 * @brief Generate provenance for the waveform simulation
 * 
 * @param filename Provenance file name
 * @param startTime start time  of the simulation
 * @param endTime end time of the simulation
 * 
 * @return pointer to the provenance generated string
 *
 */

char *gen_asdf_provenance(const char *filename, const char *startTime, const char *endTime)
{

  int err;
  std::string software_name, software_version, software_website, software_label, Par_file, parfile_label,
              trace_label, simulation_label;

  //err = parse_provfile(filename, software_name, software_version, software_website, software_label,
  //              Par_file, parfile_label, trace_label, simulation_label);

  std::string software_id = gen_provenance_ID();
  std::string software_prov = gen_provenance_software(std::string("SPECFEM3D"), std::string("6.0"), 
                                                      std::string("http://www.cig.org"), std::string("Solver"), software_id);
  std::string trace_id = gen_provenance_ID();
  std::string trace_prov = gen_provenance_trace(std::string("Trace"), trace_id);

  std::string ws_id = gen_provenance_ID();
  std::string ws_prov = gen_provenance_simulation(startTime, endTime, std::string("Simulation"), ws_id);

  std::string association_prov = gen_provenance_association(software_id, trace_id);
  std::string usage_prov = gen_provenance_usage(software_id, trace_id);
  std::string generated_prov = gen_provenance_generatedby(software_id, trace_id);

  std::string prov = generate_sf_provenance(software_prov, trace_prov, ws_prov, association_prov, usage_prov, generated_prov);

  char *provenance = new char[prov.length()+1];
  strncpy(provenance, prov.c_str(), prov.length());
  provenance[prov.length()]='\0';
   
  return provenance;
}

void clean_asdf_provenance(char *sub_provenance) {
  if (sub_provenance)
    delete[] sub_provenance;
}
