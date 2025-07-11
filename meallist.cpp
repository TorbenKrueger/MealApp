#include "meallist.h"

#include <qdebug.h>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonArray>

#include <QTimer>

#include <QQmlFile>

#include "mealitem.h"
#include "basicclass.h"
#include "tcpsocket.h"
#include "serverconnection.h"

#include "QFile"

MealList::MealList(QObject *parent) : QObject(parent)
{
    mSaveTimer = new QTimer(this);
    mSaveTimer->setSingleShot(true);
    mSaveTimer->setInterval(250); // Gerichte werden erst nach 250ms ohne weitere Änderungen gespeichert
    connect(mSaveTimer, &QTimer::timeout, this, [=](){saveAfterTimeout();});
    loadMeals();

    QSettings settings("krueger", "mealApp");
    const auto orderS = settings.value("orderTypeName").toString();
    const auto asc = settings.value("orderTypeAsc").toBool();
    order(orderS, asc);

    mStyleMode = settings.value("styleMode").toInt();
    mPriColor = settings.value("priCol").toString();
    mSecColor = settings.value("secCol").toString();
    mTextColor = settings.value("textCol").toString();
    mHighColor = settings.value("highCol").toString();

    emit styleChanged(mStyleMode);
    connect(this, &MealList::styleChanged, this, &MealList::saveStyle);

    loadCategories();

    connect(this, &MealList::styleChanged, this, [=]() {qDebug() << "emit MealList::styleChanged";});
    connect(this, &MealList::propertyChanged, this, [=]() {qDebug() << "emit MealList::propertyChanged";});
    connect(this, &MealList::preItemAppended, this, [=]() {qDebug() << "emit MealList::preItemAppended";});
    connect(this, &MealList::postItemAppended, this, [=]() {qDebug() << "emit MealList::postItemAppended";});
    connect(this, &MealList::preItemRemoved, this, [=]() {qDebug() << "emit MealList::preItemRemoved";});
    connect(this, &MealList::postItemRemoved, this, [=]() {qDebug() << "emit MealList::postItemRemoved";});
    connect(this, &MealList::removedItem, this, [=]() {qDebug() << "emit MealList::removedItem";});
    connect(this, &MealList::error, this, [=]() {qDebug() << "emit MealList::error";});
    connect(this, &MealList::countChanged, this, [=]() {qDebug() << "emit MealList::countChanged";});
    connect(this, &MealList::backupsChanged, this, [=]() {qDebug() << "emit MealList::backupsChanged";});
    connect(this, &MealList::orderAscChanged, this, [=]() {qDebug() << "emit MealList::orderAscChanged";});
    connect(this, &MealList::orderTypeChanged, this, [=]() {qDebug() << "emit MealList::orderTypeChanged";});
}

QVector<MealItem*> MealList::items() const
{
    return mItems;
}

bool MealList::setItemAt(int index, MealItem *item)
{
    if (index < 0 || index >= mItems.size())
        return false;

    MealItem *oldItem = mItems.at(index);

    if (oldItem && oldItem->uuid() == item->uuid())
        return false;

    mItems[index] = item;
    return true;
}

MealItem* MealList::createAndAppend(QString name)
{
    auto item = new MealItem(this);
    item->setUuid(BasicClass::createUUID(name.isEmpty() ? "NoNameGiven" : name));
    item->setName(name);

    appendItem(item);

    saveMeals();
    return item;
}

bool MealList::appendItem(MealItem *item)
{
    for (const auto &tmp : mItems)
        if (tmp->uuid() == item->uuid())
            return false;

    emit preItemAppended();
    mItems.append(item);

    connect(item, &MealItem::dataChanged, this, [=]()
    {
        emit dataChanged(indexOfItem(item));
        saveMeals();
    });

    connect(item, &MealItem::propertiesChanged, this, [=]()
    {
        emit dataChanged(indexOfItem(item));
        emit propertyChanged(item->uuid());
        saveMeals();
    });

    connect(item, &MealItem::toRemove, this, [=](MealItem *item)
    {
        const int index = indexOfItem(item);
        const auto uuid = item ? item->uuid() : "";
        emit removedItem(uuid);

        delete item;

        QMetaObject::invokeMethod(this, "removeItem", Qt::QueuedConnection, Q_ARG(int, index));
        QMetaObject::invokeMethod(this, "saveMeals", Qt::QueuedConnection);
    }, Qt::QueuedConnection);

    connect(item, &MealItem::saveAll, this, [=]()
    {
        saveMeals();
    });

    emit postItemAppended();
    emit countChanged(mItems.count());
    return true;
}

