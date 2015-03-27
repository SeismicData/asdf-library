subroutine write_ASDF(filename, event_name, start_time, sampling_rate, &
    quakeml_string)
  ! About functions that must be called collectively:
  !    http://www.hdfgroup.org/HDF5/doc/RM/CollectiveCalls.html

  use hdf5
  use iso_c_bindinG
  use mpi

  implicit none

  ! --- Parameters ---
  character(len=*) :: filename, event_name
  integer(hid_t) :: start_time
  real(kind=8) :: sampling_rate
  character(len=539) :: quakeml_string


  ! --- Variables ---
  integer(size_t), parameter :: sdim = 4
  character(len=236) :: stationxml_string = "StationXML string !!"
  character(len=5, kind=c_char), dimension(1), target :: file_format_string = "ASDF"//C_NULL_CHAR
  character(len=7, kind=c_char), dimension(1), target :: file_version ="0.0.1b"//C_NULL_CHAR
  
  integer(hid_t) :: file_id, filetype, plist_id
  integer(hid_t) :: space_scalar, space_array
  integer(hid_t) :: attr3, attr, attr2, dataset_Source, dataset_Receiver
  integer(hid_t) :: group, dcpl, memtype
  integer(hsize_t), dimension(1) :: dims, maxdims
  integer :: hdferr

  type(c_ptr) :: f_ptr
  type(c_ptr) :: f_ptr2

  ! MPI definitions and calls.
  INTEGER :: mpi_err       ! MPI error flag
  INTEGER :: mpi_size, mpi_rank

  character(len=256) :: char_buf
  integer :: i 
  integer(hid_t), allocatable :: groups(:), dataset_Sources(:)

  call MPI_Comm_size(MPI_COMM_WORLD, mpi_size, mpi_err)
  call MPI_Comm_rank(MPI_COMM_WORLD, mpi_rank, mpi_err)

  allocate(groups(mpi_size)) ! Actually the total number of waveforms
  allocate(dataset_Sources(mpi_size)) ! Actually the total number of waveforms

  maxdims = (/H5S_UNLIMITED_F/) ! Must be done after opening the interface

  ! Create a new file using default properties.
  !call create_new_asdf_file(filename, file_id)
  call h5pcreate_f(H5P_FILE_ACCESS_F, plist_id, hdferr)
  call h5pset_fapl_mpio_f(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL, hdferr)

  ! Create the file collectively.
  call h5fcreate_f(filename, H5F_ACC_TRUNC_F, file_id, hdferr, &
                   access_prp=plist_id)

  ! Create file and memory datatypes.
  call h5tcopy_f(h5t_string, filetype, hdferr)
  if (hdferr .eq. -1) print *, "Error creating file and memory datatypes."
 
  call write_string_attribute(file_id, "file_format", file_format_string(1))
  !call write_string_attribute(file_id, "file_version", file_version(1))

  ! Create group "AuxiliaryData" and "Provenance" in the root group
  call h5gcreate_f(file_id, "AuxiliaryData", group, hdferr)
  call h5gclose_f(group, hdferr)
  call h5gcreate_f(file_id, "Provenance", group, hdferr)
  call h5gclose_f(group, hdferr)

  print *, len_trim(quakeml_string)
  dims = (/539/) ! Length of QuakeML string
  ! Create a dataset with unlimited dimensions
  !call h5screate_simple_f(1, dims, space_array, hdferr, maxdims)
  call h5screate_simple_f(1, dims, space_array, hdferr)
  ! Create the dataset creation property list and set the chunk size
  !call h5pcreate_f(H5P_DATASET_CREATE_F, dcpl, hdferr)
  !call h5pset_chunk_f(dcpl, 1, dims, hdferr)
  ! Create the QuakeML dataset
  call h5dcreate_f(file_id, "QuakeML", H5T_STD_I8LE, space_array, dataset_Source, hdferr)
  ! Write QuakeML dataset
  if (mpi_rank == 0) then
    call h5dwrite_f(dataset_Source, H5T_STD_I8LE, quakeml_string, dims, hdferr) 
  endif
  call h5dclose_f(dataset_Source, hdferr)
  call h5sclose_f(space_array, hdferr)

  ! Create group "Waveforms" in the root group
  call h5gcreate_f(file_id, "Waveforms", group, hdferr)
  call h5gclose_f(group, hdferr)
  
  ! Create SEED Group
  do i = 1, mpi_size
    write(char_buf, "(A17,I1)"), "Waveforms/AF.CVNA", i 
    call h5gcreate_f(file_id, char_buf, groups(i), hdferr)

    dims = (/300/) ! Length of waveform
    !! Create sample Seismogram dataspace inside the SEED group
    call h5screate_simple_f(1, dims, space_array, hdferr, maxdims)
    !! Create the dataset creation property list and set the chunk size
    call h5pcreate_f(H5P_DATASET_CREATE_F, dcpl, hdferr)
    call h5pset_chunk_f(dcpl, 1, dims, hdferr)
    !! Create the waveeform dataset
    write(char_buf, "(A17,I1)"), "Synthetics_number", i 
    !call h5dcreate_f(group, "AF.CVNA.S3.MXE__2010-03-11T06:22:17__2010-03-11T08:02:45__synthetic",&
    !       H5T_IEEE_F32LE, space_array, dataset_Source, hdferr, dcpl)
    call h5dcreate_f(groups(i), char_buf, & 
           H5T_IEEE_F32LE, space_array, dataset_Sources(i), hdferr, dcpl)

    ! Now create the sampling rate and start time attributes
    call h5screate_f(H5S_SCALAR_F, space_scalar, hdferr)
    !call h5acreate_f(dataset_Sources(i), "event_id", filetype, space_scalar, attr, hdferr)
    !call write_string_attribute(dataset_Sources(i), "event_id", event_name)
    call write_double_attribute(dataset_Sources(i), "sampling_rate", sampling_rate)
    !call h5acreate_f(dataset_Source, "starttime", H5T_STD_I64LE, space_scalar, attr2, hdferr)
    !f_ptr2 = C_LOC(start_time)
    !call h5awrite_f(attr2, H5T_STD_I64LE, f_ptr2, hdferr)
    call h5sclose_f(space_scalar, hdferr)
    call h5pclose_f(dcpl, hdferr)
    call h5sclose_f(space_array, hdferr)
  enddo

  do i = 1, mpi_size
    call h5gclose_f(groups(i), hdferr)
    call h5dclose_f(dataset_Sources(i), hdferr)
  enddo
  call h5pclose_f(plist_id, hdferr)
  call h5fclose_f(file_id, hdferr)

