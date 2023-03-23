
// (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
// Hinnant & John Maddock 2000.  Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_IS_MEMBER_POINTER_HPP_INCLUDED
#define BOOST_TT_IS_MEMBER_POINTER_HPP_INCLUDED

#include "boost/type_traits/config.hpp"

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(__BORLANDC__)
#   include "boost/type_traits/is_member_function_pointer.hpp"
#else
#   include "boost/type_traits/is_reference.hpp"
#   include "boost/type_traits/is_array.hpp"
#   include "boost/type_traits/detail/is_mem_fun_pointer_tester.hpp"
#   include "boost/type_traits/detail/yes_no_type.hpp"
#   include "boost/type_traits/detail/false_result.hpp"
#   include "boost/type_traits/detail/ice_or.hpp"
#endif

// should be the last #include
#include "boost/type_traits/detail/bool_trait_def.hpp"

namespace boost {

#if defined(__BORLANDC__)
BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_member_pointer,T,false)
BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,typename U,is_member_pointer,U T::*,true)

#elif !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_member_pointer,T,::boost::is_member_function_pointer<T>::value)
BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,typename U,is_member_pointer,U T::*,true)

#else // no partial template specialization

namespace detail {

template <typename R, typename T>
::boost::type_traits::yes_type BOOST_TT_DECL is_member_pointer_tester(R T::*);
::boost::type_traits::no_type BOOST_TT_DECL is_member_pointer_tester(...);

template <bool>
struct is_member_pointer_select
    : ::boost::type_traits::false_result
{
};

template <>
struct is_member_pointer_select<false>
{
    template <typename T> struct result_
    {
        static T& make_t();
        BOOST_STATIC_CONSTANT(
            bool, value = 
            (::boost::type_traits::ice_or<
                (1 == sizeof(::boost::type_traits::is_mem_fun_pointer_tester(make_t()))),
                (1 == sizeof(is_member_pointer_tester(make_t())))
            >::value) );
    };
};

template <typename T>
struct is_member_pointer_impl
    : is_member_pointer_select<
          ::boost::type_traits::ice_or<
              ::boost::is_reference<T>::value
            , ::boost::is_array<T>::value
            >::value
        >::template result_<T>
{
};

BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_member_pointer,void,false)
#ifndef BOOST_NO_CV_VOID_SPECIALIZATIONS
BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_member_pointer,void const,false)
BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_member_pointer,void volatile,false)
BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_member_pointer,void const volatile,false)
#endif

} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_member_pointer,T,::boost::detail::is_member_pointer_impl<T>::value)

#endif // __BORLANDC__

} // namespace boost

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif // BOOST_TT_IS_MEMBER_POINTER_HPP_INCLUDED
