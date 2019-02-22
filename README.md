![forest](forest.png)

[![Build Status](https://ci.appveyor.com/api/projects/status/8e5jutnq0a8b458f/branch/master?svg=true)](https://ci.appveyor.com/project/xorz57/forest/branch/master)
[![Build Status](https://travis-ci.org/xorz57/forest.svg?branch=master)](https://travis-ci.org/xorz57/forest)
[![CodeFactor](https://www.codefactor.io/repository/github/xorz57/forest/badge/master)](https://www.codefactor.io/repository/github/xorz57/forest/overview/master)

## How to Install

#### Using [vcpkg](https://github.com/Microsoft/vcpkg) (CMake)
- `vcpkg install forest --head`

#### Using [buckaroo](https://github.com/LoopPerfect/buckaroo) (Buck)
- `buckaroo install xorz57/forest`

#### Using [CMake](https://cmake.org/)

```sh
$ mkdir build
$ cmake ..
$ sudo make install
```

## How to Build Tests, Examples and Benchmarks

#### Using [CMake](https://cmake.org/)

```sh
$ mkdir build
$ cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON -DBUILD_BENCHMARKS=ON
$ make -j2
```

#### Using [Buck](https://buckbuild.com/)

```sh
$ buck run test:AVLTree
$ buck run test:BinarySearchTree
$ buck run test:KDTree
$ buck run test:QuadTree
```

```sh
$ buck run example:AVLTree
$ buck run example:BinarySearchTree
$ buck run example:KDTree
$ buck run example:QuadTree
```

## Credits

- Icon made by Freepik from www.flaticon.com
