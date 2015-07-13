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
 * @file gen_sf_provenance.cpp.h
 * @brief
 * @author Matthieu Lefebvre
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>

#include "prov_parameter.h"
#include "parse_sf_parfile.h"

#include "gen_sf_parfile_provenance.h"
#include "gen_simulation_provenance.h"

char *generate_simulation_provenance(const char *startTime,
                                     const char *endTime,
                                     const char *prov_label,
                                     const char *prov_id) {

  std::ostringstream simulation_prov;
  std::string prov;

  simulation_prov << "<prov:activity prov:id=" << prov_id << "><prov:startTime>" << startTime
                  << "</prov:startTime><prov:endTime>" << endTime << "</prov:endtime>" << "<prov:label>" << prov_label
                  << "</prov:label><prov:type xsi:type=\"xsd:string\">seis_prov:waveform_simulation</prov:type></prov:activity>";

  prov = simulation_prov.str();

  // 3) Copy the std::string to a C-string to interface
  //    with the C and Fortran APIs.
  char *simulation_provenance = new char[prov.length() +1];
  strncpy(simulation_provenance, prov.c_str(), prov.length());
  simulation_provenance[prov.length()] = '\0';

  return simulation_provenance;
}

void clean_simulation_provenance(char *simulation_provenance) {
  if (simulation_provenance)
    delete[] simulation_provenance;
}
