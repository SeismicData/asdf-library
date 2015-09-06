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

#ifndef _GENERATE_SF_PROVENANCE_
#define _GENERATE_SF_PROVENANCE_

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @brief Generate provenance for the waveform simulation
 * 
 * @param startTime start time  of the simulation
 * @param endTime end time of the simulation
 *
 * @return Provenance string 
 *
 */
char *generate_sf_provenance(const char *startTime, const char *endTime);

void clean_sf_provenance(char *provenance);

#ifdef __cplusplus
}
#endif

#endif  // _ASDF_GEN_SF_PROVENANCE_
