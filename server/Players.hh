#ifndef PLAYERS_HH
# define PLAYERS_HH

# include <QVector>
# include <QSharedPointer>

# include "Player.hh"

# include <cassert>

using PlayersList = QVector<QSharedPointer<Player>>;

class Players : public PlayersList
{
private:
    const quint32           _maxPlayers;
    quint32                 _playersCount;

public:
    Players(quint32 maxPlayers = MAX_PLAYERS);

    QSharedPointer<Entity>  &newPlayer(qint32 idClient);
    void                    deletePlayer(qint32 idClient);

    /* Events */
    void                    keyRight(qint32 idClient);
    void                    keyLeft(qint32 idClient);
    void                    keyUp(qint32 idClient);
    void                    keyDown(qint32 idClient);
    void                    keySpace(qint32 idClient);

    bool                    playerAvailable() const;
    bool                    contains(qint32 idClient) const;
    quint32                 count() const;

private:
    void                    initPlayers();
    QSharedPointer<Player>  &findPlayer(qint32 idClient);
    QSharedPointer<Player>  &findAvailable();
    QSharedPointer<Player>  get(quint32 i);
};

#endif // PLAYERS_HH
