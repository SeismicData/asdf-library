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
 * @file gen_wasGeneratedby_provenance.cpp
 * @brief
 * @author Matthieu Lefebvre
 */

#include <iostream>
#include <sstream>
#include <cstring>

#include "gen_wasGeneratedby_provenance.h"

char *generate_wasGeneratedby_provenance(const char *prov_id1,
                                         const char *prov_id2) {

  std::ostringstream wasGeneratedby_prov;
  std::string prov;

  // 1) Generate Association provenace
  wasGeneratedby_prov << "<prov:wasGeneratedBy><prov:entity prov:ref=" << prov_id1 << "/><prov:activity prov:ref=" << prov_id2 << "/></prov:wasGeneratedBy>\0";
  prov = wasGeneratedby_prov.str();

  // 3) Copy the std::string to a C-string to interface
  //    with the C and Fortran APIs.
  char *wasGeneratedBy_provenance = new char[prov.length() +1];
  strncpy(wasGeneratedBy_provenance, prov.c_str(), prov.length());
  wasGeneratedBy_provenance[prov.length()] = '\0';

  return wasGeneratedBy_provenance;
}

void clean_wasGeneratedBy_provenance(char *wasGeneratedBy_provenance) {
  if (wasGeneratedBy_provenance)
    delete[] wasGeneratedBy_provenance;
}
