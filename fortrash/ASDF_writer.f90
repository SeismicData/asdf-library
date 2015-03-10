program ASDF_writer
  ! About functions that must be called collectively:
  !    http://www.hdfgroup.org/HDF5/doc/RM/CollectiveCalls.html

  use hdf5, only: hid_t
  !use iso_c_bindinG
  use mpi, only: MPI_Init, MPI_Comm_rank, MPI_COMM_WORLD, MPI_Finalize

  implicit none

  character(len=12), parameter :: filename = "synthetic.h5"
  integer(hid_t) :: start_time
  real(kind=8) :: sampling_rate
  character(len=*), parameter :: event_name = "event0123456789"
  character(len=539) :: quakeml_string

  ! MPI definitions and calls.
  integer :: mpi_rank, mpi_err       ! MPI error flag

  call MPI_Init(mpi_err)
  call MPI_Comm_rank(MPI_COMM_WORLD, mpi_rank, mpi_err)

  ! --- Create dummy data ----------------------------------
  start_time = 393838
  sampling_rate = 0.40*(mpi_rank+1)
  if (mpi_rank == 0) then
    call generateQuakeml(quakeml_string, mpi_err)
  endif
  
  ! --- ASDF / HDF5 related stuffs -------------------------
  call initialize_hdf5_asdf()
  call write_ASDF(filename, event_name, start_time, sampling_rate, &
                  quakeml_string)
  call finalize_hdf5_asdf()

  call MPI_Finalize(mpi_err)
end program ASDF_writer 
