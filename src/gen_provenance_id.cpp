#include <iostream>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/lexical_cast.hpp>

#include "gen_provenance_id.h"

using namespace boost::uuids;

std::string gen_provenance_ID() {

  random_generator gen;
  
  uuid random_id = gen();

  std::string full_prov_id = boost::lexical_cast<std::string>(random_id);
  std::string prov_id = full_prov_id.substr(26);

  return prov_id;
}
