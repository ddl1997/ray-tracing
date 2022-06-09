#ifndef IMAGE_H
#define IMAGE_H

#include <Eigen/Dense>
#include <vector>
#include <string>

class Image {
public:

    Image() : width(0), height(0) {};

    Image(int width, int height);

    Image(int width, int height, float* data);

    Image(int width, int height, char* data);

    Image(const Image& img);

    bool load(std::string filename);

    void resize(int width, int height);

    Eigen::Vector3f getPixel(int x, int y);

    Eigen::Vector3f getPixel(float x, float y);

    void setPixel(int x, int y, const Eigen::Vector3f color);

    inline bool loaded()
    {
        return buf.size() > 0;
    }

    int getWidth() { return width; }

    int getHeight() { return height; }

    void normalize();

    void save(std::string filename);

    Image& operator= (const Image& img);

private:
    int width, height;
    std::vector<Eigen::Vector3f> buf;

    int parseId(int x, int y)
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
            return x + y * width;
        else
            return -1;
    }
};
#endif