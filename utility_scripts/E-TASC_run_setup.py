#!/usr/bin/env python

import numpy as np
import ual

shot=28906
run=44

itmp = ual.itm(shot, run)
itmp.create()

ntime = 1	# number of time steps
rho_size = 1	# number of rho coordinates
n_ion = 1	# number of main ions
n_imp = 1	# number of impurities
n_zimp = 1	# number ionization states of the given impurity (might be multiple n_zimp corresponding to different impurities)
psi_size = 1 	# number of poloidal flux coordinates

# coreprof cpo
itmp.coreprofArray.resize(ntime)
itmp.coreimpurArray.resize(ntime)
itmp.distributionArray.resize(ntime)
itmp.equilibriumArray.resize(ntime)
for i in range(ntime):
	
	# +++++++++ fill coreprof
	itmp.coreprofArray.array[i].toroid_field.b0 = 0
	itmp.coreprofArray.array[i].profiles1d.zeff.value.resize(rho_size)
	itmp.coreprofArray.array[i].profiles1d.zeff.value[0] = 1
	itmp.coreprofArray.array[i].ne.value.resize(rho_size)
	itmp.coreprofArray.array[i].ne.value[0] = 5e19
	itmp.coreprofArray.array[i].te.value.resize(rho_size)
	itmp.coreprofArray.array[i].te.value[0] = 2e3
	
	# calculate E_c
	e = 1.602176e-19       # elementary charge, [C]
	epsilon0 = 8.85e-12     # vacuum permittivity, [F/m]
	electronDensity = itmp.coreprofArray.array[i].ne.value[0]
	electronTemperature = itmp.coreprofArray.array[i].te.value[0]
	
	CoulombLogarithm = 14.9-0.5*np.log(electronDensity*1e-20)+np.log(electronTemperature*1e-3)
	
	Ec = (electronDensity*np.power(e, 3)*CoulombLogarithm)/(4*np.pi*np.power(epsilon0, 2)* 8.187104744057426e-14)
	
	itmp.coreprofArray.array[i].profiles1d.eparallel.value.resize(rho_size)
	itmp.coreprofArray.array[i].profiles1d.eparallel.value[0] = 8 *Ec
	
	itmp.coreprofArray.array[i].ni.value.resize(rho_size, n_ion)
	itmp.coreprofArray.array[i].ni.value[0,0] = 5e19
	itmp.coreprofArray.array[i].compositions.ions.resize(n_ion)
	itmp.coreprofArray.array[i].compositions.ions[0].zion = 1
	
	itmp.coreprofArray.array[i].rho_tor.resize(rho_size)
	itmp.coreprofArray.array[i].rho_tor_norm.resize(rho_size)
	itmp.coreprofArray.array[i].rho_tor[0] = 0.5
	itmp.coreprofArray.array[i].rho_tor_norm[0] = 1
	
	itmp.coreprofArray.array[i].time = 0
	
	# +++++++++ fill coreimpur
	itmp.coreimpurArray.array[i].rho_tor.resize(rho_size)
	itmp.coreimpurArray.array[i].rho_tor_norm.resize(rho_size)
	itmp.coreimpurArray.array[i].rho_tor[0] = 0.5
	itmp.coreimpurArray.array[i].rho_tor_norm[0] = 1
	
	itmp.coreimpurArray.array[i].impurity.resize(n_imp)
	itmp.coreimpurArray.array[i].impurity[0].z.resize(rho_size,n_zimp)
	itmp.coreimpurArray.array[i].impurity[0].nz.resize(rho_size,n_zimp)
	itmp.coreimpurArray.array[i].impurity[0].z[0,0] = 0
	itmp.coreimpurArray.array[i].impurity[0].nz[0,0] = 0
	
	itmp.coreimpurArray.array[i].time = 0
	
	# +++++++++ fill equilibrium
	itmp.equilibriumArray.array[i].profiles_1d.rho_tor.resize(rho_size)
	itmp.equilibriumArray.array[i].profiles_1d.rho_tor[0] = 0.5
	
	itmp.equilibriumArray.array[i].profiles_1d.b_av.resize(psi_size)
	itmp.equilibriumArray.array[i].profiles_1d.b_av[0] = 0
	
	itmp.equilibriumArray.array[i].time = 0
	
	# +++++++++ fill distribution
	itmp.distributionArray.array[i].distri_vec.resize(1)
	
	itmp.distributionArray.array[i].distri_vec[0].profiles_1d.geometry.rho_tor.resize(rho_size)
	itmp.distributionArray.array[i].distri_vec[0].profiles_1d.geometry.rho_tor_norm.resize(rho_size)
	itmp.distributionArray.array[i].distri_vec[0].profiles_1d.geometry.rho_tor[0] = 0.5
	itmp.distributionArray.array[i].distri_vec[0].profiles_1d.geometry.rho_tor_norm[0] = 1

	itmp.distributionArray.array[i].distri_vec[0].profiles_1d.state.dens.resize(rho_size)
	itmp.distributionArray.array[i].distri_vec[0].profiles_1d.state.dens[0] = 0
	
	itmp.distributionArray.array[i].time = 0
	
	
	
# put data 

itmp.coreprofArray.put()
itmp.coreimpurArray.put()
itmp.distributionArray.put()
itmp.equilibriumArray.put()
itmp.close()







