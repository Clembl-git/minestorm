#ifndef DISPLAY_HH
# define DISPLAY_HH

# include <QObject>
# include <QTimer>
# include <QSize>
# include <QMutex>
# include <QPainter>
# include <QBrush>

# include <list>
# include <memory>

# include "Minestorm.hh"
# include "DrawableObject.hh"

class QPainter;
class QRect;

using namespace std;

class Display : public QObject
{
    Q_OBJECT

private:
    QTimer              _timer;
    bool                _isRunning;
    QSize               _size;
    DrawableObjectList  _objects;
    QMutex              _objectsMutex;

public:
    Display(const QSize &size, QObject *parent = nullptr);

    void                draw(QPainter &painter, QRect &size);

    void                mousePressed(int x, int y);
    void                mouseReleased(int x, int y);
    void                mouseMoved(int x, int y);
    void                keyPressed(int key);
    void                keyReleased(int key);

    void                start();
    void                pause();
    void                reset();
    void                test();

    const QSize         &size() const;
    bool                isRunning() const;
    const DrawableObjectList  &objects() const;

protected:
    void                initialize();


signals:
    void                changed();
    void                sigMousePressed(int x, int y);
    void                sigMouseReleased(int x, int y);
    void                sigMouseMoved(int x, int y);
    void                sigKeyPressed(int key);
    void                sigKeyReleased(int key);

public slots:
    void                receiveObjects(const DrawableObjectList &objects);

private slots:
    void                update();
};

#endif // DISPLAY_HH
