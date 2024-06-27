#include <Arduino.h>
#include <math.h>

// define the coefficients from the logistic regression model
#define INTERCEPT 2.3823336105565747
#define COEFF_1 1.01339574
#define COEFF_2 -1.17619725
#define COEFF_3 1.64983467
#define COEFF_4 1.48109115

// define mean and standard deviation for scaling input values
#define MEAN_X1 5.856462585034014
#define STD_X1 0.8262749807650088
#define MEAN_X2 3.05578231292517
#define STD_X2 0.435519746063226
#define MEAN_X3 3.780272108843538
#define STD_X3 1.7531173189835423
#define MEAN_X4 1.2088435374149662
#define STD_X4 0.7552920028261023

// sigmoid function for classification
float sigmoid(float z) {
    return 1.0 / (1.0 + exp(-z));
}

// Logistic Regression prediction function with scaling
float predict(float X1, float X2, float X3, float X4) {
    
    // scale the input values
    float scaled_X1 = scale_value(X1, MEAN_X1, STD_X1);
    float scaled_X2 = scale_value(X2, MEAN_X2, STD_X2);
    float scaled_X3 = scale_value(X3, MEAN_X3, STD_X3);
    float scaled_X4 = scale_value(X4, MEAN_X4, STD_X4);

    // linear combination of scaled inputs and coefficients
    float linear_combination = INTERCEPT + (COEFF_1 * scaled_X1) + (COEFF_2 * scaled_X2) + (COEFF_3 * scaled_X3) + (COEFF_4 * scaled_X4);

    // apply the sigmoid function
    float probability = sigmoid(linear_combination);

    return probability;
}

// function to scale a value
float scale_value(float value, float mean, float std) {
    return (value - mean) / std;
}

void setup() {
    // initialize serial communication for debugging
    Serial.begin(115200);
}

void loop() {
    // check if data is available in the serial buffer
    if (Serial.available() > 0) {
        // read the input string
        String input = Serial.readStringUntil('\n');

        // convert the input string to a mutable char array
        char inputArray[input.length() + 1];
        input.toCharArray(inputArray, input.length() + 1);

        // parse the input string
        float values[4];
        int index = 0;
        char* token = strtok(inputArray, ",");
        while (token != NULL && index < 4) {
            values[index] = atof(token);
            token = strtok(NULL, ",");
            index++;
        }

        // check if we have the correct number of input values
        if (index == 4) {

            // predict the probability with scaled values
            float probability = predict(values[0], values[1], values[2], values[3]);

            if(probability > 0.5) {
              Serial.println("Versicolor");
            } else {
              Serial.println("Setosa");
            }

        } else {
            Serial.println("ERROR!!");
        }
    }
}
