program diagnostic

	use euitm_schemas
	use euitm_routines
	use write_structures
	implicit none

	! data input declaration
	type (type_topinfo),      pointer :: topinfo(:)
	type (type_coreprof),     pointer :: coreprof(:)
	type (type_coresource),   pointer :: coresource(:)
	type (type_coreimpur),    pointer :: coreimpur(:)
	type (type_equilibrium),  pointer :: equilibrium(:)
	type (type_distribution), pointer :: distribution(:)
	integer :: idx, num_args
	
	! command line input declaration
	integer :: shotnumber, runnumber
	character(len=20) :: shotnumber_str
	character(len=20) :: runnumber_str
	character(len=20) :: format_str
	
	
	! command line input handle
	num_args = command_argument_count()
	
	if (num_args < 1) then
		 shotnumber_str = '28906'
	else
		 call get_command_argument(1, shotnumber_str)	
	end if	 
	
	if (num_args < 2) then
		 runnumber_str = '666'
	else
		 call get_command_argument(2, runnumber_str)	
	end if			
	
	read (shotnumber_str, *) shotnumber
	read (runnumber_str, *)  runnumber
	
	
	
	!if ((format_str == 'l') .or. (format_str == 'L') .or. (format_str == 'long') .or. (format_str == 'LONG')) then
	
		! read data
		write(*,*) 'Reading data'
		call euitm_open('euitm', shotnumber, runnumber, idx)

		call euitm_get(idx, 'topinfo',      topinfo)
		call euitm_get(idx, 'coreprof',     coreprof)
		call euitm_get(idx, 'coresource',   coresource)
		call euitm_get(idx, 'coreimpur',    coreimpur)
		call euitm_get(idx, 'equilibrium',  equilibrium)
		call euitm_get(idx, 'distribution', distribution)
		call euitm_close(idx)
	
		! write data	
		write(*,*) 'Writing data'
		call euitm_create_hdf5('euitm', shotnumber, runnumber, 0, 0, idx)
		call euitm_put(idx, 'topinfo',      topinfo)	
		call euitm_put(idx, 'coreprof',     coreprof)	
		call euitm_put(idx, 'coresource',   coresource)	
		call euitm_get(idx, 'coreimpur',    coreimpur)
		call euitm_put(idx, 'equilibrium',  equilibrium)
		call euitm_put(idx, 'distribution', distribution)	
		call euitm_close(idx)
		
	!else
	!
	!end if



end program

