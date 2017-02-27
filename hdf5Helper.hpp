// Copyright 2017 Ralf Kohrt

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef HDF5_HELPER
#define HDF5_HELPER

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>

#include <hdf5.h>

#include "getTypeName.hpp"

namespace RecHDF
{
    namespace Hdf5Helper
    {
        //writes an attribute of name 'name' and value 'val' to the hdf5 group (or file) 'id'
        template<typename T>
        void writeAttribute(hid_t const & id, std::string const & name, T const & val) {
            if(H5Aexists(id, name.c_str())) {
                throw std::runtime_error("attribute " + name + " already exists");
                return;
            }
            
            hsize_t dims[] = {1};
            auto dataspace_id = H5Screate_simple(1, dims, NULL);
            hid_t attribute_id;
            hid_t type = H5T_NATIVE_INT;
            if(std::is_same<T, double>::value)
                type = H5T_NATIVE_DOUBLE;
            else if(std::is_same<T, int>::value)
                type = H5T_NATIVE_INT;
            else if(std::is_same<T, long long>::value)
                type = H5T_NATIVE_LLONG;
            else if(std::is_same<T, std::size_t>::value)
                type = H5T_NATIVE_ULLONG;
            else {
                throw std::runtime_error("unkown variable type <"+getTypeName<T>()+"> to store as an Attribute");
                return;
            }
            attribute_id = H5Acreate2 (id, name.c_str(), type, dataspace_id, H5P_DEFAULT, H5P_DEFAULT);
            H5Awrite(attribute_id, type, &val);
            H5Sclose(dataspace_id);
            H5Aclose(attribute_id);
        }

        template<>
        void writeAttribute<std::vector<double>>(hid_t const & id, std::string const & name, std::vector<double> const & val) {
            if(H5Aexists(id, name.c_str())) {
                throw std::runtime_error("attribute " + name + " already exists");
                return;
            }
            
            hsize_t dims[] = {val.size()};
            auto dataspace_id = H5Screate_simple(1, dims, NULL);
            hid_t attribute_id;
            hid_t type = H5T_NATIVE_DOUBLE;
            attribute_id = H5Acreate2 (id, name.c_str(), type, dataspace_id, H5P_DEFAULT, H5P_DEFAULT);
            H5Awrite(attribute_id, type, val.data());
            H5Sclose(dataspace_id);
            H5Aclose(attribute_id);
        }

        // writes a vector of data to the hdf5 group 'group' as a dataset with the name 'name' 
        template<typename T>
        hid_t writeDataset( hid_t const & group
                           , std::string const & name
                           , std::vector<T>& data) {
            size_t pos = name.find("/");
            if(pos != std::string::npos) {
                hid_t tmp;
                if(H5Lexists(group, name.substr(0,pos).c_str(), H5P_DEFAULT))
                    tmp = H5Gopen(group, name.substr(0,pos).c_str(), H5P_DEFAULT); 
                else
                    tmp = H5Gcreate(group, name.substr(0,pos).c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
                std::string tname = name;
                return writeDataset(tmp,tname.erase(0,pos+1),data);
            }
            
            if(H5Lexists(group, name.c_str(), H5P_DEFAULT)) {
                throw std::runtime_error("dataset " + name + " already exists");
                return 0;
            }
            
            hsize_t dims[] = {data.size()};            
            hid_t dataSpaceId = H5Screate_simple(1, dims, nullptr);
            hid_t type = H5T_NATIVE_INT;
            if(std::is_same<T, double>::value)
                type = H5T_NATIVE_DOUBLE;
            else if(std::is_same<T, int>::value)
                type = H5T_NATIVE_INT;
            else if(std::is_same<T, long long>::value)
                type = H5T_NATIVE_LLONG;
            else if(std::is_same<T, std::size_t>::value)
                type = H5T_NATIVE_ULLONG;
            else {
                throw std::runtime_error("unkown variable type <"+getTypeName<T>()+"> to store as an Dataset");
                return 0;
            }
            hid_t dataSetId = H5Dcreate2(group, name.c_str(), type, dataSpaceId, 
                                  H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
            H5Dwrite(dataSetId, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, data.data());
            H5Sclose(dataSpaceId);
            return dataSetId;
        }

        // recursively opens an hdf5 group relative to the parent variable
        hid_t openGroup(std::string name, hid_t parent, bool showError = true) {
            size_t pos = name.find("/");
            hid_t tmp;
            if(H5Lexists(parent, name.substr(0,pos).c_str(), H5P_DEFAULT)) {
                tmp = H5Gopen(parent, name.substr(0,pos).c_str(), H5P_DEFAULT); 
                if(pos==std::string::npos && showError)
                    std::cerr << "dataWriterHdf5 warning: this file already contains a group of name " << name << std::endl;
            } else
                tmp = H5Gcreate(parent, name.substr(0,pos).c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
            if(pos!=std::string::npos)
                return openGroup(name.erase(0,pos+1),tmp);
            else
                return tmp;
        }

        //Opens (if file exists) or creates (if it does not) a hdf5 file to read/write and returns  
        hid_t openFile(boost::filesystem::path& file) {
            std::ifstream f(file.c_str());
            bool exists = f.good();
            f.close();
            if(exists)
                return H5Fopen(file.c_str(), H5F_ACC_RDWR, H5P_DEFAULT); //open existing file for reading and writing
            else
                return H5Fcreate(file.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT); 
        }

    };
}

#endif //HDF5_HELPER