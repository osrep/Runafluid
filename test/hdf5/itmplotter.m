%% HDF5 reader for go from ETS database (euitm_28906_2017)
function itmplotter
	close all
	clc
	
	%% Initialisation
	%itm.folder = '.';
	itm.folder = '/u/maradi/public/aug_hdf5';%'~/svn/deep/trunk/go';
	itm.datastruc = 'euitm';
	itm.machine = 'aug';
	itm.shotnumber = '28906';
	itm.runnumber='666';%'3000';
	
	itm.timeflag = 200;%000;%200;	
	itm.time=1e-6;%0.0046 % not used
	%% Body
	itm.filepath = [itm.folder,'/',itm.datastruc,'_',itm.shotnumber,'_',itm.runnumber,'.hd5'];


	% timeslice plot
	if false	
		% temperature plot
		figure
		[b,a] = read_itm_data_simple('ti', itm);
		plot(a,b,'r-')
		hold on
		[b,a] = read_itm_data_simple('te', itm);
		plot(a,b,'--')
		title([upper(itm.machine),' \#',itm.shotnumber,' (',itm.runnumber,')'], 'fontsize', 16,'interpreter', 'latex')
		xlabel('normalised minor radius ($$\rho$$)', 'fontsize', 14,'interpreter', 'latex')
		ylabel('temperature [eV]', 'fontsize', 14,'interpreter', 'latex')    
		legend({'$T_i$','$T_e$'}, 'fontsize', 14,'interpreter', 'latex')

		% density plot
		figure
		[b,a] = read_itm_data_simple('ni', itm);
		plot(a,b,'r-')
		hold on
		[b,a] = read_itm_data_simple('ne', itm);
		plot(a,b,'--')
		title([upper(itm.machine),' \#',itm.shotnumber,' (',itm.runnumber,')'], 'fontsize', 16,'interpreter', 'latex')
		xlabel('normalised minor radius ($$\rho$$)', 'fontsize', 14,'interpreter', 'latex')
		ylabel('density [m$^{-3}$]', 'fontsize', 14,'interpreter', 'latex')    
		legend({'$n_i$','$n_e$'}, 'fontsize', 14,'interpreter', 'latex')
	
		% current plot
		figure
		[b,a] = read_itm_data_simple('runaway', itm);
		plot(a,b,'r-')
		hold on
		[b,a] = read_itm_data_simple('total_current', itm);
		plot(a,b,'--')	
		title([upper(itm.machine),' \#',itm.shotnumber,' (',itm.runnumber,')'], 'fontsize', 16,'interpreter', 'latex')
		xlabel('normalised minor radius ($$\rho$$)', 'fontsize', 14,'interpreter', 'latex')
		ylabel('current [A/m$^2$]', 'fontsize', 14,'interpreter', 'latex')    
		legend({'runaways','total'}, 'fontsize', 14,'interpreter', 'latex')
	
		% runaway current plot
		figure
		[b,a] = read_itm_data_simple('runaway', itm);
		plot(a,b,'r-')
		title([upper(itm.machine),' \#',itm.shotnumber,' (',itm.runnumber,')'], 'fontsize', 16,'interpreter', 'latex')
		xlabel('normalised minor radius ($$\rho$$)', 'fontsize', 14,'interpreter', 'latex')
		ylabel('runaway current [A/m$^2$]', 'fontsize', 14,'interpreter', 'latex')    
		legend({'runaways'}, 'fontsize', 14,'interpreter', 'latex')
	end
	
	% number of timeslices and rho
	rho_norm_mx = read_itm_rho_norm(itm);
	[N_time, N_rho] = size(rho_norm_mx);
	runaway_mx = zeros(N_time, N_rho);
	size(runaway_mx)
		
	% runaway current plot
	[b,a,t] = read_itm_data_all('runaway', itm);	
	figure
	contourf(t,a,b,100,'edgecolor','none')
	title([upper(itm.machine),' \#',itm.shotnumber,' (',itm.runnumber,') $j_\mathrm{runaway} \left[\frac{\mathrm{A}}{\mathrm{m}^3}\right]$'], 'fontsize', 16,'interpreter', 'latex')
    xlabel('time [s]', 'fontsize', 14,'interpreter', 'latex')    
	ylabel('normalised minor radius ($$\rho$$)', 'fontsize', 14,'interpreter', 'latex')
	colorbar
	saveas(gcf,[itm.datastruc,'_',itm.machine,'_',itm.shotnumber,'_',itm.runnumber,'_runaway.png'])
	
	% electron temperature plot
	[b,a,t] = read_itm_data_all('te', itm);
	figure
	contourf(t,a,log10(b),100,'edgecolor','none')
	title([upper(itm.machine),' \#',itm.shotnumber,' (',itm.runnumber,') $\mathrm{log_{10}}~ T_\mathrm{e} \left[\mathrm{eV}\right]$'], 'fontsize', 16,'interpreter', 'latex')
    xlabel('time [s]', 'fontsize', 14,'interpreter', 'latex')    
	ylabel('normalised minor radius ($$\rho$$)', 'fontsize', 14,'interpreter', 'latex')
	colorbar
	saveas(gcf,[itm.datastruc,'_',itm.machine,'_',itm.shotnumber,'_',itm.runnumber,'_te.png'])
	
	% electron density plot
	[b,a,t] = read_itm_data_all('ne', itm);
	figure
	contourf(t,a,log10(b),100,'edgecolor','none')
	title([upper(itm.machine),' \#',itm.shotnumber,' (',itm.runnumber,') $\mathrm{log_{10}}~ n_\mathrm{e} \left[\frac{1}{\mathrm{m}}\right]$'], 'fontsize', 16,'interpreter', 'latex')
    xlabel('time [s]', 'fontsize', 14,'interpreter', 'latex')    
	ylabel('normalised minor radius ($$\rho$$)', 'fontsize', 14,'interpreter', 'latex')
	colorbar
	saveas(gcf,[itm.datastruc,'_',itm.machine,'_',itm.shotnumber,'_',itm.runnumber,'_ne.png'])	
	
	time_cp = read_itm_time(itm);
    disp(['Last time: ',num2str(time_cp(end))])
