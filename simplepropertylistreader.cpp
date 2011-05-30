/* === This file is part of libsimpleplist-qt4 - <http://github.com/dschmidt/libsimpleplist-qt4> ===
 *
 *   Copyright 2011, Dominik Schmidt <dev@dominik-schmidt.de>
 *
 *   libsimpleplist-qt4 is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   libsimpleplist-qt4 is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with libsimpleplist-qt4. If not, see <http://www.gnu.org/licenses/>.
 */

#include "simplepropertylistreader.h"

#include <QDebug>
#include <QXmlStreamReader>
#include <QVariant>

SimplePropertyListReader::~SimplePropertyListReader()
{

}

SimplePropertyListReader::SimplePropertyListReader(QIODevice* device)
    : m_reader(0)
    , m_level(0)
    , m_previousElementType(ElementInvalid)
{
    device->open(QIODevice::ReadOnly | QIODevice::Text);

    m_reader = new QXmlStreamReader(device);
}

void SimplePropertyListReader::next()
{
    m_reader->readNext();

    while(m_reader->isWhitespace())
        m_reader->readNext();

    switch(m_reader->tokenType())
    {
        case QXmlStreamReader::Characters:
            if(m_reader->isCDATA() || m_reader->isWhitespace())
            {
                next();
                return;
            }
            break;

        case QXmlStreamReader::Invalid:
        case QXmlStreamReader::EndDocument:
            // do nothing
            break;

        case QXmlStreamReader::NoToken:
        case QXmlStreamReader::StartDocument:
        case QXmlStreamReader::Comment:
        case QXmlStreamReader::DTD:
        case QXmlStreamReader::EntityReference:
        case QXmlStreamReader::ProcessingInstruction:
            next();
            return;
            break;

        case QXmlStreamReader::StartElement:
        case QXmlStreamReader::EndElement:
            if(m_reader->name() == "plist")
            {
                next();
                return;
            }
            break;
    }

    if(m_reader->name() == "key")
    {
        m_tokenType = Key;
        m_reader->readNext();
        m_lastKey = m_reader->text().toString();
        m_reader->readNext();
        next();
        return;
    }
    else if(m_reader->name() == "integer")
    {
        m_tokenType = Integer;
        m_reader->readNext();
        m_lastValue = m_reader->text().toString();
        m_reader->readNext();
    }
    else if(m_reader->name() == "string")
    {
        m_tokenType = String;
        m_reader->readNext();
        m_lastValue = m_reader->text().toString();
        m_reader->readNext();
    }
    else if(m_reader->name() == "date")
    {
        m_tokenType = Date;
        m_reader->readNext();
        m_lastValue = m_reader->text().toString();
        m_reader->readNext();
    }
    else if(m_reader->name() == "real")
    {
        m_tokenType = Real;
    }
    else if(m_reader->name() == "true" || m_reader->name() == "false")
    {
        m_tokenType = Boolean;
        m_lastValue = m_reader->name().toString();
        m_reader->readNext();
    }
    else if(m_reader->name() == "dict")
    {
        m_tokenType = Dict;
        m_lastValue = QVariant();
    }
    else if(m_reader->name() == "array")
    {
        m_tokenType = Array;
        m_lastValue = QVariant();
    }


    //qDebug() << m_reader->text() << m_reader->name();
    if(m_previousElementType == ElementSingle || m_previousElementType == ElementEnd)
        m_level--;
    switch(elementType())
    {
        case SimplePropertyListReader::ElementStart:
            m_level++;
            break;

        case SimplePropertyListReader::ElementEnd:
            break;

        case SimplePropertyListReader::ElementSingle:
            m_level++;
            break;
    }
    m_previousElementType = elementType();
}

bool SimplePropertyListReader::atEnd()
{
    return m_reader->atEnd();
}

CFProperty::Type SimplePropertyListReader::propertyType()
{
    qDebug() << Q_FUNC_INFO << "not implemented yet";
    Q_ASSERT(false);
    return m_propertyType;
}

bool SimplePropertyListReader::isEndElement()
{
    switch(m_tokenType)
    {
        case Dict:
        case Array:
            return m_reader->isEndElement();

        default:
            return false;
    }
}

bool SimplePropertyListReader::isStartElement()
{
    switch(m_tokenType)
    {
        case Dict:
        case Array:
            return m_reader->isStartElement();

        default:
            return false;
    }
}

bool SimplePropertyListReader::isSingleElement()
{
    switch(m_tokenType)
    {
        case Dict:
        case Array:
            return false;

        default:
            return true;
    }
}

QStringRef SimplePropertyListReader::text()
{
    return m_reader->text();
}

QStringRef SimplePropertyListReader::name()
{
    return m_reader->name();
}

QString SimplePropertyListReader::tokenString()
{
    //Invalid, String, Real, Integer, Boolean, Date, Data, Array, Dict, Key
    switch(m_tokenType)
    {
        case Array:
            return "Array";
        case Boolean:
            return "Boolean";
        case Data:
            return "Data";
        case Date:
            return "Date";
        case Dict:
            return "Dict";
        case Integer:
            return "Integer";
        case Invalid:
            return "Invalid";
        case Real:
            return "Real";
        case String:
            return "String";

        /*case Key:
            qDebug() << "The Key should never be the active token from user perspective";
            Q_ASSERT(false);
            break;*/
        case Key:
            return "Key";

        default:
            qDebug() << "This kind of token is not handled by tokenString()" << m_tokenType;
            Q_ASSERT(false);
    }
}

SimplePropertyListReader::TokenType SimplePropertyListReader::tokenType()
{
    return m_tokenType;
}

SimplePropertyListReader::ElementType SimplePropertyListReader::elementType()
{
    switch(m_tokenType)
    {
        case Dict:
        case Array:
            if(m_reader->isStartElement())
                return ElementStart;
            else
                return ElementEnd;

        default:
            return ElementSingle;
    }
}

int SimplePropertyListReader::level()
{
    return m_level;
}

QString SimplePropertyListReader::key()
{
    return m_lastKey;
}

QVariant SimplePropertyListReader::value()
{
    return m_lastValue;
}

