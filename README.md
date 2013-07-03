#LevelDB Component Wrapper

##What is it?
This wraps the LevelDB database <http://code.google.com/p/leveldb/> into a cross compiler component.
This makes it easier to incorporate it into your program, especially on Windows.
You do not need to worry about compilers or standard library versions being compatible with the compiler/standard library used
to build the component.

##Why is this useful?
This saves you from having to build LevelDB. This is mostly useful on Windows, where it can be a major pain to build LevelDB. 
See <http://ayende.com/blog/161409/leveldb-windows-it-aint-a-love-story>.

##What does this depend on?
This depends on cross_compiler_call found at <https://github.com/jbandela/cross_compiler_call>. The cross_compiler_call library is a header only library
that works on Windows and Linux and uses C++11 to make it easy to integrate code written that is written using different compilers or uses different
standard libraries. For example on Windows, you can write code in g++ and easily call it from Visual C++. On linux you can write code in clang with libc++
and call it from g++.

##Will it work with my compiler?
cross_compiler_call which this library uses requires excellent C++11 support especially for variadic templates. This code has been tested with 
Microsoft Visual Studio 2013 Preview, Mingw g++ 4.8.1 on Windows and g++ 4.7.2 and clang++ 3.4 with libc++ on Linux. Make sure to include
`-std=c++11` with g++ and clang++

##How do I build code that uses this library?
1. Make sure you have `cross_compiler_call` in your header search path. You can download from <https://github.com/jbandela/cross_compiler_call>
	- You can use the `-I /path_to_cross_compiler_call` flag
	- If using a Visual C++ project add that to the Visual C++ Include files directories

2. Include `leveldb_interfaces.h` in your `.cpp` file
3. On Windows, that is all you need 
4. On Linux, you need to add `-ldl` to your compiler command, otherwise you will get `dlopen symbol not found` errors
5. Copy either the `leveldb_cc_dll.dll` (for Windows) or `leveldb_cc_dll.so` (for Linux) to the same directory as the executable file. 
You can find prebuilt binaries for Win32 and Linux_x64 in the Releases <https://github.com/jbandela/leveldb_cross_compiler/releases> in `binaries.zip` or `binaries.tar.gz`

That's it, no complicated build steps, no copying different files for different compilers!

##How do I use this library?
Take a look at `example.cpp` for a simple example

##How do I rebuild the .dll or .so file?


###On Windows

####Prerequisites
1. Msys
2. Mingw - I got mine from <http://www.nuwen.net>

####Steps
1. Get LevelDB from <https://github.com/ripple/leveldb>
2. Open msys and navigate to where you have the directory
3. Add the path to ming binaries to the **end** of the `PATH` variable. For example `set PATH=$PATH:/c/Mingw/bin/`
4. `export TARGET_OS=NATIVE_WINDOWS`
5. `make`
6. Download leveldb_cross_compiler from https://github.com/jbandela/leveldb_cross_compiler
7. `C:\Users\jrb\Source\Repos\leveldb_cross_compiler>g++ leveldb_cc_dll.cpp -O2 -shared -o leveldb_cc_dll.dll ..\leveldb\libleveldb.a -I ..\leveldb\include -I ..\cr
oss_compiler_call -lshlwapi -std=c++11` replacing the paths to `libleveldb.a` and `leveldb\include` and `cross_compiler_call` as necessary

###On Linux
1. Download LevelDB
2. Go to the directory where you downloaded LevelDB
3. Edit the `Makefile` and add `-fPIC` to the end of the `OPT ?= `
4. `make`
5. Download `cross_compiler_call`
6. Downlaod `leveldb_cross_compiler`
7. Go the leveldb_cross_compiler directory
8. `g++ -shared -o leveldb_cc_dll.so leveldb_cc_dll.cpp ../leveldb-1.12.0/libleveldb.a -lpthread -I ../leveldb-1.12.0/include/  -I ..\cr
oss_compiler_call-std=c++11 -fPIC -O2` replacing the paths to `libleveldb.a` and `leveldb\include` and `cross_compiler_call` as necessary



##License

The interface files and implementation and example are distributed under

```
         Copyright John R. Bandela 2013.
 Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)
```
The LevelDB files are

```
Copyright (c) 2011 The LevelDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file. See the AUTHORS file for names of contributors.
```
