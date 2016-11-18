import matplotlib.pyplot as plt

rho_tor_norm = distribution0[0].distri_vec[0].profiles_1d.geometry.rho_tor_norm
runaway_density = distribution0[0].distri_vec[0].profiles_1d.state.dens
runaway_current = distribution0[0].distri_vec[0].profiles_1d.state.current

dreicer_rate = temporary0[0].timed.float1d[0].value
dreicer_rate63 = temporary0[0].timed.float1d[0].value
dreicer_rate66 = temporary0[0].timed.float1d[0].value
dreicer_rate67 = temporary0[0].timed.float1d[0].value


fig1=plt.figure(1)
plt.plot(rho_tor_norm, runaway_density)
plt.xlabel('normalised rho tor')
plt.ylabel('runaway density [1/m3$] ')
fig1.show()
fig1.savefig('/u/maradi/svn/runafluid/trunk/workflow/plot_density.pdf')

fig2=plt.figure(2)
plt.plot(rho_tor_norm, runaway_current)
plt.xlabel('normalised rho tor')
plt.ylabel('runaway current [A/m3] ')
fig2.show()
fig2.savefig('/u/maradi/svn/runafluid/trunk/workflow/plot_current.pdf')



fig3=plt.figure(3)
plt.plot(rho_tor_norm, dreicer_rate)
plt.xlabel('normalised rho tor')
plt.ylabel('Dreicer rate [1/m3s] ')
fig3.show()
fig3.savefig('/u/maradi/svn/runafluid/trunk/workflow/plot_dreicer.pdf')




fig3=plt.figure(4)
plt.plot(rho_tor_norm, dreicer_rate67, legend='H&C (67)')
plt.plot(rho_tor_norm, dreicer_rate66, legend='H&C (66)')
plt.plot(rho_tor_norm, dreicer_rate63, legend='H&C (63)')
plt.xlabel('normalised rho tor')
plt.ylabel('Dreicer rate [1/m3s] ')
plt.legend()
fig3.show()
fig3.savefig('/u/maradi/svn/runafluid/trunk/workflow/plot_dreicer_all.pdf')



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

#fig3.savefig('/u/maradi/svn/runafluid/trunk/workflow/plot_current.pdf')

#raw_input()
