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


#ifndef CAMP_VALUE_HPP
#define CAMP_VALUE_HPP

#include <camp/type.hpp>
#include <camp/enumobject.hpp>
#include <camp/userobject.hpp>
#include <camp/valuemapper.hpp>
#include <camp/detail/valueimpl.hpp>
#include <camp/detail/variant.hpp>
#include <iosfwd>
#include <string>


namespace camp
{
/**
 * \brief Variant class which is used to wrap values in the CAMP system
 *
 * The Value class can store any type of variable, and supports conversion
 * to any type compatible with the stored type.
 *
 * \code
 * camp::Value v1 = true;
 * camp::Value v2 = 10;
 * camp::Value v3 = "24.5";
 * camp::Value v4 = myObject;
 *
 * bool        b = v1; // b == true
 * std::string s = v2; // s == "10"
 * float       f = v3; // f == 24.5
 * MyObject    o = v4; // o == myObject
 * \endcode
 *
 * It also supports unary and binary visitation for type-safe processing
 * depending on the stored type.
 *
 * \remark The set of supported types can be extended by specializing the
 * camp_ext::ValueMapper template.
 *
 * \sa ValueVisitor, camp_ext::ValueMapper
 */
class CAMP_API Value
{
public:

    /**
     * \brief Default constructor, constructs a null value
     */
    Value();

    /**
     * \brief Copy constructor
     *
     * \param other Value to copy
     */
    Value(const Value& other);

    /**
     * \brief Construct the value from a variable of type T
     *
     * \param val Value to store
     */
    template <typename T>
    Value(const T& val);

    /**
     * \brief Return the CAMP type of the value
     *
     * \return Type of the value
     */
    Type type() const;

    /**
     * \brief Convert the value to the type T
     *
     * \return Value converted to T
     *
     * \throw BadType the stored value is not convertible to T
     */
    template <typename T>
    T to() const;

    /**
     * \brief Cast operator to implicitely convert the value to a type T
     *
     * \return Value converted to T
     *
     * \throw BadType the stored value is not convertible to T
     */
    template <typename T>
    operator T() const;

    /**
     * \brief Check if the stored value can be converted to a type T
     *
     * If this function returns true, then calling to<T>() or operator T() will succeed.
     *
     * \return True if conversion is possible, false otherwise
     */
    template <typename T>
    bool isCompatible() const;

    /**
     * \brief Visit the value with a unary visitor
     *
     * Using this function allows to dispatch an operation depending on the stored type.
     *
     * \param visitor Visitor to apply (must inherit from ValueVisitor<type_to_return>)
     *
     * \return Value returned by the visitor
     */
    template <typename T>
    typename T::result_type visit(T visitor) const;

    /**
     * \brief Visit the value and another one with a binary visitor
     *
     * Using this function allows to dispatch a binary operation depending on the stored type
     * of both values.
     *
     * \param visitor Visitor to apply (must inherit from ValueVisitor<type_to_return>)
     * \param other Other value to visit
     *
     * \return Value returned by the visitor
     */
    template <typename T>
    typename T::result_type visit(T visitor, const Value& other) const;

    /**
     * \brief Operator == to compare equality between two values
     *
     * Two values are equal if their CAMP type and value are equal.
     * It uses the == operator of the stored type.
     *
     * \param other Value to compare with this
     *
     * \return True if both values are the same, false otherwise
     */
    bool operator==(const Value& other) const;

    /**
     * \brief Operator != to compare equality between two values
     *
     * \see operator==
     *
     * \return True if both values are not the same, false otherwise
     */
    bool operator!=(const Value& other) const {return !(*this == other);}

    /**
     * \brief Operator < to compare two values
     *
     * \param other Value to compare with this
     *
     * \return True if this < other
     */
    bool operator<(const Value& other) const;

public:

    /**
     * \brief Special Value instance representing an empty value
     */
    static const Value nothing;

private:

    typedef camp::util::variant<NoType, bool, long, double, std::string, EnumObject, UserObject> Variant;

    Variant m_value; ///< Stored value
    Type m_type; ///< CAMP type of the value
};

/**
 * \brief Overload of operator >> to extract a camp::Value from a standard stream
 *
 * \param stream Source input stream
 * \param value Value to fill
 *
 * \return Reference to the input stream
 */
CAMP_API std::istream& operator>>(std::istream& stream, Value& value);

/**
 * \brief Overload of operator << to print a camp::Value into a standard stream
 *
 * \param stream Target output stream
 * \param value Value to print
 *
 * \return Reference to the output stream
 */
CAMP_API std::ostream& operator<<(std::ostream& stream, const Value& value);

} // namespace camp

#include <camp/value.inl>


#endif // CAMP_VALUE_HPP
