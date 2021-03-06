// Copyright 2017 Madis Ollikainen and Ralf Kohrt

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// 
// Class for getting the program configurations
// from the configuration file and storing them. 


#ifndef CONFIG_HPP
#define CONFIG_HPP



// std include 
#include <cassert>
#include <vector>
#include <string>
#include <utility>
#include <tuple>
#include <iostream>
#include <fstream>


// boost include
#include <boost/program_options.hpp>
#include <boost/filesystem/operations.hpp>


namespace RecHDF
{

    // A class which will hold the configuration as well as read it 
    template < typename scalar_t, typename vector_t >
    class Config
    {
    public: 

        // ----------------------- //
        // ----- Constructor ----- // 
        // ----------------------- //


        // input Vectors of tuples for specifing the parameter names, descriptions and default values
        //
        //  1) 1st entry == string for parameter name
        //  2) 2nd entry == string for parameter description
        //  3) 3rd entry == numeric for default value

        Config(  boost::filesystem::path cfile, 
                    std::vector< std::tuple<std::string, std::string, scalar_t > > scalars,
                    std::vector< std::tuple<std::string, std::string, std::vector<vector_t> > > vectors, 
                    std::vector< std::tuple<std::string, std::string, std::string > > strings, 
                    std::vector< std::string >& overwrite_values)
            : file_(cfile)
        {
            // Wrap everything into a try-catch block
            try
            {

            // Generate the structure for the configVM
            boost::program_options::options_description desc("Config");
            
            // Loop over the scalar_t variables
            for( uint i=0; i < scalars.size(); ++i)
            {
                desc_.add_options()
                    (   (std::get<0>(scalars[i])).c_str(),
                        boost::program_options::value<scalar_t>()->default_value((std::get<2>(scalars[i]))),
                        (std::get<1>(scalars[i])).c_str()
                    );
            }
            
            // Loop over the vector_t-vector variables
            for( uint i=0; i < vectors.size(); ++i)
            {
                desc_.add_options()
                    (   (std::get<0>(vectors[i])).c_str(),
                        boost::program_options::value<std::vector<vector_t>>()->default_value((std::get<2>(vectors[i])), std::to_string(std::get<2>(vectors[i])[0])),
                        (std::get<1>(vectors[i])).c_str()
                    );
            }

            // Loop over the string variables
            for( uint i=0; i < strings.size(); ++i)
            {
                desc_.add_options()
                    (   (std::get<0>(strings[i])).c_str(), 
                        boost::program_options::value<std::string>()->default_value((std::get<2>(strings[i]))), 
                        (std::get<1>(strings[i])).c_str()
                    );

            }

            boost::program_options::options_description visible("Allowed options");
            visible.add(desc_);

            //add possibility to set seed as a paramter to this class - and not from the config file
            if(overwrite_values.size() > 1) {
                store(boost::program_options::command_line_parser(overwrite_values).options(desc_).run(), par_); 
            }

            // Open the configuration file
            std::ifstream ifs(file_.string());

            if (!ifs)
            {
                std::cout << "ERROR : Can not open config file: " << file_ << std::endl;
                //return 0; // ? Should I add terminate() ? 
            }
            else
            {
                store(parse_config_file(ifs, desc_), par_);
                notify(par_);
            }
     
            } // end try

            catch(std::exception& e)
            {
                std::cout << e.what() << "\n";
                //return 1; // ? Should I add terminate() ? 
            }

        }   // end constructor


        // method for checkign is a variable is in par_ variable map
        bool count(std::string name)
        {
            if ( par_.count(name) )
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        // Templated method for getting a variable from the par_ variable map
        template< typename T > 
        bool get(T& var, std::string name)
        {
            if ( par_.count(name) )
            {
                var = par_[name].as<T>();
                return true;
            }
            else
            {
                return false;
            }
        }

        // Templated get for getting a variable from the par_ variable map
        template< typename T > 
        T get(std::string name )
        {
            if(!par_.count(name))
            {
                std::cout << "\n" << "ERROR: Asking for a non existant parameter : "
                            << name << "\n" << std::endl;
                assert( par_.count(name));
            }
             
            
            return par_[name].as<T>();
        }

        boost::program_options::variables_map& getMap()
        {
            return par_;
        }   


    private:

        // Configuration file 
        boost::filesystem::path file_;
        
        // The name-value variable map for the configuration
        boost::program_options::variables_map par_;

        // The options description sturcture 
        boost::program_options::options_description desc_;







    }; // end class Config

}



#endif // CONFIG_HPP


