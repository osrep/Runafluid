import matplotlib.pyplot as plt


rho_tor_norm = distribution0[0].distri_vec[0].profiles_1d.geometry.rho_tor_norm
runaway_density = distribution0[0].distri_vec[0].profiles_1d.state.dens

fig1 = plt.figure()
ax1 = fig1.add_subplot(111)
ax1.plot(rho_tor_norm, runaway_density)
ax1.xlabel('normalised rho tor')
ax1.ylabel('runaway density [1/m3] ')
ax1.show()
