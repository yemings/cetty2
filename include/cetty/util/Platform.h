#if !defined(CETTY_UTIL_PLATFORM_H)
#define CETTY_UTIL_PLATFORM_H
//
// Platform.h
//
// $Id: //poco/1.4/Foundation/include/Poco/Platform.h#4 $
//
// Library: Foundation
// Package: Core
// Module:  Platform
//
// Platform and architecture identification macros.
//
// NOTE: This file may be included from both C++ and C code, so it
//       must not contain any C++ specific things.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

//
// Platform Identification
//
#define CETTY_OS_FREE_BSD      0x0001
#define CETTY_OS_AIX           0x0002
#define CETTY_OS_HPUX          0x0003
#define CETTY_OS_TRU64         0x0004
#define CETTY_OS_LINUX         0x0005
#define CETTY_OS_MAC_OS_X      0x0006
#define CETTY_OS_NET_BSD       0x0007
#define CETTY_OS_OPEN_BSD      0x0008
#define CETTY_OS_IRIX          0x0009
#define CETTY_OS_SOLARIS       0x000a
#define CETTY_OS_QNX           0x000b
#define CETTY_OS_VXWORKS       0x000c
#define CETTY_OS_CYGWIN        0x000d
#define CETTY_OS_UNKNOWN_UNIX  0x00ff
#define CETTY_OS_WINDOWS_NT    0x1001
#define CETTY_OS_WINDOWS_CE    0x1011
#define CETTY_OS_VMS           0x2001


#if defined(__FreeBSD__)
	#define CETTY_OS_FAMILY_UNIX 1
	#define CETTY_OS_FAMILY_BSD 1
	#define CETTY_OS CETTY_OS_FREE_BSD
#elif defined(_AIX) || defined(__TOS_AIX__)
	#define CETTY_OS_FAMILY_UNIX 1
	#define CETTY_OS CETTY_OS_AIX
#elif defined(hpux) || defined(_hpux)
	#define CETTY_OS_FAMILY_UNIX 1
	#define CETTY_OS CETTY_OS_HPUX
#elif defined(__digital__) || defined(__osf__)
	#define CETTY_OS_FAMILY_UNIX 1
	#define CETTY_OS CETTY_OS_TRU64
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
	#define CETTY_OS_FAMILY_UNIX 1
	#define CETTY_OS CETTY_OS_LINUX
#elif defined(__APPLE__) || defined(__TOS_MACOS__)
	#define CETTY_OS_FAMILY_UNIX 1
	#define CETTY_OS_FAMILY_BSD 1
	#define CETTY_OS CETTY_OS_MAC_OS_X
#elif defined(__NetBSD__)
	#define CETTY_OS_FAMILY_UNIX 1
	#define CETTY_OS_FAMILY_BSD 1
	#define CETTY_OS CETTY_OS_NET_BSD
#elif defined(__OpenBSD__)
	#define CETTY_OS_FAMILY_UNIX 1
	#define CETTY_OS_FAMILY_BSD 1
	#define CETTY_OS CETTY_OS_OPEN_BSD
#elif defined(sgi) || defined(__sgi)
	#define CETTY_OS_FAMILY_UNIX 1
	#define CETTY_OS CETTY_OS_IRIX
#elif defined(sun) || defined(__sun)
	#define CETTY_OS_FAMILY_UNIX 1
	#define CETTY_OS CETTY_OS_SOLARIS
#elif defined(__QNX__)
	#define CETTY_OS_FAMILY_UNIX 1
	#define CETTY_OS CETTY_OS_QNX
#elif defined(unix) || defined(__unix) || defined(__unix__)
	#define CETTY_OS_FAMILY_UNIX 1
	#define CETTY_OS CETTY_OS_UNKNOWN_UNIX
#elif defined(_WIN32_WCE)
	#define CETTY_OS_FAMILY_WINDOWS 1
	#define CETTY_OS CETTY_OS_WINDOWS_CE
#elif defined(_WIN32) || defined(_WIN64)
	#define CETTY_OS_FAMILY_WINDOWS 1
	#define CETTY_OS CETTY_OS_WINDOWS_NT
#elif defined(__CYGWIN__)
	#define CETTY_OS_FAMILY_UNIX 1
	#define CETTY_OS CETTY_OS_CYGWIN
#elif defined(__VMS)
	#define CETTY_OS_FAMILY_VMS 1
	#define CETTY_OS CETTY_OS_VMS
#elif defined(CETTY_VXWORKS)
  #define CETTY_OS_FAMILY_UNIX 1
  #define CETTY_OS CETTY_OS_VXWORKS
#endif


