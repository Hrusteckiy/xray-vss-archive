
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_HAS_SIGACTION
// This file should not compile, if it does then
// BOOST_HAS_SIGACTION may be defined.
// see boost_has_sigaction.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_has_sigaction.cxx on
// Thu Feb 13 12:02:51 GMTST 2003

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include "test.hpp"

#ifndef BOOST_HAS_SIGACTION
#include "boost_has_sigaction.cxx"
#else
#error "this file should not compile"
#endif

int cpp_main( int, char *[] )
{
   return boost_has_sigaction::test();
}  
   
