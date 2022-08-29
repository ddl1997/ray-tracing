#include "src/ray/ray.h"
#include "src/utils/image.h"
#include "src/ray/hittable_list.h"
#include "src/ray/sphere.h"
#include "src/camera/camera.h"
#include "src/utils/global.h"
#include "src/utils/material.h"
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

Eigen::Vector3f ray_color(const Ray& r, const Hittable& world, int depth)
{
    if (depth <= 0)
        return Eigen::Vector3f(0, 0, 0);

    HitRecord rec;
    float bias = 0.001;
    if (world.hit(r, bias, FLT_MAX, rec)) 
    {
        Ray scattered;
        Eigen::Vector3f attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return multi_respectively(attenuation, ray_color(scattered, world, depth - 1));
        return Eigen::Vector3f(0, 0, 0);
        //Eigen::Vector3f target = rec.p + random_in_hemisphere(rec.normal);
        //return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth - 1);
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
    const int samples_per_pixel = 20;
    const int max_depth = 10;

    // World
    HittableList world;
    /*world.add(make_shared<Sphere>(Eigen::Vector3f(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Eigen::Vector3f(0, -100.5, -1), 100));*/

    auto material_ground = make_shared<Lambertian>(Eigen::Vector3f(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Eigen::Vector3f(0.1, 0.2, 0.5));
    //auto material_center = make_shared<Dielectric>(1.5);
    auto material_left = make_shared<Dielectric>(1.5);
    /*auto material_center = make_shared<Lambertian>(Eigen::Vector3f(0.7, 0.3, 0.3));
    auto material_left = make_shared<Metal>(Eigen::Vector3f(0.8, 0.8, 0.8), 0.3);*/
    auto material_right = make_shared<Metal>(Eigen::Vector3f(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(Eigen::Vector3f(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Eigen::Vector3f(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<Sphere>(Eigen::Vector3f(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(Eigen::Vector3f(1.0, 0.0, -1.0), 0.5, material_right));

    // Camera
    Camera cam;

    // Render

    std::cout << image_width << " " << image_height << "\n255\n";
    Image img(image_width, image_height);
    float scale = 1.0 / samples_per_pixel;
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Eigen::Vector3f pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                float u = (i + random_float()) / (image_width - 1);
                float v = (j + random_float()) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            pixel_color = gamma_correction(scale * pixel_color, 2.0);
            img.setPixel(i, j, pixel_color);
        }
    }
    img.save("test");
    std::cerr << "\nDone.\n";
}