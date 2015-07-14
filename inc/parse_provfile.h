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
 * @file parse_provfile.h
 * @brief
 * @author Matthieu Lefebvre
 */

#ifndef _ASDF_PARSE_PROVFILE_H_
#define _ASDF_PARSE_PROVFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Parse a SpecFem-like (sf) parameter file.
 *
 * @param filename Name of the parameter file.
 *
 * @return A vector of parameters (name, type, value).
 */
void parse_provfile(const char *filename, char *software_name, char *software_version, char *software_website,
                    char *software_label, char *parfile, char *parfile_label,
                    char *trace_label, char *simulation_label);

#ifdef __cplusplus
}
#endif

#endif  // _ASDF_PARSE_PROVFILE_H_
