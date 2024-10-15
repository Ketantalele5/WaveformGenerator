#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>

class Waveform {
public:
    enum WaveformType { SINE, SQUARE, TRIANGLE };

private:
    WaveformType type;
    float frequency;
    float amplitude;
    float phase;

public:
    Waveform(WaveformType t, float f, float a, float p = 0.0f)
        : type(t), frequency(f), amplitude(a), phase(p) {}

    float generate(float time) {
        switch (type) {
            case SINE:
                return amplitude * sin(2 * M_PI * frequency * time + phase);
            case SQUARE:
                return (sin(2 * M_PI * frequency * time) >= 0) ? amplitude : -amplitude;
            case TRIANGLE: {
                float period = 1.0f / frequency;
                float t = fmod(time, period) / period;
                return amplitude * (2 * (t < 0.5f ? t : 1 - t));
            }
            default:
                return 0.0f;
        }
    }

    void saveToCSV(const std::vector<float>& times, const std::vector<float>& values, const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << "Time,Value\n";
            for (size_t i = 0; i < times.size(); ++i) {
                file << times[i] << "," << values[i] << "\n";
            }
            file.close();
            std::cout << "Data saved to " << filename << std::endl;
        } else {
            std::cerr << "Error opening file!" << std::endl;
        }
    }
};

int main() {
    std::string waveformTypeInput;
    float frequency, amplitude, phase = 0.0f;

    std::cout << "Enter waveform type (sine/square/triangle): ";
    std::cin >> waveformTypeInput;

    Waveform::WaveformType type;
    if (waveformTypeInput == "sine") {
        type = Waveform::SINE;
    } else if (waveformTypeInput == "square") {
        type = Waveform::SQUARE;
    } else if (waveformTypeInput == "triangle") {
        type = Waveform::TRIANGLE;
    } else {
        std::cerr << "Invalid waveform type!" << std::endl;
        return 1;
    }

    std::cout << "Enter frequency (Hz): ";
    std::cin >> frequency;
    std::cout << "Enter amplitude: ";
    std::cin >> amplitude;
    std::cout << "Enter phase shift (radians, optional, default is 0): ";
    std::cin >> phase;

    Waveform waveform(type, frequency, amplitude, phase);

    // Generate waveform data
    std::vector<float> times;
    std::vector<float> values;
    const int sampleRate = 1000; // 1000 samples per second
    const float duration = 1.0f; // 1 second
    for (int i = 0; i < sampleRate * duration; ++i) {
        float time = i / static_cast<float>(sampleRate);
        times.push_back(time);
        values.push_back(waveform.generate(time));
    }

    // Save to CSV
    waveform.saveToCSV(times, values, "waveform.csv");

    return 0;
}
