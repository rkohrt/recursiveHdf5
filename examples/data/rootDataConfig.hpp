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

#ifndef ROOT_DATA_HEADER
#define ROOT_DATA_HEADER

#include "storableWrapper.hpp"
#include "dataBuilder.hpp"
#include "leafData.hpp"
#include "config.hpp"

namespace RecHDF
{
    class rootDataConfig
        // needed so that we can get a shared pointer of this
        : public std::enable_shared_from_this<rootDataConfig>   
    {

    public:
        rootDataConfig(Config<double, double> & config, std::string name = "rootDataConfig" )
            :   name(name),
                data1(100, config.get<double>(name+"_ValueData1")),
                data2(5, config.get<double>(name+"_ValueData2"))
            {
                std::vector<double> LeafFactors = config.get<std::vector<double>>(name+"_LeafFactors");
                for (unsigned i = 0; i < LeafFactors.size(); ++i) {
                    leafs.push_back(std::make_shared<leafData>((i+1)*LeafFactors[i]));
                }
            }

        // -------------------------------------------- //
        // --- provide storageWrapperBase interface --- //
        // -------------------------------------------- // 


        // Data vectors gathering and outputing 
        virtual std::vector< named_double_vect_t > data() const
        {
            dataVectorBuilder dataV;
            dataV.add("data1",data1);
            dataV.add("data2",data2);
            return dataV.get(); 
        }

        // Getter to get all children recursivly
        virtual std::vector< named_instances_t > children() const
        {
            instanceVectorBuilder inst(name, std::make_shared<storableWrapper >(shared_from_this()));
            std::vector< named_instances_t > instances = inst.get();
            for ( uint i=0; i < leafs.size(); ++i)
            {
                std::vector<named_instances_t> tmp = leafs[i]->children();
                std::move(tmp.begin(), tmp.end(), std::back_inserter(instances));
                
            }
            return instances;
        }

    protected:

        // name
        std::string name;

        // data
        std::vector<double> data1;
        std::vector<double> data2;

        std::vector<std::shared_ptr<leafData>> leafs;
    };
}
#endif //ROOT_DATA_HEADER