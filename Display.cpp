#include "Display.hh"

Display::Display(const QSize &size, QObject *parent)
    : QObject(parent),
      _isRunning(false),
      _size(size)
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
    painter.setPen(QColor(236, 248, 230));
    painter.setBrush(QBrush(QColor(236, 248, 230)));

    _objectsMutex.lock();
    for (auto object : _objects)
    {
        painter.drawConvexPolygon(*object);
    }
    _objectsMutex.unlock();
}

void Display::initialize()
{
}

void Display::start()
{
    _timer.start(40); // Répète le timer toutes les 40ms (25 fps)
    _isRunning = true;
    DEBUG("start()");
}

void Display::pause()
{
    _timer.stop();
    _isRunning = false;
    DEBUG("pause()");
}

void Display::reset()
{
    pause();
    initialize();
    emit changed();
    DEBUG("reset()");
}

void Display::test()
{
    DEBUG("test()");
}

void Display::update()
{
    if (_isRunning)
    {
        emit changed();
        DEBUG("update()");
    }
}


/* EVENTS */
void Display::mousePressed(int x, int y)
{
    shared_ptr<DrawableObject>  rec = make_shared<DrawableObject>();

    *rec << QPoint(x, y) << QPoint(x + 42, y) << QPoint(x + 42, y + 21) << QPoint(x, y + 42);

    _objectsMutex.lock();
    _objects.push_front(rec);
    _objectsMutex.unlock();

    DEBUG("mousePressed(" << x << ", " << y << ")");
}

void Display::mouseReleased(int x, int y)
{
    DEBUG("mouseReleased(" << x << ", " << y << ")");
}

void Display::mouseMoved(int x, int y)
{
    DEBUG("mouseMoved(" << x << ", " << y << ")");
}

void Display::keyPressed(int key)
{
    DEBUG("keyPressed(" << key << ")");
}

void Display::keyReleased(int key)
{
    DEBUG("keyReleased(" << key << ")");
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

DrawableObjectList &Display::objects() const
{
    return _objects;
}

void Display::objects(const DrawableObjectList &objects)
{
    _objectsMutex.lock();
    _objects = objects;
    _objectsMutex.unlock();
}
