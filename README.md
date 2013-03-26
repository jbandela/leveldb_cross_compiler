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

Intial project of leveldb from 
https://code.google.com/p/leveldbwin/