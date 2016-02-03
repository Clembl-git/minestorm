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
# include "Client.hh"

class QPainter;
class QRect;

using namespace std;

class Display : public QObject
{
    Q_OBJECT

private:
    QTimer              _timer;
    bool                _isRunning;
    const QSize         _size;
    const int           _fps;
    QSharedPointer<Client>              _client;
    QSharedPointer<QVector<QPolygon>>   _objects;
    QMutex              _objectsMutex;

public:
    Display(const QSize &size, int fps = 25, QObject *parent = nullptr);

    void                draw(QPainter &painter, QRect &size);
    void                startDisplay();

    // Events triggered from Gameboard
    void                mousePressed(int x, int y);
    void                mouseReleased(int x, int y);
    void                mouseMoved(int x, int y);
    void                keyPressed(int key);
    void                keyReleased(int key);

    // Events triggered from Controller
    void                startNewGame();
    void                joinGame(const QString &host);
    void                exitGame();


    const QSize         &size() const;
    bool                isRunning() const;
    const QVector<QPolygon>  &objects() const;

protected:
    void                initialize();


signals:
    void                changed();

public slots:
    void                receiveObjects(const QSharedPointer<QVector<QPolygon>> &objects);

private slots:
    void                update();
    void                messageDispatcher(qint32 socketFd, const QString &msg);
};

#endif // DISPLAY_HH
