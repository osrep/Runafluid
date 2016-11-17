import matplotlib.pyplot as plt

rho_tor_norm = distribution0[0].distri_vec[0].profiles_1d.geometry.rho_tor_norm
runaway_density = distribution0[0].distri_vec[0].profiles_1d.state.dens
runaway_current = distribution0[0].distri_vec[0].profiles_1d.state.current

fig1=plt.figure(1)
plt.plot(rho_tor_norm, runaway_density)
plt.xlabel('normalised rho tor')
plt.ylabel('runaway density [1/m^3] ')
fig1.show()


fig2=plt.figure(2)
plt.plot(rho_tor_norm, runaway_current)
plt.xlabel('normalised rho tor')
plt.ylabel('runaway current [A/m^3] ')
fig2.show()
