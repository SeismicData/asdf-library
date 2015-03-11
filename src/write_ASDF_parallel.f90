
program write_ASDF
  use hdf5
  use mpi
  use iso_c_binding

  implicit none

  integer :: i
  !----------------------------------------------------------------------------
  ! Variables to write down
  !----------------------------------------------------------------------------
  integer :: nsamples
  real, allocatable, dimension(:) :: waveform
  type(c_ptr), dimension(1:2), target :: wdata
  !----------------------------------------------------------------------------
  ! MPI variables
  !----------------------------------------------------------------------------
  integer :: comm, info, mpi_err, mpi_size, mpi_rank
  !----------------------------------------------------------------------------
  ! HDF5 variables
  !----------------------------------------------------------------------------
  character(len=*), parameter :: filename = "synthetic.h5"
  integer(hid_t) :: file_id
  integer(hid_t) :: plist_id
  integer :: hdferr
  integer(hid_t) :: filetype
  integer(hid_t) :: space_scalar, space_array
  integer(hid_t) :: attr, attr2, dataset_Source, dataset_Receiver
  integer(hid_t) :: group, dcpl, memtype
  integer(hid_t) :: start_time
  integer(hsize_t), dimension(1) :: maxdims
  type(c_ptr) :: f_ptr
  type(c_ptr) :: f_ptr2
  ! Groups
  character(len=256) :: group_name
  integer(hid_t) :: groupAuxiliary, groupProvenance
  integer(hid_t), allocatable :: groups(:)
  ! Datasets in groups
  integer(hid_t) :: dataspace
  integer(hsize_t), dimension(1) :: dims
  character(len=256) :: dataset_name
  integer(hid_t), allocatable :: datasets(:)

  ! --- Initialize MPI ---
  comm = MPI_COMM_WORLD
  info = MPI_INFO_NULL

  call MPI_Init(mpi_err)
  call MPI_Comm_size(comm, mpi_size, mpi_err)
  call MPI_Comm_rank(comm, mpi_rank, mpi_err)

  ! --- Allocate and set our variables

  nsamples = 10
  allocate(waveform(nsamples))
  do i = 1, nsamples
    waveform(i) = (mpi_rank + 1) * i * 0.1
  enddo

  ! Set the number of hdf5 groups and datasets
  ! Note: This is not the correct size
  !     The number of groups should be the TOTAL number of STATIONS
  !     The number of datasets should be the TOTAL number of WAVEFORMS
  allocate(groups(mpi_size))
  allocate(datasets(mpi_size))

  call h5open_f(hdferr) 
  ! Setup file access property list with parallel I/O access
  call h5pcreate_f(H5P_FILE_ACCESS_F, plist_id, hdferr)
  call h5pset_fapl_mpio_f(plist_id, comm, info, hdferr)

  ! Create the file collectively
  call h5fopen_f(filename, H5F_ACC_RDWR_F, file_id, hdferr, access_prp = plist_id)

  !--------------------------------------------------------------------
  ! --- Declare data
  !     Everything should be declared collectively.
  !     Therefore, all sizes must be known beforehand.
  !     Wenjei knows how to gather all 'size' information
  !--------------------------------------------------------------------

  ! Create the dataspaces (what buffers looks likes)
  ! Multiple dataspaces might be necessary if there is data with different
  ! sizes to write
  dims(1) = nsamples
  call h5screate_simple_f(1, dims, dataspace, hdferr)

  do i = 1, mpi_size
    write(group_name, "(A6, I1)") "Group ", i
    call h5gcreate_f(file_id, group_name, groups(i), hdferr)
  
    write(dataset_name, "(A8, I1)") "Dataset ", i
    call h5dcreate_f(groups(i), dataset_name, H5T_IEEE_F32LE, dataspace, datasets(i), hdferr)
  enddo

  ! --- Now write the data. Each process write different data
  !     It wil not be "mpi_rank+1" but rather a loop over the waveform
  !     needs to be written
  dims(1) = nsamples
  call h5dwrite_f(datasets(mpi_rank+1), H5T_IEEE_F32LE, waveform, dims, hdferr)

  ! --- Clean up
  do i = 1, mpi_size
    call h5dclose_f(datasets(i), hdferr)
    call h5gclose_f(groups(i), hdferr)
  enddo

  call h5sclose_f(dataspace, hdferr)
  call h5pclose_f(plist_id, hdferr)
  call h5fclose_f(file_id, hdferr)

  call MPI_Finalize(mpi_err)

end program write_ASDF
