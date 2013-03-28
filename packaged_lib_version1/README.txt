Packaged library for leveldb_cc

The files are from
https://github.com/jbandela/leveldb_cross_compiler

and 

https://github.com/jbandela/cross_compiler_call

This is an adaptation of leveldb.  The leveldb source comes from

https://github.com/bitcoin/bitcoin/tree/master/src/leveldb 

msys and ming g++ 4.7.2 from nuwen.net were use to build

leveldb_cc_dll.dll

a.exe was compiled with gcc (so no suprise that it works)
example.exe was compiled with Visual C++11 November CTP (for variadic template support)
you may need the runtime library installed to run example.exe if you do not have MSVC 11 installed

Files by me are released under the Boost Software License
