/****************************************************************************
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/

#include "enumproperty.hpp"
#include <camp/classget.hpp>
#include <camp/enumget.hpp>
#include <camp/enumproperty.hpp>
#include <boost/test/unit_test.hpp>

using namespace EnumPropertyTest;

//-----------------------------------------------------------------------------
struct EnumPropertyFixture
{
    EnumPropertyFixture()
    {
        const camp::Class& metaclass = camp::classByType<MyClass>();
        property = &static_cast<const camp::EnumProperty&>(metaclass.property("x"));
    }

    const camp::EnumProperty* property;
};

//-----------------------------------------------------------------------------
//                         Tests for camp::EnumProperty
//-----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_SUITE(ENUMPROPERTY, EnumPropertyFixture)

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(type)
{
    BOOST_CHECK_EQUAL(property->type(), camp::enumType);
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(getEnum)
{
    BOOST_CHECK(property->getEnum() == camp::enumByType<MyEnum>());
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(get)
{
    BOOST_CHECK_EQUAL(property->get(MyClass(Zero)), camp::Value(Zero));
    BOOST_CHECK_EQUAL(property->get(MyClass(One)),  camp::Value(One));
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(set)
{
    MyClass object(Zero);
    property->set(object, One);

    BOOST_CHECK_EQUAL(property->get(object), camp::Value(One));
}

BOOST_AUTO_TEST_SUITE_END()
