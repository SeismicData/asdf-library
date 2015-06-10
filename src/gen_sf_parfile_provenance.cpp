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
 * @file gen_sf_parfile_provenance.cpp.h
 * @brief 
 * @author Matthieu Lefebvre
 */

#include <iostream>
#include <sstream>
#include <vector>

#include "prov_parameter.h"
#include "parse_sf_parfile.h"
#include "gen_provenance_entity.h"

#include "gen_sf_parfile_provenance.h"

char *generate_sf_parfile_provenance(const char *filename,
                                     const char *prov_label,
                                     const char *prov_id) {
  std::vector<parameter> params = parse_sf_parfile(std::string(filename));

  std::string xml_str = generate_provenance_entity(std::string(prov_label),
                                                   std::string(prov_id),
                                                   params);
  char *sub_provenance = new char[xml_str.length() +1];
  std::strncpy(sub_provenance, xml_str.c_str(), xml_str.length());

  return sub_provenance;
}

void clean_sf_parfile_provenance(char *sub_provenance) {
  if (sub_provenance)
    delete[] sub_provenance;
}
