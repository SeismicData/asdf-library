#ifndef _ASDF_WRITE_H_
#define _ASDF_WRITE_H_

#include <mpi.h>

/**
 * \brief Create a new parallel HDF-5 file.
 *
 * \param filename Name of the ASDF file to create.
 * \param comm The communicator in which the file will be accessed.
 *
 * \return The id of the created file.
 */
hid_t ASDF_create_new_file(char *filename, MPI_Comm comm);

/**
 * \brief Write an attribute attr_name with value attr_value in dataset_id 
 *
 * \param dataset_id Id of the dataset in which the attribute will be written..
 * \param attr_name Name of the attribute
 * \param attr_value
 *
 * \return 0 for success.
 */
herr_t ASDF_write_string_attribute(hid_t dataset_id, 
                                   char *attr_name, char *attr_value);

/**
 * \brief Write an ASDF specfic "Auxiliary" group
 *
 * \param loc_id Where this group should be written in the hDF5 file
 *
 * \return 0 for success.
 */
herr_t ASDF_write_auxiliary_data(hid_t loc_id);

/**
 * \brief Write an ASDF specfic "Provenance" group
 *
 * \param loc_id Where this group should be written in the hDF5 file
 *
 * \return 0 for success.
 */
herr_t ASDF_write_provenance_data(hid_t loc_id);

/**
 * \brief Write a QuakeML string as a dataset
 *
 * \param loc_id Where this dataset should be written in the hDF5 file
 * \param quakeml_string A XML string following the QuakeML convention.
 *
 * \return 0 for success.
 */
herr_t ASDF_write_quakeml(hid_t loc_id, char *quakeml_string);

/**
 * \brief Define waveforms groups and datasets.
 *
 * There should be as many groups as waveforms. They will be later used
 * to actually write waveform data. Every array should be preallocated.
 *
 * \param loc_id Where this dataset should be written in the hDF5 file
 * \param num_waveforms Number of groups to define
 * \param nsamples number of samples per waveform. 
 *                 FIXME: James, nsamples should be an array?
 * \param start_time 
 * \param sampling rate
 * \param event_name Name of the earthquake, shot, ...
 * \param waveform_names Names of stations, channel, ... FIXME: explain better
 * \param groups Groups where waveform will be written. FIXME seems useless
 * \param data_id Dataset in which waveform will be written
 *
 * \return 0 for success.
 *
 * \note this function should be called collectively as every hdf5 call
 *       defining data should be done collectively. See:
 *       http://www.hdfgroup.org/HDF5/doc/RM/CollectiveCalls.html
 */
herr_t ASDF_define_waveforms(hid_t loc_id, int num_waveforms, int nsamples, 
                             int start_time, double sampling_rate, 
                             char *event_name, char **waveform_names,
                             int *groups, int *data_id);

/**
 * \brief Write a full seismogram 'waveform' in a dataset data_id
 *
 * \param data_id The id of the containing dataset
 * \param waveform The time serie to be written
 *
 * \return 0 for success.
 */
herr_t ASDF_write_full_waveform(hid_t data_id, float *waveform);

/**
 * \brief Write a slice of seismogram 'waveform' in a dataset data_id
 *
 * \param data_id The id of the containing dataset
 * \param waveform The time serie to be written
 * \param offset Where to start writing in the dataset
 * \param nsamples How many elements to write
 *
 * \return 0 for success.
 */
herr_t ASDF_write_partial_waveform(hid_t data_id, float *waveform,
                                   int offset, int nsamples);

#endif
