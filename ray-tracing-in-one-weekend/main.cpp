#include "src/ray/ray.h"
#include "src/utils/image.h"
#include "src/ray/hittable_list.h"
#include "src/ray/sphere.h"
#include <iostream>

float hit_sphere(const Eigen::Vector3f& center, float radius, const Ray& r) {
    Eigen::Vector3f oc = r.start() - center;
    auto a = r.direction().dot(r.direction());
    auto half_b = oc.dot(r.direction());
    auto c = oc.dot(oc) - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

Eigen::Vector3f ray_color(const Ray& r, const Hittable& world) {

    HitRecord rec;
    if (world.hit(r, 0, FLT_MAX, rec)) {
        return 0.5 * (rec.normal + Eigen::Vector3f(1, 1, 1));
    }
    Eigen::Vector3f unit_direction = r.direction().normalized();
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Eigen::Vector3f(1.0, 1.0, 1.0) + t * Eigen::Vector3f(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // World
    HittableList world;
    world.add(make_shared<Sphere>(Eigen::Vector3f(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Eigen::Vector3f(0, -100.5, -1), 100));

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = Eigen::Vector3f(0, 0, 0);
    auto horizontal = Eigen::Vector3f(viewport_width, 0, 0);
    auto vertical = Eigen::Vector3f(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Eigen::Vector3f(0, 0, focal_length);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    Image img(image_width, image_height);
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = float(i) / (image_width - 1);
            auto v = float(j) / (image_height - 1);
            Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            Eigen::Vector3f pixel_color = ray_color(r, world);
            img.setPixel(i, j, pixel_color);
        }
    }
    img.save("test");
    std::cerr << "\nDone.\n";
}