int MealList::indexOfItem(MealItem *item)
{
    if (!item)
        return -1;

    for (int i = 0; i < mItems.length(); i++)
        if (mItems.at(i)->uuid() == item->uuid())
            return i;
    return -1;
}

void MealList::removeAllItems()
{
    while (mItems.size()) {
        emit preItemRemoved(0);
        mItems.remove(0);
        emit postItemRemoved();
    }
}

void MealList::removeItem(int index)
{
    emit preItemRemoved(index);
    mItems.remove(index);
    emit postItemRemoved();

    emit countChanged(mItems.count());
}

void MealList::removeItem(MealItem *item)
{
    removeItem(indexOfItem(item));
}

void MealList::loadMeals(const QString &location, const QString &doc)
{
    QSettings settings("krueger", "mealApp");

    QJsonArray array;

    QString _location = location;
    if (location.isEmpty())
        _location = "meals";

    auto tmp = settings.value(_location).toByteArray();
    if(!doc.isEmpty()) {
        tmp = doc.toLatin1();
    }
    array = QJsonDocument::fromJson(tmp).array();

    qDebug() << tmp;

    removeAllItems();

    for (const auto &tmp : array)
    {
        qDebug() << tmp;
        auto obj = tmp.toObject();

        if (obj["mealName"].toString().isEmpty())
            continue;

        auto uuid = obj["uuid"].toString();

        if (uuid.isEmpty()) {
            uuid = BasicClass::createUUID(obj["mealName"].toString());
            obj["uuid"] = uuid;
        }

        qDebug() << "create Item";

        MealItem *item = new MealItem(obj, this);
        appendItem(item);
    }
}

void MealList::saveMeals(const QString &location)
{
    if (location.isEmpty())
        qDebug() << "Save Timer started";

    if (location.isEmpty())
        mSaveTimer->start();
    else
        saveAfterTimeout(location);
}

void MealList::saveAfterTimeout(const QString &location)
{
    QJsonDocument doc;
    QJsonArray array;

    for (const auto &item : mItems)
    {
        if (item->name().isEmpty())
            continue;

        const auto obj = item->getJsonObject();
        if (obj.isEmpty()) {
            qDebug() << "Fehler im JSON1";
            continue;
        }

        QJsonDocument tmpDoc;
        tmpDoc.setObject(obj);

        auto jsonText = QString(tmpDoc.toJson());
        jsonText.remove(" ");
        jsonText.remove("\n");
        jsonText.remove("\t");
        jsonText.remove(" ");
        if (jsonText.isEmpty()) {
            qDebug() << "Fehler im JSON2";
            continue;
        }

        array.append(obj);
    }

    doc.setArray(array);

    const auto data = doc.toJson();

    QString tmpData = data;
    tmpData.remove(" ");
    tmpData.remove("\n");
    tmpData.remove("\t");
    tmpData.remove(" ");
    tmpData.remove("{");
    tmpData.remove("}");
    tmpData.remove("[");
    tmpData.remove("]");
    if (tmpData.isEmpty()) {
        qDebug() << "Fehler im JSON3";
        return;
    }

    QSettings settings("krueger", "mealApp");
    if (location.isEmpty()) {

        const auto oldData = settings.value("meals").toByteArray();

        settings.setValue("meals", data);
        const auto tmp = settings.value("meals").toByteArray();

        QJsonArray loadedArray;

        const auto loadedTmp = settings.value("meals").toByteArray();
        loadedArray = QJsonDocument::fromJson(loadedTmp).array();

        qDebug() << "loaded" <<loadedArray.size();

        int counter = 0;
        for (const auto &tmp : loadedArray)
        {
            auto loadedObj = tmp.toObject();

            if (loadedObj["mealName"].toString().isEmpty())
                continue;

            auto uuid = loadedObj["uuid"].toString();

            if (uuid.isEmpty())
                continue;

            counter ++;
        }

        if (counter != mItems.length()) {

            qDebug() << counter << mItems.length();
            for (auto &item: mItems)
                qDebug() << item->name();

            qDebug("---");

            qDebug() << tmp;
            qDebug() << loadedTmp;

            settings.setValue("meals", oldData);
            qDebug() << "Es wurde ein Fehler beim speichern festgestellt: Ungleich anzahl an Gerichten";
            emit error("Es wurde ein Fehler beim speichern festgestellt");
        }

        else if (data != tmp) {
            settings.setValue("meals", oldData);
            qDebug() << "Es wurde ein Fehler beim speichern festgestellt: Ungleiche Daten";
            emit error("Es wurde ein Fehler beim speichern festgestellt");
        }
        else {
            settings.setValue("autoBackup_" + QDate::currentDate().toString(BasicClass::dateFormat()), oldData);
        }
    }
    else {
        settings.setValue(location, data);
    }
}

