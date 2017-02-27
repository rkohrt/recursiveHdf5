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

#include <hdf5.h>
#include <boost/filesystem/operations.hpp>
#include <string>
#include <iostream>

#include "rootDataConfig.hpp"
#include "storableWrapper.hpp"
#include "hdf5Helper.hpp"
#include "dataWriterHdf5.hpp"

using namespace RecHDF;

int main(int argc, const char* argv[] ) {
    
    if(argc < 3) {
        std::cout << "usage: " << argv[0] << " [output-filename] [config-file] \n";
        return 1;
    } 
    boost::filesystem::path p(argv[1]);
    boost::filesystem::path configFile(argv[2]);
    if(boost::filesystem::exists(p)){
        std::cout << "file " << argv[1] << " does already exist" << std::endl;
    }
    hid_t fileId = Hdf5Helper::openFile(p);

    std::vector< std::string > overwrite = {""}; //can be used to programmatically overwrite given parameters
    std::vector< std::tuple<std::string, std::string, double > > confScalarPar = { std::make_tuple("rootDataConfig_ValueData1", "This is the value used for the vector data1", 1.5), 
                                                                                  std::make_tuple("rootDataConfig_ValueData2", "This is the value used for the vector data1", 2.5) };
    std::vector< std::tuple<std::string, std::string, std::vector<double> > > confVectorPar = { std::make_tuple("rootDataConfig_LeafFactors", "This is the factor used by rootDataConfig to determine the value of its leafs", std::vector<double> { 100.1, 50.5 ,11.11, -3.42 }) };
    std::vector< std::tuple<std::string, std::string, std::string > > confStringPar;  //String parameteres are not supported by the hdf5 beckend yet
    Config<double, double> config(configFile, confScalarPar, confVectorPar, confStringPar, overwrite);

    //generate data
    std::shared_ptr<rootDataConfig> root = std::make_shared<rootDataConfig>(config);

    if(!Hdf5Helper::doesGroupExist("recursiveOutput",fileId)) {
        dataWriterHdf5<double, double> d(fileId, "recursiveOutput");
        d.store(config, std::make_shared<storableWrapper >( root ));
    }
    H5Fclose(fileId);
}