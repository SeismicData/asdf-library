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
 * @file gen_sf_provenance.cpp
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
#include "gen_sf_provenance.h"
#include "ASDF_generate_provenance.h"

char *generate_sf_provenance(char *sf_prov,
                             char *pf_prov,
                             char *tr_prov,
                             char *ws_prov,
                             char *association_prov,
                             char *usage_prov,
                             char *generated_prov) {

  std::string prov, initial_prov;
  std::string sf(sf_prov), pf(pf_prov), tr(tr_prov), ws(ws_prov), as(association_prov), us(usage_prov), gr(generated_prov);

  // 1) Generate initial provenance
  initial_prov = "<?xml version='1.0' encoding='UTF-8'?><prov:document xmlns:prov=\"http://www.w3.org/ns/prov#\" xmlns:seis_prov=\"http://asdf.readthedocs.org/seis_prov/0.0/#\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">";

  // 3) Combine sub_provenance into one string
  prov = initial_prov+sf+pf+tr+ws+as+us+gr+"</prov:document>";

  // 4) Copy the std::string to a C-string to interface
  //    with the C and Fortran APIs.
  char *sf_provenance = new char[prov.length() +1];
  strncpy(sf_provenance, prov.c_str(), prov.length());
  sf_provenance[prov.length()] = '\0';

  return sf_provenance;
}

void clean_sf_provenance(char *sf_provenance) {
  if (sf_provenance)
    delete[] sf_provenance;
}