void MealList::order(const QString &type, bool asc)
{
    OrderTypes orderType;

    if (type.toLower() == "name")
    {
        orderType = OrderTypes::Name;
    }
    else if (type.toLower() == "creationdate")
    {
        orderType = OrderTypes::CreationDate;
    }
    else if (type.toLower() == "lastcooked")
    {
        orderType = OrderTypes::LastCooked;
    }
    else if (type.toLower() == "rating")
    {
        orderType = OrderTypes::Rating;
    }
    else {
        return;
    }

    mOrderType = type;
    mOrderAsc = asc;

    saveOrderTypes();

    sortMeals(orderType, asc);
}

void MealList::createBackup(const QString &name)
{
    auto list = backups();
    if (!list.contains(name)) {
        list.append(name);
        setBackups(list);
    }

    saveMeals(name);
}
void MealList::loadBackup(const QString &name)
{
    loadMeals(name);
    saveMeals();
}

MealItem* MealList::getItemByUuid(const QString &uuid)
{
    for (int i = 0; i < mItems.length(); i++)
        if (mItems.at(i)->uuid() == uuid)
            return mItems.at(i);

    return nullptr;
}

int MealList::count()
{
    return mItems.length();
}

void MealList::saveStyle(int style)
{
    Q_UNUSED(style);

    QSettings settings("krueger", "mealApp");
    settings.setValue("styleMode", mStyleMode);
    settings.setValue("priCol", mPriColor);
    settings.setValue("secCol", mSecColor);
    settings.setValue("textCol", mTextColor);
    settings.setValue("highCol", mHighColor);
}

int MealList::styleMode() const
{
    return mStyleMode;
}
void MealList::setStyleMode(int mode)
{
    if (mStyleMode != mode) {
        mStyleMode = mode;
        emit styleChanged(mStyleMode);
    }
}

QString MealList::primaryColor() const
{
    return mPriColor;
}
void MealList::setPrimaryColor(const QString &color)
{
    if (mPriColor != color) {
        mPriColor = color;
        emit styleChanged(mStyleMode);
    }
}

QString MealList::secondaryColor() const
{
    return mSecColor;
}
void MealList::setSecondaryColor(const QString &color)
{
    if (mSecColor != color) {
        mSecColor = color;
        emit styleChanged(mStyleMode);
    }
}

QString MealList::textColor() const
{
    return mTextColor;
}
void MealList::setTextColor(const QString &color)
{
    if (mTextColor != color) {
        mTextColor = color;
        emit styleChanged(mStyleMode);
    }
}

QString MealList::highlightColor() const
{
    return mHighColor;
}
void MealList::setHighlightColor(const QString &color)
{
    if (mHighColor != color) {
        mHighColor = color;
        emit styleChanged(mStyleMode);
    }
}

