#include "MessageObjects.hh"

MessageObjects::MessageObjects(const QString &msg)
    : MessageBase(MessageBase::INFO_OBJECTS, msg),
      _elements(nullptr)
{
    QTextStream         stream(msg.toUtf8());
    qint32              type;
    quint32             objectsCount;
    quint32             pointsCount;
    qint32              objectType;
    quint32             shipNumber;
    qint32              x;
    qint32              y;

    stream >> type;
    assert(type == (qint32) MessageBase::INFO_OBJECTS);

    /* Stream: "" */
    // Initialize vector size with objects count
    stream >> objectsCount;
    _elements = QSharedPointer<QVector<Element>>(new QVector<Element>(objectsCount));

    for (quint32 i = 0; i < objectsCount; ++i)
    {
        // Initialize a element with pointsCount points
        stream >> objectType >> shipNumber >> pointsCount;

        QPolygon    polygon(pointsCount);

        for (quint32 j = 0; j < pointsCount; ++j)
        {
            // For each pair of coordinate, add point to the polygon
            stream >> x >> y;
            polygon[j] = QPoint(x, y);
        }

        _elements->insert(i, Element((Element::Type) shipNumber, polygon));
    }
}

MessageObjects::MessageObjects(const EntityHash &entities)
    : MessageBase(MessageBase::INFO_OBJECTS, ""),
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

        case Entity::CARRE:
            serializeCarre(dynamic_cast<Carre&>(*entity));
            break;

        case Entity::MINE:

            break;

        default:
            DEBUG("MessageObjects::MessageObjects() : Unknown entity" << entity->type(), true);
            assert(false);
            break;
        }
    }
}

MessageObjects::~MessageObjects()
{
}

const QSharedPointer<QVector<Element>> &MessageObjects::elements() const
{
    return _elements;
}

void MessageObjects::serializeShip(const Ship &ship)
{
    /* Write ship number */
    _messageString += QString::number(ship.shipNumber()) + " ";

    /* Write points count */
    _messageString += QString::number(ship.count()) + " ";

    /* Write coordinates for each point */
    for (const QPoint &point : ship)
    {
        _messageString += QString::number(point.x()) + " ";
        _messageString += QString::number(point.y()) + " ";
    }
}

void MessageObjects::serializeCarre(const Carre &carre)
{
    /* Write color carre */
    _messageString += QString::number(carre.color()) + " ";

    /* Write points count */
    _messageString += QString::number(carre.count()) + " ";

    /* Write coordinates for each point */
    for (const QPoint &point : carre)
    {
        _messageString += QString::number(point.x()) + " ";
        _messageString += QString::number(point.y()) + " ";
    }
}

