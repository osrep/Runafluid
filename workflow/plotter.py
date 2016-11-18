import matplotlib.pyplot as plt

rho_tor_norm = distribution0[0].distri_vec[0].profiles_1d.geometry.rho_tor_norm

indices = np.where(rho_tor_norm < 0.95)[0]

runaway_density = distribution0[0].distri_vec[0].profiles_1d.state.dens[indices]
runaway_current = distribution0[0].distri_vec[0].profiles_1d.state.current[indices]

dreicer_rate = temporary0[0].timed.float1d[0].value[indices]
dreicer_rate63 = temporary0[0].timed.float1d[2].value[indices]
dreicer_rate66 = temporary0[0].timed.float1d[3].value[indices]
dreicer_rate67 = temporary0[0].timed.float1d[4].value[indices]


avalanche_rate = temporary0[0].timed.float1d[1].value[indices]
avalanche_rate_lin = temporary0[0].timed.float1d[12].value[indices]
avalanche_rate_thr = temporary0[0].timed.float1d[13].value[indices]

rel_efield = temporary0[0].timed.float1d[19].value[indices]


fig1=plt.figure(1)
plt.plot(rel_efield, runaway_density)
plt.xlabel('relative electric field (E/Ec)')
plt.ylabel('runaway density [1/m3$] ')
fig1.show()
fig1.savefig('/u/maradi/svn/runafluid/trunk/workflow/plot_density.pdf')

fig2=plt.figure(2)
plt.plot(rel_efield, runaway_current)
plt.xlabel('relative electric field (E/Ec)')
plt.ylabel('runaway current [A/m3] ')
fig2.show()
fig2.savefig('/u/maradi/svn/runafluid/trunk/workflow/plot_current.pdf')



fig3=plt.figure(3)
plt.plot(rel_efield, dreicer_rate)
plt.xlabel('relative electric field (E/Ec)')
plt.ylabel('Dreicer rate [1/m3s] ')
fig3.show()
fig3.savefig('/u/maradi/svn/runafluid/trunk/workflow/plot_dreicer.pdf')




fig4=plt.figure(4)
plt.plot(rel_efield, dreicer_rate67, label='H&C (67)')
plt.plot(rel_efield, dreicer_rate66, label='H&C (66)')
plt.plot(rel_efield, dreicer_rate63, label='H&C (63)')
plt.xlabel('relative electric field (E/Ec)')
plt.ylabel('Dreicer rate [1/m3s] ')
plt.legend(loc=2)
fig4.show()
fig4.savefig('/u/maradi/svn/runafluid/trunk/workflow/plot_dreicer_all.pdf')

fig5=plt.figure(5)
plt.plot(rel_efield, dreicer_rate63, label='H&C (63) without toroidicity')
plt.plot(rel_efield, dreicer_rate, label='H&C (63)  with toroidicity')
plt.xlabel('relative electric field (E/Ec)')
plt.ylabel('Dreicer rate [1/m3s] ')
plt.legend(loc=2)
fig5.show()
fig5.savefig('/u/maradi/svn/runafluid/trunk/workflow/plot_dreicer_tor.pdf')



fig7=plt.figure(7)
plt.plot(rel_efield, avalanche_rate_lin, label='Rosenbluth-Putvinski linear model')
plt.plot(rel_efield, avalanche_rate_thr, label='Onset threshold model')
plt.xlabel('relative electric field (E/Ec)')
plt.ylabel('normalised Avalanche rate [1/s]')
plt.legend(loc=2)
fig7.show()
fig7.savefig('/u/maradi/svn/runafluid/trunk/workflow/plot_avalanche_threshold.pdf')



fig8=plt.figure(8)
plt.plot(rel_efield, avalanche_rate_thr, label='OTM without toroidicity')
plt.plot(rel_efield, avalanche_rate, label='OTM  with toroidicity')
plt.xlabel('relative electric field (E/Ec)')
plt.ylabel('normalised Avalanche rate [1/s]')
plt.legend(loc=2)
fig8.show()
fig8.savefig('/u/maradi/svn/runafluid/trunk/workflow/plot_avalanche_tor.pdf')

# Three subplots sharing both x/y axes
#fig3, (ax1, ax2, ax3) = plt.subplots(3, sharex=True, sharey=True)
#ax1.plot(rel_efield, runaway_density)
#ax1.set_title('Sharing both axes')
#ax2.scatterrel_efield, runaway_density)
#ax3.scatterrel_efield, runaway_density, color='r')
# Fine-tune figure; make subplots close to each other and hide x ticks for
# all but bottom plot.
#fig3.subplots_adjust(hspace=0)
#plt.setp([a.get_xticklabels() for a in fig3.axes[:-1]], visible=False)

#fig3.savefig('/u/maradi/svn/runafluid/trunk/workflow/plot_current.pdf')

#raw_input()
