program ASDF_writer
  ! About functions that must be called collectively:
  !    http://www.hdfgroup.org/HDF5/doc/RM/CollectiveCalls.html

  use hdf5
  use iso_c_bindinG
  use mpi

  implicit none

  character(len=12), parameter :: filename = "synthetic.h5"
  integer(hid_t) :: start_time
  real(kind=8) :: sampling_rate

  ! MPI definitions and calls.
  integer :: mpi_rank, mpi_err       ! MPI error flag

  call MPI_Init(mpi_err)
  call MPI_Comm_rank(MPI_COMM_WORLD, mpi_rank, mpi_err)

  start_time = 393838
  sampling_rate = 0.40*(mpi_rank+1)
  
  call write_ASDF(filename, start_time, sampling_rate)

  call MPI_Finalize(mpi_err)
end program ASDF_writer 
