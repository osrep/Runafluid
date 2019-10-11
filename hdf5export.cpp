#include <iostream>
#include <string>
#include "H5Cpp.h"
#include "hdf5export.h"
using namespace H5;
using namespace std;


int init_hdf5_file(H5std_string FILE_NAME, int cols, string* dataset_name_list, int dataset_name_length){
	try{
		Exception::dontPrint();

		H5File file;
		file.openFile(FILE_NAME, H5F_ACC_RDWR);
		file.close();
		cout << "  [Runaway Fluid] \tHDF5 output file: "  << FILE_NAME << endl;

		return 0;
	}catch(FileIException error){
		try{
			cout << "  [Runaway Fluid] \tHDF5 output file does not exist: "  << FILE_NAME << endl;
			hsize_t size_empty[2] = {0,0};
			hsize_t size_max[2] = {H5S_UNLIMITED, H5S_UNLIMITED};

			H5File file_out(FILE_NAME, H5F_ACC_TRUNC);
			hsize_t size_ext[2] = {1, (hsize_t)cols};
			DataSpace *dataspace_out = new DataSpace (2, size_empty, size_max);
			DSetCreatPropList prop_out;
			prop_out.setChunk(2, size_ext);

			for (int i=0;i<dataset_name_length;i++){		
				DataSet *dataset_out = new DataSet(file_out.createDataSet(dataset_name_list[i], PredType::NATIVE_DOUBLE, *dataspace_out, prop_out) );
				delete dataset_out;
			}

			prop_out.close();
			delete dataspace_out;
			file_out.close();

			cout << "  [Runaway Fluid] \tHDF5 output file created: "  << FILE_NAME << endl;

			return 0;
		}catch(...){	

			cout << "  [Runaway Fluid] \tCould not create HDF5 output file: "  << FILE_NAME << endl;
			return -1;
		}

	}catch(...){	

		cout << "  [Runaway Fluid] \tCould not initialise HDF5 output file: "  << FILE_NAME << endl;
		return -1;
	}
}

int write_data_to_hdf5(H5std_string FILE_NAME, H5std_string DATASETNAME, double dataext_scalar){
	double dataext[1] = {dataext_scalar};
	return write_data_to_hdf5(FILE_NAME, DATASETNAME, dataext, 1);
}


int write_data_to_hdf5(H5std_string FILE_NAME, H5std_string DATASETNAME,  blitz::Array<double,1> dataext_blitz){

	int cols =  dataext_blitz.rows();
	double dataext[cols];
	for (int i=0;i<cols;i++){
		dataext[i] = dataext_blitz(i);
	}
	return write_data_to_hdf5(FILE_NAME, DATASETNAME, dataext, cols);

}

int write_data_to_hdf5(H5std_string FILE_NAME, H5std_string DATASETNAME, double* dataext, int cols){
	try{
		Exception::dontPrint();

		H5File file;
		DataSet *dataset;
		DataSpace *filespace;
		DataSpace *memspace;
		DSetCreatPropList prop;

		hsize_t size_in[2];		hsize_t size_out[2];
		hsize_t offset[2];		hsize_t size_ext[2] = {1, (hsize_t)cols};

		file.openFile(FILE_NAME, H5F_ACC_RDWR);
		dataset = new DataSet(file.openDataSet( DATASETNAME));
		filespace = new DataSpace(dataset->getSpace());
		prop = dataset->getCreatePlist();
		herr_t status_n = filespace->getSimpleExtentDims(size_in);

		memspace = new DataSpace(2, size_in);
		double data_in[size_in[0]][size_in[1]];
		dataset->read(data_in, PredType::NATIVE_DOUBLE, *memspace, *filespace);	

		size_out[0] = size_in[0] + size_ext[0]; size_out[1] = size_ext[1];
		offset[0] = size_in[0]; offset[1] = 0;

		dataset->extend(size_out);
		filespace = new DataSpace(dataset->getSpace());
		filespace->selectHyperslab(H5S_SELECT_SET, size_ext, offset);
		memspace = new DataSpace(2, size_ext); 
		dataset->write(dataext, PredType::NATIVE_DOUBLE, *memspace, *filespace);

		prop.close();
		delete filespace;
		delete memspace;
		delete dataset;

		file.close();

	}catch(FileIException error){
		cout << "  [Runaway Fluid] \tHDF5 error in H5File operations"<< endl;
		error.printError();
		return -1;	

	}catch(DataSetIException error)	{
		cout << "  [Runaway Fluid] \tHDF5 error in DataSet operations"<< endl;
		error.printError();
		return -1;	

	}catch(DataSpaceIException error) {
		cout << "  [Runaway Fluid] \tHDF5 error in DataSpace operations"<< endl;
		error.printError();
		return -1;
	}catch(...) {
		cout << "  [Runaway Fluid] \tHDF5 general error" << endl;
		return -1;
	}

	return 0; 
}
