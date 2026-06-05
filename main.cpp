#include "daisysp.h"
#include "daisysp-lgpl.h"
#include "daisy_patch_sm.h"

#include "library/ui/UIManager.h"
#include "library/ui/UIManager.cpp"
#include "library/tools.cpp"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;
using namespace sudwalfulkaan;

// Constants
#define SAMPLE_RATE 48000
#define NUM_POTS 4
#define AUDIO_BLOCK_SIZE 4

// Hardware and UI
static DaisyPatchSM hw;
static UIManager ui;

// Callback functions
void OnPotChanged(int index, float value) {
    switch (index) {
        case patch_sm::CV_1:
            // Pot 1 changed
            break;

        case patch_sm::CV_2:
            // Pot 2 changed
            break;

        case patch_sm::CV_3:
            // Pot 3 changed
            break;

        case patch_sm::CV_4:
            // Pot 4 changed
            break;
    }
}

void OnToggleChanged(bool toggleState) {
    hw.PrintLine("Toggle is now %s\n", toggleState ? "ON" : "OFF");
}

void OnButtonPressed() {
    hw.PrintLine("Button pressed\n");
}

void UpdateValues() {
    // Update pot values
    for (int i = 0; i < NUM_POTS; i++) {
        ui.SetPotValue(i, hw.GetAdcValue(i));
    }
    ui.Update();
    
    // Set callbacks only once
    static bool callbacksSet = false;
    if (!callbacksSet) {
        ui.SetPotChangedCallback(OnPotChanged);
        ui.SetToggleChangedCallback(OnToggleChanged);
        ui.SetButtonPressedCallback(OnButtonPressed);
        callbacksSet = true;
    }
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) {
    hw.ProcessAllControls();
    
    for (size_t i = 0; i < size; i++) {
        float left = in[0][i];
        float right = in[1][i];

        // Audio processing goes here
        
        // Output
        out[0][i] = left;
        out[1][i] = right;
    }
}

int main(void) {
    // Hardware initialization
    hw.Init();
    hw.SetAudioBlockSize(AUDIO_BLOCK_SIZE);
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
    hw.PrintLine("TwinningDelay starting...");
    hw.StartAdc();

    // Setup UI with pot configurations
    PotConfig configs[NUM_POTS] = {
        {0.0f, 0.05f, 0.0f, 1.0f, MappingType::Linear},      // CV_1: (0..1)
        {0.0f, 0.1f, 0.0f, 1.0f, MappingType::Linear},       // CV_2: (0..1)
        {0.0f, 0.1f, 0.0f, 1.0f, MappingType::Linear},       // CV_3: (0..1)
        {0.0f, 0.1f, 0.0f, 1.0f, MappingType::Linear}        // CV_4: (0..1)
    };
    ui.Init(configs);

    // Wait for controls to stabilize
    System::Delay(100);

    // Start audio processing
    hw.StartAudio(AudioCallback);

    // Main loop
    while (1) {
        UpdateValues();
        System::Delay(10);
    }
}

