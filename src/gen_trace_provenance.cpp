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
#include "gen_trace_provenance.h"

char *generate_trace_provenance(const char *prov_label,
                                const char *prov_id) {

  std::ostringstream trace_prov;
  std::string prov;

  // 1) Generate Trace provenance
  trace_prov << "<prov:entity prov:id=" << prov_id << "><prov:label>" << prov_label
             << "</prov:label><prov:type xsi:type=xsd:string>seis_prov:waveform_trace</prov:entity>";
  prov = trace_prov.str();

  // 3) Copy the std::string to a C-string to interface
  //    with the C and Fortran APIs.
  char *trace_provenance = new char[prov.length() +1];
  strncpy(trace_provenance, prov.c_str(), prov.length());
  trace_provenance[prov.length()] = '\0';

  return trace_provenance;
}

void clean_trace_provenance(char *sf_provenance) {
  if (sf_provenance)
    delete[] sf_provenance;
}
