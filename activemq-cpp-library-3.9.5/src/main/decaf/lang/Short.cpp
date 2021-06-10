/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Short.h"
#include "Integer.h"

#include <sstream>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const int Short::SIZE = 16;
const short Short::MAX_VALUE = (short)0x7FFF;
const short Short::MIN_VALUE = (short)0x8000;

////////////////////////////////////////////////////////////////////////////////
Short::Short(short value) : value(value) {
}

////////////////////////////////////////////////////////////////////////////////
Short::Short(const String& value) : value(0) {
    Short::parseShort(value);
}

////////////////////////////////////////////////////////////////////////////////
Short::~Short() {
}

////////////////////////////////////////////////////////////////////////////////
int Short::compareTo(const Short& s) const {
    return value == s.value ? 0 : (value > s.value ? 1 : -1);
}

////////////////////////////////////////////////////////////////////////////////
int Short::compareTo(const short& s) const {
    return value == s ? 0 : (value > s ? 1 : -1);
}

////////////////////////////////////////////////////////////////////////////////
std::string Short::toString() const {
    return Integer::toString(this->value);
}

////////////////////////////////////////////////////////////////////////////////
std::string Short::toString(short value) {
    return Integer::toString(value);
}

////////////////////////////////////////////////////////////////////////////////
short Short::parseShort(const String& s, int radix) {

    int intValue = Integer::parseInt(s, radix);
    short result = (short) intValue;
    if (result != intValue) {
        throw NumberFormatException(
        __FILE__, __LINE__, "Short::parseShort - Not a valid short encoded string.");
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
short Short::parseShort(const String& s) {
    return parseShort(s, 10);
}

////////////////////////////////////////////////////////////////////////////////
Short Short::decode(const String& value) {

    int intValue = Integer::decode(value).intValue();
    short result = (short) intValue;
    if (result != intValue) {
        throw NumberFormatException(
            __FILE__, __LINE__,
            "Short::decode - Not a valid short encoded string.");
    }

    return Short(result);
}

////////////////////////////////////////////////////////////////////////////////
short Short::reverseBytes(short value) {
    unsigned short temp = value;
    temp = (unsigned short) (((temp & 0xFF00) >> 8) | ((temp & 0x00FF) << 8));
    return temp;
}

////////////////////////////////////////////////////////////////////////////////
Short Short::valueOf(short value) {
    return Short(value);
}

////////////////////////////////////////////////////////////////////////////////
Short Short::valueOf(const String& value) {
    return Short(parseShort(value, 10));
}

////////////////////////////////////////////////////////////////////////////////
Short Short::valueOf(const String& value, int radix) {
    return Short(parseShort(value, radix));
}
