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

#ifndef SIMPLEPROPERTYLISTREADER_H
#define SIMPLEPROPERTYLISTREADER_H

#include <QVariant>
#include <QXmlStreamReader>

struct CFProperty {
    enum Type {String, Real, Integer, Boolean, Date, Data, Array, Dict};
};

class SimplePropertyListReader : public QObject
{
Q_OBJECT

public:
    enum TokenType {Invalid, String, Real, Integer, Boolean, Date, Data, Array, Dict, Key};
    enum ElementType {ElementStart, ElementEnd, ElementSingle, ElementInvalid};

    SimplePropertyListReader(QIODevice* device);
    virtual ~SimplePropertyListReader();


    void next();
    int level();
    bool isStartElement();
    bool isSingleElement();
    bool isEndElement();
    bool atEnd();


    QStringRef text();
    QStringRef name();
    CFProperty::Type propertyType();
    TokenType tokenType();
    ElementType elementType();
    QString tokenString();

    QString key();
    QVariant value();

private:
    QXmlStreamReader* m_reader;
    CFProperty::Type m_propertyType;
    TokenType m_tokenType;
    QString m_lastKey;
    QVariant m_lastValue;
    int m_level;
    ElementType m_previousElementType;
};

#endif // SIMPLEPROPERTYLISTREADER_H
