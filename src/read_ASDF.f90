
program read_asdf

  use hdf5 ! This module contains all necessary modules

  IMPLICIT NONE

  CHARACTER(LEN=12), PARAMETER :: filename = "synthetic.h5" ! File name
  CHARACTER(LEN=4), PARAMETER :: dsetname = "dset"     ! Dataset name

  INTEGER(HID_T) :: file_id       ! File identifier
  INTEGER(HID_T) :: dset_id       ! Dataset identifier

  INTEGER     ::   error ! Error flag
  INTEGER     ::  i, j

  INTEGER, DIMENSION(4,6) :: dset_data, data_out ! Data buffers
  INTEGER(HSIZE_T), DIMENSION(2) :: data_dims

  !
  ! Initialize FORTRAN interface.
  !
  CALL h5open_f(error)

  !
  ! Open an existing file.
  !
  CALL h5fopen_f (filename, H5F_ACC_RDWR_F, file_id, error)

  !
  ! Open an existing dataset.
  !
  !CALL h5dopen_f(file_id, dsetname, dset_id, error)

  !
  ! Read the dataset.
  !
  !CALL h5dread_f(dset_id, H5T_NATIVE_INTEGER, data_out, data_dims, error)

  !
  ! Close the dataset.
  !
  !CALL h5dclose_f(dset_id, error)

  !
  ! Close the file.
  !
  CALL h5fclose_f(file_id, error)

  !
  ! Close FORTRAN interface.
  !
  CALL h5close_f(error)
end program read_asdf
