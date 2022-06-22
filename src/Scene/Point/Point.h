
/*
 * Copyright (c) 2022 Ruben Rodriguez
 *
 * This file is part of Out Run.
 * Out Run is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Out Run is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Out Run.  If not, see <https://www.gnu.org/licenses/>.
 */



/*
 * Interface file of the module Point
 */

#ifndef POINT_H
#define POINT_H



/**
 * Represents a point in space given by to
 * coordinates (axis X and Y)
 */
template<class TYPE>
class Point {

    public:

        // Coordinates of the point in the space
        TYPE x, y;


        /**
         * Default constructor
         */
        Point(){}



        /**
         * Second constructor
         * @param x is the coordinate of the point in axis X
         * @param y is the coordinate of the point in axis Y
         */
        Point(TYPE x, TYPE y) : x(x), y(y){}



        /**
         * Subtracts the coordinates of another point from a point
         * @param v is the point whose are going to be subtracted
         */
        Point operator -(const Point &v) const {
            Point r;

            // Subtraction of points
            r.x = x - v.x;
            r.y = y - v.y;

            return(r);
        }



        /**
         * Add the coordinates of another point from a point
         * @param v is the point whose are going to be added
         */
        Point operator + (const Point &v) const {
            Point r;

            // Adding of points
            r.x = x + v.x;
            r.y = y + v.y;

            return(r);
        }



        /**
         * Subtracts the coordinates of another point from a point
         * @param v is the point whose are going to be subtracted
         */
        const Point& operator -=(const Point &v){
            // Subtraction of points
            x -= v.x;
            y -= v.y;

            return(*this);
        }



        /**
         * Add the coordinates of another point from a point
         * @param v is the point whose are going to be added
         */
        const Point& operator +=(const Point &v){
            // Adding of points
            x += v.x;
            y += v.y;

            return(*this);
        }



        /**
         * Returns true if two points are the same.
         * Otherwise returns false
         */
        bool operator ==(const Point& v) const {
            // Compare both components
            return (x == v.x && y == v.y);
        }



        /**
         * Returns true if two points are different.
         * Otherwise returns false
         */
        bool operator !=(const Point& v) const {
            // Compare both components
            return (x != v.x || y != v.y);
        }



        /**
         * Returns true if the point the origin of
         * coordinates (0,0). Otherwise returns false
         */
        bool IsZero() const {
            return (x == 0 && y == 0);
        }



        /**
         * Set the point with the origin of coordinates (0,0).
         */
        Point& SetToZero(){
            x = y = 0;
            return(*this);
        }



        /**
         * Change the sign of the point (not operator)
         */
        Point& Negate(){
            x = -x;
            y = -y;

            return(*this);
        }



        /**
         * Returns the euclidean distance between two points
         */
        TYPE DistanceTo(const Point& v) const {
            TYPE fx = x - v.x;
            TYPE fy = y - v.y;

            return sqrt((fx*fx) + (fy*fy));
        }
};

typedef Point<int> iPoint;

typedef Point<float> fPoint;

#endif // POINT_H
