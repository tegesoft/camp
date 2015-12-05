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


#include <camp/class.hpp>
#include <camp/constructor.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
const std::string& Class::name() const
{
    return m_id;
}

//-------------------------------------------------------------------------------------------------
std::size_t Class::baseCount() const
{
    return m_bases.size();
}

//-------------------------------------------------------------------------------------------------
const Class& Class::base(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_bases.size())
        CAMP_ERROR(OutOfRange(index, m_bases.size()));

    return *m_bases[index].base;
}

//-------------------------------------------------------------------------------------------------
std::size_t Class::functionCount() const
{
    return m_functions.size();
}

//-------------------------------------------------------------------------------------------------
bool Class::hasFunction(const std::string& id) const
{
    return m_functions.containsKey(id);
}

//-------------------------------------------------------------------------------------------------
const Function& Class::function(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_functions.size())
        CAMP_ERROR(OutOfRange(index, m_functions.size()));

    FunctionTable::const_iterator it = m_functions.begin();
    std::advance(it, index);

    return *it->second;
}

//-------------------------------------------------------------------------------------------------
const Function& Class::function(const std::string& id) const
{
    FunctionTable::const_iterator it;
    if (!m_functions.tryFind(id, it))
    {
        CAMP_ERROR(FunctionNotFound(id, name()));
    }

    return *it->second;
}

//-------------------------------------------------------------------------------------------------
std::size_t Class::propertyCount() const
{
    return m_properties.size();
}

//-------------------------------------------------------------------------------------------------
bool Class::hasProperty(const std::string& id) const
{
    return m_properties.containsKey(id);
}

//-------------------------------------------------------------------------------------------------
const Property& Class::property(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_properties.size())
        CAMP_ERROR(OutOfRange(index, m_properties.size()));

    PropertyTable::const_iterator it = m_properties.begin();
    std::advance(it, index);

    return *it->second;
}

//-------------------------------------------------------------------------------------------------
const Property& Class::property(const std::string& id) const
{
    PropertyTable::const_iterator it;
    if (!m_properties.tryFind(id, it))
    {
        CAMP_ERROR(PropertyNotFound(id, name()));
    }

    return *it->second;
}

//-------------------------------------------------------------------------------------------------
std::size_t Class::constructorCount() const
{
    return m_constructors.size();
}

//-------------------------------------------------------------------------------------------------
UserObject Class::construct(const Args& args) const
{
    // Search an arguments match among the list of available constructors
    ConstructorList::const_iterator end = m_constructors.end();
    for (ConstructorList::const_iterator it = m_constructors.begin();
         it != end;
         ++it)
    {
        Constructor& constructor = **it;
        if (constructor.matches(args))
        {
            // Match found: use the constructor to create the new instance
            return constructor.create(args);
        }
    }

    // No match found
    return UserObject::nothing;
}

//-------------------------------------------------------------------------------------------------
void Class::destroy(const UserObject& object) const
{
    m_destructor(object);
}

//-------------------------------------------------------------------------------------------------
void Class::visit(ClassVisitor& visitor) const
{
    // First visit properties
    for (PropertyTable::const_iterator it = m_properties.begin(); it != m_properties.end(); ++it)
    {
        it->second->accept(visitor);
    }

    // Then visit functions
    for (FunctionTable::const_iterator it = m_functions.begin(); it != m_functions.end(); ++it)
    {
        it->second->accept(visitor);
    }
}

//-------------------------------------------------------------------------------------------------
void* Class::applyOffset(void* pointer, const Class& target) const
{
    // Special case for null pointers: don't apply offset to leave them null
    if (!pointer)
        return pointer;

    // Check target as a base class of this
    int offset = baseOffset(target);
    if (offset != -1)
        return static_cast<void*>(static_cast<char*>(pointer) + offset);

    // Check target as a derived class of this
    offset = target.baseOffset(*this);
    if (offset != -1)
        return static_cast<void*>(static_cast<char*>(pointer) - offset);

    // No match found, target is not a base class nor a derived class of this
    CAMP_ERROR(ClassUnrelated(name(), target.name()));
}

//-------------------------------------------------------------------------------------------------
bool Class::operator==(const Class& other) const
{
    return m_id == other.m_id;
}

//-------------------------------------------------------------------------------------------------
bool Class::operator!=(const Class& other) const
{
    return m_id != other.m_id;
}

//-------------------------------------------------------------------------------------------------
Class::Class(const std::string& name)
    : m_id(name)
{
}

//-------------------------------------------------------------------------------------------------
int Class::baseOffset(const Class& base) const
{
    // Check self
    if (&base == this)
        return 0;

    // Search base in the base classes
    BaseList::const_iterator end = m_bases.end();
    for (BaseList::const_iterator it = m_bases.begin(); it != end; ++it)
    {
        const int offset = it->base->baseOffset(base);
        if (offset != -1)
            return offset + it->offset;
    }

    return -1;
}

} // namespace camp
