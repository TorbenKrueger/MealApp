#include "unit.h"

Unit::Unit(QObject *parent) : QObject(parent)
{

}

Unit::Units Unit::stringToUnit(const QString &text)
{
    const auto _text = text.toLower();

    if (_text == "gramm" || _text == "gr"){
        return Units::grammRole;
    }
    else if (_text == "kilo" || _text == "kg") {
        return Units::kgRole;
    }
    else if (_text == "stk" || _text == "stk." || _text == "stück" || _text == "stueck" || _text == "x") {
        return Units::picesRole;
    }
    else if (_text == "mililiter" || _text == "ml") {
        return Units::miliLitreRole;
    }
    else if (_text == "liter" || _text == "l") {
        return Units::litreRole;
    }
    else if (_text == "dose" || _text == "dosen" || _text == "dose(n)") {
        return Units::can;
    }
    else if (_text == "packung" || _text == "packung(en)" || _text == "pck" || _text == "pck.") {
        return Units::package;
    }
    else if (_text == "glas") {
        return Units::glas;
    }
    else if (_text == "flasche") {
        return Units::bottle;
    }
    else if (_text == "tl"|| _text == "teelöffel" || _text == "teeloeffel") {
        return Units::tl;
    }
    else if (_text == "el" || _text == "esslöffel" || _text == "essloeffel") {
        return Units::el;
    }
    else if (_text.isEmpty()) {
        return Unit::Units::emptyRole;
    }
    else {
        return Unit::Units::noUnitRole;
    }
}

QString Unit::unitToString(Unit::Units unit)
{
    if (unit == Units::grammRole) {
        return "gramm";
    }
    else if (unit == Units::kgRole) {
        return "Kilo";
    }
    else if (unit == Units::picesRole) {
        return "Stk.";
    }
    else if (unit == Units::miliLitreRole) {
        return "ml";
    }
    else if (unit == Units::litreRole) {
        return "Liter";
    }
    else if (unit == Units::can) {
        return "Dose(n)";
    }
    else if (unit == Units::package) {
        return "Packung(en)";
    }
    else if (unit == Units::glas) {
        return "Glas";
    }
    else if (unit == Units::bottle) {
        return "Flasche";
    }
    else if (unit == Units::tl) {
        return "TL";
    }
    else if (unit == Units::el) {
        return "EL";
    }
    else if (unit == Units::emptyRole) {
        return "";
    }
    else {
        return "*";
    }
}

double Unit::convertUnit(QPair<Unit::Units, double> from, Unit::Units to)
{
    return convertUnit(from.first, from.second, to);
}

double Unit::convertUnit(Unit::Units fromUnit, double from, Unit::Units to)
{
    if (fromUnit == Units::grammRole && to == Units::kgRole) {
        return from / 1000;
    }
    else if (fromUnit == Units::kgRole && to == Units::grammRole) {
        return from * 1000;
    }
    else if (fromUnit == Units::miliLitreRole && to == Units::litreRole) {
        return from / 1000;
    }
    else if (fromUnit == Units::litreRole && to == Units::miliLitreRole) {
        return from * 1000;
    }
    else {
        return from;
    }
}
