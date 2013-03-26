leveldb_cross_compiler
======================
An example of an using cross_compiler_call to define an interface
that can be used from other compilers

In this case, use a version of leveldbwin modified for version 1.9 and then
write an interface. Implement the interface in an MSVC project

Now use the interface from g++.

Depends on cross_compiler_call repository

Note when using this in MSVC Milan (VC++ 2012, November CTP) you need
a using namespace leveldb_cc due to a bug in the interaction
of namespaces and variadic templates.

License

The interface files and implementation and test are distributed under

         Copyright John R. Bandela 2013.
 Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)

The LevelDB files are

// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

There are 2 leveldb sources here
leveldbwin is from
https://code.google.com/p/leveldbwin/
but has been modified by me for 1.9
use the Visual Studio project to  build

bitcoin_leveldb is from
https://github.com/bitcoin/bitcoin/tree/master/src/leveldb
To build the leveldb library on Windows use
msys
make sure you are in the leveldb directory
the execute the following
export TARGET_OS=NATIVE_WINDOWS
make

this will create libleveldb.a

Then to create the implementation of the interface using g++
change to leveldb_cross_compiler and type the following

C:\Users\jrb\Source\Repos\leveldb_cross_compiler>g++ -shared 
-o leveldb_cc_dll.dll leveldb_cc_dll.cpp msvc_project\leveldb_cc_dll\leveldb_cc_dll.def 
-Ibitcoin_leveldb\include bitcoin_leveldb\libleveldb.a -lshlwapi

Then copy the resulting dll to where you will be running the exe
You can build the exe with either g++ or msvc


