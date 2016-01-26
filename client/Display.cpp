#include "Display.hh"

Display::Display(const QSize &size, int fps, QObject *parent)
    : QObject(parent),
      _isRunning(false),
      _size(size),
      _fps(fps),
      _client("127.0.0.1", 4242)
{
    _timer.setSingleShot(false);
    /* Le signal timeout() est envoyé toutes les 40ms,
    ** on le connecte à la fonction Display::update()
    ** qui fait appel à GameBoard::paintEvent()
    ** qui fait appel à Display::draw()
    */
    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
}

/* *** */
void Display::draw(QPainter &painter, QRect &size)
{
    (void) size;
    DEBUG("Display::draw() : " << _objects.size() << " objects to draw", 1);
    painter.setPen(QColor(0, 0, 0));
    painter.setBrush(QBrush(QColor(0, 0, 0)));

    _objectsMutex.lock();
    for (auto object : _objects)
    {
        painter.drawConvexPolygon(object);
    }
    _objectsMutex.unlock();
}

void Display::initialize()
{
}

void Display::start()
{
    _timer.start(1000 / _fps); // Répète le timer en fonction des fps
    _isRunning = true;
    DEBUG("Display::start()", 0);
    emit sigStart();
}

void Display::pause()
{
    _timer.stop();
    _isRunning = false;
    DEBUG("Display::pause()", 0);
    emit sigPause();
}

void Display::reset()
{
    pause();
    initialize();
    emit changed();
    DEBUG("Display::reset()", 0);
    emit sigReset();
}

void Display::test()
{
    DEBUG("Display::test()", 1);
    _client.connectServer();

    emit sigTest();
}

void Display::update()
{
    if (_isRunning)
    {
        emit changed();
        DEBUG("Display::update()", 0);
    }
}


/* EVENTS */
void Display::mousePressed(int x, int y)
{
    emit sigMousePressed(x, y);
}

void Display::mouseReleased(int x, int y)
{
    emit sigMouseReleased(x, y);
}

void Display::mouseMoved(int x, int y)
{
    emit sigMouseMoved(x, y);
}

void Display::keyPressed(int key)
{
    emit sigKeyPressed(key);
}

void Display::keyReleased(int key)
{
    emit sigKeyReleased(key);
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
    return _objects;
}

void Display::receiveObjects(const QVector<QPolygon> &objects)
{
    DEBUG("Display::receiveObjects() : " << objects.size() << " objects received", 1);
    _objectsMutex.lock();
    _objects.clear();
    _objects = objects;
    _objectsMutex.unlock();
}
