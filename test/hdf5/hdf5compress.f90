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
	integer :: idx, num_args, coreprof_slices, coresource_slices, equilibrium_slices, rho_length, source_length, i, j, k
	
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
		coreprof_out(i)%time = coreprof_in(i)%time
		coreprof_out(i)%toroid_field%r0 = coreprof_in(i)%toroid_field%r0
		
		allocate(coreprof_out(i)%rho_tor(rho_length))
		allocate(coreprof_out(i)%rho_tor_norm(rho_length))
		
		allocate(coreprof_out(i)%te%value(rho_length))
		allocate(coreprof_out(i)%ne%value(rho_length))	
		
		allocate(coreprof_out(i)%profiles1d%q%value(rho_length))
		allocate(coreprof_out(i)%profiles1d%zeff%value(rho_length))
		allocate(coreprof_out(i)%profiles1d%eparallel%value(rho_length))
			
		allocate(coreprof_out(i)%profiles1d%joh%value(rho_length))
		allocate(coreprof_out(i)%profiles1d%jtot%value(rho_length))
		allocate(coreprof_out(i)%profiles1d%jphi%value(rho_length))
				
		do j=1,rho_length
			coreprof_out(i)%rho_tor(j) = coreprof_in(i)%rho_tor(j)
			coreprof_out(i)%rho_tor_norm(j) = coreprof_in(i)%rho_tor_norm(j)
			
			coreprof_out(i)%te%value(j) = coreprof_in(i)%te%value(j)
			coreprof_out(i)%ne%value(j) = coreprof_in(i)%ne%value(j)	
			
			coreprof_out(i)%profiles1d%q%value(j) = coreprof_in(i)%profiles1d%q%value(j)
			coreprof_out(i)%profiles1d%zeff%value(j) = coreprof_in(i)%profiles1d%zeff%value(j)
			coreprof_out(i)%profiles1d%eparallel%value(j) = coreprof_in(i)%profiles1d%eparallel%value(j)
					
			coreprof_out(i)%profiles1d%joh%value(j) = coreprof_in(i)%profiles1d%joh%value(j)
			coreprof_out(i)%profiles1d%jtot%value(j) = coreprof_in(i)%profiles1d%jtot%value(j)
			coreprof_out(i)%profiles1d%jphi%value(j) = coreprof_in(i)%profiles1d%jphi%value(j)
		end do	
	end do
		
	coresource_slices = size(coresource_in)
	allocate(coresource_out(coresource_slices))
	source_length= size(coresource_in(1)%values)
	do i=1,coresource_slices
		coresource_out(i)%time = coresource_in(i)%time		
		allocate(coresource_out(i)%values(source_length))	
		
		do k=1,source_length
		
			rho_length = size(coresource_in(i)%values(k)%j)
			allocate(coresource_out(i)%values(k)%j(rho_length))	
			do j=1,rho_length
				coresource_out(i)%values(k)%j(j) = coresource_in(i)%values(k)%j(j)
			end do	
		end do
	end do
	
	equilibrium_slices = size(equilibrium_in)
	allocate(equilibrium_out(equilibrium_slices))
	rho_length = size(equilibrium_in(1)%profiles_1d%rho_tor)
	
	do i=1,equilibrium_slices
		equilibrium_out(i)%time = equilibrium_in(i)%time
		equilibrium_out(i)%eqgeometry%a_minor = equilibrium_in(i)%eqgeometry%a_minor
		equilibrium_out(i)%eqgeometry%elongation = equilibrium_in(i)%eqgeometry%elongation
		equilibrium_out(i)%global_param%i_plasma = equilibrium_in(i)%global_param%i_plasma
		equilibrium_out(i)%global_param%toroid_field%b0 = equilibrium_in(i)%global_param%toroid_field%b0
		
		allocate(equilibrium_out(i)%profiles_1d%rho_tor(rho_length))
		allocate(equilibrium_out(i)%profiles_1d%area(rho_length))
		
		do j=1,rho_length
				equilibrium_out(i)%profiles_1d%rho_tor(j) = equilibrium_in(i)%profiles_1d%rho_tor(j)
				equilibrium_out(i)%profiles_1d%area(j) = equilibrium_in(i)%profiles_1d%area(j)
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

