program diagnostic

    use euitm_schemas
    use euitm_routines
    use write_structures
    implicit none

  TYPE (TYPE_COREPROF), pointer :: COREPROF(:)
  TYPE (TYPE_EQUILIBRIUM), pointer :: EQUILIBRIUM(:)
  TYPE (TYPE_CORESOURCE), pointer :: CORESOURCE(:)
  TYPE (TYPE_CORETRANSP), pointer :: CORETRANSP(:)
  TYPE (TYPE_EDGE), pointer :: EDGE(:)
  TYPE (TYPE_DISTRIBUTION), pointer :: DISTRIBUTION(:)
  integer idx, num_args, shotnumber, runnumber
  
  num_args = command_argument_count()
  
  if (num_args < 1) then
     shotnumber = 28906
  else
     call get_command_argument(1, shotnumber)  
  end if   
  
  if (num_args < 2) then
     runnumber = 1007
  else
     call get_command_argument(2, runnumber)  
  end if    
     

  write(*,*) 'Reading data'
  call euitm_open('euitm', shotnumber, runnumber, idx)

  call euitm_get(idx, 'equilibrium', equilibrium)
  call euitm_get(idx, 'coreprof', coreprof)
  call euitm_get(idx, 'distribution', distribution)
  !call euitm_get(idx, 'coresource', coresource)
  !call euitm_get(idx, 'coretransp', coretransp)
  !call euitm_get(idx, 'edge', edge)
  call euitm_close(idx)
  
  write(*,*) 'Writing data'
  call euitm_create_hdf5('euitm', shotnumber, runnumber, 0, 0, idx)
  call euitm_put(idx,"coreprof",coreprof)
  call euitm_put(idx,"equilibrium",equilibrium)
  call euitm_put(idx,"distribution",distribution)


  call euitm_close(idx)



end program

