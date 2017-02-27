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

#include "rootData.hpp"
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
 
    //generate data
    std::shared_ptr<rootData> root = std::make_shared<rootData>();

    std::vector< std::string > overwrite = {""}; //can be used to programmatically overwrite given parameters
    std::vector< std::tuple<std::string, std::string, double > > confScalarPar = { std::make_tuple("Scalar_1", "This is an example config variable of type double with a Description", 1.5), 
                                                                                  std::make_tuple("Scalar_2", "This is an other example config variable of type double with a Description", 2.5) };
    std::vector< std::tuple<std::string, std::string, std::vector<double> > > confVectorPar = { std::make_tuple("Vector_1", "This is an example config variable of type std::vector<double> with a Description", std::vector<double> { 100.0, 50.0 ,11.1 }) };
    std::vector< std::tuple<std::string, std::string, std::string > > confStringPar;  //String parameteres are not supported by the hdf5 beckend yet
    Config<double, double> config(configFile, confScalarPar, confVectorPar, confStringPar, overwrite);

    if(!Hdf5Helper::doesGroupExist("recursiveOutput",fileId)) {
        dataWriterHdf5<double, double> d(fileId, "recursiveOutput");
        d.store(config, std::make_shared<storableWrapper >( root ));
    }
    H5Fclose(fileId);
}