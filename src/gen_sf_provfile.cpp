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
#include <cstring>

#include "parse_provfile.h"
#include "ASDF_generate_provenance.h"
#include "gen_sf_provfile.h"

void generate_sf_provfile(const char *filename, char *software_name, char *software_version, char *software_website,
                           char *software_label, char *parfile, char *parfile_label, char *trace_label, char *simulation_label)
{
  //parse_provfile(filename, software_name, software_version, software_website, software_label,
  //                  parfile, parfile_label, trace_label, simulation_label);

  //3) Copy the std::string to a C-string to interface
  //    with the C and Fortran APIs.
  //char *sub_provenance = new char[xml_str.length() +1];
  //strncpy(sub_provenance, xml_str.c_str(), xml_str.length());
  //sub_provenance[xml_str.length()] = '\0';

  //return sub_provenance;
}
