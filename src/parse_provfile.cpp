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
 * @file parse_provfile.cpp
 * @author James Smith
 */

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include "parse_provfile.h"

void parse_provfile(const char *filename, char *software_name, char *software_version, char *software_website, char *software_label,
                    char *parfile, char *parfile_label, char *trace_label, char *simulation_label)
{
    using namespace boost::property_tree;

    ptree pt;
    read_json(filename, pt);

    // convert char to C++?
    std::string name = pt.get<std::string>("Software.software_name");
    std::string version = pt.get<std::string>("Software.software_version");
    std::string website = pt.get<std::string>("Software.software_website");
    std::string label = pt.get<std::string>("Software.software_label");
}
