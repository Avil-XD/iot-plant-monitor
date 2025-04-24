#ifndef TINYML_INFERENCE_H
#define TINYML_INFERENCE_H

#include "model.h"
#include "scaler.h"
#include <TensorFlowLite_ESP.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>

// TensorFlow Lite globals
extern tflite::MicroErrorReporter micro_error_reporter;
extern tflite::AllOpsResolver resolver;
extern const tflite::Model* model_tfl;
extern tflite::MicroInterpreter* interpreter;
extern TfLiteTensor* input;
extern TfLiteTensor* output;

// Constants
constexpr int kTensorArenaSize = 8 * 1024;
extern uint8_t tensor_arena[];

// Function declarations
bool setupTinyML();
float predictWateringNeed(float temperature, float humidity, float light, float soilMoisture);

#endif // TINYML_INFERENCE_H