#ifndef UNIT_H
#define UNIT_H

#include <QObject>

class Unit : public QObject
{
    Q_OBJECT
public:
    explicit Unit(QObject *parent = nullptr);

    enum Units{
        noUnitRole,
        emptyRole,
        grammRole,
        kgRole,
        picesRole,
        miliLitreRole,
        litreRole,
        can,
        package,
        glas,
        bottle,
        tl,
        el
    };

    static Unit::Units stringToUnit(const QString &text);
    static QString unitToString(Unit::Units unit);

    static double convertUnit(QPair<Units, double> from, Unit::Units to);
    static double convertUnit(Unit::Units fromUnit, double from, Unit::Units to);
signals:

};

#endif // UNIT_H
