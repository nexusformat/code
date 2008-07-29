//
//  NeXus - Neutron & X-ray Common Data Format
//  
//  $Id: Makefile.am 598 2005-08-19 16:19:15Z faa59 $
//  
//  Stream interface to NeXus C++ Bindings
//
//  Copyright (C) 2008 Freddie Akeroyd, STFC ISIS facility
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
// 
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
// 
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free 
//  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
//  MA  02111-1307  USA
//             
//  For further information, see http://www.nexusformat.org/
//

/////////////////// Subversion Repository Details ////////////////////////
// Repository Location     $HeadURL$
// Revision of last commit $LastChangedRevision$ 
// Date of last commit     $LastChangedDate$
// Author of last commit   $Author$
//////////////////////////////////////////////////////////////////////////

/**
 * \file
 * Stream interface for use with NeXus C++ Bindings
 * \version $LastChangedRevision$
 * \date    $LastChangedDate$
 */

#include <iostream>
#include "NeXusStream.hpp"
#include "NeXusException.hpp"
namespace NeXus {

HolderBase::HolderBase(const std::string& name) : m_name(name)
{
}

void HolderBase::setName(const std::string& name)
{
    m_name = name;
}
    
template<typename NumT>
AttrHolder<NumT>::AttrHolder(const std::string& name, NumT& value) : HolderBase(name), m_c_value(NULL), m_value(&value)
{
}

template<typename NumT>
AttrHolder<NumT>::AttrHolder(const std::string& name, const NumT& value) : HolderBase(name), m_c_value(&value), m_value(NULL)
{
}

template<typename NumT>
AttrHolder<NumT>::AttrHolder(NumT& value) : HolderBase(""), m_c_value(NULL), m_value(&value)
{
}

template<typename NumT>
AttrHolder<NumT>::AttrHolder(const NumT& value) : HolderBase(""), m_c_value(&value), m_value(NULL)
{
}

template<typename NumT>
NXnumtype AttrHolder<NumT>::getType()
{
    return NeXus::getType<NumT>();
}

template<typename NumT>
void AttrHolder<NumT>::readFromFile(File& nf) const
{
    if (m_value != NULL)
    {
        nf.getAttr(m_name, *m_value);
    }
    else
    {
	throw Exception("AttrHolder<NumT>::readFromFile - not able to read into a constant");
    }
}

template<typename NumT>
void AttrHolder<NumT>::writeToFile(File& nf) const
{
    if (m_value != NULL)
    {
        nf.putAttr(m_name, *m_value);
    }
    else if (m_c_value != NULL)
    {
        nf.putAttr(m_name, *m_c_value);
    }
    else
    {
	throw Exception("AttrHolder<NumT>::writeToFile - no value to write");
    }
}


template<typename NumT>
void DataHolder<NumT>::readFromFile(File& nf) const
{
    if (m_value != NULL)
    {
    	nf.openData(m_name);
	nf.getData(*m_value);
	nf.closeData();
    }
    else if (m_c_value != NULL)
    {
	throw Exception("DataHolder<NumT>::readFromFile - not able to read into a constant");
    }
    else
    {
    	nf.openData(m_name);
    }
}

template<typename NumT>
void DataHolder<NumT>::writeToFile(File& nf) const
{
    if (m_value != NULL)
    {
        nf.writeData(m_name, *m_value);
    }
    else if (m_c_value != NULL)
    {
        nf.writeData(m_name, *m_c_value);
    }
    else
    {
	throw Exception("DataHolder<NumT>::writeToFile - no value to write");
    }
}

template<typename NumT>
DataHolder<NumT>::DataHolder(const std::string& name, std::vector<NumT>& value) : HolderBase(name), m_c_value(NULL), m_value(&value)
{
}

template<typename NumT>
DataHolder<NumT>::DataHolder(const std::string& name) : HolderBase(name), m_c_value(NULL), m_value(NULL)
{
}

template<typename NumT>
DataHolder<NumT>::DataHolder(const std::string& name, const std::vector<NumT>& value) : HolderBase(name), m_c_value(&value), m_value(NULL)
{
}

template<typename NumT>
DataHolder<NumT>::DataHolder(std::vector<NumT>& value) : HolderBase(""), m_c_value(NULL), m_value(&value)
{
}

template<typename NumT>
DataHolder<NumT>::DataHolder(const std::vector<NumT>& value) : HolderBase(""), m_c_value(&value), m_value(NULL)
{
}

template class AttrHolder<double>;
template class AttrHolder<int>;

template class DataHolder<double>;
template class DataHolder<int>;

void Data::readFromFile(File& nf) const
{
    m_holder->readFromFile(nf);
    if (m_attr.size() > 0)
    {
    	nf.openData(m_holder->getName());
	ObjectWithAttr::readFromFile(nf);
    	nf.closeData();
    }
}

void Data::writeToFile(File& nf) const
{
    m_holder->writeToFile(nf);
    if (m_attr.size() > 0)
    {
        nf.openData(m_holder->getName());
	ObjectWithAttr::writeToFile(nf);
        nf.closeData();
    }
}

File& operator<<(File& nf, const ISerialisable& obj)
{
    obj.writeToFile(nf);
    return nf;
}

File& operator>>(File& nf, const ISerialisable& obj)
{
    obj.readFromFile(nf);
    return nf;
}

}
