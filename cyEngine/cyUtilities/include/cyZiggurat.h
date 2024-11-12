#pragma once

#include "cyUtilitiesPrerequisites.h"

#include "cyMath.h"
#include "cyRNG.h"

#include <random>

namespace CYLLENE_SDK
{

  class Ziggurat : public RNG
  {
  public:
    virtual float 
    next() override {
      uint32 seed = m_generator();
      uint32 congruent = cong_seeded(seed);
      float r4Uni = r4_uni(seed);
      return static_cast<float>(congruent * r4Uni);
    }


    uint32 cong_seeded(uint32& jcong) {
      uint32 value;

      jcong = 69069 * (jcong)+1234567;

      value = jcong;

      return value;
    }
    
    uint32 kiss_seeded(uint32& jcong, uint32& jsr, uint32& w, uint32& z) {
      uint32 value;

      value = (mwc_seeded(w, z) ^ cong_seeded(jcong)) + shr3_seeded(jsr);

      return value;
    }

    uint32 mwc_seeded(uint32& w, uint32& z) {
      uint32 value;

      z = 36969 * (z & 65535) + (z >> 16);
      w = 18000 * (w & 65535) + (w >> 16);

      value = (z << 16) + w;

      return value;
    }

    float r4_exp(uint32& jsr, uint32 ke[256], float fe[256], float we[256]) {
      uint32 iz;
      uint32 jz;
      float value;
      float x;

      jz = shr3_seeded(jsr);
      iz = (jz & 255);

      if (jz < ke[iz]) {
        value = static_cast<float>(jz)*we[iz];
      }
      else {
        for (; ; ) {
          if (iz == 0) {
            value = static_cast<float>(7.69711 - log(r4_uni(jsr)));
            break;
          }

          x = static_cast<float>(jz)*we[iz];

          if (fe[iz] + r4_uni(jsr) * (fe[iz - 1] - fe[iz]) < exp(-x)) {
            value = x;
            break;
          }

          jz = shr3_seeded(jsr);
          iz = (jz & 255);

          if (jz < ke[iz]) {
            value = static_cast<float>(jz)*we[iz];
            break;
          }
        }
      }
      return value;
    }

    void r4_exp_setup(uint32 ke[256], float fe[256], float we[256]) {
      double de = 7.697117470131487;
      int i;
      const double m2 = 2147483648.0;
      double q;
      double te = 7.697117470131487;
      const double ve = 3.949659822581572E-03;

      q = ve / exp(-de);

      ke[0] = static_cast<uint32>((de / q) * m2);
      ke[1] = 0;

      we[0] = static_cast<float>(q / m2);
      we[255] = static_cast<float>(de / m2);

      fe[0] = 1.0;
      fe[255] = static_cast<float>(exp(-de));

      for (i = 254; 1 <= i; i--) {
        de = -log(ve / de + exp(-de));
        ke[i + 1] = static_cast<uint32>((de / te) * m2);
        te = de;
        fe[i] = static_cast<float>(exp(-de));
        we[i] = static_cast<float>(de / m2);
      }
      return;
    }
    
    float r4_nor(uint32& jsr, uint32 kn[128], float fn[128], float wn[128]) {
      int32 hz;
      uint32 iz;
      const float r = 3.442620f;
      float value;
      float x;
      float y;

      hz = static_cast<int32>(shr3_seeded(jsr));
      iz = (hz & 127);

      if (fabs(hz) < kn[iz]) {
        value = static_cast<float>(hz)*wn[iz];
      }
      else {
        for (; ; ) {
          if (iz == 0) {
            for (; ; ) {
              x = static_cast<float>(- 0.2904764 * log(r4_uni(jsr)));
              y = -log(r4_uni(jsr));
              if (x * x <= y + y) {
                break;
              }
            }

            if (hz <= 0) {
              value = -r - x;
            }
            else {
              value = +r + x;
            }
            break;
          }

          x = static_cast<float>(hz)*wn[iz];

          if (fn[iz] + r4_uni(jsr) * (fn[iz - 1] - fn[iz]) < 
              exp(-0.5 * x * x)) {
            value = x;
            break;
          }

          hz = static_cast<int32>(shr3_seeded(jsr));
          iz = (hz & 127);

          if (fabs(hz) < kn[iz]) {
            value = static_cast<float>(hz)*wn[iz];
            break;
          }
        }
      }

      return value;
    }
    
