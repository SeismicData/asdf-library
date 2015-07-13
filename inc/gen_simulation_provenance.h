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
 * @file gen_sf_provenance.h
 * @brief
 * @author Matthieu Lefebvre
 */

#ifndef _ASDF_GEN_SIMULATION_PROVENANCE_
#define _ASDF_GEN_SIMULATION_PROVENANCE_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generate provenance for the waveform simulation
 *
 * @param person The name of the person, for instance "Hans Mustermann"
 * @param email The email of the person, for instance "hans.mustermann@email.com"
 * @param software_version The version of the waveform solver, for instance "7.0"
 * @param software_website The website that hosts the solver, for instance "https://geodynamics.org/cig/software/specfem3d_globe/"
 * @param model_name The name of the initial model used in the solver, for instance "PREM"
 * @param model_website The website that hosts the inital model, for instance "http://ds.iris.edu/ds/products/emc-prem/"
 *
 * @return A pointer to the generated C string.
 *
 * @note Bind C++ functions with the C API.
 *
 * @warning DO NOT FORGET TO CALL clean_sf_provenance
 */
char *generate_simulation_provenance(const char *startTime,
                                     const char *endTime,
                                     const char *prov_label,
                                     const char *prov_id);

/**
 * @brief Deallocate the provenance string build by
 *        generate_sf_provenance.
 *
 * @param sf_provenance Pointer to the C string to deallocate.
 *
 * @note Bind C++ functions with the C API.
 */
void clean_sf_provenance(char *sf_provenance);

#ifdef __cplusplus
}
#endif

#endif  // _ASDF_GEN_SF_PROVENANCE_