end


% read data from CPOs
function [data, rho_norm] = read_itm_data_simple(data_name, itm)

	% time vector
	time_cp = read_itm_time(itm);
	
	% normalised minor radius vector 
	rho_norm_cp = read_itm_rho_norm(itm);
	
	% time index for timeslices
	index = size(rho_norm_cp,2)*[itm.timeflag-1 itm.timeflag]+[1 0];
	index = index(1):index(2);
	
	% slice of normalised minor radius
	rho_norm = rho_norm_cp(index);
	
	% find data in database and set data structure for reading
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
	
	% read data from CPO vector	
	data_cp = h5read(itm.filepath,data_path);
	
	% time slice 
	switch data_mode
		%coreprof data
		case 0
			data = data_cp(index);	
			
		%coresource data	
		case 1
			data = data_cp{itm.timeflag};
			
		end

end

%testing
function [data, rho_norm, time_mx] = read_itm_data_all(data_name, itm)	% time vector
	time_cp = read_itm_time(itm);
	
	% normalised minor radius vector 
	rho_norm_cp = read_itm_rho_norm(itm);	
	
	% find data in database and set data structure for reading
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
	
	% read data from CPO vector	
	data_cp = h5read(itm.filepath,data_path);
	
	% all data 	
	s = size(rho_norm_cp');
	
	%time matrix from time vector

	time_mx = (repmat((time_cp),1,s(1)))';	
	
	rho_norm = reshape(rho_norm_cp,s(1),s(2));
	
	switch data_mode
		%coreprof data
		case 0
			data = reshape(data_cp,s(1),s(2));	
			
		%coresource data	
		case 1
			data = cell2mat(data_cp);
			data = reshape(data,s(1),s(2));	
			
		end

end

% not working
function [data, rho_norm] = read_itm_data(data_name, itm)

	time_cp = read_itm_time(itm);
	rho_norm_cp = read_itm_rho_norm(itm);
	rho_norm_cp2 = reshape(rho_norm_cp,length(time_cp),[]);
	rho_norm = interp1(time_cp,rho_norm_cp2,itm.time);
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
	data = interp1(time_cp,data_cp2,itm.time);
	data = data(rho_index);

end


% read time vector
function time_cp = read_itm_time(itm)
	time_cp = h5read(itm.filepath,'/coreprof/time');
end

% read normalised minor radius vector
function rho_norm_cp = read_itm_rho_norm(itm)
	rho_norm_cp = h5read(itm.filepath,'/coreprof/rho_tor_norm');
end

