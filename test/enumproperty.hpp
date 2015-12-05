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

#ifndef CAMPTEST_ENUMPROPERTY_HPP
#define CAMPTEST_ENUMPROPERTY_HPP

#include <camp/camptype.hpp>
#include <camp/enum.hpp>
#include <camp/class.hpp>
#include <camp/classbuilder.hpp>
#include <list>
#include <vector>

namespace EnumPropertyTest
{
    enum MyEnum
    {
        Zero = 0,
        One  = 1
    };

    struct MyClass
    {
        MyClass(MyEnum x_) : x(x_)
        {
        }

        MyEnum x;
    };

    void declare()
    {
        camp::Enum::declare<MyEnum>("EnumPropertyTest::MyEnum");

        camp::Class::declare<MyClass>("EnumPropertyTest::MyClass")
            .property("x", &MyClass::x);
    }
}

CAMP_AUTO_TYPE(EnumPropertyTest::MyEnum, &EnumPropertyTest::declare)
CAMP_AUTO_TYPE(EnumPropertyTest::MyClass, &EnumPropertyTest::declare)

#endif // CAMPTEST_ENUMPROPERTY_HPP
