# recursiveHdf5
This is a header only c++11 library to have a simple interface to store data of nested classes recursively to hdf5. It is in particular useful in a case where one has a tree like structure of objects, where every node has a name and some data vectors that need to be stored into an hdf5 file. 

## Examples

The code of all examples can be found [here](./examples). They can be built by running `make`.

To print the data generated by the mock classes in [examples/data](./examples/data) recursively run
```bash
make
./build/printRecursively
```

To store the data generated by the mock classes in [examples/data](./examples/data) recursively to a new or existing hdf5 file with the name 'test.pb' run
```bash
make
./build/storeRecursivelyToHdf5 test.pb
```

To store the data and the configuration that was used to generate the date by the mock classes in [examples/data](./examples/data) recursively to a new or existing hdf5 file with the name 'test.pb' run
```bash
make
touch test.conf
./build/storeConfigAndDataToHdf5 test.pb test.conf
```
Note: this will use the default configuration as defined in [examples/storeRecursivelyToHdf5.cpp](./examples/storeRecursivelyToHdf5.cpp), to change the values of a parameter change the configuration file, you can see an example configuration file below.
```
rootDataConfig_ValueData1 = 9.9
rootDataConfig_LeafFactors = 12
rootDataConfig_LeafFactors = 13

```
(Notice, that by using this configuartion only create two leafs will be created below the root)


## Contributors
[Ralf Kohrt](https://github.com/rkohrt)

[Madis Ollikainen](https://github.com/madisollikainen)
