#include "tinyml_inference.h"

// TensorFlow Lite globals
tflite::MicroErrorReporter micro_error_reporter;
tflite::AllOpsResolver resolver;
const tflite::Model* model_tfl = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

// Allocate tensor arena
uint8_t tensor_arena[kTensorArenaSize];

bool setupTinyML() {
    // Map the model into a usable data structure
    model_tfl = tflite::GetModel(model);
    if (model_tfl->version() != TFLITE_SCHEMA_VERSION) {
        Serial.println("Model schema mismatch!");
        return false;
    }

    // Create an interpreter to run the model
    static tflite::MicroInterpreter static_interpreter(
        model_tfl, resolver, tensor_arena, kTensorArenaSize, &micro_error_reporter);
    interpreter = &static_interpreter;

    // Allocate memory for the model's input and output tensors
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        Serial.println("AllocateTensors() failed");
        return false;
    }

    // Get pointers for the model's input and output tensors
    input = interpreter->input(0);
    output = interpreter->output(0);

    return true;
}

float predictWateringNeed(float temperature, float humidity, float light, float soilMoisture) {
    // Prepare input data
    float input_data[4] = {temperature, humidity, light, soilMoisture};
    
    // Standardize input using saved scaler parameters
    for (int i = 0; i < 4; i++) {
        input_data[i] = (input_data[i] - scaler_mean[i]) / scaler_scale[i];
    }
    
    // Copy input data to model's input tensor
    for (int i = 0; i < 4; i++) {
        input->data.f[i] = input_data[i];
    }
    
    // Run inference
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
        Serial.println("Invoke failed!");
        return -1;
    }
    
    // Get prediction (0 = no water needed, 1 = needs water)
    float prediction = output->data.f[0];
    return prediction;
}