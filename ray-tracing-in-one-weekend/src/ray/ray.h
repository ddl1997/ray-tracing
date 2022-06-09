#ifndef RAY_H
#define RAY_H

#include <Eigen/Dense>

class Ray {
public:
    Ray() {}

    Ray(const Eigen::Vector3f& start, const Eigen::Vector3f& direction) :
        st(start), dir(direction)
    {}

    Eigen::Vector3f start() const { return st; }

    Eigen::Vector3f direction() const { return dir; }

    Eigen::Vector3f at(float t) const 
    {
        return st + t * dir;
    }

public:
    Eigen::Vector3f st;
    Eigen::Vector3f dir;
};

#endif