    void r4_nor_setup(uint32 kn[128], float fn[128], float wn[128]) {
      double dn = 3.442619855899;
      int i;
      const double m1 = 2147483648.0;
      double q;
      double tn = 3.442619855899;
      const double vn = 9.91256303526217E-03;

      q = vn / exp(-0.5 * dn * dn);

      kn[0] = static_cast<uint32>((dn / q) * m1);
      kn[1] = 0;

      wn[0] = static_cast<float>(q / m1);
      wn[127] = static_cast<float>(dn / m1);

      fn[0] = 1.0;
      fn[127] = static_cast<float>(exp(-0.5 * dn * dn));

      for (i = 126; 1 <= i; i--) {
        dn = sqrt(-2.0 * log(vn / dn + exp(-0.5 * dn * dn)));
        kn[i + 1] = static_cast<uint32>((dn / tn) * m1);
        tn = dn;
        fn[i] = static_cast<float>(exp(-0.5 * dn * dn));
        wn[i] = static_cast<float>(dn / m1);
      }

      return;
    }
    
    float r4_uni(uint32& jsr) {
      uint32 jsr_input;
      float value;

      jsr_input = jsr;

      jsr = (jsr ^ (jsr << 13));
      jsr = (jsr ^ (jsr >> 17));
      jsr = (jsr ^ (jsr << 5));

      value = static_cast<float>(fmod(0.5 + static_cast<float>(jsr_input + jsr) / 65536.0 / 65536.0, 
                                      1.0));

      return value;
    }
    
    uint32 shr3_seeded(uint32& jsr) {
      uint32 jsr_input;
      uint32 value;

      jsr_input = jsr;

      jsr = (jsr ^ (jsr << 13));
      jsr = (jsr ^ (jsr >> 17));
      jsr = (jsr ^ (jsr << 5));

      value = jsr_input + jsr;

      return value;
    }
    std::mt19937 m_generator;
    float m_fn[128];
    uint32 m_kn[128];
    float m_wn[128];
  };





  /*
  class Ziggurat : public  RNG
  {
  public:
    Ziggurat(double mean = 0.0f, double stddev = 1.0f)
      : m_mean(mean),
        m_stddev(stddev),
        m_generator(std::random_device{}()) {
          initializeTables();
        }

    void
    set(uint32 newSeed)
    {
      m_generator = { newSeed };
      initializeTables();
    }

    double next() {
      while (true) {
        // Select a layer
        int layer = m_distLayer(m_generator);
        double x = m_distUniform(m_generator) * x_[layer];

        // Check if x is within the layer's rectangle
        if (x < x_[layer + 1]) {
          return m_mean + m_stddev * x;
        }

        // Handle tail (exponential distribution)
        if (layer == 0) {
          return m_mean + m_stddev * generateTail();
        }

        // Apply rejection sampling
        double y = m_distUniform(m_generator);
        if (y < std::exp(-0.5 * (x * x - x_[layer] * x_[layer]))) {
          return m_mean + m_stddev * x;
        }
      }
    }

  private:

    double m_mean;
    double m_stddev;
    std::mt19937 m_generator;
    std::uniform_real_distribution<double> m_distUniform{0.0, 1.0};
    std::uniform_int_distribution<int> m_distLayer{0, 255}; // For selecting the layer

    // Precomputed tables
    std::array<double, 257> x_{};
    std::array<double, 256> y_{};

    double generateTail() {
      double u;
      do {
        u = m_distUniform(m_generator);
      } while (u == 0.0);  // Prevent log(0)
      return x_[0] - std::log(u) / x_[0];
    }

    // Initializes x and y tables for the Ziggurat algorithm
    void initializeTables() {
      const double m = std::sqrt(2.0 * Math::PI);
      const double R = 3.442619855899; // Approximate end of the rectangles
      x_[0] = R;
      y_[0] = std::exp(-0.5 * R * R);

      // Fill x and y tables using the Ziggurat method
      for (int i = 1; i < 256; ++i) {
        x_[i] = std::sqrt(-2.0 * std::log(y_[i - 1]));
        y_[i] = std::exp(-0.5 * x_[i] * x_[i]);
      }
      x_[256] = 0.0; // Last x value is zero, for the boundary condition
    }
  };
  */
}
