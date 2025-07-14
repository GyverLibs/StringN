This is an automatic translation, may be incorrect in some places. See sources and examples!

# Stringn
Easy and fast static String book
- Static buffer
- Fast and light functions of numbers converting
- support 64-bit numbers
- ~ 5 times faster Arduino string
- on ~ 3 KB easier arduino string

## compatibility
Compatible with all arduino platforms (used arduino functions)

## Content
- [use] (#usage)
- [versions] (#varsions)
- [installation] (# Install)
- [bugs and feedback] (#fedback)

<a id = "USAGE"> </A>

## Usage
## sbuild
Functions for manual assembly of the line in the array

`` `CPP
uint16_t sbuild :: adshar (char, char* buf, int16_t left = -1);
uint16_t sbuild :: Addpstr (const VOID* PSTR, Int16_T LEN, Char* BUF, Int16_T Left = -1);
uint16_t sbuild :: Addpstr (Constr* pstr, char* buf, int16_t left = -1);
uint16_t sbuild :: Addstr (Cost char* str, int16_t len, char* buf, int16_t left = -1);
uint16_t sbuild :: Addstr (const Char* str, char* buf, int16_t left = -1);
uint8_t sbuild :: aduint (uint32_t v, uint8_t base, char* buf, int16_t left = -1);
uint8_t sbuild :: addint (int32_t v, uint8_t base, char* buf, int16_t left = -1);
uint8_t sbuild :: aduint64 (uint64_t v, uint8_t base, char* buf, int16_t left = -1);
uint8_t sbuild :: addint64 (int64_t v, uint8_t base, char* buf, int16_t left = -1);
uint8_t sbuild :: adfloat (float v, uint8_t dec, char* buf, int16_t left = -1);
`` `

The `left` parameter is the remaining place in the buffer, if it is negative - the function ignores the limitation of the length.Example of use:

`` `CPP
uint8_t maxlen = 21;
Chard [maxlen + 1];

uint8_t len = 0;
Len + = sbuild :: adshar ('h', str + leen, maxlen - leen);
LEN + = SBUILD :: Addstr ("Ello", Str + Len, Maxlen - Len);
LEN + = SBUILD :: Addpstr (F ("World!"), Str + Len, Maxlen - Len);
Len + = SBUILD :: Addint (123, 10, Str + Len, Maxlen - Len);
Len + = SBUILD :: Addfloat (456.789, 5, Str + Len, Maxlen - Len);

Serial.println (str);// Hello World!123456.7
`` `

### Stringn
Class String Bilder.A manual size option is available:

`` `CPP
Stringn <max_len>;
`` `

And somewhat fixed:

`` `CPP
String8;
String16;
String24;
String32;
String64;
String128;
String256;
`` `

Class description:

`` `CPP
Stringn ();
Stringn (any_Tip);
Stringn (whole, base);
Stringn (Float, Dec);

Operator+(any_Tip);// mutating!
Operator+= (any_Tip);
Operator = (any_Tip);

Stringn & Add (any_Tip);
Stringn & Add (whole, basy);
Stringn & Add (Float, Dec);
Stringn & Add (line, length);

// Conclusion, access
const char* c_str ();
Operator const Char*();

// New line \ r \ n
Stringn & Rn ();

// Clean
Void Clear ();

// Hash
uint32_t hash ();

// current length
uint16_t Length ();

// Max.length
Uint16_T Capacy ();

// The line is filled (perhaps the text is cut)
Bool ISFull ();
`` `

The tool is conceived for assembling lines and transmission in the functions that take `COST Char*`.For example, in print, to create files (generation of names and paths), generating lines with numerical constants, and so on.Example of use:

`` `CPP
Serial.println (String64 ('H') + "Ello" + F ("World") + 12345 + True);

Serial.println (string32 ("val:") + string8 (3.1415, 3));

SeRial.println (Stringn <20> ("Val:") .add (3.1415, 3));
`` `

Notes:
- the indicated number is the maximum number of readable characters (not counting the terminator)
- Operator `+` - mutating, he changes the original line
- When the chain is added, the first term should be `stringx`.If the line is needed by the second component, we create it empty: `foo (string64 () + 123 +" HELLO ")` `

<a ID = "Versions"> </a>

## versions
- V1.0

<a id = "Install"> </a>
## Installation
- The library can be found by the name ** stringn ** and installed through the library manager in:
- Arduino ide
- Arduino ide v2
- Platformio
- [download the library] (https://github.com/gyverlibs/stringn/archive/refs/heads/main.zip) .Zip archive for manual installation:
- unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
- unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
- unpack and put in *documents/arduino/libraries/ *
- (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries[here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!

<a id = "Feedback"> </a>

## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!

When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code