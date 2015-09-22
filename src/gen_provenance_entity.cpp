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
 * @file gen_provenance_entity.h
 * @brief
 * @author Matthieu Lefebvre
 */

#include <iostream>
#include <sstream>
#include <vector>

#include "private/prov_parameter.h"
#include "private/gen_provenance_entity.h"

std::string generate_provenance_entity(const std::string label,
                                       const std::string id,
                                       const std::vector<parameter> params) {

  std::ostringstream prov;

  // Write entity start tag and general information
  prov << "<prov:entity prov:id=\"seis_prov:" << id << "\">"
       << "<prov:label>" << label << "</prov:label>"
       << "<prov:type></prov:type>";

  for (auto p : params) {
    prov << "<seis_prov:" << p.name << " xsi:type=\"" << p.type << "\">"
         << p.value
         << "</seis_prov:" << p.name << ">";
  }

  // Write entity end tag
  prov << "</prov:entity>\0";
  prov.flush();

  return prov.str();
}
