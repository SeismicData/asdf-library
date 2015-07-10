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
#include <cstring>

#include "gen_association_provenance.h"

char *generate_association_provenance(const char *prov_id1,
                                      const char *prov_id2) {

  std::ostringstream association_prov;
  std::string prov;

  // 1) Generate Association provenace
  association_prov << "<prov:wasAssociatedWith><prov:activity prov:ref=" << prov_id1 << "/><prov:agent prov:ref=" << prov_id2 << "/></prov:wasAssociatedWith>\0";
  prov = association_prov.str();

  // 3) Copy the std::string to a C-string to interface
  //    with the C and Fortran APIs.
  char *association_provenance = new char[prov.length() +1];
  strncpy(association_provenance, prov.c_str(), prov.length());
  association_provenance[prov.length()] = '\0';

  return association_provenance;
}

void clean_association_provenance(char *association_provenance) {
  if (association_provenance)
    delete[] association_provenance;
}
