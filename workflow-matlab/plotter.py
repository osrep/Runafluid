#import matplotlib.pyplot as plt
import matlab.engine 
eng = matlab.engine.start_matlab()

rho_tor_norm = distribution0[0].distri_vec[0].profiles_1d.geometry.rho_tor_norm

indices = where(rho_tor_norm < 0.95)[0]

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


eng.figure(1)
eng.plot(rel_efield, runaway_density)
eng.xlabel('relative electric field (E/Ec)')
eng.ylabel('runaway density [1/m3$] ')
eng.show()
eng.savefig('/u/ahadar/svn/runafluid/trunk/workflow/plot_density.pdf')

eng.figure(2)
eng.plot(rel_efield, runaway_current)
eng.xlabel('relative electric field (E/Ec)')
eng.ylabel('runaway current [A/m3] ')
eng.show()
eng.savefig('/u/ahadar/svn/runafluid/trunk/workflow/plot_current.pdf')



eng.figure(3)
eng.plot(rel_efield, dreicer_rate)
eng.xlabel('relative electric field (E/Ec)')
eng.ylabel('Dreicer rate [1/m3s] ')
eng.show()
eng.savefig('/u/ahadar/svn/runafluid/trunk/workflow/plot_dreicer.pdf')




eng.figure(4)
eng.plot(rel_efield, dreicer_rate67, label='H&C (67)')
eng.plot(rel_efield, dreicer_rate66, label='H&C (66)')
eng.plot(rel_efield, dreicer_rate63, label='H&C (63)')
eng.xlabel('relative electric field (E/Ec)')
eng.ylabel('Dreicer rate [1/m3s] ')
eng.legend(loc=2)
eng.show()
eng.savefig('/u/ahadar/svn/runafluid/trunk/workflow/plot_dreicer_all.pdf')

eng.figure(5)
eng.plot(rel_efield, dreicer_rate63, label='H&C (63) without toroidicity')
eng.plot(rel_efield, dreicer_rate, label='H&C (63)  with toroidicity')
eng.xlabel('relative electric field (E/Ec)')
eng.ylabel('Dreicer rate [1/m3s] ')
eng.legend(loc=2)
eng.show()
eng.savefig('/u/ahadar/svn/runafluid/trunk/workflow/plot_dreicer_tor.pdf')



eng.figure(7)
eng.plot(rel_efield, avalanche_rate_lin, label='Rosenbluth-Putvinski linear model')
eng.plot(rel_efield, avalanche_rate_thr, label='Onset threshold model')
eng.xlabel('relative electric field (E/Ec)')
eng.ylabel('normalised Avalanche rate [1/s]')
eng.legend(loc=2)
eng.show()
eng.savefig('/u/ahadar/svn/runafluid/trunk/workflow/plot_avalanche_threshold.pdf')



eng.figure(8)
eng.plot(rel_efield, avalanche_rate_thr, label='OTM without toroidicity')
eng.plot(rel_efield, avalanche_rate, label='OTM  with toroidicity')
eng.xlabel('relative electric field (E/Ec)')
eng.ylabel('normalised Avalanche rate [1/s]')
eng.legend(loc=2)
eng.show()
eng.savefig('/u/ahadar/svn/runafluid/trunk/workflow/plot_avalanche_tor.pdf')

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
