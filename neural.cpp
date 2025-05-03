#include "neural.h"

// TensorFlow Lite Micro başlıqları
#include <tensorflow/lite/micro/micro_interpreter.h>
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/system_setup.h"

// Model məlumatları (binary formatda daxil edilmiş)
#include "model.h"

// Modeli TensorFlow Lite-dan yükləyirik
const tflite::Model *model = tflite::GetModel(g_model);

// Lazım olan əməliyyatları qeyd edirik (burada yalnız FullyConnected)
static tflite::MicroMutableOpResolver<2> resolver;

// Tensör yaddaşı (modelin giriş/çıxış və ara nəticələri üçün istifadə olunur)
static uint8_t tensor_arena[TENSOR_ARENA_SIZE];

// Interpreter: modelin işləməsi üçün əsas obyekt
static tflite::MicroInterpreter interpreter(
  model, resolver, tensor_arena, TENSOR_ARENA_SIZE);

// NeuralNet sinifinin konstruktoru
NeuralNet::NeuralNet() {
  // Model versiyasını yoxlayırıq
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    MicroPrintf("Model versiyası uyğun gəlmir: %d", model->version());
    return;
  }

  // Lazım olan operatorları əlavə edirik (burada FullyConnected)
  if (resolver.AddFullyConnected() != kTfLiteOk) {
    MicroPrintf("FullyConnected operatoru əlavə edilə bilmədi!");
    return;
  }
  // Lazım olan operatorları əlavə edirik (burada FullyConnected)
  if (resolver.AddSoftmax() != kTfLiteOk) {
    MicroPrintf("Softmax operatoru əlavə edilə bilmədi!");
    return;
  }
  // Tensörləri ayırırıq
  if (interpreter.AllocateTensors() != kTfLiteOk) {
    MicroPrintf("Tensörlərin ayrılması uğursuz oldu!");
    return;
  }
}
int NeuralNet::getData(float data[], bool quant_status) {
  TfLiteTensor *input = interpreter.input(0);
  TfLiteTensor *output = interpreter.output(0);

  // Giriş tensor boyutunu al
  int input_size = input->dims->data[1];

  // Giriş verisini yükle
  if (quant_status) {
    for (int i = 0; i < input_size; i++) {
      input->data.int8[i] = static_cast<int8_t>(data[i] / input->params.scale + input->params.zero_point);
    }
  } else {
    for (int i = 0; i < input_size; i++) {
      input->data.f[i] = data[i];
    }
  }

  // Modeli çalıştır
  if (interpreter.Invoke() != kTfLiteOk) {
    MicroPrintf("Model çalıştırılamadı!");
    return -1;
  }

  // Çıkış boyutunu al
  int output_size = output->dims->data[1];

  // En yüksek olasılığı bul
  int best_index = 0;
  float max_score = output->data.f[0];

  for (int i = 1; i < output_size; i++) {
    if (output->data.f[i] > max_score) {
      max_score = output->data.f[i];
      best_index = i;
    }
  }
  printf("Tahmin edilen score: %f\n", max_score);
  return best_index;
}