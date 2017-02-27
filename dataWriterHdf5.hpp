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

#ifndef DATA_WRITER_HDF5
#define DATA_WRITER_HDF5

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>

#include <hdf5.h>

#include "dataWriter.hpp"
#include "storableWrapper.hpp"
#include "config.hpp"
#include "hdf5Helper.hpp"


namespace RecHDF
{

    template < typename scalar_t, typename vector_t >
    class dataWriterHdf5: public dataWriter<scalar_t, vector_t>
    {
    protected:
        boost::filesystem::path file;
        std::string  GroupName;
        hid_t fileId;
        hid_t groupId;
        bool fileIdControlledExternally = false;
        
        void openFile() {
            if(!fileIdControlledExternally) {
                fileId = Hdf5Helper::openFile(file);
            }
            groupId = Hdf5Helper::openGroup(GroupName, fileId);
        }

        void closeFile() {
            H5Gclose(groupId);
            if(!fileIdControlledExternally)
                H5Fclose(fileId);
        }

        virtual void store(hid_t group, std::shared_ptr<storableWrapper> C) {
            std::vector<named_instances_t> instances = C->children();
            for (uint j = 0; j < instances.size(); ++j) {
                named_instances_t inst = instances[j];
                uint i = 0;
                while(H5Lexists(group, (inst.first+"_"+std::to_string(i)).c_str(), H5P_DEFAULT)) 
                    ++i;
                std::cout << (inst.first+"_"+std::to_string(i)) << std::endl;
                bool store = true;
                if(i > 0) {
                    for (uint k = 0; k < j; ++k) {
                        if(instances[k].first == inst.first && instances[k].second == inst.second) {
                            store = false;
                            break;
                        }
                    }
                }
                if(store) {
                    hid_t group2 = H5Gcreate(group, (inst.first+"_"+std::to_string(i)).c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
                    std::vector< named_double_vect_t > data = inst.second->data();
                    for(auto& d : data)
                        H5Dclose(Hdf5Helper::writeDataset(group2,d.first,d.second));
                    inst.second->saveData(group2);
                    H5Gclose(group2);
                }
            }
        }

    public:
        dataWriterHdf5(hid_t fileId, std::string dataSetName): GroupName(dataSetName), fileId(fileId), fileIdControlledExternally(true) {
            openFile();
        }
        dataWriterHdf5(boost::filesystem::path file, std::string dataSetName): file(file), GroupName(dataSetName) {
            openFile();
        }
        ~dataWriterHdf5() {
            closeFile();
        }
        virtual void store( Config<scalar_t, vector_t>& config, std::shared_ptr<storableWrapper> C) override {
            store(groupId, config);    
            store(groupId, C);            
        }
        virtual void store( std::shared_ptr<storableWrapper> C) override {
            store(groupId, C);            
        }

        static void store(hid_t group, Config<scalar_t, vector_t>& config) {
            typedef boost::program_options::variables_map::const_iterator iter;
            boost::program_options::variables_map& m = config.getMap();

            for ( iter i = m.begin(); i != m.end(); ++i )
            {
                std::string attrName = "config_"+i->first;
                {
                    try {
                        Hdf5Helper::writeAttribute<std::vector<vector_t>>(group,attrName,i->second.as<std::vector<vector_t>>()); 
                        continue;  
                    } catch(...) {/* do nothing */ } 
                }
                try { 
                    Hdf5Helper::writeAttribute<scalar_t>(group,attrName,i->second.as<scalar_t>()); 
                    continue;   
                } catch(...) {/* do nothing */ }
                try { 
                    Hdf5Helper::writeAttribute<double>(group,attrName,i->second.as<double>()); 
                    continue;   
                } catch(...) {/* do nothing */ }
                try { 
                    Hdf5Helper::writeAttribute<int>(group,attrName,i->second.as<int>());
                    continue;
                } catch(...) {/* do nothing */ }
                try { 
                    Hdf5Helper::writeAttribute<std::size_t>(group,attrName,i->second.as<std::size_t>());
                    continue;
                } catch(...) {/* do nothing */ }
                try { 
                    Hdf5Helper::writeAttribute<long long>(group,attrName,i->second.as<long long>());
                    continue;
                } catch(...) {/* do nothing */ }
                try { 
                    std::string s = i->second.as<std::string>();
                    continue;
                } catch(...) {/* do nothing */ }
                throw std::runtime_error("unknown config variable type");
            }
        }


    };

}

#endif //DATA_WRITER_HDF5
