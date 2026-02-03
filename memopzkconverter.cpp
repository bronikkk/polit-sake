#include "memopzkconverter.h"

#include <QMap>

QString MemoPZKConverter::convertToSearchURL(QString prisonerName)
{
    QString result = "https://memopzk.org/search/?q=";

    for (auto character : prisonerName) {
        if (character == '(' || character == ')' || (character >= 'A' && character <= 'Z')
                || (character >= 'a' && character <= 'z')) {
            continue;
        }

        if (character == ' ') {
            result += '+';
            continue;
        }

        result += character;
    }

    if (result.endsWith("+")) {
        result.removeLast();
    }

    return result;
}

QString MemoPZKConverter::convertToURL(QString prisonerName)
{
    static QMap<QChar, QString> transliterationMap{
        {QChar{0x0020}, "-"},   // ␣
        {QChar{0x0410}, "a"},   // А
        {QChar{0x0411}, "b"},   // Б
        {QChar{0x0412}, "v"},   // В
        {QChar{0x0413}, "g"},   // Г
        {QChar{0x0414}, "d"},   // Д
        {QChar{0x0415}, "e"},   // Е
        {QChar{0x0401}, "yo"},  // Ё
        {QChar{0x0416}, "zh"},  // Ж
        {QChar{0x0417}, "z"},   // З
        {QChar{0x0418}, "i"},   // И
        {QChar{0x0419}, "j"},   // Й
        {QChar{0x041A}, "k"},   // К
        {QChar{0x041B}, "l"},   // Л
        {QChar{0x041C}, "m"},   // М
        {QChar{0x041D}, "n"},   // Н
        {QChar{0x041E}, "o"},   // О
        {QChar{0x041F}, "p"},   // П
        {QChar{0x0420}, "r"},   // Р
        {QChar{0x0421}, "s"},   // C
        {QChar{0x0422}, "t"},   // Т
        {QChar{0x0423}, "u"},   // У
        {QChar{0x0424}, "f"},   // Ф
        {QChar{0x0425}, "h"},   // Х
        {QChar{0x0426}, "cz"},  // Ц
        {QChar{0x0427}, "ch"},  // Ч
        {QChar{0x0428}, "sh"},  // Ш
        {QChar{0x0429}, "shh"}, // Щ
        {QChar{0x042A}, ""},    // Ъ
        {QChar{0x042B}, "y"},   // Ы
        {QChar{0x042C}, ""},    // Ь
        {QChar{0x042D}, "e"},   // Э
        {QChar{0x042E}, "yu"},  // Ю
        {QChar{0x042F}, "ya"},  // Я
        {QChar{0x0430}, "a"},   // а
        {QChar{0x0431}, "b"},   // б
        {QChar{0x0432}, "v"},   // в
        {QChar{0x0433}, "g"},   // г
        {QChar{0x0434}, "d"},   // д
        {QChar{0x0435}, "e"},   // е
        {QChar{0x0451}, "yo"},  // ё
        {QChar{0x0436}, "zh"},  // ж
        {QChar{0x0437}, "z"},   // з
        {QChar{0x0438}, "i"},   // и
        {QChar{0x0439}, "j"},   // й
        {QChar{0x043A}, "k"},   // к
        {QChar{0x043B}, "l"},   // л
        {QChar{0x043C}, "m"},   // м
        {QChar{0x043D}, "n"},   // н
        {QChar{0x043E}, "o"},   // о
        {QChar{0x043F}, "p"},   // п
        {QChar{0x0440}, "r"},   // р
        {QChar{0x0441}, "s"},   // с
        {QChar{0x0442}, "t"},   // т
        {QChar{0x0443}, "u"},   // у
        {QChar{0x0444}, "f"},   // ф
        {QChar{0x0445}, "h"},   // х
        {QChar{0x0446}, "cz"},  // ц
        {QChar{0x0447}, "ch"},  // ч
        {QChar{0x0448}, "sh"},  // ш
        {QChar{0x0449}, "shh"}, // щ
        {QChar{0x044A}, ""},    // ъ
        {QChar{0x044B}, "y"},   // ы
        {QChar{0x044C}, ""},    // ь
        {QChar{0x044D}, "e"},   // э
        {QChar{0x044E}, "yu"},  // ю
        {QChar{0x044F}, "ya"}   // я
    };

    QString result = "https://memopzk.org/figurant/";

    for (auto character : prisonerName) {
        if (character == '(') {
            break;
        }

        result += transliterationMap[character];
    }

    if (result.endsWith("-")) {
        result.removeLast();
    }

    return result;
}

QString MemoPZKConverter::getLoadingPage()
{
    return "https://memopzk.donorsupport.co/page/ny_miracle25";
}
