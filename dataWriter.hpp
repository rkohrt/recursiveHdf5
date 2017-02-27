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

#ifndef DATA_WRITER
#define DATA_WRITER

#include <memory>

#include "storableWrapper.hpp"
#include "config.hpp"

namespace RecHDF
{

    template < typename float_t, typename int_t >
    class dataWriter
    {
    public:
        virtual void store( Config<float_t, int_t>& config, std::shared_ptr<storableWrapper> C) = 0;
        virtual void store( std::shared_ptr<storableWrapper> C) = 0;
    };
}

#endif //DATA_WRITER
