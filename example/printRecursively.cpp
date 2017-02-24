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

#include <iostream>

#include "rootData.hpp"
#include "storageWrapper.hpp"

using namespace RecHDF;

int main(int argc, const char* argv[] ) {
 
    std::shared_ptr<rootData> root = std::make_shared<rootData>();

    std::vector< named_instances_t > instances = root->children();

    for (std::vector<named_instances_t>::iterator v = instances.begin(); v != instances.end(); ++v)
    {
        std::cout << v->first << std::endl;
        std::vector< named_double_vect_t > d = v->second->data();
        for (std::vector<named_double_vect_t>::iterator i = d.begin(); i != d.end(); ++i)
        {
            std::cout << "   " << i->first << std::endl;
            for (std::vector<double>::iterator vals = i->second.begin(); vals != i->second.end(); ++vals)
            {
                std::cout << "      " << *vals << std::endl;
            }
        }
    }
    
}