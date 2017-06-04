program hdf5compress

	use euitm_schemas
	use euitm_routines
	use write_structures
	implicit none

	! data input declaration
!	type (type_topinfo),      pointer :: topinfo(:)
	type (type_coreprof),     pointer :: coreprof_in(:)
	type (type_coresource),   pointer :: coresource_in(:)
	type (type_equilibrium),  pointer :: equilibrium_in(:)
	
	
	type (type_coreprof),     pointer :: coreprof_out(:)
	type (type_coresource),   pointer :: coresource_out(:)
	type (type_equilibrium),  pointer :: equilibrium_out(:)
	integer :: idx, num_args, coreprof_slices, coresource_slices, equilibrium_slices, i
	
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
	
	
	
	! read data
	write(*,*) 'Reading data'
	call euitm_open('euitm', shotnumber, runnumber, idx)

!		call euitm_get(idx, 'topinfo',      topinfo)
	call euitm_get(idx, 'coreprof',     coreprof_in)
	call euitm_get(idx, 'coresource',   coresource_in)
	call euitm_get(idx, 'equilibrium',  equilibrium_in)
	call euitm_close(idx)

	! fill up data
	coreprof_slices = size(coreprof_in)
	allocate(coreprof_out(coreprof_slices))
	rho_length = size(coreprof_in(1)%rho_tor_norm)
	
	do i=1,coreprof_slices
		allocate(oreprof_out(i)%rho_tor_norm(rho_length))
		do j=1,rho_length
			coreprof_out(i)%rho_tor_norm(j) = coreprof_in(i)%rho_tor_norm(j)	
		end do	
	end do

	shotnumber=99999
	! write data	
	write(*,*) 'Writing data'
	call euitm_create_hdf5('euitm', shotnumber, runnumber, 0, 0, idx)
!		call euitm_put(idx, 'topinfo',      topinfo)	
	call euitm_put(idx, 'coreprof',     coreprof_out)	
	call euitm_put(idx, 'coresource',   coresource_out)	
	call euitm_put(idx, 'equilibrium',  equilibrium_out)
	call euitm_close(idx)




end program

