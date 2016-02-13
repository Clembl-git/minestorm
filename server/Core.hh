#ifndef CORE_HH
# define CORE_HH

# include <QObject>
# include <QTime>
# include <QTimer>
# include <QPoint>
# include <QVector>
# include <QPolygon>
# include <QString>
# include <QTextStream>

# include <algorithm>

# include "Minestorm.hh"
# include "Server.hh"
# include "Ship.hh"
# include "Mine.hh"
# include "Projectile.hh"
# include "MessageMouse.hh"
# include "MessageKey.hh"
# include "MessageObjects.hh"
# include "MessageScore.hh"
# include "MessageLives.hh"
# include "Collision.hh"
# include "Players.hh"

class Core : public QObject
{    
    Q_OBJECT

private:
    bool                _isRunning;
    const qint32        _cps;
    QTimer              _timer;
    qint32              _step;
    Server              _server;
    quint32             _uniqId;
    EntityHash          _entitiesMap;
    EntityVector        _entitiesToDelete;
    Players             _players;
    QHash<Entity::Type, QList<QSharedPointer<Entity>>>   _entities;

public:
    Core(qint32 cps = CYCLE_PER_S);

    void                mousePressed(qint32 idClient, qint32 x, qint32 y);
    void                keyPressed(qint32 idClient, qint32 key);
    void                keyReleased(qint32 idClient, qint32 key);
    void                startGame();
    // Call from Ship::addScore()
    void                scoreChanged(qint32 idClient, quint32 score);

private:
    void                initMines();
    void                removeEntitiesToDelete();
    quint32             getID();

private slots:
    void                step();
    void                messageDispatcher(qint32 idClient, const QString &msg);
    void                clientJoin(qint32 idClient);
    void                clientLeft(qint32 idClient);
};

#endif // CORE_HH
