#include "neural.h"
#include "predict.h"

#ifdef __cplusplus
extern "C" {
  int predict(float a1, float a2, float b1, float b2) {
    NeuralNet net = NeuralNet();
    float input[4] = { a1, a2, b1, b2 };
    int result = net.getData(input, false);
    return result;
  }
}
#endif
