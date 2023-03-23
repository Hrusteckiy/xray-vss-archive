#ifndef DATE_TIME_GREGORIAN_CALENDAR_HPP__
#define DATE_TIME_GREGORIAN_CALENDAR_HPP__
/* Copyright (c) 2002 CrystalClear Software, Inc.
 * Disclaimer & Full Copyright at end of file
 * Author: Jeff Garland 
 */


namespace boost {
namespace date_time {


  //! An implementation of the Gregorian calendar
  /*! This is a parameterized implementation of a proleptic Gregorian Calendar that
      can be used in the creation of date systems or just to perform calculations.
      All the methods of this class are static functions, so the intent is to
      never create instances of this class.
    @param ymd_type_ Struct type representing the year, month, day.  The ymd_type must
           define a of types for the year, month, and day.  These types need to be
           arithmetic types.
    @param date_int_type_ Underlying type for the date count.  Must be an arithmetic type.
  */
  template<typename ymd_type_, typename date_int_type_>
  class gregorian_calendar_base {
  public:
    //! define a type a date split into components 
    typedef ymd_type_  ymd_type;
    //! define a type for representing months
    typedef typename ymd_type::month_type  month_type;
    //! define a type for representing days
    typedef typename ymd_type::day_type  day_type;
    //! Type to hold a stand alone year value (eg: 2002)
    typedef typename ymd_type::year_type  year_type;
    //! Define the integer type to use for internal calculations
    typedef date_int_type_ date_int_type;


    static unsigned short day_of_week(const ymd_type& ymd);
    static int week_number(const ymd_type&ymd);
    //static unsigned short day_of_year(date_int_type);
    static date_int_type day_number(const ymd_type& ymd);
    static date_int_type julian_day_number(const ymd_type& ymd);
    static long modjulian_day_number(const ymd_type& ymd);
    static ymd_type from_day_number(date_int_type);
    static ymd_type from_julian_day_number(date_int_type);
    static ymd_type from_modjulian_day_number(long);
    static bool is_leap_year(year_type);
    static unsigned short end_of_month_day(year_type y, month_type m);
    static ymd_type epoch();
    static unsigned short days_in_week();

  };



} } //namespace
  
#ifndef NO_BOOST_DATE_TIME_INLINE
#include "gregorian_calendar.ipp"
#endif

/* Copyright (c) 2002
 * CrystalClear Software, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  CrystalClear Software makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */


#endif
  

