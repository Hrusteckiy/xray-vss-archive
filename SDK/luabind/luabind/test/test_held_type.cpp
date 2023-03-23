// Copyright (c) 2004 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#include "test.hpp"
#include <luabind/luabind.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>

namespace luabind {

    template<class T>
    T* get_pointer(boost::shared_ptr<T>& p) { return p.get(); }

    template<class A>
    boost::shared_ptr<const A>* get_const_holder(boost::shared_ptr<A>*)
    {
        return 0;
    }
}

namespace {

struct base : counted_type<base>
{
    base(): n(4) {}
    virtual ~base() {}

    void f(int)
    {
    }

    int n;
};
    
// this is here to make sure the pointer offsetting works
struct first_base : counted_type<first_base>
{
    virtual ~first_base() {}
    virtual void a() {}
    int padding;
};

struct derived : first_base, base
{
    derived(): n2(7) {}
    void f() {}
    int n2;
};

int feedback = 0;

void tester(base* t)
{
    if (t->n == 4) feedback = 1;
}

void tester_(derived* t)
{
    if (t->n2 == 7) feedback = 2;
}

void tester2(boost::shared_ptr<base> t)
{
    if (t->n == 4) feedback = 3;
}

void tester3(boost::shared_ptr<const base> t)
{
    if (t->n == 4) feedback = 4;
}

void tester4(const boost::shared_ptr<const base>& t)
{
    if (t->n == 4) feedback = 5;
}

void tester5(const boost::shared_ptr<const base>* t)
{
    if ((*t)->n == 4) feedback = 6;
}

void tester6(const boost::shared_ptr<base>* t)
{
    if ((*t)->n == 4) feedback = 7;
}

void tester7(boost::shared_ptr<base>* t)
{
    if ((*t)->n == 4) feedback = 8;
}

boost::shared_ptr<base> tester9()
{
    feedback = 9;
    return boost::shared_ptr<base>(new base());
}

void tester10(boost::shared_ptr<base> const& r)
{
	if (r->n == 4) feedback = 10;
}

void tester11(boost::shared_ptr<const base> const& r)
{
	if (r->n == 4) feedback = 11;
}

void tester12(boost::shared_ptr<derived> const& r)
{
	if (r->n2 == 7) feedback = 12;
}

} // namespace unnamed

void test_held_type()
{
    COUNTER_GUARD(first_base);
    COUNTER_GUARD(base);

    boost::shared_ptr<base> base_ptr(new base());

    lua_state L;

    using namespace luabind;
  
    module(L)
    [
        def("tester", &tester),
        def("tester", &tester_),
        def("tester2", &tester2),
        def("tester3", &tester3),
        def("tester4", &tester4),
        def("tester5", &tester5),
        def("tester6", &tester6),
        def("tester7", &tester7),
        def("tester9", &tester9),
		def("tester10", &tester10),
		def("tester11", &tester11),
		def("tester12", &tester12),

        class_<base, boost::shared_ptr<base> >("base")
            .def(constructor<>())
            .def("f", &base::f),

        class_<derived, base, boost::shared_ptr<base> >("derived")
            .def(constructor<>())
            .def("f", &derived::f)
    ];

    object g = get_globals(L);
    g["ptr"] = base_ptr;

    DOSTRING(L, "tester(ptr)");
    BOOST_CHECK(feedback == 1);

    DOSTRING(L, 
        "a = base()\n"
        "b = derived()\n");

    DOSTRING(L, "tester(b)");
    BOOST_CHECK(feedback == 2);

    DOSTRING(L, "tester(a)");
    BOOST_CHECK(feedback == 1);

    DOSTRING(L, "tester2(b)");
    BOOST_CHECK(feedback == 3);

    DOSTRING(L, "tester3(b)");
    BOOST_CHECK(feedback == 4);

    DOSTRING(L, "tester4(b)");
    BOOST_CHECK(feedback == 5);

    feedback = 0;

    DOSTRING(L, "tester4(a)");
    BOOST_CHECK(feedback == 5);

    DOSTRING(L, "tester10(b)");
    BOOST_CHECK(feedback == 10);

    DOSTRING(L, "tester11(b)");
    BOOST_CHECK(feedback == 11);
/* this test is messed up, shared_ptr<derived> isn't even registered
	DOSTRING_EXPECTED(
		L
		, "tester12(b)"
		, "no match for function call 'tester12' with the parameters (derived)\n"
		"candidates are:\n"
		"tester12(const custom&)\n");
*/
	object nil = get_globals(L)["non_existing_variable_is_nil"];
	BOOST_CHECK(object_cast<boost::shared_ptr<base> >(nil).get() == 0);
	BOOST_CHECK(object_cast<boost::shared_ptr<const base> >(nil).get() == 0);

}

