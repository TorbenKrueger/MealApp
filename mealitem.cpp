#include "mealitem.h"

#include <QFile>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QBuffer>
#include <QDir>
#include <QStandardPaths>

#include "basicclass.h"

MealItem::MealItem(QObject *parent)
{
    setParent(parent);
}

MealItem::MealItem(const QJsonObject &jsonObj, QObject *parent)
{
    mUuid = jsonObj["uuid"].toString();
    mName = BasicClass::umlauts(jsonObj["mealName"].toString());
    mPicturePath = jsonObj["path"].toString();
    mDescription = BasicClass::umlauts(jsonObj["description"].toString());

    mLastCookedDate = QDate::fromString(jsonObj["lastCooked"].toString(), "dd.MM.yyyy");
    mCreationDate = QDate::fromString(jsonObj["created"].toString(), "dd.MM.yyyy");
    mRating = jsonObj["rating"].toDouble();
    mPersCount = jsonObj["persons"].toInt();
    mRotation = jsonObj["rotation"].toInt();
    mCategorie = jsonObj["categorie"].toInt();

    const auto array = jsonObj["properties"].toArray();

    mProperties = new MealProperties(this);

    for (int i = 0; i < array.count(); i++)
    {
        const auto prop = array.at(i).toObject();
        addProperty(prop["quantity"].toDouble(), prop["unit"].toString(),  BasicClass::umlauts(prop["name"].toString()));
    }
    connect(mProperties, &MealProperties::dataChanged, this, &MealItem::propertiesChanged); // erst wenn alles gesetzt wurde
    setParent(parent);

    connect(this, &MealItem::dataChanged, this, [=]() {qDebug() << "emit MealItem::dateChanged";});
    connect(this, &MealItem::toRemove, this, [=]() {qDebug() << "emit MealItem::toRemove";});
    connect(this, &MealItem::descriptionChanged, this, [=]() {qDebug() << "emit MealItem::descriptionChanged";});
    connect(this, &MealItem::picturePathChanged, this, [=]() {qDebug() << "emit MealItem::picturePathChanged";});
    connect(this, &MealItem::lastCookedChanged, this, [=]() {qDebug() << "emit MealItem::lastCookedChanged";});
    connect(this, &MealItem::ratingChanged, this, [=]() {qDebug() << "emit MealItem::ratingChanged";});
    connect(this, &MealItem::personCountChanged, this, [=]() {qDebug() << "emit MealItem::personCountChanged";});
    connect(this, &MealItem::pictureRotationChanged, this, [=]() {qDebug() << "emit MealItem::pictureRotationChanged";});
    connect(this, &MealItem::propertiesChanged, this, [=]() {qDebug() << "emit MealItem::propertiesChanged";});
    connect(this, &MealItem::saveAll, this, [=]() {qDebug() << "emit MealItem::saveAll";});
}

MealItem::MealItem(const QString &uuid, const QString &name, const QString &picturePath, const QString &description, QObject *parent)
{
    setParent(parent);

    mUuid = uuid;
    mName = name;
    mPicturePath = picturePath;
    mDescription = BasicClass::umlauts(description);
}

QString MealItem::uuid() const
{
    return mUuid;
}
void MealItem::setUuid(const QString &uuid)
{
    mUuid = uuid;
    emit uuidChanged(uuid);
    emit dataChanged();
}

QString MealItem::name() const
{
    return mName;
}
void MealItem::setName(const QString &name)
{
    if (mName != name) {
        mName = name;
        emit nameChanged(name);
        emit dataChanged();
    }
}

QString MealItem::picturePath() const
{
    if (mPicturePath.isEmpty())
        return "qrc:/placeholderjpg.jpg";

    if (mPicturePath.startsWith("content://media/external/images/media/"))
        return mPicturePath;
    else
        return "file://" + mPicturePath;
}
void MealItem::setPicturePath(const QString &path)
{
    qDebug() << "Path cahnged" << path;
    if (path != mPicturePath) {
        mPicturePath = path;
        mPicturePath.remove("file://");
        qDebug() << "new Path"<< mPicturePath;
        emit picturePathChanged(mPicturePath);
        emit dataChanged();
    }
}

QString MealItem::description() const
{
    return mDescription;
}
void MealItem::setDescription(const QString &description)
{
    if (mDescription != BasicClass::umlauts(description)) {
        mDescription = BasicClass::umlauts(description);
        emit descriptionChanged(mDescription);
        emit dataChanged();
    }
}

QString MealItem::lastCooked()
{
    return mLastCookedDate.toString(BasicClass::dateFormat());
}
void MealItem::setLastCooked(const QString &date)
{
    setLastCooked(QDate::fromString(date, BasicClass::dateFormat()));
}
void MealItem::setLastCooked(const QDate &date)
{
    if (mLastCookedDate != date) {
        mLastCookedDate = date;
        emit lastCookedChanged(date.toString(BasicClass::dateFormat()));
        emit dataChanged();
    }
}

