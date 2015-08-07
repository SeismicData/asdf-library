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

#include "gen_provenance_id.h"
#include "gen_sf_parfile_provenance.h"
#include "gen_sf_provenance.h"
#include "ASDF_provenance.h"

char *generate_sf_provenance(const char *startTime,
                             const char *endTime) {

  std::ostringstream software_prov, trace_prov, simulation_prov, constants_prov, association_prov, usage_prov, generatedBy_prov, parfile_prov;
  std::string initial_string, association_string, constants_string, usage_string, generatedBy_string, simulation_string, parfile_string;
  std::string sf_prov, software_string ,trace_string;

  initial_string = "<?xml version='1.0' encoding='UTF-8'?><prov:document xmlns:prov=\"http://www.w3.org/ns/prov#\" xmlns:seis_prov=\"http://seisprov.org/seis_prov/0.1/#\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">";

  std::string software_id = gen_provenance_ID();
  software_prov << "<prov:softwareAgent prov:id=\"seis_prov:sp000_sa_" << software_id << "\"><prov:label>SPECFEM3D_GLOBE"
                << "</prov:label><seis_prov:software_name>SPECFEM3D_GLOBE</seis_prov:software_name><seis_prov:software_version>"
                << "7.0.0</seis_prov:software_version><seis_prov:website>http://geodynamics.org/cig/software/specfem3d</seis_prov:website></prov:softwareAgent>";
  software_prov.flush();
  software_string = software_prov.str();

  std::string constants_id = gen_provenance_ID();
  constants_prov << "<prov:entity prov:id=\"seis_prov:sp000_fi_" << constants_id << "\"><prov:label>File</prov:label><prov:type xsi:type=\"xsd:string\">seis_prov:file</prov:type>"
    << "<seis_prov:filename>constants.h</seis_prov:filename>"
    << "<seis_prov:location>/AuxiliaryData/Files/constants_h</seis_prov:location>"
    << "<seis_prov:location_type>HDF5 Data Set</seis_prov:location_type></prov:entity>";
  constants_prov.flush();
  constants_string = constants_prov.str();

  std::string parfile_id = gen_provenance_ID();
  parfile_prov << "<prov:entity prov:id=\"seis_prov:sp000_fi_" << parfile_id << "\"><prov:label>File</prov:label><prov:type xsi:type=\"xsd:string\">seis_prov:file</prov:type>"
    << "<seis_prov:filename>Parfile</seis_prov:filename>"
    << "<seis_prov:location>/AuxiliaryData/Files/Parfile</seis_prov:location>"
    << "<seis_prov:location_type>HDF5 Data Set</seis_prov:location_type></prov:entity>";
  parfile_prov.flush();
  parfile_string = parfile_prov.str();

  std::string trace_id = gen_provenance_ID();
  trace_prov << "<prov:entity prov:id=\"seis_prov:sp000_wf_" << trace_id << "\"><prov:label>Waveform Trace"
             << "</prov:label><prov:type xsi:type=\"xsd:string\">seis_prov:waveform_trace</prov:type></prov:entity>";
  trace_prov.flush();
  trace_string = trace_prov.str();

  std::string simulation_id = gen_provenance_ID();
  simulation_prov << "<prov:activity prov:id=\"seis_prov:sp000_ws_" << simulation_id << "\"><prov:startTime>" << startTime
                  << "</prov:startTime><prov:endTime>" << endTime << "</prov:endTime>" << "<prov:label>Waveform Simulation" 
                  << "</prov:label><prov:type xsi:type=\"xsd:string\">seis_prov:waveform_simulation</prov:type></prov:activity>";
  simulation_prov.flush();  
  simulation_string = simulation_prov.str();

  association_prov << "<prov:wasAssociatedWith><prov:activity prov:ref=\"seis_prov:sp000_ws_" << simulation_id << "\"/><prov:agent prov:ref=\"seis_prov:sp000_sa_" << software_id << "\"/></prov:wasAssociatedWith>";
  association_prov.flush();
  association_string = association_prov.str();

  usage_prov << "<prov:used><prov:activity prov:ref=\"seis_prov:sp000_ws_" << simulation_id << "\"/><prov:entity prov:ref=\"seis_prov:sp000_wf_" << trace_id << "\"/></prov:used>";
  usage_prov.flush();
  usage_string = usage_prov.str();

  generatedBy_prov << "<prov:wasGeneratedBy><prov:entity prov:ref=\"seis_prov:sp000_wf_" << trace_id << "\"/><prov:activity prov:ref=\"seis_prov:sp000_ws_" << simulation_id << "\"/></prov:wasGeneratedBy>";
  generatedBy_prov.flush();
  generatedBy_string = generatedBy_prov.str();

  sf_prov = initial_string + software_string + constants_string + parfile_string + trace_string + simulation_string + association_string + usage_string + generatedBy_string + "</prov:document>";

  // Copy the std::string to a C-string to interface
  // with the C and Fortran APIs.
  char *sf_provenance = new char[sf_prov.length() +1];
  strncpy(sf_provenance, sf_prov.c_str(), sf_prov.length());
  sf_provenance[sf_prov.length()] = '\0';

  return sf_provenance;
}

void clean_sf_provenance(char *sf_provenance) {
  if (sf_provenance)
    delete[] sf_provenance;
}
