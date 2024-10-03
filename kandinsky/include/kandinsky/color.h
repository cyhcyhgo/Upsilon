#ifndef KANDINSKY_COLOR_H
#define KANDINSKY_COLOR_H

#include <stdint.h>

class KDColor {
public:
  constexpr KDColor() : m_value(0) {}
  // FIXME: This should not be needed, and is probably wasting CPU cycles
  static constexpr KDColor RGB16(uint16_t rgb) {
    return KDColor(rgb);
  }
  static constexpr KDColor RGB24(uint32_t rgb) {
    return KDColor(((rgb&0xF80000)>>8)|((rgb&0x00FC00)>>5)|((rgb&0x0000F8)>>3));
  }
  static constexpr KDColor RGB888(uint8_t r, uint8_t g, uint8_t b) {
    return KDColor((r>>3)<<11 | (g>>2) << 5 | (b>>3));
  }
  uint8_t red() const {
    uint8_t r5 = (m_value>>11)&0x1F;
    return r5 << 3;
  }

  uint8_t green() const {
    uint8_t g6 = (m_value>>5)&0x3F;
    return g6 << 2;
  }

  uint8_t blue() const {
    uint8_t b5 = m_value&0x1F;
    return b5 << 3;
  }

  static KDColor blend(KDColor first, KDColor second, uint8_t alpha);
  KDColor invert() const { return KDColor(~m_value); }
  operator uint16_t() const { return m_value; }
private:
  inline static uint16_t GrayScale(uint16_t rgb565) {
    // 提取 RGB 各分量
    uint8_t r5 = (rgb565 >> 11) & 0x1F;  // 5 位红色
    uint8_t g6 = (rgb565 >> 5) & 0x3F;   // 6 位绿色
    uint8_t b5 = rgb565 & 0x1F;          // 5 位蓝色

    // 将 RGB565 转换为 RGB888 格式
    uint8_t r = (r5 << 3) | (r5 >> 2);   // 5 位转 8 位 (移位)
    uint8_t g = (g6 << 2) | (g6 >> 4);   // 6 位转 8 位 (移位)
    uint8_t b = (b5 << 3) | (b5 >> 2);   // 5 位转 8 位 (移位)

    // 使用加权平均法计算灰度值
    uint8_t gray = (r * 76 + g * 150 + b * 29) >> 8;

    // 将灰度值转换回 RGB565 格式
    uint8_t grayR5 = (gray >> 3) & 0x1F;  // 8 位转 5 位
    uint8_t grayG6 = (gray >> 2) & 0x3F;  // 8 位转 6 位
    uint8_t grayB5 = (gray >> 3) & 0x1F;  // 8 位转 5 位
    uint16_t gray_rgb565 = (grayR5 << 11) | (grayG6 << 5) | grayB5;
    // 返回灰度值的 RGB565 格式
    return gray_rgb565;
  }
  constexpr KDColor(uint16_t value) : m_value(GrayScale(value)) {}
  uint16_t m_value;
};

constexpr KDColor KDColorBlack = KDColor::RGB24(0x000000);
constexpr KDColor KDColorWhite = KDColor::RGB24(0xFFFFFF);
constexpr KDColor KDColorRed = KDColor::RGB24(0xFF0000);
constexpr KDColor KDColorGreen = KDColor::RGB24(0x00FF00);
constexpr KDColor KDColorBlue = KDColor::RGB24(0x0000FF);
constexpr KDColor KDColorYellow = KDColor::RGB24(0xFFFF00);
constexpr KDColor KDColorOrange = KDColor::RGB24(0xFF9900);
constexpr KDColor KDColorPurple = KDColor::RGB24(0xFF00DD);

#endif
