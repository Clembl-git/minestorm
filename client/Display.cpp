#include "Display.hh"

Display::Display(const QSize &size, int fps, QObject *parent)
    : QObject(parent),
      _isRunning(false),
      _size(size),
      _fps(fps),
      _client(QSharedPointer<Client>(new Client)),
      _objects(nullptr)
{
    DEBUG("Display::Display()", true);
    _timer.setSingleShot(false);
    /* Le signal timeout() est envoyé toutes les 40ms,
    ** on le connecte à la fonction Display::update()
    ** qui fait appel à GameBoard::paintEvent()
    ** qui fait appel à Display::draw()
    */
    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));

    connect(_client.data(),   SIGNAL(transfertMessage(qint32, QString)),
            this,       SLOT(messageDispatcher(qint32,QString)));
}

/* *** */
void Display::draw(QPainter &painter, QRect &size)
{
    (void) size;
    DEBUG("Display::draw() : " << _objects->size() << " objects to draw", false);
    painter.setPen(QColor(0, 0, 0));
    painter.setBrush(QBrush(QColor(0, 0, 0)));

    if (_objects != nullptr)
    {
        _objectsMutex.lock();
        for (auto object : *_objects)
        {
            painter.drawConvexPolygon(object);
        }
        _objectsMutex.unlock();
    }
}

void Display::startDisplay()
{
    _timer.start(1000 / _fps); // Répète le timer en fonction des fps
    _isRunning = true;
}

void Display::initialize()
{
}

void Display::messageDispatcher(qint32 socketFd, const QString &msg)
{
    (void) socketFd;
    DEBUG("Display::messageDispatcher() :" << msg, false);

    MessageBase::Type       msgType = MessageBase::getMessageType(msg);

    switch (msgType)
    {
    case MessageBase::INFO_OBJECTS:
    {
        MessageObjects      message(msg);

        DEBUG("Client::MessageDispatcher() : Receive " << message.objects()->size() << " objects", false);
        receiveObjects(message.objects());
        break;
    }
    default:
    {
        DEBUG("Display::messageDispatcher() : Unknown message" << msg, true);
        break;
    }
    }
}


/* EVENTS */
void Display::mousePressed(int x, int y)
{
    DEBUG("Display::mousePressed() : x = " << x << ", y = " << y, true);
    _client->sendMessage("1 " + QString::number(x) + " " + QString::number(y));
}

void Display::mouseReleased(int x, int y)
{
}

void Display::mouseMoved(int x, int y)
{
}

void Display::keyPressed(int key)
{
}

void Display::keyReleased(int key)
{
}

void Display::startNewGame()
{
    DEBUG("Display::startNewGame()", true);

    _client->start("localhost");
    startDisplay();
}

void Display::joinGame(const QString &host)
{
    DEBUG("Display::joinGame() : Join" << host, true);

    _client->start(host);
    startDisplay();
}

void Display::exitGame()
{
    DEBUG("Display::exitGame()", true);

    _client->stop();

    QApplication::quit();
}

/* GETTERS/SETTERS */

const QSize &Display::size() const
{
    return _size;
}

bool Display::isRunning() const
{
    return _isRunning;
}

const QVector<QPolygon> &Display::objects() const
{
    return *_objects;
}

void Display::receiveObjects(const QSharedPointer<QVector<QPolygon>> &objects)
{
    DEBUG("Display::receiveObjects() : " << objects->size() << " objects received", false);
    _objectsMutex.lock();
    _objects = objects;
    _objectsMutex.unlock();
}

void Display::update()
{
    if (_isRunning)
    {
        DEBUG("Display::Update()", false);
        emit changed();
    }
}