double MealItem::rating() const
{
    return mRating;
}
void MealItem::setRating(double rating)
{
    if (mRating != rating) {
        mRating = rating;
        emit ratingChanged(rating);
        emit dataChanged();
    }
}

int MealItem::persCount()
{
    return mPersCount;
}
void MealItem::setPersCount(int cnt)
{
    if(mPersCount != cnt) {
        mPersCount = cnt;
        emit personCountChanged(cnt);
        emit dataChanged();
    }
}

QString MealItem::creationDate() const
{
    return mCreationDate.toString(BasicClass::dateFormat());
}

int MealItem::rotation() const
{
    if (mPicturePath.isEmpty())
        return 0;

    return mRotation;
}
void MealItem::setRotation(int rotation)
{
    if (mRotation != rotation) {
        mRotation = rotation;
        emit pictureRotationChanged(rotation);
        emit dataChanged();
    }
}

int MealItem::categorie() const
{
    return mCategorie;
}
void MealItem::setCategorie(int cat)
{
    qDebug() << mCategorie << cat;
    if (mCategorie != cat) {
        mCategorie = cat;
        emit categorieChanged(cat);
    }
}

bool MealItem::addProperty(const QString &text, int persCount)
{
    if (text.isEmpty())
        return false;

    // Liste von Items hionzufügen
    if (text.contains("\n")) {
        const auto list = text.split("\n");
        for (const auto &listText : list)
            addProperty(listText);

        return false;
    }

    const auto list = text.split(QStringLiteral(" "));

    QStringList tmp;

    for (int i = 0; i < list.length(); i++) {
        if (list.at(i).isEmpty())
            continue;

        tmp.append(list.at(i));
    }

    bool ok;
    const double count = tmp.first().toDouble(&ok) / persCount;
    if (!ok) {
        //        showError("Es ist eine ungültige Menge angegeben: '" + tmp.first() + "'");
        return false;
    }

    QString unit = "";
    QString name = "";

    if (tmp.length() == 2) {
        name = tmp.last();
    }
    else if(tmp.length() == 3) {
        unit = tmp.at(1);
        name = tmp.last();
        const auto letter = name.at(0).toUpper();
        name.replace(0, 1, letter);
    }
    else if (tmp.length() == 4) {
        unit = tmp.at(1);
        const auto adj = tmp.at(2).toLower();
        name = tmp.last();

        const auto letter = name.at(0).toUpper();
        name.replace(0, 1, letter);
        name = adj + " " + name;
    }
    else {
        //        showError("Fehlende oder zuviele Angaben: '" + tmp.join(", ") + "'");
        return false;
    }

    return addProperty(count, unit, name);
}
bool MealItem::addProperty(double quantity, const QString &unit, const QString &name)
{
    return mProperties->addProperty("mealProperty_" + BasicClass::createUUID(name), quantity, unit, name);
}
void MealItem::removeProperty(const QString &name)
{
    mProperties->removeProperty(name);
}
void MealItem::removeAllProperties()
{
    mProperties->removeAll();
}

QString MealItem::getAllProperties()
{
    QString text;
    const auto list = getProperties();

    for (const auto &l : list)
        text.append(l.join(" ") + "\n");

    text.remove(text.length() - 2, 2);
    return text;
}

QList<QStringList> MealItem::getProperties()
{
    QList<QStringList> list;

    if (!mProperties)
        return list;

    for (const auto &prop : mProperties->property()) {
        QStringList tmp;
        tmp << QString::number(prop->count()) << prop->unit()<< prop->name();

        list.append(tmp);
    }

    return list;
}

void MealItem::remove()
{
    if (mProperties) {
        mProperties->removeAll();
    }

    emit toRemove(this);
}

bool MealItem::filter(const QString &filter)
{
    return name().contains(filter, Qt::CaseInsensitive);
}

bool MealItem::categorieType(const QList<int> &types)
{
    for (const auto type : types)
        if (type == mCategorie)
            return true;

    return false;
}

void MealItem::remveAllProperties()
{
    mProperties->removeAll();
}

QString MealItem::categorieText()
{
    switch (mCategorie) {
    case 0:
        return "Mittagessen";
    case 1:
        return "Nachtisch";
    case 2:
        return "Vegetarisch";
    case 3:
        return "Geht auch vegetarisch";
    }
}

MealProperties* MealItem::properties()
{
    if (!mProperties) {
        mProperties = new MealProperties(this);
    }
    return mProperties;
}

