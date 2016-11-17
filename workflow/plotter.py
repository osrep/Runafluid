import matplotlib.pyplot as plt

rho_tor_norm = distribution0[0].distri_vec[0].profiles_1d.geometry.rho_tor_norm
runaway_density = distribution0[0].distri_vec[0].profiles_1d.state.dens
plt.figure(1)
plt.plot(rho_tor_norm, runaway_density)
plt.xlabel('normalised rho tor')
plt.ylabel('runaway density [1/m3] ')
plt.show()
