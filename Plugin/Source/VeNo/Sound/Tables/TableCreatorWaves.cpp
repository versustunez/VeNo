#include "TableCreator.h"

#include <valarray>

namespace VeNo::Audio {
enum WaveForms {
  SAW = 0,
  SINE,
  SQUARE,
  TRIANGLE,
};
void TableCreator::CreateAll() {
  CreateTable(SAW, [](RawTable *rawTable) {
    auto &data = rawTable->Data;
    data[0] = data[RawTable::TableLength >> 1u] = 0;
    for (size_t i = 1; i < (RawTable::TableLength >> 1u); i++) {
      data[i] = 1.0f / i;
      data[RawTable::TableLength - i] = -data[i];
    }
  });
  CreateTable(SINE, [](RawTable *rawTable) {
    rawTable->IsNonImpulse = true;
    double angleDelta = 6.28318530718 / (RawTable::TableLength);
    double currentAngle = 0;
    for (size_t i = 0; i < RawTable::TableLength; i++) {
      rawTable->Data[i] = std::sin(currentAngle);
      currentAngle += angleDelta;
    }
  });

  CreateTable(SQUARE, [](RawTable *rawTable) {
    rawTable->Data[0] = rawTable->Data[RawTable::TableLength >> 1u] = 0.0;
    for (unsigned int i = 1; i < (RawTable::TableLength >> 1u); i++) {
      double temp = (i & 0x01u) ? 1.0 / i : 0.0;
      rawTable->Data[i] = -temp;
      rawTable->Data[RawTable::TableLength - i] = temp;
    }
  });

  CreateTable(TRIANGLE, [](RawTable *rawTable) {
    double sign = 0.9;
    for (unsigned int i = 1; i < (RawTable::TableLength >> 1u); i++) {
      double temp = (i & 0x01u) ? 1.0 / (i * i) * (sign = -sign) : 0.0;
      rawTable->Data[i] = -temp;
      rawTable->Data[RawTable::TableLength - i] = temp;
    }
  });
}
} // namespace VeNo::Audio
