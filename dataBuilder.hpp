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

#ifndef DATA_BUILDER_HEADER
#define DATA_BUILDER_HEADER

#include <vector>
#include <string>
#include <memory>
#include <utility>

#include "storableWrapper.hpp"

namespace RecHDF
{
    template<typename T>
    class dataBuilder{

        public:
            dataBuilder() {}
            dataBuilder(std::string name, T value) {
                add(name, value);
            }

            void add(std::string name, T value) {
                std::pair<std::string, T> tmp(name, value);
                data.push_back(tmp);
            }

            std::vector< std::pair<std::string, T> > get() {
                return data;
            }

        protected:
            std::vector< std::pair<std::string, T> > data;
    };

    typedef dataBuilder<std::vector<double>> dataVectorBuilder;
    typedef dataBuilder<std::shared_ptr<storableWrapper>> instanceVectorBuilder;
}
#endif //DATA_BUILDER_HEADER