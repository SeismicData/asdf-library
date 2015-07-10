
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
