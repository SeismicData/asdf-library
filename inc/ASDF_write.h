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
 * @file ASDF_write.h
 * @brief
 * @author Matthieu Lefebvre
 */

#ifndef _ASDF_WRITE_H_
#define _ASDF_WRITE_H_

#include <mpi.h>
#include <hdf5.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a new parallel HDF5 file.
 *
 * @param filename Name of the ASDF file to create.
 * @param comm The communicator in which the file will be accessed.
 *
 * @return The id of the created file.
 */
hid_t ASDF_create_new_file(const char *filename, MPI_Comm comm);

/**
 * @brief Create a new parallel HDF5 file.
 *
 * @param filename Name of the ASDF file to create.
 *
 * @return The id of the created file.
 */
hid_t ASDF_create_new_file_serial(const char *filename);

/**
 * @brief Write a string attribute \p attr_name with value \p attr_value in
 *        \p dataset_id
 *
 * @param dataset_id Id of the dataset in which the attribute will be written..
 * @param attr_name Name of the attribute
 * @param attr_value
 *
 * @return 0 for success.
 */
herr_t ASDF_write_string_attribute(hid_t dataset_id,
                                   const char *attr_name,
                                   const char *attr_value);

/**
 *  @brief Write an integer attribute \p attr_name with value \p attr_value in
 *         \p dataset_id
 *
 *  @param dataset_id Id of the dataset in which the attribute will be written..
 *  @param attr_name Name of the attribute
 *  @param attr_value
 *
 *  @return 0 for success.
 */
herr_t ASDF_write_integer_attribute(hid_t dataset_id,
				    const char *attr_name,
				    long long int attr_value);

/**
 *  @brief Write a double attribute \p attr_name with value \p attr_value in
 *         \p dataset_id
 *
 *  @param dataset_id Id of the dataset in which the attribute will be written..
 *  @param attr_name Name of the attribute
 *  @param attr_value
 *
 *  @return 0 for success.
 */
herr_t ASDF_write_double_attribute(hid_t dataset_id,
				    const char *attr_name,
				    double attr_value);

/**
 * @brief Write an ASDF specfic "Auxiliary" group
 *
 * @param loc_id Where this group should be written in the HDF5 file
 *
 * @return 0 for success.
 */
herr_t ASDF_write_auxiliary_data(hid_t loc_id,
                                 const char *sf_constants_file,
                                 const char *sf_Parfile);

/**
 * @brief Write an ASDF specfic "Provenance" group
 *
 * @param loc_id Where this group should be written in the HDF5 file
 *
 * @return 0 for success.
 */
herr_t ASDF_write_provenance_data(hid_t loc_id, const char *provenance_string);

/**
 * @brief Write a QuakeML string as a dataset
 *
 * @param loc_id Where this dataset should be written in the HDF5 file
 * @param quakeml_string A XML string following the QuakeML convention.
 *
 * @return 0 for success.
 */
herr_t ASDF_write_quakeml(hid_t loc_id, const char *quakeml_string);

/**
 * @brief Create a group called "Waveforms" that will contain the seismograms:
 *
 * @param loc_id Where this group will be created, likely at the root of the file
 *
 * @return The id of the created group, or a negative number in case of failure
 */
hid_t ASDF_create_waveforms_group(hid_t loc_id);

/**
 * @brief Create a group \p station_name under \p loc_id,
 *        with the associated \p station_xml description
 *
 * @param loc_id Where to create the group
 * @param station_name Name of the station, eg AF.CVNA
 *
 * @return group id if successful, negative number otherwise.
 */
hid_t ASDF_create_stations_group(hid_t loc_id, const char *station_name);

/**
 * @param group_id The group id for station name
 * @param StationXML_length The length of the StationXML
 *
 * @return data id if successful, negative number otherwise.
 */
hid_t ASDF_define_station_xml(hid_t group_id, int StationXML_length);

/**
 * @brief Define a dataset to write a waveform in.
 *
 * @param loc_id Where to define the dataset
 * @param nsamples The number of sample in the waveform
 * @param start_time The start time of the seismogram
 * @param sampling_rate The sampling rate of the waveform
 * @param event_name The name of the event, useful in case of several quakeML?
 * @param waveform_name The name of the waveform, e.g. NET.STA.MXE.20100102.123045
 *
 * @return The dataset id if successful, negative number otherwise.
 *
 * @note this function should be called collectively as every HDF5 call
 *       defining data should be done collectively. See:
 *       http://www.hdfgroup.org/HDF5/doc/RM/CollectiveCalls.html
 */
hid_t ASDF_define_waveform(hid_t loc_id, int nsamples,
                           long long int start_time, double sampling_rate,
                           const char *event_name, const char *waveform_name);

/**
 * @brief Define waveforms groups and datasets.
 *
 * There should be as many groups as waveforms. They will be later used
 * to actually write waveform data. Every array should be preallocated.
 *
 * @param loc_id Where this dataset should be written in the HDF5 file
 * @param num_waveforms Number of groups to define
 * @param nsamples number of samples per waveform.
 *                 FIXME: James, nsamples should be an array?
 * @param start_time
 * @param sampling_rate
 * @param event_name Name of the earthquake, shot, ...
 * @param waveform_names Names of stations, channel, ... FIXME: explain better
 * @param data_id Dataset in which waveform will be written
 *
 * @return 0 for success.
 *
 * @note this function should be called collectively as every HDF5 call
 *       defining data should be done collectively. See:
 *       http://www.hdfgroup.org/HDF5/doc/RM/CollectiveCalls.html
 */
herr_t ASDF_define_waveforms(hid_t loc_id, int num_waveforms, int nsamples,
                             long long int start_time, double sampling_rate,
                             char *event_name, char **waveform_names,
                             hid_t *data_id);

/**
 * @brief Write the station xml in data_id
 * 
 * @param data_id The id of the containing dataset
 * @param station_xml XML string following the stationXML format
 */
herr_t ASDF_write_station_xml(hid_t data_id, const char* StationXML);


/**
 * @brief Write a full seismogram \p waveform in a dataset \p data_id
 *
 * @param data_id The id of the containing dataset
 * @param waveform The time series to be written
 *
 * @return 0 for success.
 */
herr_t ASDF_write_full_waveform(hid_t data_id, const float *waveform);

/**
 * @brief Write a slice of seismogram \p waveform in a dataset \p data_id
 *
 * @param data_id The id of the containing dataset
 * @param waveform The time series to be written
 * @param offset Where to start writing in the dataset
 * @param nsamples How many elements to write
 *
 * @return 0 for success.
 */
herr_t ASDF_write_partial_waveform(hid_t data_id, const float *waveform,
                                   int offset, int nsamples);

/**
 * @brief Close HDF5 groups opened by ASDF calls.
 *
 * @param group_id The group to close
 *
 * @return 0 for success
 *
 * @see The ASDF calls opening groups are:
 *       - ASDF_create_waveforms_group
 *       - ASDF_create_stations_group
 */
herr_t ASDF_close_group(hid_t group_id);

/**
 * @brief Close HDF5 datasets opened by ASDF calls.
 *
 * @param dataset_id The dataset to close
 *
 * @return 0 for success
 *
 * @see The ASDF calls opening datasets are:
 *       - ASDF_define_waveform
 */
herr_t ASDF_close_dataset(hid_t dataset_id);

#ifdef __cplusplus
}
#endif

#endif
