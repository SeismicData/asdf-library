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
 * @file gen_provenance_id.cpp
 * @brief
 * @author James Smith
 */

#include <iostream>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/lexical_cast.hpp>
#include "gen_provenance_id.h"


using namespace boost::uuids;

char *generate_provenance_ID() {

  random_generator gen;

  uuid random_id = gen();

  std::string full_prov_id = boost::lexical_cast<std::string>(random_id);
  std::string prov_id = full_prov_id.substr(26);

  // Copy the std:: string to a C-string to interface with the C and Fortran APIs.
  char *provenance_id = new char[prov_id.length()+1];
  strncpy(provenance_id, prov_id.c_str(), prov_id.length());
  provenance_id[prov_id.length()] = '\0';

  return provenance_id;
}