void MealList::sortMeals(OrderTypes type, bool asc)
{
    QStringList uuids;
    if (type == OrderTypes::Name) {
        QList<QPair<QString, MealItem*>> list;
        QPair<QString, MealItem*> pair;

        for (int i = 0; i < mItems.length(); i++) {
            const auto item = mItems.at(i);
            pair.first = item->name().toLower();
            pair.second = item;
            list.append(pair);
        }
        std::sort(list.begin(), list.end());

        while (mItems.size()) {
            emit preItemRemoved(0);
            mItems.remove(0);
            emit postItemRemoved();
        }

        mItems.clear();

        for (int i = !asc ? 0 : list.length() - 1; !asc ? i < list.length() : i >= 0 ; !asc ? i++ : i--) {
            emit preItemAppended();
            mItems.append(list.at(i).second);
            emit postItemAppended();
        }
    }
    else if (type == OrderTypes::CreationDate) {
        QList<QPair<QDate, MealItem*>> list;
        QPair<QDate, MealItem*> pair;

        for (int i = 0; i < mItems.length(); i++) {
            const auto item = mItems.at(i);
            pair.first = QDate::fromString(item->creationDate(), BasicClass::dateFormat());
            pair.second = item;
            list.append(pair);
        }

        std::sort(list.begin(), list.end());

        while (mItems.size()) {
            emit preItemRemoved(0);
            mItems.remove(0);
            emit postItemRemoved();
        }

        mItems.clear();

        for (int i = asc ? 0 : list.length() - 1; asc ? i < list.length() : i >= 0 ; asc ? i++ : i--) {
            emit preItemAppended();
            mItems.append(list.at(i).second);
            emit postItemAppended();
        }
    }
    else if (type == OrderTypes::LastCooked) {
        QList<QPair<QDate, MealItem*>> list;
        QPair<QDate, MealItem*> pair;
        for (int i = 0; i < mItems.length(); i++) {
            const auto item = mItems.at(i);
            pair.first = QDate::fromString(item->lastCooked(), BasicClass::dateFormat());
            pair.second = item;
            list.append(pair);
        }

        std::sort(list.begin(), list.end());

        while (mItems.size()) {
            emit preItemRemoved(0);
            mItems.remove(0);
            emit postItemRemoved();
        }

        mItems.clear();

        for (int i = asc ? 0 : list.length() - 1; asc ? i < list.length() : i >= 0 ; asc ? i++ : i--) {
            emit preItemAppended();
            mItems.append(list.at(i).second);
            emit postItemAppended();
        }
    }
    else if (type == OrderTypes::Rating) {
        QList<QPair<double, MealItem*>> list;
        QPair<double, MealItem*> pair;

        for (int i = 0; i < mItems.length(); i++) {
            const auto item = mItems.at(i);
            pair.first = item->rating();
            pair.second = item;
            list.append(pair);
        }

        std::sort(list.begin(), list.end());

        while (mItems.size()) {
            emit preItemRemoved(0);
            mItems.remove(0);
            emit postItemRemoved();
        }

        mItems.clear();

        for (int i = asc ? 0 : list.length() - 1; asc ? i < list.length() : i >= 0 ; asc ? i++ : i--) {
            emit preItemAppended();
            mItems.append(list.at(i).second);
            emit postItemAppended();
        }
    }
}

QString MealList::orderType() const
{
    return mOrderType;
}
void MealList::setOrderType(const QString &type)
{
    mOrderType = type;
    emit orderAscChanged();
}

bool MealList::orderAsc() const
{
    return mOrderAsc;
}
void MealList::setOrderAsc(bool asc)
{
    mOrderAsc = asc;
    emit orderTypeChanged();
}

void MealList::saveOrderTypes()
{
    QSettings settings("krueger", "mealApp");
    settings.setValue("orderTypeName", mOrderType);
    settings.setValue("orderTypeAsc", mOrderAsc);
}

MealItem* MealList::item(int index)
{
    if (index < 0)
        index = 0;
    if (index >= mItems.length())
        index = mItems.length() - 1;

    const auto item = mItems.at(index);
    return item;
}

QStringList MealList::backups()
{
    return mBackups;
    if (mBackups.isEmpty()) {
        QSettings settings("krueger", "mealApp");
        const auto data = settings.value("backupNames").toByteArray();
        QJsonDocument doc;
        doc.fromBinaryData(data);
        const auto ary = doc.array();
        for (int i = 0; i < ary.count(); i++)
            mBackups.append(ary.at(i).toString());
    }
    return mBackups;
}
void MealList::setBackups(const QStringList &list)
{
    return;
    QJsonArray l;

    for (int i = 0; i < list.length(); i++)
        l.append(list.at(i));

    QJsonDocument doc;
    doc.setArray(l);

    const auto data = doc.toJson();

    QSettings settings("krueger", "mealApp");
    settings.value("backupNames", data);

    qDebug() << settings.value("backupNames") << settings.value("backupNames").toString();

    mBackups.append(list);

    emit backupsChanged(list);
}

bool MealList::lunch() const
{
    return mLunch;
}
void MealList::setLunch(bool b)
{
    if (mLunch != b) {
        mLunch = b;
        saveCategories();
        emit lunchChanged(b);
    }
}

bool MealList::dessert() const
{
    return mDessert;
}
void MealList::setDessert(bool b)
{
    if (mDessert != b) {
        mDessert = b;
        saveCategories();
        emit dessertChanged(b);
    }
}

bool MealList::vegetarian() const
{
    return mVegetarian;
}
void MealList::setVegetarian(bool b)
{
    if (mVegetarian != b) {
        mVegetarian = b;
        saveCategories();
        emit vegetarianChanged(b);
    }
}

bool MealList::canVegetarian() const
{
    return mCanVegetarian;
}
void MealList::setCanVegetarian(bool b)
{
    if (mCanVegetarian != b) {
        mCanVegetarian = b;
        saveCategories();
        emit canVegetarianChanged(b);
    }
}

