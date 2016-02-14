#ifndef ELEMENT_HH
# define ELEMENT_HH

# include <QPolygon>
# include <QPainter>

# include "IDrawable.hh"
# include "Minestorm.hh"

class Element : public IDrawable
{
public:
    /*   /!\ MUST MATCH Mine::TypeMine /!\   */
    enum Type
    {
        MINE_S      = 0,
        SHIP_1      = 1,
        SHIP_2      = 2,
        SHIP_3      = 3,
        SHIP_4      = 4,
        MINE_M      = 5,
        MINE_L      = 6,
        MINE_S_ON   = 7,
        MINE_M_ON   = 8,
        MINE_L_ON   = 9,
        SHOT        = 10,
        MINE_EXPLO  = 11
    };

private:
    Type            _type;
    QPolygon        _polygon;
    qreal           _angle;
    QPoint          _center;
    QPoint          _imageCenter;
    quint32         _armed;

public:
    // Ship
    Element(Type type, const QPolygon &polygon, qreal angle, const QPoint &center);
    // Shot
    Element(Type type, const QPolygon &polygon);
    // Mine
    Element(Type type, const QPolygon &polygon, const QPoint &center);

    Element();
    ~Element();

    const QPoint    &center() const;
    const QPoint    &imageCenter() const;

    Type            type() const;
    const QPolygon  &polygon() const;
    qreal           angle() const;
    quint32         armed() const;

    // IDrawable interface
public:
    void            draw(QPainter &painter, Images &images) const override;
};

#endif // ELEMENT_HH

# include "Images.hh"
