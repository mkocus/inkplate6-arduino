#ifndef __HELPERS_H__
#define __HELPERS_H__

uint16_t rgbto565(uint8_t rgb) {
  return (((rgb & 0xf8)<<8) + ((rgb & 0xfc)<<3)+(rgb>>3));
}

//void displayTestImage() {
//  display.clearDisplay();
//  for (int i=0; i< 8; ++i) {
//      display.fillRect(i*128, 0, 128, 758, rgbto565(i*8));
//  }
//}

#endif
