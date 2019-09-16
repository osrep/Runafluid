#!/usr/bin/env python

import pickle
import numpy as np
import ual
from time import asctime
import copy

shot=28906
run=44

itmp = ual.itm(shot, run)
itmp.create()

ntime = 1
#npoints = shape(tr2itm['rbnd'])[0]

# coreprof cpo
itmp.coreprofArray.resize(ntime)
for i in range(ntime):
	# +++++++++ fill coreprof
	
	itmp.coreprofArray.array[i].profiles1d.zeff.value.resize(1)
	itmp.coreprofArray.array[i].profiles1d.zeff.value[0] = 1
	itmp.coreprofArray.array[i].toroid_field.b0 = 0
	itmp.coreprofArray.array[i].ne.value.resize(1)
	itmp.coreprofArray.array[i].ne.value[0] = 5e19
	itmp.coreprofArray.array[i].te.value.resize(1)
	itmp.coreprofArray.array[i].te.value[0] = 2e3
	
	# calculate E_c
	e = 1.602176e-19       # elementary charge, [C]
	epsilon0 = 8.85e-12     # vacuum permittivity, [F/m]
	electronDensity = itmp.coreprofArray.array[i].ne.value[0]
	electronTemperature = itmp.coreprofArray.array[i].te.value[0]
	
	CoulombLogarithm = 14.9-0.5*np.log(electronDensity*1e-20)+np.log(electronTemperature*1e-3)
	
	Ec = (electronDensity*np.power(e, 3)*CoulombLogarithm)/(4*np.pi*np.power(epsilon0, 2)* 8.187104744057426e-14)
	
	itmp.coreprofArray.array[i].profiles1d.eparallel.value.resize(1)
	itmp.coreprofArray.array[i].profiles1d.eparallel.value[0] = 8 *Ec
	itmp.coreprofArray.array[i].time = 0
	
# put data 

itmp.coreprofArray.put()
itmp.close()







