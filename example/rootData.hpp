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

#include "storageWrapper.hpp"
#include "dataBuilder.hpp"
#include "leafData.hpp"

namespace RecHDF
{
    class rootData
        // needed so that we can get a shared pointer of this
        : public std::enable_shared_from_this<rootData>   
    {

    public:
        rootData( std::string name = "rootData" )
            :   name(name),
                data1(100, 42.24),
                data2(5, -1.042)
            {
                for (int i = 0; i < 5; ++i) {
                    leafs.push_back(std::make_shared<leafData>(i*2.1));
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