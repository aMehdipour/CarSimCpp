#pragma once

#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>
#include "MathTools.h"

class Telemetry {

public:
    Telemetry(const std::string& filename, const std::vector<std::string>& columnHeaders)
        : file_(filename), headers_(columnHeaders), isFirstRow_(true) {
        if (!file_.is_open()) {
            throw std::runtime_error("Unable to open telemetry file: " + filename);
        }
    }

    ~Telemetry() {
        if (file_.is_open()) {
            file_.close();
        }
    }

    void log(const std::vector<double>& data) {
        if (!file_.is_open()) return;

        // Write headers on the first row
        if (isFirstRow_) {
            for (size_t i = 0; i < headers_.size(); ++i) {
                file_ << headers_[i] << (i + 1 < headers_.size() ? "," : "\n");
            }
            isFirstRow_ = false;
        }

        // Write numerical data
        for (size_t i = 0; i < data.size(); ++i) {
            file_ << data[i] << (i + 1 < data.size() ? "," : "\n");
        }
    }

private:
    std::ofstream file_;
    std::vector<std::string> headers_;
    bool isFirstRow_;  // Ensures headers are written once
};

class Output {
public:
    // Default construcor -- no output
    Output() : maxPoints_(-1), flagUseDenseOutput_(false), savedCount_(0) {};

    // Constructor for normal or dense output
    Output(
        const int numSaveIntervals,
        const int maxPoints = 500
    ) :
    numSaveIntervals_(numSaveIntervals),
    maxPoints_(maxPoints) {
        savedCount_ = 0;
        flagUseDenseOutput_ = (numSaveIntervals > 0);
        
        if (maxPoints_ > 0) {
            timePoints_.resize(maxPoints_);
        }
    }

    void initialize(const int numStates, const double startTime, const double endTime) {
        numStates_ = numStates;
        if (maxPoints_ == -1) {
            return;
        }

        statePoints_.resize(numStates_);
        for (size_t i = 0; i < numStates_; ++i) {
            statePoints_[i].resize(maxPoints_);
        }

        if (flagUseDenseOutput_) {
            startTime_ = startTime;
            endTime_ = endTime;
            nextOutputTime_ = startTime;
            outputTimeStep_ = (endTime - startTime) / numSaveIntervals_;
        }
    }

    // Resize old storage if necessary
    void resize() {
        int oldMaxPoints = maxPoints_;
        maxPoints_ *= 2;

        std::vector<double> tempTimePoints = timePoints_;
        timePoints_.resize(maxPoints_);
        for (size_t k = 0; k < oldMaxPoints; ++k) {
            timePoints_[k] = tempTimePoints[k];
        }

        std::vector<std::vector<double>> tempStatePoints = statePoints_;
        for (size_t i = 0; i < numStates_; ++i) {
            statePoints_[i].resize(maxPoints_);
            for (size_t k = 0; k < oldMaxPoints; ++k) {
                statePoints_[i][k] = tempStatePoints[i][k];
            }
        }
    }

    void save(const double time, const MathTools::StateVector& stateVector) {
        if (maxPoints_ <= 0) {
            return;
        }

        if (savedCount_ == maxPoints_) {
            resize();
        }

        for (size_t i = 0; i < numStates_; ++i) {
            statePoints_[i][savedCount_] = stateVector(i);
        }

        timePoints_[savedCount_++] = time;
    }

    template <class IntegratorType>
    void saveDenseOutput(IntegratorType& integrator, const double outputTime, const double stepSize) {
        if (savedCount_ == maxPoints_) {
            resize();
        }

        for (size_t i = 0; i < numStates_; ++i) {
            statePoints_[i][savedCount_] = integrator.getDenseOutput(i, outputTime, stepSize);
        }

        timePoints_[savedCount_++] = outputTime;

    }

    template <class IntegratorType>
    void processOutput(const int stepNum, const double time, const MathTools::StateVector& stateVector, IntegratorType& integrator, const double stepSize) {
        if (!flagUseDenseOutput_) {
            throw std::runtime_error("[Telemetry.h] Calling processOutput in Telemetry without setting flagUseDenseOutput!!!");
        }

        if (stepNum == -1) {
            save(time, stateVector);
            nextOutputTime_ += outputTimeStep_;
        } else {
            while ((time - nextOutputTime_) * (endTime_ - startTime_) > 0.) {
                saveDenseOutput(integrator, nextOutputTime_, stepSize);
                nextOutputTime_ += outputTimeStep_;
            }
        }
    }

    void writeToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open output file: " + filename);
        }

        // Write header
        file << "Time";
        for (int i = 0; i < numStates_; i++) {
            file << ",State" << i;
        }
        file << std::endl;

        // Write data
        for (int i = 0; i < savedCount_; i++) {
            file << timePoints_[i];
            for (int j = 0; j < numStates_; j++) {
                file << "," << statePoints_[j][i];
            }
            file << std::endl;
        }

        file.close();
    }

    bool getUseDenseOutput() const { return flagUseDenseOutput_; }
    int getMaxPoints() const { return maxPoints_; }
    double getLastSavedTime() const { return savedCount_ > 0 ? timePoints_[savedCount_] : 0.; }
    int getSavedCount() const { return savedCount_; }

    const std::vector<double>& getTimePoints() const { return timePoints_; }
    const std::vector<std::vector<double>>& getStatePoints() const { return statePoints_; }

private:
    int maxPoints_;
    int numStates_;
    int numSaveIntervals_;
    int savedCount_;

    bool flagUseDenseOutput_;

    double startTime_;
    double endTime_;
    double nextOutputTime_;
    double outputTimeStep_;

    std::vector<double> timePoints_;
    std::vector<std::vector<double>> statePoints_;
};

