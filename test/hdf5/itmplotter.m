%% HDF5 reader for go from ETS database (euitm_28906_2017)
function itmplotter
	close all
	clc
	
	%itm.folder = '.';
	itm.folder = '~/svn/deep/trunk/go';
	itm.datastruc = 'euitm';
	itm.machine = 'aug';
	itm.shotnumber = '28906';
	itm.runnumber='1037';

	itm.filepath = [itm.folder,'/',itm.datastruc,'_',itm.shotnumber,'_',itm.runnumber,'.hd5'];


	timeflag = 200;%000;%200;	
	t=1e-6;%0.0046
	
	
	figure
	[b,a] = read_itm_data_simple('ti', timeflag, itm);
	plot(a,b,'r-')
	hold on
	[b,a] = read_itm_data_simple('te', timeflag, itm);
	plot(a,b,'--')
	title([upper(itm.machine),' #',itm.shotnumber,' (',itm.runnumber,')'], 'fontsize', 16,'interpreter', 'latex')
    xlabel('normalised minor radius ($$\rho$$)', 'fontsize', 14,'interpreter', 'latex')
    ylabel('temperature [eV]', 'fontsize', 14,'interpreter', 'latex')    
	legend({'$T_i$','$T_e$'}, 'fontsize', 14,'interpreter', 'latex')

	
	figure
	[b,a] = read_itm_data_simple('ni', timeflag, itm);
	plot(a,b,'r-')
	hold on
	[b,a] = read_itm_data_simple('ne', timeflag, itm);
	plot(a,b,'--')
	title([upper(itm.machine),' #',itm.shotnumber,' (',itm.runnumber,')'], 'fontsize', 16,'interpreter', 'latex')
    xlabel('normalised minor radius ($$\rho$$)', 'fontsize', 14,'interpreter', 'latex')
    ylabel('density [m$^{-3}$]', 'fontsize', 14,'interpreter', 'latex')    
	legend({'$n_i$','$n_e$'}, 'fontsize', 14,'interpreter', 'latex')
	
	figure
	[b,a] = read_itm_data_simple('runaway', timeflag, itm);
	plot(a,b,'r-')
	hold on
	[b,a] = read_itm_data_simple('total_current', timeflag, itm);
	plot(a,b,'--')	
	title([upper(itm.machine),' #',itm.shotnumber,' (',itm.runnumber,')'], 'fontsize', 16,'interpreter', 'latex')
    xlabel('normalised minor radius ($$\rho$$)', 'fontsize', 14,'interpreter', 'latex')
    ylabel('current [A/m$^2$]', 'fontsize', 14,'interpreter', 'latex')    
	legend({'runaways','total'}, 'fontsize', 14,'interpreter', 'latex')
	
	figure
	[b,a] = read_itm_data_simple('runaway', timeflag, itm);
	plot(a,b,'r-')
	title([upper(itm.machine),' #',itm.shotnumber,' (',itm.runnumber,')'], 'fontsize', 16,'interpreter', 'latex')
    xlabel('normalised minor radius ($$\rho$$)', 'fontsize', 14,'interpreter', 'latex')
    ylabel('runaway current [A/m$^2$]', 'fontsize', 14,'interpreter', 'latex')    
	legend({'runaways'}, 'fontsize', 14,'interpreter', 'latex')
	
end



function [data, rho_norm] = read_itm_data_simple(data_name, timeflag, itm)

	time_cp = h5read(itm.filepath,'/coreprof/time');
	rho_norm_cp = h5read(itm.filepath,'/coreprof/rho_tor_norm');
	
	index = size(rho_norm_cp,2)*[timeflag-1 timeflag]+[1 0];
	index = index(1):index(2);
	rho_norm = rho_norm_cp(index);
	
	switch data_name
		case 'te'
			data_path = '/coreprof/te/value'; data_mode = 0;
		case 'ti'
			data_path = '/coreprof/ti/value'; data_mode = 0;
		case 'ne'
			data_path = '/coreprof/ne/value'; data_mode = 0;
		case 'ni'
			data_path = '/coreprof/ni/value'; data_mode = 0;
		case 'runaway'
			data_path = '/coresource/values/timed/3/j'; data_mode = 1;
		case 'total_current'
			data_path = '/coresource/values/timed/0/j';	data_mode = 1;
		end
		
	data_cp = h5read(itm.filepath,data_path);
	switch data_mode
		case 0
			data = data_cp(index);	
		case 1
			data = data_cp{timeflag};
		end

end


function [data, rho_norm] = read_itm_data(data_name, t, itm)

	time_cp = h5read(itm.filepath,'/coreprof/time');
	rho_norm_cp = h5read(itm.filepath,'/coreprof/rho_tor_norm');
	rho_norm_cp2 = reshape(rho_norm_cp,length(time_cp),[]);
	rho_norm = interp1(time_cp,rho_norm_cp2,t);
	[rho_norm, rho_index] = sort(rho_norm);
	
	switch data_name
		case 'te'
			data_path = '/coreprof/te/value';
		case 'ti'
			data_path = '/coreprof/ti/value';
		case 'ne'
			data_path = '/coreprof/ne/value';
		case 'ni'
			data_path = '/coreprof/ni/value';
		case 'runaway'
			data_path = '/coresource/values/timed/3/j';	
		case 'total_current'
			data_path = '/coresource/values/timed/0/j';	
		end
		
	data_cp = h5read(itm.filepath,data_path);
	data_cp2 = reshape(data_cp,length(time_cp),[]);	
	data = interp1(time_cp,data_cp2,t);
	data = data(rho_index);

end

