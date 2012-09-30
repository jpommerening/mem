/* -*- Mode: C; tab-width: 8; c-basic-offset: 8; indent-tabs-mode: t -*- */
/* vim:set softtabstop=8 shiftwidth=8 noet: */
#ifndef _JEMALLOC_CONFIG_H_
#define _JEMALLOC_CONFIG_H_

#define MOZ_MEMORY

#if defined(__APPLE__) || defined(__MACH__)
#  define MOZ_MEMORY_DARWIN
#elif defined(_WIN64) || defined(_WIN32)
#  define MOZ_MEMORY_WINDOWS
#elif defined(__linux__)
#  define MOZ_MEMORY_LINUX
#  if defined(__ANDROID__)
#    define MOZ_MEMORY_ANDROID
#  endif
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#  define MOZ_MEMORY_BSD
#elif defined(__SVR4) && defined(__sun)
#  define MOZ_MEMORY_SOLARIS
#endif


#if defined(DEBUG)
#define MOZ_MEMORY_DEBUG
#endif

#endif
