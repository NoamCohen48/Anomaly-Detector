/*
 * Created by:
 * Shaked Cohen, Shakedc159@gmail.com.
 * Noam Cohen, cohennoam48@gmail.com.
*/

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include <tuple>
#include "HybridAnomalyDetector.h"

using namespace std;


class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() = default;

    // you may add additional methods here
    void readFile(const string &filePath) {
        ofstream file(filePath);
        string line = read();
        while (line != "done") {
            file << line + '\n';
            line = read();
        }
        file.close();
    }
};

struct seqResult {
    int start;
    int end;
    string description;
    bool truePositive;
};

struct Database {
    float threshold;
    vector<AnomalyReport> result;
    vector<seqResult> seqResults;
    int numOfRows;

    Database() : threshold(HybridAnomalyDetector().getPearsonThreshold()) {
    }
};

class Command {
protected:
    DefaultIO *dio;
public:
    explicit Command(DefaultIO *dio) : dio(dio) {}

    virtual ~Command() = default;

    virtual void execute(Database &database) = 0;

    string description;
};

// option 1
class uploadCSVCommand : public Command {
public:

    explicit uploadCSVCommand(DefaultIO *dio) : Command(dio) {
        this->description = "upload a time series csv file\n";
    }

    void execute(Database &database) override {
        // get train csv
        dio->write("Please upload your local train CSV file.\n");
        dio->readFile("anomalyTrain.csv");
        dio->write("Upload complete.\n");

        // get test csv
        dio->write("Please upload your local test CSV file.\n");
        dio->readFile("anomalyTest.csv");
        dio->write("Upload complete.\n");
    }
};

// option 2
class algorithmSettingsCommand : public Command {
public:
    explicit algorithmSettingsCommand(DefaultIO *dio) : Command(dio) {
        this->description = "algorithm settings\n";
    }

    void execute(Database &database) override {
        float wantedThreshold;

        while (true) {

            // Write to the client.
            dio->write("The current correlation threshold is ");
            dio->write(database.threshold);
            dio->write("\nType a new threshold\n");

            // Get wanted threshold from the client.
            dio->read(&wantedThreshold);

            // Check if the Threshold is between 0 and 1.
            if (wantedThreshold > 0 && wantedThreshold <= 1) {

                // save the new threshold.
                database.threshold = wantedThreshold;
                break;
            }
            dio->write("please choose a value between 0 and 1.\n");
        }
    }
};

// option 3
class detectAnomaliesCommand : public Command {
public:
    explicit detectAnomaliesCommand(DefaultIO *dio) : Command(dio) {
        this->description = "detect anomalies\n";
    }


    void initSequencesVec(Database &database) {
        seqResult result;
        result.start = 0;
        result.end = 0;
        result.truePositive = false;
        result.description = "";

        // Run through the lines in the database.
        for (auto &line: database.result) {

            // Check if the last line where in the same description and next timeStep.
            if (line.timeStep == result.end + 1 && line.description == result.description) {
                result.end++;
            } else {

                // Else update the result to point the next line.
                database.seqResults.push_back(result);
                result.start = line.timeStep;
                result.end = result.start;
                result.description = line.description;
            }
        }

        // Push the last result.
        database.seqResults.push_back(result);

        // Delete the first result (always empty).
        database.seqResults.erase(database.seqResults.begin());
    }

    void execute(Database &database) override {
        TimeSeries timeSeries("anomalyTrain.csv");

        HybridAnomalyDetector anomalyDetector;
        anomalyDetector.setPearsonThreshold(database.threshold);

        // learning
        anomalyDetector.learnNormal(timeSeries);

        // detecting
        timeSeries = TimeSeries("anomalyTest.csv");
        database.result = anomalyDetector.detect(timeSeries);

        // building the sequences vectors
        initSequencesVec(database);

        // Get the size.
        database.numOfRows = timeSeries.getRowsSize();

        // sending completed message.
        dio->write("anomaly detection complete.\n");
    }
};

// option 4
class displayResultsCommand : public Command {
public:
    explicit displayResultsCommand(DefaultIO *dio) : Command(dio) {
        this->description = "display results\n";
    }

    void execute(Database &database) override {

        // Run through lines in the result vector and write them.
        for (const auto &line: database.result) {
            dio->write(to_string(line.timeStep) + " \t" + line.description + "\n");
        }

        // Write done.
        dio->write("Done.\n");
    }
};

// option 5
class uploadAnomaliesCommand : public Command {
public:
    explicit uploadAnomaliesCommand(DefaultIO *dio) : Command(dio) {
        this->description = "upload anomalies and analyze results\n";
    }

    bool isTruePositive(int start, int end, Database &database) {
        const auto &anomalies = database.result;

        // Run through all the lines in the result.
        for (auto &report: database.seqResults) {

            if (start <= report.end && end >= report.start) {
                report.truePositive = true;
                return true;
            }
        }
        return false;
    }

    void execute(Database &database) override {

        string clientLine;
        int truePositive = 0;
        int falsePositive = 0;
        int sum = 0;
        int positive = 0;
        int n = 0;

        dio->write("Please upload your local anomalies file.\n");

        // resetting the flag
        for (auto &result: database.seqResults) {
            result.truePositive = false;
        }

        // Run through the received lines from the client.
        while ((clientLine = dio->read()) != "done") {

            // Get the start time.
            string startStr = clientLine.substr(0, clientLine.find(','));
            int start = stoi(startStr);

            // Get the end time.
            string endStr = clientLine.erase(0, startStr.size() + 1); // +\n
            //int end = stoi(endStr.erase(endStr.size() - 1, endStr.size()));
            int end = stoi(endStr);

            // Check if the received in the range.
            if (isTruePositive(start, end, database)) {
                truePositive++;
            }

            // Summering the table length.
            sum += end - start + 1;
            positive++;
        }

        // Calculate the FP.
        for (auto &result: database.seqResults) {

            if (!result.truePositive) {
                falsePositive++;
            }
        }

        // Calculate N.
        n = database.numOfRows - sum;

        dio->write("Upload complete.\n");

        float truePositiveRatio = ((int) (1000.0 * truePositive / positive)) / 1000.0f;
        float falsePositiveRatio = ((int) (1000.0 * falsePositive / n)) / 1000.0f;

        // Write TP/P.
        dio->write("True Positive Rate: ");
        dio->write(truePositiveRatio);

        // Write FP/N.
        dio->write("\nFalse Positive Rate: ");
        dio->write(falsePositiveRatio);
        dio->write("\n");
    }
};

// option 6
class exitCommand : public Command {
public:
    explicit exitCommand(DefaultIO *dio) : Command(dio) {
        this->description = "exit\n";
    }

    void execute(Database &database) override {
        //TODO: exit the program
    }
};

#endif /* COMMANDS_H_ */
