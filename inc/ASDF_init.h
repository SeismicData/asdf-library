#ifndef _ASDF_INIT_H_
#define _ASDF_INIT_H_

/** 
 * \brief Initialize ASDF by initializing HDF5.
 *
 * \return 0 for success, -1 for failure.
 */
int ASDF_initialize_hdf5();

/** 
 * \brief Finalize ASDF by finalizing HDF5.
 *
 * \return 0 for success, -1 for failure.
 */
int ASDF_finalize_hdf5();

#endif
