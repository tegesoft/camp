/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
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


#ifndef CAMP_DETAIL_TYPEID_HPP
#define CAMP_DETAIL_TYPEID_HPP


#include <camp/detail/objecttraits.hpp>
#include <camp/detail/yesnotype.hpp>


namespace camp
{
namespace detail
{
/**
 * \brief Utility class to get the CAMP identifier associated to a C++ type
 *
 * A compiler error will be triggered if requesting the identifier of a type
 * which hasn't been registered with the CAMP_TYPE macro.
 */
template <typename T>
struct StaticTypeId
{
    static const char* get(bool = true)
    {
        // If you get this error, it means you didn't register
        // your class/enum T with the CAMP_TYPE macro
        return T::CAMP_TYPE_NOT_REGISTERED();
    }

    enum
    {
        defined = false,
        copyable = true
    };
};

/**
 * \brief Utility class to check if a type has a CAMP id (i.e. has been registered with CAMP_TYPE)
 */
template <typename T>
struct HasStaticTypeId
{
    enum
    {
        value = StaticTypeId<typename RawType<T>::Type>::defined
    };
};

/**
 * \brief Return the static type identifier of a C++ type T
 */
template <typename T> const char* staticTypeId()         {return StaticTypeId<typename RawType<T>::Type>::get();}
template <typename T> const char* staticTypeId(const T&) {return StaticTypeId<typename RawType<T>::Type>::get();}

/**
 * \brief Utility class used to check at compile-time if a type T implements the CAMP RTTI
 */
template <typename T>
struct HasCampRtti
{
    template <typename U, const char* (U::*)() const> struct TestForMember {};
    template <typename U> static TypeYes check(TestForMember<U, &U::campClassId>*);
    template <typename U> static TypeNo  check(...);

    enum {value = sizeof(check<typename RawType<T>::Type>(0)) == sizeof(TypeYes)};
};

/**
 * \brief Utility class to get the CAMP identifier associated to a C++ object
 *
 * If the object has a dynamic type which is different from its static type
 * (i.e. <tt>Base* obj = new Derived</tt>), and both classes use the
 * CAMP_RTTI macro, then the system is able to return the identifier of
 * the true dynamic type of the object.
 */
template <typename T, typename E = void>
struct DynamicTypeId
{
    typedef ObjectTraits<const T&> Traits;

    static const char* get(const T& object)
    {
        typename Traits::PointerType pointer = Traits::getPointer(object);
        return pointer ? pointer->campClassId() : staticTypeId<T>();
    }
};

/**
 * Specialization of DynamicTypeId for types that don't implement CAMP RTTI
 */
template <typename T>
struct DynamicTypeId<T, typename std::enable_if<!HasCampRtti<T>::value >::type>
{
    static const char* get(const T&)
    {
        return staticTypeId<T>();
    }
};

/**
 * \brief Return the dynamic type identifier of a C++ object
 */
template <typename T> const char* typeId()                {return staticTypeId<T>();}
template <typename T> const char* typeId(const T& object) {return DynamicTypeId<T>::get(object);}

/**
 * \brief Utility class to get a valid CAMP identifier from a C++ type even if the type wasn't declared
 */
template <typename T, typename E = void>
struct SafeTypeId
{
    static const char* get()
    {
        return typeId<T>();
    }

    static const char* get(const T& object)
    {
        return typeId(object);
    }
};

/**
 * Specialization of SafeTypeId for types that have no CAMP id
 */
template <typename T>
struct SafeTypeId<T, typename std::enable_if<!HasStaticTypeId<T>::value >::type>
{
    static const char* get()
    {
        return "";
    }

    static const char* get(const T&)
    {
        return "";
    }
};

/**
 * Specialization of SafeTypeId needed because "const void&" is not a valid expression
 */
template <>
struct SafeTypeId<void>
{
    static const char* get() {return "";}
};

/**
 * \brief Return the dynamic type identifier of a C++ object even if it doesn't exist (i.e. it can't fail)
 */
template <typename T> const char* safeTypeId()                {return SafeTypeId<typename RawType<T>::Type>::get();}
template <typename T> const char* safeTypeId(const T& object) {return SafeTypeId<T>::get(object);}

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_TYPEID_HPP
