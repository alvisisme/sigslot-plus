# sigslot-plus

sigslot 改进版 

[![Build Status](https://travis-ci.org/alvisisme/sigslot-plus.svg?branch=master)](https://travis-ci.org/alvisisme/sigslot-plus)

## 使用说明

一些宏定义说明

SIGSLOT_PURE_ISO			

> Define this to force ISO C++ compliance. 
> This also disables all of the thread safety support on platforms where it is available.

SIGSLOT_USE_POSIX_THREADS

> Force use of Posix threads when using a C++ compiler other than gcc on a platform that supports Posix threads. (When using gcc, this is the default - use SIGSLOT_PURE_ISO to disable this if necessary)

SIGSLOT_DEFAULT_MT_POLICY

> Where thread support is enabled, this defaults to multi_threaded_global.
> Otherwise, the default is single_threaded. #define this yourself to override the default.
> In pure ISO mode, anything other than single_threaded will cause a compiler error.

PLATFORM NOTES

Win32

> On Win32, the WIN32 symbol must be #defined. 
> Most mainstream compilers do this by default, but you may need to define it yourself if your build environment is less standard. 
> This causes the Win32 thread support to be compiled in and used automatically.

Unix/Linux/BSD, etc.

> If you're using gcc, it is assumed that you have Posix threads available, so they are used automatically. 
> You can override this (as under Windows) with the SIGSLOT_PURE_ISO switch. 
> If you're using something other than gcc but still want to use Posix threads, you need to #define SIGSLOT_USE_POSIX_THREADS.

ISO C++

> If none of the supported platforms are detected, or if SIGSLOT_PURE_ISO is defined, all multithreading support is turned off, along with any code that might cause a pure ISO C++ environment to complain. 
> Before you ask, gcc -ansi -pedantic won't compile this library, but gcc -ansi is fine. 
> Pedantic mode seems to throw a lot of errors that aren't really there.
> If you feel like investigating this, please contact the author.


THREADING MODES

single_threaded

> Your program is assumed to be single threaded from the point of view of signal/slot usage (i.e. all objects using signals and slots are created and destroyed from a single thread).
> Behaviour if objects are destroyed concurrently is undefined (i.e. you'll get the occasional segmentation fault/memory exception).

multi_threaded_global

> Your program is assumed to be multi threaded. 
> Objects using signals and slots can be safely created and destroyed from any thread, even when connections exist. 
> In multi_threaded_global mode, this is achieved by a single global mutex (actually a critical section on Windows because they are faster). 
> This option uses less OS resources, but results in more opportunities for contention, possibly resulting in more context switches than are strictly necessary.

multi_threaded_local

> Behaviour in this mode is essentially the same as multi_threaded_global, except that each signal, and each object that inherits has_slots, all have their own mutex/critical section.
> In practice, this means that mutex collisions (and hence context switches) only happen if they are absolutely essential.
> However, on some platforms, creating a lot of mutexes can slow down the whole OS, so use this option with care.

## 参考引用

* [sigslot Written by Sarah Thompson (sarah@telergy.com) 2002.](https://sourceforge.net/projects/sigslot/)