void MealList::loadCategories()
{
    QSettings settings("krueger", "mealApp");
    setLunch(settings.value("lunch").toBool());
    setDessert(mDessert = settings.value("dessert").toBool());
    setVegetarian(mVegetarian = settings.value("vegetarian").toBool());
    setCanVegetarian(mCanVegetarian = settings.value("canVegetarian").toBool());

    qDebug() << "load" << mLunch << mDessert << mVegetarian << mCanVegetarian;
}
void MealList::saveCategories()
{
    QSettings settings("krueger", "mealApp");
    settings.setValue("lunch", mLunch);
    settings.setValue("dessert", mDessert);
    settings.setValue("vegetarian", mVegetarian);
    settings.setValue("canVegetarian", mCanVegetarian);

    qDebug()<< "Save" << mLunch << mDessert << mVegetarian << mCanVegetarian;
}

QString MealList::formatFile(const QString &file)
{
    qDebug() << "formatFile" << file;
    if (file.startsWith("content://com.android.providers.")) {
        const auto l = file.split("%3A");
        const auto newFile = "content://media/external/images/media/" + l[1];
        qDebug() << "new" << newFile;
        return newFile;
    }
    return file;
}

void MealList::resetAll()
{
    QSettings settings("krueger", "mealApp");
    settings.setValue("backupNames", "");

    settings.setValue("orderTypeName", "");
    settings.setValue("orderTypeAsc", "");

    settings.setValue("styleMode", "");
    settings.setValue("priCol", "");
    settings.setValue("secCol", "");
    settings.setValue("textCol", "");
    settings.setValue("highCol", "");

    settings.setValue("meals", "");

    settings.setValue("shoppingList", "");

    settings.setValue("weekMeals", "");

    emit close();
}

void MealList::sendAllToServer(const QString &ip, int port)
{
    QString _ip = ip;
    if (ip.isEmpty())
        _ip = mIp;

    int _port = port;
    if (port < 1)
        _port = mPort;

    TcpSocket *socket = new TcpSocket(nullptr, this);
    const auto connected = socket->connectToHost(_ip, _port);

    qDebug() << "Verbunden zu " << _ip << ":" << _port << connected;

    if (connected) {
        mServerConnection = new ServerConnection(socket);
        connect(mServerConnection, &ServerConnection::loadedBackup, this, &MealList::loadedBackup);
        connect(mServerConnection, &ServerConnection::showServerMeal, this, &MealList::showServerMeal);
        connect(mServerConnection, &ServerConnection::addNewMeal, this, [=](QJsonObject obj)
        {
            qDebug() << obj;
        });
        mServerConnection->backup();
    }
    else {
        delete socket;
    }

    if (socket && false)
        socket->disconnectFromHost();
}

void MealList::getAllFromServer(const QString &ip, int port)
{
    QString _ip = ip;
    if (ip.isEmpty())
        _ip = mIp;

    int _port = port;
    if (port < 1)
        _port = mPort;

    TcpSocket *socket = new TcpSocket(nullptr, this);
    const auto connected = socket->connectToHost(_ip, _port);

    qDebug() << "Verbunden zu " << _ip << ":" << _port << connected;

    if (connected) {
        mServerConnection = new ServerConnection(socket);
        //connect(mServerConnection, &ServerConnection::loadedBackup, this, &MealList::loadedBackup);
        mServerConnection->loadBackup();
        connect(mServerConnection, &ServerConnection::loadedBackup, this, &MealList::loadServerBackup);
    }
    else {
        delete socket;
    }

    if (socket && false)
        socket->disconnectFromHost();
}

void MealList::loadBackupFromServer(const QString &backup)
{

}

void MealList::showServerMeal(const QJsonObject &obj)
{
    const auto uuid = obj["uuid"].toString();
    const auto name = BasicClass::umlauts(obj["mealName"].toString());
    const auto creationDate = QDate::fromString(obj["created"].toString(), "dd.MM.yyyy");

    emit newMealFromServer(uuid, name, creationDate.toString(BasicClass::dateFormat()));
}

void MealList::nextServerMeal(const QString &uuid)
{
    if (mServerConnection)
        mServerConnection->next(uuid, false);
}

void MealList::acceptedServerMeal(const QString &uuid)
{
    if (mServerConnection)
        mServerConnection->next(uuid, true);
}

void MealList::loadServerBackup(QJsonArray ary)
{
    qDebug() << "--- LOADED BACKUP FROM SERVER ---";

    qDebug() << ary;

    qDebug() << "---";

    qDebug() << "---";
    QJsonDocument doc;
    doc.setArray(ary);

    QString data = doc.toJson();

    qDebug() << data.right(100);

    qDebug() << "Länge"<<doc.toJson().length();

    createBackup("autoBackupBeforServerLoad");
    loadMeals("", doc.toJson());
    saveMeals();
}