//
// Hardware Architecture and Byte Order
//
#define CETTY_ARCH_ALPHA   0x01
#define CETTY_ARCH_IA32    0x02
#define CETTY_ARCH_IA64    0x03
#define CETTY_ARCH_MIPS    0x04
#define CETTY_ARCH_HPPA    0x05
#define CETTY_ARCH_PPC     0x06
#define CETTY_ARCH_POWER   0x07
#define CETTY_ARCH_SPARC   0x08
#define CETTY_ARCH_AMD64   0x09
#define CETTY_ARCH_ARM     0x0a
#define CETTY_ARCH_M68K    0x0b
#define CETTY_ARCH_S390    0x0c
#define CETTY_ARCH_SH      0x0d
#define CETTY_ARCH_NIOS2   0x0e


#if defined(__ALPHA) || defined(__alpha) || defined(__alpha__) || defined(_M_ALPHA)
	#define CETTY_ARCH CETTY_ARCH_ALPHA
	#define CETTY_ARCH_LITTLE_ENDIAN 1
#elif defined(i386) || defined(__i386) || defined(__i386__) || defined(_M_IX86)
	#define CETTY_ARCH CETTY_ARCH_IA32
	#define CETTY_ARCH_LITTLE_ENDIAN 1
#elif defined(_IA64) || defined(__IA64__) || defined(__ia64__) || defined(__ia64) || defined(_M_IA64)
	#define CETTY_ARCH CETTY_ARCH_IA64
	#if defined(hpux) || defined(_hpux)
		#define CETTY_ARCH_BIG_ENDIAN 1
	#else
		#define CETTY_ARCH_LITTLE_ENDIAN 1
	#endif
#elif defined(__x86_64__) || defined(_M_X64)
	#define CETTY_ARCH CETTY_ARCH_AMD64
	#define CETTY_ARCH_LITTLE_ENDIAN 1
#elif defined(__mips__) || defined(__mips) || defined(__MIPS__) || defined(_M_MRX000)
	#define CETTY_ARCH CETTY_ARCH_MIPS
	#define CETTY_ARCH_BIG_ENDIAN 1
#elif defined(__hppa) || defined(__hppa__)
	#define CETTY_ARCH CETTY_ARCH_HPPA
	#define CETTY_ARCH_BIG_ENDIAN 1
#elif defined(__PPC) || defined(__POWERPC__) || defined(__powerpc) || defined(__PPC__) || \
      defined(__powerpc__) || defined(__ppc__) || defined(__ppc) || defined(_ARCH_PPC) || defined(_M_PPC)
	#define CETTY_ARCH CETTY_ARCH_PPC
	#define CETTY_ARCH_BIG_ENDIAN 1
#elif defined(_POWER) || defined(_ARCH_PWR) || defined(_ARCH_PWR2) || defined(_ARCH_PWR3) || \
      defined(_ARCH_PWR4) || defined(__THW_RS6000)
	#define CETTY_ARCH CETTY_ARCH_POWER
	#define CETTY_ARCH_BIG_ENDIAN 1
#elif defined(__sparc__) || defined(__sparc) || defined(sparc)
	#define CETTY_ARCH CETTY_ARCH_SPARC
	#define CETTY_ARCH_BIG_ENDIAN 1
#elif defined(__arm__) || defined(__arm) || defined(ARM) || defined(_ARM_) || defined(__ARM__) || defined(_M_ARM)
	#define CETTY_ARCH CETTY_ARCH_ARM
	#if defined(__ARMEB__)
		#define CETTY_ARCH_BIG_ENDIAN 1
	#else
		#define CETTY_ARCH_LITTLE_ENDIAN 1
	#endif
#elif defined(__m68k__)
	#define CETTY_ARCH CETTY_ARCH_M68K
	#define CETTY_ARCH_BIG_ENDIAN 1
#elif defined(__s390__)
	#define CETTY_ARCH CETTY_ARCH_S390
	#define CETTY_ARCH_BIG_ENDIAN 1
#elif defined(__sh__) || defined(__sh)
	#define CETTY_ARCH CETTY_ARCH_SH
	#if defined(__LITTLE_ENDIAN__)
		#define CETTY_ARCH_LITTLE_ENDIAN 1
	#else
		#define CETTY_ARCH_BIG_ENDIAN 1
	#endif
#elif defined (nios2) || defined(__nios2) || defined(__nios2__)
    #define CETTY_ARCH CETTY_ARCH_NIOS2
    #if defined(__nios2_little_endian) || defined(nios2_little_endian) || defined(__nios2_little_endian__)
		#define CETTY_ARCH_LITTLE_ENDIAN 1
	#else
		#define CETTY_ARCH_BIG_ENDIAN 1
	#endif

#endif

#endif //#if !defined(CETTY_UTIL_PLATFORM_H)

// Local Variables:
// mode: c++
// End:
