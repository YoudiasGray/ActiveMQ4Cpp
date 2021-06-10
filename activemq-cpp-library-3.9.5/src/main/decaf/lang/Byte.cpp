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

#include "Byte.h"
#include <decaf/lang/Integer.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const unsigned char Byte::MIN_VALUE = 0x7F;
const unsigned char Byte::MAX_VALUE = 0x80;
const int Byte::SIZE = 8;

////////////////////////////////////////////////////////////////////////////////
Byte::Byte(unsigned char value) : value(value) {
}

////////////////////////////////////////////////////////////////////////////////
Byte::Byte(const String& value) : value(0) {
    this->value = parseByte(value);
}

////////////////////////////////////////////////////////////////////////////////
std::string Byte::toString() const {
    return Integer::toString(value);
}

////////////////////////////////////////////////////////////////////////////////
std::string Byte::toString(unsigned char value) {
    return Integer::toString(value);
}

////////////////////////////////////////////////////////////////////////////////
unsigned char Byte::parseByte(const String& s, int radix) {

    int intValue = Integer::parseInt(s, radix);
    unsigned char result = (unsigned char) intValue;
    if (result != intValue) {
        throw NumberFormatException(
            __FILE__, __LINE__, "Byte::parseByte - Not a valid unsigned char encoded string.");
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
unsigned char Byte::parseByte(const String& s) {
    return parseByte(s, 10);
}

////////////////////////////////////////////////////////////////////////////////
Byte Byte::decode(const String& value) {

    int intValue = Integer::decode(value).intValue();
    unsigned char result = (unsigned char) intValue;
    if (result != intValue) {
        throw NumberFormatException(
            __FILE__, __LINE__,
            "Byte::decode - Not a valid unsigned char encoded string.");
    }

    return Byte(result);
}

////////////////////////////////////////////////////////////////////////////////
Byte Byte::valueOf(const String& value) {
    return Byte(parseByte(value, 10));
}

////////////////////////////////////////////////////////////////////////////////
Byte Byte::valueOf(const String& value, int radix) {
    return Byte(parseByte(value, radix));
}
