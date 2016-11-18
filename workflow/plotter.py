import matplotlib.pyplot as plt

rho_tor_norm = distribution0[0].distri_vec[0].profiles_1d.geometry.rho_tor_norm
runaway_density = distribution0[0].distri_vec[0].profiles_1d.state.dens
runaway_current = distribution0[0].distri_vec[0].profiles_1d.state.current

fig1=plt.figure(1)
plt.plot(rho_tor_norm, runaway_density)
plt.xlabel('normalised rho tor')
plt.ylabel('runaway density [1/m3$] ')
fig1.show()
fig1.savefig('plot_density.pdf', format='pdf')

fig2=plt.figure(2)
plt.plot(rho_tor_norm, runaway_current)
plt.xlabel('normalised rho tor')
plt.ylabel('runaway current [A/m3] ')
fig2.show()
fig2.savefig('plot_current.pdf', format='pdf')

# Three subplots sharing both x/y axes
#fig3, (ax1, ax2, ax3) = plt.subplots(3, sharex=True, sharey=True)
#ax1.plot(rho_tor_norm, runaway_density)
#ax1.set_title('Sharing both axes')
#ax2.scatterrho_tor_norm, runaway_density)
#ax3.scatterrho_tor_norm, runaway_density, color='r')
# Fine-tune figure; make subplots close to each other and hide x ticks for
# all but bottom plot.
#fig3.subplots_adjust(hspace=0)
#plt.setp([a.get_xticklabels() for a in fig3.axes[:-1]], visible=False)

#raw_input()
