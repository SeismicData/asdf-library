#ifndef _ASDF_READ_H_
#define _ASDF_READ_H_

#include <mpi.h>
#include <hdf5.h>

/**
 * \brief Open an existing ASDF file in read-only mode.
 *
 * \param filename Name of the ASDF file to open
 * \param comm The communicator in which the file will be opened.
 *
 * \return The id of the opened file.
 */
hid_t ASDF_open_read_only(char *filename, MPI_Comm comm);

/**
 * \brief Read the value of "attr_value" of a string attribute "attr_name"
 *        within the HDF5 group "grp_name".
 *
 * \param file_id Id of the file to read the attribute from.
 * \param grp_name Path / name of the HDF5 group containing the attribute.
 * \param attr_name Name of the attribute
 * \param attr_value Value read
 *
 * \return 0 for success
 *
 * \note attr_value will be allocated within this routine. Users are
 *       responsible for deallocating it.
 */
int ASDF_read_str_attr(hid_t file_id, char *grp_name,
                       char *attr_name, char **attr_value);

/**
 * \brief Get the number of elements of a dataset (i.e. array)
 *
 * \param dataset_id Id of the dataset to get the the number of element.
 *
 * \return Number of the elements in the dataset
 */
int ASDF_get_num_elements_dataset(hid_t dataset_id);

/**
 * \brief Get the number of elements of a dataset from its path.
 *
 * \param file_id Id of the file containing the dataset.
 * \param path Path of the dataset in the hdf5 file.
 *
 * \return 0 for success
 */
int ASDF_get_num_elements_from_path(hid_t file_id, char *path);

/**
 * \brief Read a complete waveform from a dataset
 * 
 * \param file_id Id of the file containing the waveform.
 * \param path The path to the waveform
 * \param waveform The array to read the waveform in.
 *
 * \return 0 for success
 *
 * \note waveform should be preallocated and large enough to store
 *       the data in full.
 */
int ASDF_read_full_waveform(hid_t file_id, char *path, float *waveform);

/**
 * \brief Read a partial waveform from a dataset
 * 
 * \param file_id Id of the file containing the waveform.
 * \param path The path to the waveform
 * \param offset Where to start reading in the dataset
 * \param nsamples How many elements to read 
 * \param waveform The array to read the waveform in.
 *
 * \return 0 for success
 *
 * \note waveform should be preallocated and large enough to store
 *       the nsamples of data.
 */
int ASDF_read_partial_waveform(hid_t file_id, char *path, int offset,
                               int nsamples, float *waveform);

int ASDF_exists_in_path(hid_t file_id, const char *path, const char *name);
int ASDF_station_exists(hid_t file_id, const char *name);


#endif
