//
// Copyright (c) 2019.03
// Zhong
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.
//

//
// --> @ file abstract :
//  use get_automat_category(char) to get the automat category,
//  and do not have to know the inner woking.

#ifndef _ZCC_CONVERT_H_
#define _ZCC_CONVERT_H_

#include "core/core.h"
#include <cctype>

_CML_BEGIN_ // namespace cml begin

    constexpr char eof = -1;
constexpr char blank = ' ';
constexpr char newline = '\n';
constexpr char tab = '\t';

enum AUTOMAT_CATEGORY : uint8_t {
    WHITE_SPACE,    // white space
    IDENTIFIER,     // identifier
    DIGIT,          // digit
    OPERATOR,       // operator
    DELIMITER,      // delimiter
    LITERAL_CHAR,   // literal char
    LITERAL_STRING, // literal string
    PRE_PROCESSOR,  // pre processor
    BACK_SLASH,     // back slash
    UNDEFINED,      // undefined
};

// --> @ abstract :
//  fast mapping to the automat category
//  get_low(category) = automat_category; --> @ get_low
//  get_high(categoy) = automat_category.char_category; --> @ get_high
constexpr uint8_t ascii_convert[128] = {
    WHITE_SPACE, // 0x00 NUT

    UNDEFINED, // 0x01 SOH (start of headline)
    UNDEFINED, // 0x02 STX (start of text)
    UNDEFINED, // 0x03 ETX (end of text)
    UNDEFINED, // 0x04 EOT (end of transmission)
    UNDEFINED, // 0x05 ENQ (enquiry)
    UNDEFINED, // 0x06 ACK (acknowledge)
    UNDEFINED, // 0x07 BEL (bell)
    UNDEFINED, // 0x08 BS  (backspace)

    WHITE_SPACE, // 0x09 HT  (horizontal tab)
    WHITE_SPACE, // 0x0A LF  (new line)
    WHITE_SPACE, // 0x0B VT  (vertical tab)
    WHITE_SPACE, // 0x0C FF  (new page)
    WHITE_SPACE, // 0x0D CR  (carriage return)

    UNDEFINED, // 0x0E SO  (shift out)
    UNDEFINED, // 0x0F SI  (shift in)
    UNDEFINED, // 0x10 DLE (data link escape)
    UNDEFINED, // 0x11 DC1 (device control 1）
    UNDEFINED, // 0x12 DC2 (device control 2)
    UNDEFINED, // 0x13 DC3 (device control 3)
    UNDEFINED, // 0x14 DC4 (device control 4)
    UNDEFINED, // 0x15 NAK (negative acknowledge)
    UNDEFINED, // 0x16 SYN (synchromous idle)
    UNDEFINED, // 0x17 ETB (end of trans block)
    UNDEFINED, // 0x18 CAN (cancel)
    UNDEFINED, // 0x19 EM  (end of medium)
    UNDEFINED, // 0x1A SUB (substitute)
    UNDEFINED, // 0x1B ESC (escape)
    UNDEFINED, // 0x1C FS  (file separator)
    UNDEFINED, // 0x1D GS  (group separator)
    UNDEFINED, // 0x1E RS  (record separator)
    UNDEFINED, // 0x1F US  (unit separator)

    WHITE_SPACE, // 0x20 SPA (space)

    OPERATOR | 0x00,  // 0x21 ! (opr 0)
    LITERAL_STRING,   // 0x22 "
    PRE_PROCESSOR,    // 0x23 #
    UNDEFINED,        // 0x24 $
    OPERATOR | 0x10,  // 0x25 % (opr 1)
    OPERATOR | 0x20,  // 0x26 & (opr 2)
    LITERAL_CHAR,     // 0x27 '
    DELIMITER | 0x00, // 0x28 ( (dlm 0)
    DELIMITER | 0x10, // 0x29 ) (dlm 1)
    OPERATOR | 0x30,  // 0x2A * (opr 3)
    OPERATOR | 0x40,  // 0x2B + (opr 4)
    DELIMITER | 0x20, // 0x2C , (dlm 2)
    OPERATOR | 0x50,  // 0x2D - (opr 5)
    OPERATOR | 0x60,  // 0x2E . (opr 6)
    OPERATOR | 0x70,  // 0x2F / (opr 7)

    DIGIT | 0x00, // 0x30 0
    DIGIT | 0x10, // 0x31 1
    DIGIT | 0x20, // 0x32 2
    DIGIT | 0x30, // 0x33 3
    DIGIT | 0x40, // 0x34 4
    DIGIT | 0x50, // 0x35 5
    DIGIT | 0x60, // 0x36 6
    DIGIT | 0x70, // 0x37 7
    DIGIT | 0x80, // 0x38 8
    DIGIT | 0x90, // 0x39 9

    OPERATOR | 0x80,  // 0x3A : (opr 8)
    DELIMITER | 0x30, // 0x3B ; (dlm 3)
    OPERATOR | 0x90,  // 0x3C < (opr 9)
    OPERATOR | 0xA0,  // 0x3D = (opr 10)
    OPERATOR | 0xB0,  // 0x3E > (opr 11)
    OPERATOR | 0xC0,  // 0x3F ? (opr 12)

    UNDEFINED, // 0x40 @

    IDENTIFIER | 0x10, // 0x41 A
    IDENTIFIER | 0x10, // 0x42 B
    IDENTIFIER | 0x10, // 0x43 C
    IDENTIFIER | 0x10, // 0x44 D
    IDENTIFIER | 0x10, // 0x45 E
    IDENTIFIER | 0x10, // 0x46 F
    IDENTIFIER | 0x10, // 0x47 G
    IDENTIFIER | 0x10, // 0x48 H
    IDENTIFIER | 0x10, // 0x49 I
    IDENTIFIER | 0x10, // 0x4A J
    IDENTIFIER | 0x10, // 0x4B K
    IDENTIFIER | 0x10, // 0x4C L
    IDENTIFIER | 0x10, // 0x4D M
    IDENTIFIER | 0x10, // 0x4E N
    IDENTIFIER | 0x10, // 0x4F O
    IDENTIFIER | 0x10, // 0x50 P
    IDENTIFIER | 0x10, // 0x51 Q
    IDENTIFIER | 0x10, // 0x52 R
    IDENTIFIER | 0x10, // 0x53 S
    IDENTIFIER | 0x10, // 0x54 T
    IDENTIFIER | 0x10, // 0x55 U
    IDENTIFIER | 0x10, // 0x56 V
    IDENTIFIER | 0x10, // 0x57 W
    IDENTIFIER | 0x10, // 0x58 X
    IDENTIFIER | 0x10, // 0x59 Y
    IDENTIFIER | 0x10, // 0x5A Z

    DELIMITER | 0x40, // 0x5B [ (dlm 4)
    BACK_SLASH,       // 0x5C \ (back slash)
    DELIMITER | 0x50, // 0x5D ] (dlm 5)
    OPERATOR | 0xD0,  // 0x5E ^ (opr 13)

    IDENTIFIER | 0x00, // 0x5F _
    LITERAL_CHAR,      // 0x60 '

    IDENTIFIER | 0x20, // 0x61 a
    IDENTIFIER | 0x20, // 0x62 b
    IDENTIFIER | 0x20, // 0x63 c
    IDENTIFIER | 0x20, // 0x64 d
    IDENTIFIER | 0x20, // 0x65 e
    IDENTIFIER | 0x20, // 0x66 f
    IDENTIFIER | 0x20, // 0x67 g
    IDENTIFIER | 0x20, // 0x68 h
    IDENTIFIER | 0x20, // 0x69 i
    IDENTIFIER | 0x20, // 0x6A j
    IDENTIFIER | 0x20, // 0x6B k
    IDENTIFIER | 0x20, // 0x6C l
    IDENTIFIER | 0x20, // 0x6D m
    IDENTIFIER | 0x20, // 0x6E n
    IDENTIFIER | 0x20, // 0x6F o
    IDENTIFIER | 0x20, // 0x70 p
    IDENTIFIER | 0x20, // 0x71 q
    IDENTIFIER | 0x20, // 0x72 r
    IDENTIFIER | 0x20, // 0x73 s
    IDENTIFIER | 0x20, // 0x74 t
    IDENTIFIER | 0x20, // 0x75 u
    IDENTIFIER | 0x20, // 0x76 v
    IDENTIFIER | 0x20, // 0x77 w
    IDENTIFIER | 0x20, // 0x78 x
    IDENTIFIER | 0x20, // 0x79 y
    IDENTIFIER | 0x20, // 0x7A z

    DELIMITER | 0x60, // 0x7B { (dlm 6)
    OPERATOR | 0xE0,  // 0x7C | (opr 14)
    DELIMITER | 0x70, // 0x7D } (dlm 7)
    OPERATOR | 0xF0,  // 0x7E ~ (opr 15)

    UNDEFINED, // 0x7F DEL (delete)
};

// --> @ abstract :
//  use these to convert category, do not convert it by yourself.
constexpr uint8_t to_high(uint8_t __n) noexcept { return __n << 4; }
constexpr uint8_t to_low(uint8_t __n) noexcept { return __n >> 4; }
constexpr uint8_t get_high(uint8_t __n) noexcept { return __n >> 4; }
constexpr uint8_t get_low(uint8_t __n) noexcept { return 0x0f & __n; }

// --> @ abstract :
//  use it to get automat category
constexpr uint8_t get_automat_category(char __c) noexcept {
    if (__c < 0 || __c > 127)
        return AUTOMAT_CATEGORY::UNDEFINED;
    return get_low(ascii_convert[__c]);
}

// --> @ abstract :
//  use it to get sub category
// --> @ warning :
//  do not use __c < 0 or __c > 127 , it is undefined.
constexpr uint8_t get_sub_category(char __c) noexcept {
    return get_high(ascii_convert[__c]);
}

_CML_END_ // namespace cml end

#endif // _ZCC_CONVERT_H_