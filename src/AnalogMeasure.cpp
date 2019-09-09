/* AnalogMeasure library by JJ Fenstermaker
 */
 //------------
// Measure Analog Voltage
// ------------
#include "AnalogMeasure.h"
#include "Particle.h"


/*-------------
This function measures the analog voltage read from one or more of the analog pins on Particle Argon 
Microcontroller.
The maximum voltage input to this pin is 3.3 volts.  If the raw input is higher steps must be taken to reduce
the input voltage.  
The read_Analog_Voltage function reads the voltage, and then normalizes it to the expected voltage from the system

inputs: Analog input pin to read
outputs:  A float value of voltage or temperature, depending on the data requested
Dependencies:  
  None:  This is a stand alone class
  Setup():  None required
To Do:  Add calibration of sensors

-------------*/

// Constructor initializes the calibration factor to 1.0  There is no destructor

//Constructor

AnalogMeasurement::AnalogMeasurement()
{
  calibrationFactor = 1.0;
}


//public

float AnalogMeasurement::Process_Analog_Data(char analoginputpin)
{
//   measureAnalogvalue
//   A generalized function to measure the analog data from one of the
//   Particle's analog input pins
//    Input:  Pin to measure.  Currently supports: 
//            ADC_INPUT_TEMPERATURE
//            ADC_INPUT_TEMPERATURE
//            ADC_INPUT_VOLTAGE
//            any other input returns ADC_UNSUPPORTED
//            additional inputs can be added
//
//    Calls private function sample_and_average_analog_values()  To perform sampling and averaging of readings       
//    Output:  A float value appropriate to the ADC selected.  
//             For ADC_INPUT_VOLTAGE the voltage is returned.  
//             For ADC_INPUT_TEMPERATURE the temperature in  Fahrenheit is returned
//             for ADC_REFERENCE_VOLTAGE the raw voltage is returned
//             Any other ADC (Analog pin read) requested returns ADC_UNSUPPORTED
//

  float averageofanalogsamples = 0.0;

  averageofanalogsamples = Sample_And_Average_Analog_Input(analoginputpin);
  switch (analoginputpin)
  {
    case  ADC_INPUT_VOLTAGE:       // if this is a voltage measurement
    {
  // the voltage divider changes it by an order of magnitude to make it compatible with the analog input pin on the Argon
  //the Argon's maximum input voltage is 3.3 volts, the batteries output 24 volts, the charger 27 volts 	
      float convertedvoltage = 0.0;                              // the voltage measured by the ADC_INPUT_VOLTAGE pin
                                                                 // It will be between 0 and 3.3 volts



      convertedvoltage = averageofanalogsamples * VOLTAGE_MULTIPLIER;  // turn it into a value between 0 - 27 volts
      convertedvoltage *= calibrationFactor;                           // conpensate for offset in analog pin
      return convertedvoltage;                                         // return voltage
    } 
    case ADC_INPUT_TEMPERATURE:                                // this is a temperature Measurement
    {
      float temperaturecelsius = 0;    // the temperature measure in Celsius
      //float temperaturefahrenheit = 0; // the temperature measured in Fahrenheit

      temperaturecelsius = (averageofanalogsamples  - 0.50) * 100;             // Converting from volts
      //temperaturefahrenheit = (temperaturecelsius * (9.0/5.0)) + 32 + 0.5;     // Convert to Fahrenheit
      temperaturecelsius *= TEMPERATURE_COMPENSATION;                       // compensate for Analog offset
      temperaturecelsius += ENCLOSURE_DELTA;                                // additional temperature shift 
                                                                               // for enclosure
      return temperaturecelsius;                                            // return temperature
     }
    case ADC_REFERENCE_VOLTAGE:                                  // This is a reference voltage measurement        
    {           
      return averageofanalogsamples;                                   // return the average of samples of the 
                                                                 // raw voltage
    }    
    default:                                                     // not a supported pin value          
    {             
      return ADC_UNSUPPORTED;
    }
  } 
}   

void AnalogMeasurement::Calibrate_Analog_Voltage(float CANVoltage)
//   
//    A function to calibrate the analog input pins on the Argon
//    There is an offset to all of the reading's - a grounded analog input pin
//    Does not measure 0 volts
//    This algorithm takes a known voltage as an input and uses it to calibrate  
//    of the A/D 
//    Input:  Measured CAN Voltage
//    Class variable modified:  calibrationFactor
//    Output:  none.  
//
{
  float convertedvoltage = 0.0;        // the voltage measured by the ADC_INPUT_VOLTAGE pin
  float averageofsamples = 0.0;         // Average of samples taken
  
  averageofsamples = Sample_And_Average_Analog_Input(ADC_INPUT_VOLTAGE);  // It will be between 
                                                                          //0 and 3.3 volts
  convertedvoltage = averageofsamples * VOLTAGE_MULTIPLIER;               // turn it into a value 
                                                                          // between 0 - 27 volts
  calibrationFactor = CANVoltage / convertedvoltage;

 return;    
}

// private

float AnalogMeasurement::Sample_And_Average_Analog_Input(char AnalogInputPin)
//   
//   A generalized function to measure the analog data from one of the
//   Particle's analog input pins SAMPLES_TAKEN times and average the data
//    Input:  Pin to measure
//
//    Output:  A float value that is the average in volts of the of SAMPLES_TAKEN measurements.  
//
{
  float sumofsamples = 0.0;         // the sum of all of the samples taken
  float averageofsamples;          // Average of samples taken
 
  for  (uint8_t index = 0; index < SAMPLES_TAKEN; index++)
    {
        // read the input value into sample
        // multiply it by the resolution volts of the ADC
        // add it to the sum of all measurements
      sumofsamples += (float)analogRead(AnalogInputPin) * ADC_INPUT_RESOLUTION;
      delay(ANALOG_SAMPLE_TIME);  // wait before we take the next sample. 
     }
  averageofsamples = sumofsamples/SAMPLES_TAKEN;     // now get the average
 return averageofsamples;    
}

