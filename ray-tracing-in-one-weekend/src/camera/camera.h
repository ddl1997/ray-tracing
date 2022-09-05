#ifndef CAMERA_H
#define CAMERA_H

#include "../utils/global.h"
#include "../ray/ray.h"

class Camera {
public:
    Camera() {

        auto aspect_ratio = 16.0 / 9.0;
        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        origin = Eigen::Vector3f(0, 0, 0);
        horizontal = Eigen::Vector3f(viewport_width, 0.0, 0.0);
        vertical = Eigen::Vector3f(0.0, viewport_height, 0.0);
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - Eigen::Vector3f(0, 0, focal_length);
    }

    Camera(
        Eigen::Vector3f lookfrom,
        Eigen::Vector3f lookat,
        Eigen::Vector3f vup,
        float vfov, // vertical field-of-view in degrees
        float aspect_ratio, // width / height
        float aperture, // 光圈大小
        float focus_dist, // 对焦距离
        //focal length焦距和focus distance对焦距离的区别可见
        // https://www.zhihu.com/question/29643982
        // 和
        // https://blog.csdn.net/cpongo1/article/details/114844138
        float time_open,
        float time_close
    ) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        w = (lookfrom - lookat).normalized(); // w 指向相机朝向的负向（z轴）
        u = vup.cross(w).normalized(); // x轴
        v = w.cross(u); // y轴

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

        lens_radius = aperture / 2;
        this->time_open = time_open;
        this->time_close = time_close;
    }

    Ray get_ray(float s, float t) const {

        vec3f rd = lens_radius * random_in_unit_disk();
        vec3f offset = u * rd.x() + v * rd.y();

        return Ray(
            origin + offset,
            lower_left_corner + s * horizontal + t * vertical - origin - offset,
            random_float(time_open, time_close)
        );
    }

private:
    vec3f origin;
    vec3f lower_left_corner;
    vec3f horizontal;
    vec3f vertical;
    vec3f u, v, w;
    float lens_radius;
    float time_open, time_close; // 开关快门时间
};
#endif