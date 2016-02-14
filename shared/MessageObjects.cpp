#include "MessageObjects.hh"

/* DESERIALIZE ***************************************************************/
MessageObjects::MessageObjects(const QString &msg)
    : MessageBase(MessageBase::OBJECTS, msg),
      _elements(new QVector<Element>(0))
{
    QTextStream         stream(msg.toUtf8());
    qint32              messageType;
    quint32             objectsCount;
    qint32              objectType;

    /* Read  message type*/
    stream >> messageType;
    assert(messageType == (qint32) MessageBase::OBJECTS);
    DEBUG("MessageObjects::MessageObjects() : Message:" << msg, false);

    /* Read objects count */
    stream >> objectsCount;

    for (quint32 i = 0; i < objectsCount; ++i)
    {
        /* Read object type */
        stream >> objectType;

        switch (objectType)
        {
        case Entity::SHIP:
            deserializeShip(stream);
            break;
        case Entity::SHOT:
            deserializeShot(stream);
            break;
        case Entity::MINE:
            deserializeMine(stream);
            break;
        default:
            DEBUG("MessageObjects::MessageObjects() : Unknown entity" << messageType, true);
            break;
        }
    }
}

/* shipNumber angle center_x center_y x1 y1 x2 y2 x3 y3 x4 y4 */
void MessageObjects::deserializeShip(QTextStream &stream)
{
    quint32     shipNumber;
    QPolygon    polygon(4);
    qreal       angle;
    qint32      center_x;
    qint32      center_y;
    qint32      x;
    qint32      y;

    /* Read shipNumber angle center_x center_y */
    stream >> shipNumber >> angle >> center_x >> center_y;

    /* Read 4 points */
    for (quint32 i = 0; i < 4; ++i)
    {
        stream >> x >> y;
        polygon[i] = QPoint(x, y);
    }
    DEBUG("MessageObjects::deserializeShip() : Ship n°" << shipNumber << " angle: " << angle << " center:" << x << y
          , false);
    _elements->push_back(Element((Element::Type) shipNumber,
                         polygon,
                         angle,
                         QPoint(center_x, center_y)));
}

/* x1 y1 x2 y2 playSound */
void MessageObjects::deserializeShot(QTextStream &stream)
{
    QPolygon    polygon(2);
    qint32      x;
    qint32      y;
    qint32      playSound;
    /* Read 2 points */
    for (quint32 i = 0; i < 2; ++i)
    {
        stream >> x >> y;
        polygon[i] = QPoint(x, y);
        stream >> playSound;
        DEBUG("MessageShot::playsound()" << playSound, true);

    }

    x = ((polygon)[0].x() + (polygon)[1].x()) / 2;
    y = ((polygon)[0].y() + (polygon)[1].y()) / 2;
    QPoint center(x, y);

    DEBUG("MessageObjects::deserializeShot()", false);
    _elements->push_back(Element(Element::SHOT, polygon, center));
}

/* type armed center_x center_y */
void MessageObjects::deserializeMine(QTextStream &stream)
{
    QPolygon    polygon(1);
    QPoint      center;
    qint32      type;
    quint32     armed;
    qint32      center_x;
    qint32      center_y;

    stream >> type >> armed >> center_x >> center_y;
    center = QPoint(center_x, center_y);
    polygon[0] = center;
    DEBUG("MessageObjects::deserializeMine() : type: " << type << " armed: " << armed << " center:" << center_x << center_y, false);
    _elements->push_back(Element((Element::Type) type, polygon, armed, center));
}

/* SERIALIZE *******************************************************************/
MessageObjects::MessageObjects(const EntityList &entities)
    : MessageBase(MessageBase::OBJECTS, ""),
      _elements(nullptr)
{
    /* Write message type */
    _messageString = QString::number((qint32) _type) + " ";

    /* Write total objects count */
    _messageString += QString::number(entities.count()) + " ";

    /* Write each object */
    for (const QSharedPointer<Entity> &entity : entities)
    {
        /* Write object type */
        _messageString += QString::number((qint32) entity->type()) + " ";

        switch (entity->type())
        {
        case Entity::SHIP:
            serializeShip(dynamic_cast<Ship&>(*entity));
            break;
        case Entity::SHOT:
            serializeShot(dynamic_cast<Projectile&>(*entity));
            break;
        case Entity::MINE:
            serializeMine(dynamic_cast<Mine&>(*entity));
            break;
        default:
            DEBUG("MessageObjects::MessageObjects() : Unknown entity" << entity->type(), false);
            assert(false);
            break;
        }
    }
}

/* shipNumber angle center_x center_y x1 y1 x2 y2 x3 y3 x4 y4*/
void MessageObjects::serializeShip(const Ship &ship)
{
    /* Write shipNumber */
    _messageString += QString::number(ship.shipNumber()) + " ";

    /* Write angle */
    _messageString += QString::number(ship.angle()) + " ";

    /* Write center */
    _messageString += QString::number((qint32) ship.center().x()) + " ";
    _messageString += QString::number((qint32) ship.center().y()) + " ";

    /* Write 4 points */
    for (quint32 i = 0; i < 4; ++i)
    {
        _messageString += QString::number((qint32) ship[i].x()) + " ";
        _messageString += QString::number((qint32) ship[i].y()) + " ";
    }
}

/* x1 y1 x2 y2 playSound */
void MessageObjects::serializeShot(const Projectile &shot)
{
    /* Write 2 points */
    for (quint32 i = 0; i < 2; ++i)
    {
        _messageString += QString::number((qint32)shot[i].x()) + " ";
        _messageString += QString::number((qint32)shot[i].y()) + " ";
        _messageString += QString::number((bool)shot.playSound() == true ? 1 : 0) + " ";
    }
}

/* type armed center_x center_y */
void MessageObjects::serializeMine(const Mine &mine)
{
    /* Write type */
    _messageString += QString::number(mine.typeMine()) + " ";

    /* Write armed */
    _messageString += QString::number(mine.armed() ? 1 : 0) + " ";

    /* Write center_x */
    _messageString += QString::number(mine.center().x()) + " ";

    /* Write center_y */
    _messageString += QString::number(mine.center().y()) + " ";
}

MessageObjects::~MessageObjects()
{
}

const QSharedPointer<QVector<Element>> &MessageObjects::elements() const
{
    return _elements;
}
