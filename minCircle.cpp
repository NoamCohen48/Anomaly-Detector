/*
 * Created by:
 * Shaked Cohen, Shakedc159@gmail.com.
 * Noam Cohen, cohennoam48@gmail.com.
*/

#include "minCircle.h"

/**
 * The function check if point within the circle.
 * @param circle circle.
 * @param p point.
 * @return True the point in the circle, false else.
 */
bool isInside(const Point &p, const Circle &circle) {
    return p.dist(circle.center) <= circle.radius;
}

/**
 * The function check if all the received point within the circle.
 * @param circle circle.
 * @param points array of points.
 * @return True if all the point in the circle, false else.
 */
bool isValid(Circle circle, const vector<Point *> &points) {
    // going throughout all points to check if inside circle
    for (Point *point: points) {
        if (!isInside(*point, circle)) {
            return false;
        }
    }
    return true;
}

/**
 * return a circle from tree points.
 * @param p1 first point.
 * @param p2 second point.
 * @param p1 second point.
 * @return circle from tree points.
 */
Circle circleFrom3Points(const Point &p1, const Point &p2, const Point &p3) {
    double newB = (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
    double newC = (p3.x - p1.x) * (p3.x - p1.x) + (p3.y - p1.y) * (p3.y - p1.y);
    double newD = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);

    Point center(((p3.y - p1.y) * newB - (p2.y - p1.y) * newC) / (2 * newD),
                 ((p2.x - p1.x) * newC - (p3.x - p1.x) * newB) / (2 * newD));

    center.x += p1.x;
    center.y += p1.y;

    return Circle(center, center.dist(p1));
}

/**
 * return a circle from two points.
 * @param p1 first point.
 * @param p2 second point.
 * @return circle from two points.
 */
Circle circleFrom2Points(const Point &p1, const Point &p2) {
    // Set the center to be the midpoint of A and p2
    Point center((p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f);

    // Set the radius to be half the distance between p1 and p2
    return Circle(center, p1.dist(p2) / 2.0f);
}

/**
 * return a minimum enclosing circle for the base case - 3, 2, 1 or 0 boundary points.
 * @param boundary the points on the boundary.
 * @return minimum enclosing circle.
 */
Circle baseMinCircle(vector<Point *> boundary) {
    // if boundary is 0
    if (boundary.empty()) {
        return {{0, 0}, 0};
    }

    // if boundary is 1 then circle is the point
    if (boundary.size() == 1) {
        return {*boundary[0], 0};
    }

    // if boundary is 2
    if (boundary.size() == 2) {
        return circleFrom2Points(*boundary[0], *boundary[1]);
    }

    // if boundary is 3 - try to make circle from pair
    Circle circle = circleFrom2Points(*boundary[0], *boundary[1]);
    if (isInside(*boundary[2], circle))
        return circle;

    circle = circleFrom2Points(*boundary[0], *boundary[2]);
    if (isInside(*boundary[1], circle))
        return circle;

    circle = circleFrom2Points(*boundary[1], *boundary[2]);
    if (isInside(*boundary[0], circle))
        return circle;

//    for (int first = 0; first < boundary.size(); ++first) {
//
//        for (int second = first + 1; second < boundary.size(); ++second) {
//
//            // calculating circle from pair
//            Circle circle = circleFrom2Points(*boundary[first], *boundary[second]);
//
//            // if the circle is valid return it
//            if (isValid(circle, boundary)) {
//                return circle;
//            }
//        }
//    }

    // calculating circle from 3 points
    return circleFrom3Points(*boundary[0], *boundary[1], *boundary[2]);
}

/**
 * recursive function that find a minimum enclosing circle.
 * @param points a points set.
 * @param boundary the points on the boundary of the circle.
 * @param n the size of the set
 * @return a minimum enclosing circle.
 */
Circle recursiveFindMinCircle(Point **points, vector<Point *> boundary, size_t n) {
    // if boundary is 3 or n is 0 then we can know what is the circle
    if (boundary.size() == 3 || n == 0) {
        return baseMinCircle(boundary);
    }

    //saving last point
    Point *p = points[n - 1];

    // calling recursively
    Circle minimumCircle = recursiveFindMinCircle(points, boundary, n - 1);

    // if p is in minimumCircle
    if (isInside(*p, minimumCircle)) {
        return minimumCircle;
    }

    // adding p to boundary points and calculate again
    boundary.push_back(p);
    return recursiveFindMinCircle(points, boundary, n - 1);
}

/**
 * find the minimum enclosing circle of a given points set.
 * @param points the points set.
 * @param size the size of the set
 * @return a Circle.
 */
Circle findMinCircle(Point **points, size_t size) {
    // create helper vector
    vector<Point *> boundary;
    boundary.reserve(3);
    // call the recursive function
    return recursiveFindMinCircle(points, boundary, size);
}

