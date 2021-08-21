#pragma once
#include "Essentials.h"
#include "GeneratedCube.h"

Color interPolateColors(float input1, float input2, float position, Color Color1, Color Color2) {
    float r = interPolate(input1, input2, position, Color1.r, Color2.r);
    float g = interPolate(input1, input2, position, Color1.g, Color2.g);
    float b = interPolate(input1, input2, position, Color1.b, Color2.b);
    return Color(r, g, b, 0xff);
}
float SphereFunction(const Vec3& Pos)
{
    return Pos.x * Pos.x + Pos.y * Pos.y + Pos.z * Pos.z - 1.0f;
}
float RandomFunction(const Vec3& Pos)
{
    Vec3 P = { Pos.x * 10.0f,Pos.y * 10.0f,Pos.z * 10.0f };
    return sinf(P.length()) * P.x + sinf(P.length()) * P.y;
}
float WaterFunction(const Vec3& Pos)
{
    return -Pos.y + 5.0f;
}
float PlaneFunction(const Vec3& Pos)
{
    return -Pos.y + NoiseFunction(Pos); //+Kriti ko function
    //return -Pos.y + (Fourier(Pos.x, Pos.z) + Fourier(Pos.z, Pos.z)) / 2; //Fourier terrain
    //return -Pos.y + (expf(Pos.x*10) + expf(Pos.z*10)) / 2;//Ramilo Function
}
float Fourier(float f1, float f2) {
    /*float a0 = 1.f,
        an = 5.f / (2 / 3.14),
        bn = 2.f / 3.14,
        sum = a0/2;*/
    float pi = 3.14159;
    float a0 = 2*sinhf(pi)/pi, an, bn,
        sum = a0/2;
    for (float i = 1; i < 60; i++) {
        an = 2 * cosf(i * pi) * sinhf(pi) / (pi * (1 + i * i));
        bn = -2 * i * cosf(i * pi) * sinhf(pi) / (pi * (1 + i * i));
        sum += an * cosf(i * 2 * 3.14 * f1) + bn * sinf(i * 2 * 3.14 * f1);
    }
    return sum;
}
double NoiseFunction(Vec3 Pos, int octaves, Vec3 offSet, int multiplier)
{
    //OUTPUTS
    double maxValue = 0;
    double total = 0;

    //variables
    double freq = 0.045f;
    double amp = 10.f;
    double persistence = 0.25f;
    
        for (unsigned i = 0; i < octaves; ++i)
        {
            glm::vec3 p((Pos.x * freq + offSet.x) * multiplier, (Pos.y * freq + offSet.y) * multiplier, (Pos.z * freq + offSet.z) * multiplier);

            total += ((glm::simplex(p) + 1.0) / 2.0) * amp;
            maxValue += amp;
            amp *= persistence;
            freq *= 2;
        }
        return total;// / maxValue;
}