QJsonObject MealItem::getJsonObject()
{
    QJsonObject obj;

    obj["uuid"] = mUuid;
    if (obj["uuid"].toString() != mUuid) {
        qDebug() << "Fehler beim Speichern: uuid";
        return QJsonObject();
    }

    obj["mealName"] = BasicClass::umlauts(mName, false);
    if (obj["mealName"].toString() != BasicClass::umlauts(mName, false)) {
        qDebug() << "Fehler beim Speichern: mealName";
        return QJsonObject();
    }

    obj["path"] = mPicturePath;
    if (obj["path"].toString() != mPicturePath) {
        qDebug() << "Fehler beim Speichern: path";
        return QJsonObject();
    }

    obj["picData"] = QString(getPictureData(mPicturePath));
    if (obj["path"].toString() != mPicturePath) {
        qDebug() << "Fehler beim Speichern: picData";
        return QJsonObject();
    }

    obj["description"] = BasicClass::umlauts(mDescription, false);
    if (obj["description"].toString() != BasicClass::umlauts(mDescription, false)) {
        qDebug() << "Fehler beim Speichern: description";
        return QJsonObject();
    }

    obj["lastCooked"] = mLastCookedDate.toString("dd.MM.yyyy");
    if (obj["lastCooked"].toString() != mLastCookedDate.toString("dd.MM.yyyy")) {
        qDebug() << "Fehler beim Speichern: lastCooked";
        return QJsonObject();
    }

    obj["created"] = mCreationDate.toString("dd.MM.yyyy");
    if (obj["created"].toString() != mCreationDate.toString("dd.MM.yyyy")) {
        qDebug() << "Fehler beim Speichern: created";
        return QJsonObject();
    }

    obj["rating"] = mRating;
    if (obj["rating"].toDouble() != mRating) {
        qDebug() << "Fehler beim Speichern: rating";
        return QJsonObject();
    }

    obj["persons"] = mPersCount;
    if (obj["persons"].toInt() != mPersCount) {
        qDebug() << "Fehler beim Speichern: persons";
        return QJsonObject();
    }

    obj["rotation"] = mRotation;
    if (obj["rotation"].toInt() != mRotation) {
        qDebug() << "Fehler beim Speichern: rotation";
        return QJsonObject();
    }

    obj["categorie"] = mCategorie;
    if (obj["categorie"].toInt() != mCategorie) {
        qDebug() << "Fehler beim Speichern: categorie";
        return QJsonObject();
    }

    const auto props = getProperties();

    QJsonArray array;
    for (const auto &list : props)
    {
        if (list.length() != 3) {
            qDebug() << "Fehler beim Speichern: Ungültige Länge der Properties";
            continue;
        }

        QJsonObject prop;
        prop["quantity"] = list[0].toDouble();
        if (prop["quantity"].toDouble() != list[0].toDouble()) {
            qDebug() << "Fehler beim Speichern (Property): quantity";
            return QJsonObject();
        }

        prop["unit"] = list[1];
        if (prop["unit"].toString() != list[1]) {
            qDebug() << "Fehler beim Speichern (Property): unit";
            return QJsonObject();
        }

        prop["name"] = BasicClass::umlauts(list[2], false);
        if (prop["name"].toString() != BasicClass::umlauts(list[2], false)) {
            qDebug() << "Fehler beim Speichern (Property): name";
            return QJsonObject();
        }

        array.append(prop);
    }

    obj["properties"] = QJsonValue(array);

    return obj;
}

void MealItem::save()
{
    if (!mTmpPicturePath.isEmpty()) {
        setPicturePath(mTmpPicturePath);
        mTmpPicturePath.clear();
    }
    emit saveAll();
}

void MealItem::savePicture(const QString &path)
{
    if (path == "removePic") {
        setPicturePath("");
        mTmpPicturePath = "";
        return;
    }
    if (path.isEmpty() && !mTmpPicturePath.isEmpty()) {
        QFile file(mTmpPicturePath);
        file.remove();
    }

    mTmpPicturePath = path;
}

void MealItem::update()
{
    emit dataChanged();
    emit picturePathChanged(picturePath());
}

QByteArray MealItem::getPictureData(const QString &path)
{
    qDebug() << "Start" << path;
//    QPixmap pix(700,700);
//    pix.fill(Qt::red);

//    QDir picDir = QDir(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
//    QDir saveDir = QDir(picDir.filePath("mealApp"));
//    if(!saveDir.exists())
//        picDir.mkdir("mealApp");

//    QString filePath = saveDir.filePath("test.jpg");
//    pix.save(filePath);

//    qDebug() << "end";
//    return QByteArray();

    QImage image("/storage/emulated/0/DCIM/IMG_00000004.jpg");


    QByteArray ba;
    QBuffer bu(&ba);

    image.save(&bu, "jpg");

    QString imgBase64 = ba.toBase64();
    qDebug() << "imgBase64: " << imgBase64;
    return QByteArray();
}
