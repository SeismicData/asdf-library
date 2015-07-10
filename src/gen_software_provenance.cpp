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
#include "gen_software_provenance.h"

char *generate_software_provenance(const char *software_name,
                                   const char *software_version,
                                   const char *software_website,
                                   const char *prov_label,
                                   const char *prov_id) {

  std::ostringstream software_prov;
  std::string prov;

  // 1) Generate Software provenace
  software_prov << "<prov:softwareAgent prov:id=\"seis_prov:sa_9DIG8A-TA\"><prov:label>" << prov_label
                << "</prov:label><seis_prov:software_name>" << software_name << "</seis_prov:software_name><seis_prov:software_version>"
                << software_version << "</seis_prov:software_version><seis_prov:website>" << software_website << "</seis_prov:website></prov:softwareAgent>";
  prov = software_prov.str();

  // 2) Copy the std::string to a C-string to interface
  //    with the C and Fortran APIs.
  char *software_provenance = new char[prov.length() +1];
  strncpy(software_provenance, prov.c_str(), prov.length());
  software_provenance[prov.length()] = '\0';

  return software_provenance;
}

void clean_software_provenance(char *sf_provenance) {
  if (sf_provenance)
    delete[] sf_provenance;
}
