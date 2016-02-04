#ifndef DISPLAY_HH
# define DISPLAY_HH

# include <QObject>
# include <QTimer>
# include <QSize>
# include <QMutex>
# include <QPainter>
# include <QBrush>
# include <QVector>
# include <QPolygon>
# include <QApplication>

# include "Minestorm.hh"
# include "MessageFactory.hpp"
# include "Client.hh"

class Display : public QObject
{
    Q_OBJECT

private:
    bool                _isRunning;
    const QSize         _size;
    QSharedPointer<Client>              _client;
    QSharedPointer<QVector<QPolygon>>   _objects;

public:
    Display(const QSize &size, QObject *parent = nullptr);

    void                draw(QPainter &painter, QRect &size);
    void                startDisplay();

    // Events triggered from Gameboard
    void                mousePressed(qint32 x, qint32 y);
    void                mouseReleased(qint32 x, qint32 y);
    void                keyPressed(qint32 key);
    void                keyReleased(qint32 key);

    // Events triggered from Controller
    void                startNewGame();
    void                joinGame(const QString &host);
    void                exitGame();


    const QSize         &size() const;
    bool                isRunning() const;
    const QVector<QPolygon>  &objects() const;


signals:
    void                changed();

public slots:
    void                receiveObjects(const QSharedPointer<QVector<QPolygon>> &objects);

private slots:
    void                messageDispatcher(qint32 socketFd, const QString &msg);
};

#endif // DISPLAY_HH
