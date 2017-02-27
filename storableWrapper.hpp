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

#ifndef STORABLE_WRAPPER_HEADER
#define STORABLE_WRAPPER_HEADER

#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <tuple>

#include <hdf5.h>

namespace RecHDF
{
    struct storableWrapper;
    typedef std::pair<std::string, double> named_double_t;
    typedef std::pair<std::string, std::vector<double> > named_double_vect_t;
    typedef std::pair<std::string, std::shared_ptr<storableWrapper> > named_instances_t;

    //=================== concept ===================
    struct storableWrapperBase {
        virtual std::vector< named_double_vect_t > data() const = 0;
        virtual std::vector< named_instances_t > children()  = 0;
        virtual void saveData(hid_t parent) = 0;   //classes fullfilling this concept don't need to have this function - see implementation of storableWrapperDerived
    };

    //trait to check if T has function saveData
    template <typename T>
    struct has_saveData {
    private:
        template <typename U>
        static decltype(std::declval<U>().saveData(H5P_DEFAULT), std::true_type()) test(int);
        template <typename>
        static std::false_type test(...);
    public:
        typedef decltype(test<T>(0)) type;
        enum { value = type::value };
    };


    template<typename T>
    struct storableWrapperDerived: public storableWrapperBase {
        storableWrapperDerived(std::shared_ptr<T>  t): b(t) {}
        std::vector< named_double_vect_t > data() const override 
        {
            return b->data();
        }
        std::vector< named_instances_t > children()  override
        {
            return b->children();
        }   
        
        void saveData(hid_t parent) override { saveData_<T>(parent); }

    private:
        template<typename U> 
        typename std::enable_if<has_saveData<U>::value>::type saveData_(hid_t parent){
            return b->saveData(parent);
        }              

        template<typename U> 
        typename std::enable_if<!has_saveData<U>::value>::type saveData_(hid_t parent) { /* do nothing */ }              

        std::shared_ptr<T> b;
    };

    struct storableWrapper {
        storableWrapper(): ptr() {}
        template<typename T>
        storableWrapper(std::shared_ptr<T>  t): ptr(new storableWrapperDerived<T>(t)) {}
        std::vector< named_double_vect_t > data() const
        {
            return ptr->data();
        }
        std::vector< named_instances_t > children() 
        {
            return ptr->children();
        }    
        void saveData(hid_t parent) 
        {
            return ptr->saveData(parent);
        }        
        
        std::shared_ptr<storableWrapperBase> ptr;
    };


}
#endif //STORABLE_WRAPPER_HEADER