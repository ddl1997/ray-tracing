#include "image.h"
#ifndef __STDC_LIB_EXT1__
#define __STDC_LIB_EXT1__
#endif // !__STDC_LIB_EXT1__

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif // !STB_IMAGE_WRITE_IMPLEMENTATION

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include "global.h"

Image::Image(int width, int height)
{
    this->width = width;
    this->height = height;
    buf = std::vector<Eigen::Vector3f>(width * height, Eigen::Vector3f{ 0, 0, 0 });
}

Image::Image(int width, int height, float* data)
{
    new (this) Image(width, height); // placement new
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int id = parseId(i, j), dataId = id * 3;
            Eigen::Vector3f color = Eigen::Vector3f{ data[dataId], data[dataId + 1], data[dataId + 2] };
            buf[id] = color;
        }
    }
}

Image::Image(int width, int height, char* data)
{
    new (this) Image(width, height); // placement new
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int id = parseId(i, j), dataId = id * 3;
            Eigen::Vector3f color = Eigen::Vector3f{ (float)data[dataId], (float)data[dataId + 1], (float)data[dataId + 2] };
            buf[id] = color;
        }
    }
}

Image::Image(const Image& img)
{
    this->width = img.width;
    this->height = img.height;
    this->buf = img.buf;
}

bool Image::load(std::string filename)
{
    int nrChannels;
    auto data = stbi_load((filename).c_str(), &width, &height, &nrChannels, 0);
    if (data != nullptr)
    {
        if (nrChannels == 1 || nrChannels == 3 || nrChannels == 4)
        {
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    int pos = (j * width + i) * nrChannels;
                    Eigen::Vector3f color;
                    if (nrChannels == 1)
                    {
                        color = Eigen::Vector3f{ static_cast<float>(data[pos] / 255.0),
                        static_cast<float>(data[pos] / 255.0),
                        static_cast<float>(data[pos] / 255.0) };
                    }
                    else
                    {
                       color = Eigen::Vector3f{ static_cast<float>(data[pos] / 255.0),
                       static_cast<float>(data[pos + 1] / 255.0),
                       static_cast<float>(data[pos + 2] / 255.0) };
                    }
                    buf.push_back(color);
                }
            }
        }
        stbi_image_free(data);;
        return true;
    }
    return false;
}

void Image::resize(int width, int height)
{
    this->width = width;
    this->height = height;
    buf = std::vector<Eigen::Vector3f>(width * height, Eigen::Vector3f{ 0, 0, 0 });
}

Eigen::Vector3f Image::getPixel(int x, int y)
{
    int id = parseId(x, y);
    if (id < 0) return Eigen::Vector3f{ 0, 0, 0 };
    return buf[id];
}

Eigen::Vector3f Image::getPixel(float x, float y)
{
    while (x < 0) x += 1;
    while (x >= 1) x -= 1;
    while (y < 0) y += 1;
    while (y >= 1) y -= 1;
    
    float xPos = x * width, yPos = y * height;
    int xf = (int)xPos, xc = xf + 1, yf = (int)yPos, yc = yf + 1;
    if (xc >= width) xc = xf;
    if (yc >= height) yc = yf;

    // Ë«ÏßÐÔ²åÖµ
    auto cxfyf = getPixel(xf, yf),
        cxfyc = getPixel(xf, yc),
        cxcyf = getPixel(xc, yf),
        cxcyc = getPixel(xc, yc);
    float xratio = xPos - xf, yratio = yPos - yf;
    auto cxf = Eigen::Vector3f{ (1 - yratio) * cxfyf.x() + yratio * cxfyc.x(), (1 - yratio) * cxfyf.y() + yratio * cxfyc.y(), (1 - yratio) * cxfyf.z() + yratio * cxfyc.z() },
        cxc = Eigen::Vector3f{ (1 - yratio) * cxcyf.x() + yratio * cxcyc.x(), (1 - yratio) * cxcyf.y() + yratio * cxcyc.y(), (1 - yratio) * cxcyf.z() + yratio * cxcyc.z() };
    auto color = Eigen::Vector3f{ (1 - xratio) * cxf.x() + xratio * cxc.x(), (1 - xratio) * cxf.y() + xratio * cxc.y(), (1 - xratio) * cxf.z() + xratio * cxc.z() };
    return color;
}

void Image::setPixel(int x, int y, const Eigen::Vector3f color)
{
    int id = parseId(x, y);
    if (id < 0) return;
    buf[id] = color;
}

void Image::normalize()
{
    //float range = rMax - rMin;
    for (auto iter = buf.begin(); iter != buf.end(); iter++)
    {
        Eigen::Vector3f color = (*iter);
        /*iter->x() = std::pow(mathUtil::floatNormalize(color.x()), 0.4);
        iter->y() = std::pow(mathUtil::floatNormalize(color.y()), 0.4);
        iter->z() = std::pow(mathUtil::floatNormalize(color.z()), 0.4);*/
    }
}

void Image::save(std::string filename)
{
    unsigned char* data = new unsigned char[width * height * 3];
    int bufSize = buf.size();
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int srcIndex = parseId(x, y),
                dstIndex = parseId(x, height - y - 1);
            unsigned char r = static_cast<unsigned char>(clamp(buf[srcIndex].x(), 0.0, 0.999) * 255.999);
            unsigned char g = static_cast<unsigned char>(clamp(buf[srcIndex].y(), 0.0, 0.999) * 255.999);
            unsigned char b = static_cast<unsigned char>(clamp(buf[srcIndex].z(), 0.0, 0.999) * 255.999);
            data[dstIndex * 3] = r;
            data[dstIndex * 3 + 1] = g;
            data[dstIndex * 3 + 2] = b;
        }
    }
    std::string fileDir = "output/" + filename + ".png";
    stbi_write_png(fileDir.c_str(), width, height, 3, data, width * 3);

    delete []data;
}

Image& Image::operator=(const Image& img)
{
    if (this != &img)
    {
        this->width = img.width;
        this->height = img.height;
        this->buf = img.buf;
    }
    return *this;
}
