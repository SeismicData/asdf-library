program write_example
  
  use mpi
  use iso_c_binding

  implicit none

  integer, parameter :: MAX_STRING_LENGTH = 32

  character(len=MAX_STRING_LENGTH) :: filename

  !--- Data to be written to the ASDF file
  character(len=MAX_STRING_LENGTH) :: event_name
  character(len=MAX_STRING_LENGTH) :: station_name
  character(len=MAX_STRING_LENGTH) :: station_xml
  character(len=MAX_STRING_LENGTH) :: quakeml

  integer :: num_waveforms

  integer :: start_time, nsamples
  double precision :: sampling_rate

  integer :: num_stations, num_channels_per_station
  character(len=MAX_STRING_LENGTH), dimension(:), allocatable :: network_names
  character(len=MAX_STRING_LENGTH), dimension(:), allocatable :: station_names

  real, dimension(:, :, :), allocatable :: waveforms

  !-- ASDF variables
  integer :: file_id   ! HDF5 file id, also root group "/"
  integer :: waveforms_grp  ! Group "/Waveforms/" 
  integer, dimension(:), allocatable :: station_grps
  integer, dimension(:, :, :), allocatable :: data_ids 

  !---
  integer :: myrank, mysize, comm
  integer :: i, j, ier, k

  !---
  integer, dimension(:), allocatable :: num_stations_proc, num_waveforms_proc
  integer :: max_num_stations_proc
  character(len=MAX_STRING_LENGTH), dimension(:,:), allocatable :: &
      station_names_proc, network_names_proc
  integer, dimension(:), allocatable :: displs, rcounts
  integer, dimension(:,:), allocatable :: station_grps_proc

  character(len=MAX_STRING_LENGTH) :: waveform_name


  !--------------------------------------------------------
  ! Init MPI 
  !--------------------------------------------------------
  call MPI_Init(ier)
  call MPI_Comm_rank(MPI_COMM_WORLD, myrank, ier)
  call MPI_Comm_size(MPI_COMM_WORLD, mysize, ier)
  call MPI_Comm_dup(MPI_COMM_WORLD, comm, ier)

  !--------------------------------------------------------
  ! Setup data 
  !--------------------------------------------------------
  filename = "synthetic.h5"
  event_name = "event0123456789"
  quakeml = "<quakeml>"
  station_xml = "<station_xml>"

  num_stations = 1 + myrank
  num_channels_per_station = 2
  sampling_rate = 0.1
  nsamples = 5
  start_time = 393838

  num_waveforms = num_stations * num_channels_per_station

  allocate(network_names(num_stations), stat=ier)
  allocate(station_names(num_stations), stat=ier)
  allocate(waveforms(nsamples, num_channels_per_station, num_stations), &
           stat=ier)

  do i = 1, num_stations
    write(network_names(i), '(a,i0.2)') "NTWK_", myrank
    write(station_names(i), '(a,i0.2,a,i0.2)') "STAT_", myrank, "_", i 
  enddo

  ! -- We do not care about seeding.
  call random_number(waveforms)
  
  !--------------------------------------------------------
  ! Check data 
  !--------------------------------------------------------
  if (myrank == -1) then
    print *, "Writing: "
    print *, "   filename                : ", trim(filename)
    print *, "   event                   : ", trim(event_name)
    print *, "   num_stations            : ", num_stations
    print *, "   num_channels_per_station: ", num_channels_per_station 
    print *, "   num_waveforms           : ", num_waveforms
    print *, "   networks                : ", &
        (trim(network_names(i)), i=1, num_stations)
    print *, "   stations                : ", &
        (trim(station_names(i)), i=1, num_stations)
    print *, "   waveforms               : ", &
        waveforms
    call flush()
  endif

  !--------------------------------------------------------
  ! ASDF variables
  !--------------------------------------------------------
  ! Find how many stations are managed by each process
  allocate(num_stations_proc(mysize))
  call MPI_Allgather(num_stations, 1, MPI_INTEGER, num_stations_proc, 1, &
                     MPI_INTEGER, MPI_COMM_WORLD, ier)
  ! find the largest number of stations per process
  max_num_stations_proc = maxval(num_stations_proc)
  !print *, myrank, max_num_stations_proc
  !print *, myrank, size(station_names)
  call flush()

  allocate(num_waveforms_proc(mysize))
  call MPI_Allgather(num_waveforms, 1, MPI_INTEGER, num_waveforms_proc, 1, &
                     MPI_INTEGER, MPI_COMM_WORLD, ier)


  allocate(displs(mysize))
  allocate(rcounts(mysize))

  ! Everyone should know about each and every station name
  allocate(station_names_proc(max_num_stations_proc, mysize))
  allocate(network_names_proc(max_num_stations_proc, mysize))

  do i = 1, mysize
    displs(i) = (i-1) * max_num_stations_proc * max_string_length
    rcounts(i) = num_stations_proc(i) * max_string_length
  enddo
  call MPI_Allgatherv(station_names, &
                      num_stations *  MAX_STRING_LENGTH, &
                      MPI_CHARACTER, &
                      station_names_proc, &
                      rcounts, &
                      displs, &
                      MPI_CHARACTER, &
                      MPI_COMM_WORLD, ier)
  call MPI_Allgatherv(network_names, &
                      num_stations *  MAX_STRING_LENGTH, &
                      MPI_CHARACTER, &
                      network_names_proc, &
                      rcounts, &
                      displs, &
                      MPI_CHARACTER, &
                      MPI_COMM_WORLD, ier)

  call MPI_Barrier(comm, ier)
  if (myrank == -1) then
    print *, "'''''''''''"
    print *, "num_stations", num_stations
    print *, "string lenght", MAX_STRING_LENGTH
    print *, "rcounts", rcounts
    print *, "displs", displs
    print *, "max_num_Stations", max_num_stations_proc
    print *, "'''''''''''"
    call flush()
    do i = 1, mysize
      do j = 1, num_stations_proc(i)
        print *, trim(station_names_proc(j, i)) 
        print *, trim(network_names_proc(j, i)) 
      enddo
    enddo
  endif
  !print *, myrank, station_names
  call flush()
                     

  !allocate(station_grps(num_stations)) ! should be a 2D array
  allocate(station_grps_proc(max_num_stations_proc, mysize))
  
  allocate(data_ids(num_channels_per_station, &
                    max_num_stations_proc, &
                    mysize))

  !--------------------------------------------------------
  ! write ASDF 
  !--------------------------------------------------------
  call ASDF_initialize_hdf5_f(ier);
  call ASDF_create_new_file_f(trim(filename), comm, file_id)

  call ASDF_write_string_attribute_f(file_id, "file_format" // C_NULL_CHAR, &
                                     "ASDF" // C_NULL_CHAR, ier)
  call ASDF_write_string_attribute_f(file_id, "file_version" // C_NULL_CHAR, &
                                     "0.0.1.b" // C_NULL_CHAR, ier)

  call ASDF_write_auxiliary_data_f(file_id, ier)
  call ASDF_write_provenance_data_f(file_id, ier)
  call ASDF_write_quakeml_f(file_id, trim(quakeml), ier)

  call ASDF_create_waveforms_group_f(file_id, waveforms_grp)
  
  do k = 1, mysize  
    do j = 1, num_stations_proc(k)
      call ASDF_create_stations_group_f(waveforms_grp,   &
           trim(network_names_proc(j, k)) // "." //      &
           trim(station_names_proc(j,k)) // C_NULL_CHAR, &
           trim(station_xml) // C_NULL_CHAR,             &
           station_grps_proc(j, k))

      do  i = 1, num_channels_per_station
        write(waveform_name, '(a,i0.2)') &
           trim(network_names_proc(j,k)) // "." //      &
           trim(station_names_proc(j,k)) // ".channel_", i 

        call ASDF_define_waveform_f(station_grps_proc(j,k), &
             nsamples, start_time, sampling_rate, &
             trim(event_name) // C_NULL_CHAR, &
             trim(waveform_name) // C_NULL_CHAR, &
             data_ids(i, j, k))
      enddo

    enddo
  enddo


  do j = 1, num_stations
    do i = 1, num_channels_per_station
      call ASDF_write_full_waveform_f(data_ids(i, j, myrank+1), &
                                      waveforms(:, i, j), ier)
    enddo
  enddo


  do j = 1, mysize  
    do i = 1, num_stations_proc(j)
      call ASDF_close_group_f(station_grps_proc(i, j), ier)
    enddo
  enddo
  call ASDF_close_group_f(waveforms_grp, ier)
  call ASDF_finalize_hdf5_f(ier);


  deallocate(station_grps_proc)
  deallocate(num_stations_proc)

  deallocate(waveforms)
  deallocate(station_names)
  deallocate(network_names)

  call MPI_Finalize(ier)

end program write_example
