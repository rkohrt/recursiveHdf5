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

#ifndef LEAF_DATA_HEADER
#define LEAF_DATA_HEADER

#include "storableWrapper.hpp"
#include "dataBuilder.hpp"

namespace RecHDF
{
    class leafData
        // needed so that we can get a shared pointer of this
        : public std::enable_shared_from_this<leafData>   
    {

    public:
        leafData(double val, std::string name = "leafData" )
            :   name(name),
                data_(5, val)
            {

            }

        // -------------------------------------------- //
        // --- provide storageWrapperBase interface --- //
        // -------------------------------------------- // 


        // Data vectors gathering and outputing 
        virtual std::vector< named_double_vect_t > data() const
        {
            dataVectorBuilder dataV("data",data_);
            return dataV.get(); 
        }

        // Getter to get all children recursivly
        virtual std::vector< named_instances_t > children() const
        {
            instanceVectorBuilder instances(name, std::make_shared<storableWrapper >(shared_from_this()));
            return instances.get();
        }

    protected:

        // name
        std::string name;

        // data
        std::vector<double> data_;
    };
}
#endif //LEAF_DATA_HEADER