end subroutine write_ASDF


subroutine write_string_attribute(dataset_id, attr_name, attr_value)
  use hdf5
  implicit none
  ! --- Arguments ---
  integer(hid_t), intent(in) :: dataset_id 
  character(len=*), intent(in) :: attr_name
  character(len=*, kind=c_char), dimension(1), target, intent(in) :: attr_value 
  ! --- Variables ---
  integer(hid_t) :: dataspace, attr, filetype, memtype
  type(c_ptr), target :: wdata
  type(c_ptr) :: f_ptr
  integer :: hdferr
  integer(hsize_t), dimension(1) :: dims
  integer(hsize_t) :: type_size

  !dims(1) = 
  type_size = len(attr_value)
  CALL H5Tcopy_f(H5T_FORTRAN_S1, filetype, hdferr)
  call h5tset_size_f(filetype, type_size, hdferr)
  ! Create attribute and write string to it.
  wdata = c_loc(attr_value)
  print *, len(attr_value)
  call h5screate_f(h5s_scalar_f, dataspace, hdferr)
  !call h5screate_simple_f(1, dims, dataspace, hdferr)
  call h5acreate_f(dataset_id, attr_name, filetype, dataspace, attr, hdferr)
  f_ptr = C_LOC(wdata)

  call h5awrite_f(attr, filetype, f_ptr, hdferr)

  call h5aclose_f(attr, hdferr)
  call h5sclose_f(dataspace, hdferr)
  CALL H5Tclose_f(filetype, hdferr)
end subroutine write_string_attribute


subroutine write_double_attribute(dataset_id, attr_name, attr_value)
  use hdf5
  implicit none
  ! --- Arguments ---
  integer(hid_t), intent(in) :: dataset_id 
  character(len=*), intent(in) :: attr_name
  real(kind=8), intent(in) :: attr_value 
  ! --- Variables ---
  integer(hid_t) :: dataspace, attr
  type(c_ptr), target :: wdata
  type(c_ptr) :: f_ptr
  integer :: hdferr

  ! Create attribute and write string to it.
  call h5screate_f(h5s_scalar_f, dataspace, hdferr)
  call h5acreate_f(dataset_id, attr_name, H5T_IEEE_F64LE, dataspace, attr, hdferr)
  f_ptr = C_LOC(attr_value)

  call h5awrite_f(attr, H5T_IEEE_F64LE, f_ptr, hdferr)

  call h5aclose_f(attr, hdferr)
  call h5sclose_f(dataspace, hdferr)
end subroutine write_double_attribute


subroutine create_new_asdf_file(filename, file_id)
  use hdf5
  use mpi
  implicit none
  ! --- Arguments ---
  character(len=*), intent(in) :: filename 
  integer(hid_t), intent(out) :: file_id
  ! --- Variables ---
  integer(hid_t) :: plist_id
  integer :: hdferr

  ! Create a new file using default properties.
  call h5fcreate_f(filename, H5F_ACC_TRUNC_F, file_id, hdferr)
  if (hdferr .eq. -1) print *, "Error creating HDF5 file."

   ! Setup file access property list with parallel I/O access.
  !call h5pcreate_f(H5P_FILE_ACCESS_F, plist_id, hdferr)
  !call h5pset_fapl_mpio_f(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL, hdferr)

  ! Create the file collectively.
  !call h5fcreate_f(filename, H5F_ACC_TRUNC_F, file_id, hdferr, &
  !                 access_prp=plist_id)

  !call h5pclose_f(plist_id, hdferr)

end subroutine create_new_asdf_file


subroutine initialize_hdf5_asdf()
  use hdf5
  implicit none
  ! --- Variables ---
  integer :: hdferr
  call h5open_f(hdferr)
  if (hdferr .eq. -1) print *, "Error opening HDF5 Fortran interface."
end subroutine initialize_hdf5_asdf


subroutine finalize_hdf5_asdf()
  use hdf5
  implicit none
  integer :: hdferr
  call H5close_f(hdferr)
  if (hdferr .eq. -1) print *, "Error closing HDF5 Fortran interface."
end subroutine finalize_hdf5_asdf
