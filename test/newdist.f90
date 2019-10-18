subroutine equ2dist(coreprofin,distributionout)

use euITM_schemas
!use euITM_routines ! Needed only if using euitm_deallocate and euitm_copy functions

implicit none

integer,parameter :: DP=kind(1.0D0)
!
! Always describe cpo as array 
! In case of time slice, the size of the input cpo is 1
!
type (type_coreprof),pointer :: coreprofin(:)
type (type_distribution),pointer :: distributionout(:)

type type_distri_vec  !    
  type (type_enum_instance),pointer :: wave_id(:) => null()  ! /distribution/distri_vec(i)/wave_id(i) - List all waves affecting the distribution, as specified in waves/coherentwave/wave_id (see waves_typ
  type (type_enum_instance),pointer :: source_id(:) => null()  ! /distribution/distri_vec(i)/source_id(i) - List all neutral beam injectors and reactions contributing to the source, as specified in distsource
  type (type_species_reference) :: species  ! /distribution/distri_vec(i)/species - Defines the distribution function species represented in this element of distri_vec. Time-dependent
  integer  :: gyro_type=-999999999       ! /distribution/distri_vec(i)/gyro_type - Defines how to interpret the spatial coordinates: 1 = given at the actual particle position;  2 = gi
  type (type_fast_thermal_separation_filter) :: fast_filter  ! /distribution/distri_vec(i)/fast_filter - Description of how the fast and the thermal particle populations, used in global_param and profiles_
  type (type_dist_global_param) :: global_param  ! /distribution/distri_vec(i)/global_param - Global parameters (in most cases volume integrated and surface averaged quanatities). Time-dependent
  type (type_dist_profiles_1d) :: profiles_1d  ! /distribution/distri_vec(i)/profiles_1d - Flux surface averaged profiles.
  type (type_dist_profiles_2d) :: profiles_2d  ! /distribution/distri_vec(i)/profiles_2d - 2D profiles in the poloidal plane
  type (type_dist_func) :: dist_func  ! /distribution/distri_vec(i)/dist_func - Distribution functions. The total distribution total distribution can either be given by the a set o
  type (type_codeparam) :: codeparam  ! /distribution/distri_vec(i)/codeparam - Code parameters of physics code, i.e. codes calculating a wave field.
endtype

integer :: i,j,k,l


!write(0,*) 'Entering subroutine coreprof2mhd'

!write(0,*) 'size of input CPO = ',size(coreprofin)

! The output CPO must be allocated with its number of time slices (1 for a single time slice physics module)
! Here we allocate the output CPO to the same size as the input CPO (but this is not a general rule)
allocate(distributionout(size(coreprofin)))

! Fill in the output CPO (Physical data)
do i=1,1
   ! Time : copy from input CPO
!   write(0,*) 'Received input time from Coreprof : ', coreprofin(i)%time
   distributionout(i)%time = coreprofin(i)%time  ! THE TIME FIELD MUST BE FILLED (MANDATORY) in case of multiple time slice mode for the CPO; NB this information is overwritten by the wrapper in case of single slice mode for the CPO
   !allocate(distributionout(i)%distri_vec(1))
   allocate(distributionout(i)%distri_vec(1))
   distributionout(i)%distri_vec(1)%gyro_type=1;
   
   allocate(distributionout(i)%distri_vec(1))
   
enddo

!allocate(distributionout(1)%codeparam%datainfo(1))   ! For a string of 132 characters max.
!distributionout(1)%codeparam%datainfo(1)   = 'Runaway_distribution'
allocate(distributionout(1)%codeparam%codename(1))   ! For a string of 132 characters max.
distributionout(1)%codeparam%codename(1)   = 'Runaway_fluid'
allocate(distributionout(1)%codeparam%codeversion(1))   ! For a string of 132 characters max.
distributionout(1)%codeparam%codeversion(1)   = '0.1'
!allocate(distributionout(1)%codeparam%parameters(1))   ! For a string of 132 characters max.
!distributionout(1)%codeparam%parameters(1) = 'my_code_specific_parameters'
allocate(distributionout(1)%codeparam%output_diag(1))   ! For a string of 132 characters max.
distributionout(1)%codeparam%output_diag(1) = 'my_output_diag'
distributionout(1)%codeparam%output_flag = 0   ! Integer output flag, 0 means the run was successful and can be used in the rest of the workflow, <0 means failure

!write(0,*) 'End Subroutine'

return
end subroutine

