#pragma once

/* AnalogMeasure library by JJ Fenstermaker
 */

// This will load the definition for common Particle variable types
#include "Particle.h"
//------------
// Analog Voltage Measurement header
// ------------

#ifndef ANALOG_VOLTAGE_MEASUREMENT_
#define ANALOG_VOLTAGE_MEASUREMENT_

// medium1 data tags - included for refernce purposes only
// VOLTAGE_TAG           4    data record tag number for analog voltage
// TEMPERATURE_TAG       5    data record tag number for temperature measurement

// Defines

// Common constants
#define SAMPLES_TAKEN 5                    // the number of samples taken per voltage Measurement
#define ANALOG_SAMPLE_TIME 1               // the amount of time to wait between sampling the
                                           // Argon's Analog input pins in milliseconds. 
                                           // Minimum is 1 microsecond 
#define ADC_INPUT_RESOLUTION  0.0008       // Resolution of the Argon's on board ADC 
                                           //  3.3 volts/4096 units 

// Analog Voltage Measurement
#define ADC_INPUT_VOLTAGE  A1              // The analog pin that is read to get the voltage
#define VOLTAGE_MULTIPLIER  10.0           // voltage multiplier to take input voltage and 
                                           //convert it to actual voltage

// Temperature Measuremen
#define ADC_INPUT_TEMPERATURE A2           // pin used to measure temperature
#define TEMPERATURE_COMPENSATION 0.965     // Temperature Compensation 
#define ENCLOSURE_DELTA          0.0       // degrees added by having the temperature sensor in the enclosure

// Calibration Measurement
#define ADC_REFERENCE_VOLTAGE   A4         // Reference Voltage Pin
#define CALIBRATION_SAMPLES_TAKEN 100
#define REFERENCE_VOLTAGE 3.3              //Reference for temperature#define TEMPERATURE_TAG           5        // record tag number for temperature - move to main header

//  default for undefined measurement
#define ADC_UNSUPPORTED  9999.0            // This measurement is not suported

//prototypes

class AnalogMeasurement{
    public:
    AnalogMeasurement();
    float Process_Analog_Data(char AnalogInputPin);  // process the analog data collected by sample_and_average_analog_values
                                                    // into a format that the cloud application can process
    void Calibrate_Analog_Voltage(float CANVoltage);

    float calibrationFactor;                          // float value used to calibrate the analog data pins

    private:         
     float Sample_And_Average_Analog_Input(char AnalogInputPin);  //Analog measurement and average function
     
};
#endif  // end of include guard
