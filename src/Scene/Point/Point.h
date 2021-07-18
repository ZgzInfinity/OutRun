

#ifndef POINT_H
#define POINT_H

template<class TYPE>
class Point {

    public:

        TYPE x, y;

        Point()
        {}
        Point(TYPE x, TYPE y) : x(x), y(y)
        {}

        // Operators ------------------------------------------------
        Point operator -(const Point &v) const
        {
            Point r;

            r.x = x - v.x;
            r.y = y - v.y;

            return(r);
        }

        Point operator + (const Point &v) const
        {
            Point r;

            r.x = x + v.x;
            r.y = y + v.y;

            return(r);
        }

        const Point& operator -=(const Point &v)
        {
            x -= v.x;
            y -= v.y;

            return(*this);
        }

        const Point& operator +=(const Point &v)
        {
            x += v.x;
            y += v.y;

            return(*this);
        }

        bool operator ==(const Point& v) const
        {
            return (x == v.x && y == v.y);
        }

        bool operator !=(const Point& v) const
        {
            return (x != v.x || y != v.y);
        }

        // Utils ------------------------------------------------
        bool IsZero() const
        {
            return (x == 0 && y == 0);
        }

        Point& SetToZero()
        {
            x = y = 0;
            return(*this);
        }

        Point& Negate()
        {
            x = -x;
            y = -y;

            return(*this);
        }

        // Distances ---------------------------------------------
        TYPE DistanceTo(const Point& v) const
        {
            TYPE fx = x - v.x;
            TYPE fy = y - v.y;

            return sqrt((fx*fx) + (fy*fy));
        }
};

typedef Point<int> iPoint;

typedef Point<float> fPoint;

#endif // __POINT_H__
