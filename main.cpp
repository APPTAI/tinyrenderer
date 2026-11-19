#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include "tgaimage.h"
constexpr TGAColor white = { 255, 255, 255, 255 }; // attention, BGRA order
constexpr TGAColor green = { 0, 255,   0, 255 };
constexpr TGAColor red = { 0,   0, 255, 255 };
constexpr TGAColor blue = { 255, 128,  64, 255 };
constexpr TGAColor yellow = { 0, 200, 255, 255 };

void line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color) {
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if (steep) { // if the line is steep, we transpose the image
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax > bx) { // make it left−to−right
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int y = ay;
    float error = 0;
    for (int x = ax; x <= bx; x++) {
        if (steep) // if transposed, de−transpose
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);
        error += std::abs(by - ay) / static_cast<float>(bx - ax);
        if (error > .5) {
            y += by > ay ? 1 : -1;
            error -= 1.;
        }
        /*y += (by > ay ? 1 : -1) * (ierror > bx - ax);
        ierror -= 2 * (bx-ax)   * (ierror > bx - ax);*/
    }
}
class Vertex {
public:
    float x;
    float y;
    float z;
};
class Face {
public:
    int a;
    int b;
    int c;
};
std::vector<Vertex>vertices;
std::vector<Face>faces;
int main(int argc, char** argv) {
    constexpr int width = 512;
    constexpr int height = 512;
    TGAImage framebuffer(width, height, TGAImage::RGB);
    std::ifstream file(R"(E:\tinyrenderer\tinyrenderer-start\obj\african_head\african_head.obj)"
    );
    if (!file.is_open())
    {
        std::cerr << "cannot open the file";
    }
    std::string text;
    while (std::getline(file, text)) {
        std::istringstream row(text);
        std::string type;
        row >> type;

        if (type == "v") {
            Vertex vertex{};
            if (row >> vertex.x >> vertex.y >> vertex.z) {
                vertices.push_back(vertex);
            }
        }
        else if (type == "f") {
            std::string a, b, c;
            if (row >> a >> b >> c) {
                faces.push_back({ std::stoi(a) - 1,
                                 std::stoi(b) - 1,
                                 std::stoi(c) - 1 });
            }
        }
    }
    int num = faces.size();
    
    for (int i = 0; i < num; i++)
    {
        auto pixelX = [width](float x) {
            return static_cast<int>(std::lround((x + 1.0f) * 0.5f * (width - 1)));
            };

        auto pixelY = [height](float y) {
            return static_cast<int>(std::lround((y + 1.0f) * 0.5f * (height - 1)));
            };
        int num1 = faces[i].a, num2 = faces[i].b, num3 = faces[i].c;
		
        const Vertex& a = vertices[num1];
        const Vertex& b = vertices[num2];

        line(pixelX(a.x), pixelY(a.y),
            pixelX(b.x), pixelY(b.y), framebuffer, red);
        const Vertex& a1 = vertices[num1];
        const Vertex& b1 = vertices[num3];

        line(pixelX(a.x), pixelY(a.y),
            pixelX(b.x), pixelY(b.y), framebuffer, red); 
        const Vertex& a2 = vertices[num2];
        const Vertex& b2 = vertices[num3];

        line(pixelX(a.x), pixelY(a.y),
            pixelX(b.x), pixelY(b.y), framebuffer, red);
        
    }
    

    //导入所有的点
    //导入三角形
    //每个顶点连线



    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}