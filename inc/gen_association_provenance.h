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

#ifndef _ASDF_GEN_ASSOCATION_PROVENANCE_
#define _ASDF_GEN_ASSOCATION_PROVENANCE_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generate provenance for the waveform simulation
 *
 * @param prov_id1 The first provenance id.
 * @param prov_id2 The second provenance id.
 *
 * @return A pointer to the generated C string.
 *
 * @note Bind C++ functions with the C API.
 *
 * @warning DO NOT FORGET TO CALL clean_sf_provenance
 */
char *generate_association_provenance(const char *prov_id1,
                                      const char *prov_id2);

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
