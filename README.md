# Multi-Tap Delay: Delay2
### Project Overview
This document presents a multi-tap delay effect plugin developed as part of the UWL AP2 assignment 1. The plugin uses the VST3 framework to provide a versatile and creative delay effect for audio processing.

**update your directory**
Ensure that your directory or source file locations correctly match where the files have been downloaded to. An incorrect directory path is known to cause issues with CMake and Xcode build processes.

### Usage Instructions
1. Build the Project: Before using the plugin, it is essential to build the project using the provided source code. Ensure all dependencies are correctly installed and the build process completes successfully.

2. Test with Example Audio Samples: To verify the functionality of the multi-tap delay effect, it's recommended to test the plugin with the provided example audio samples. These audio samples have been selected to showcase various aspects of the delay effect. You can compare the original audio with the processed output to assess the impact of the multi-tap delay.
   
3. Initial Setup: When you first load the multi-tap delay plugin in your preferred Digital Audio Workstation (DAW), it is crucial to adjust the following parameters to ensure proper initialization:
   
    - Master Gain: Adjust the master gain parameter to set the overall output volume of the effect. Start with a conservative value and fine-tune it per your requirements to prevent audio clipping or distortion.
      
    - Dry and Wet Mixes: The dry-wet mix parameter determines the balance between the original (dry) and processed (wet) audio signals. Adjust this parameter to hear the effect's intensity. For a purely dry call, set it to 0; for a thoroughly wet signal, set it to 100%.
      
4. Caution: Failure to follow the initial setup step may lead to audio issues. The multi-tap delay effect relies on appropriate parameter reading during initialization. If the master gain and dry-wet mix are not set correctly, the output audio may not sound as expected, and you may experience no audio output.

Please refer to the project documentation for any additional information and full references of the material used.
