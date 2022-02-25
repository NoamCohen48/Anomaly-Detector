/*
 * Created by:
 * Shaked Cohen, Shakedc159@gmail.com.
 * Noam Cohen, cohennoam48@gmail.com.
*/

#include "SimpleAnomalyDetector.h"

/**
 * Train the model on a given table.
 * @param ts TimeSeries on which we learn
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {

    // Get the columns and the rows
    int columns = ts.getColumnsSize();
    int rows = ts.getRowsSize();

    // Finding correlation
    for (int firstId = 0; firstId < columns; ++firstId) {

        // Init to the first
        float maxPearson = 0;
        int secondIdMax = -1;

        const std::vector<float> &firstVec = ts.getColumn(firstId);

        // For every column compare to the others columns
        for (int secondId = firstId + 1; secondId < columns; ++secondId) {

            // Init to the second
            const std::vector<float> &secondVec = ts.getColumn(secondId);

            // Calculating the Pearson Value
            float pearsonValue = fabs(pearson(firstVec, secondVec, rows));

            // Switching with max if needed
            if (pearsonValue > maxPearson) {
                maxPearson = pearsonValue;
                secondIdMax = secondId;
            }
        }


        // Adding to vector
        addCF(ts, firstId, secondIdMax, maxPearson);
    }
}

/**
 * Calculate max deviation of a given 2 columns which represent x and y values and a line.
 * @param firstVec, secondVec x value and y value of the points as vec.
 * @param size the size of the vectors.
 * @param regLine the line to calculate max deviation.
 * @return the max deviation.
 */
float SimpleAnomalyDetector::calculateDeviation(const std::vector<float> &firstVec, const std::vector<float> &secondVec,
                                                int size, const Line &regLine) {
    float maxDev = 0;

    // Run through the line in the columns
    for (int i = 0; i < size; ++i) {
        Point point(firstVec[i], secondVec[i]);

        // Calculate the dev of the point
        float curDev = dev(point, regLine);

        // Save the max dev
        if (curDev >= maxDev) {
            maxDev = curDev;
        }
    }
    return maxDev;
}

/**
 * Add correlated feature to the correlated features vector.
 * @param firstColumn, secondColumn the columns name as string.
 * @param pearson the correlation of the two columns.
 * @param regLine the regression line.
 * @param maxDev the max deviation of the two columns.
 */
void SimpleAnomalyDetector::addCF(const TimeSeries &ts, const int &firstId, const int &secondId, const float &pearson) {

    // If not above pearsonThreshold we continue
    if (pearson < pearsonThreshold || secondId == -1) {
        return;
    }

    // get the columns
    const std::vector<float> &firstVec = ts.getColumn(firstId);
    const std::vector<float> &secondVec = ts.getColumn(secondId);

    // adding correlated Feature
    string firstName = ts.getColumnName(firstId);
    string secondName = ts.getColumnName(secondId);
    Line line = linear_reg(firstVec, secondVec, ts.getRowsSize());
    float threshold = calculateDeviation(firstVec, secondVec, ts.getRowsSize(), line) * 1.1f;

    // adding the cf
    cf.emplace_back(firstName, secondName, pearson, line, Point(0, 0), threshold);
}

/**
 * Detect false points on a given table.
 * @param ts the table to detect false points on.
 * @return a vector of all the false points (AnomalyReport).
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> result;

    // For every correlated Feature
    for (const correlatedFeatures &correlatedFeature: cf) {

        const std::vector<float> &firstVec = ts.getColumn(correlatedFeature.feature1);
        const std::vector<float> &secondVec = ts.getColumn(correlatedFeature.feature2);
        int size = ts.getRowsSize();

        // Check for false points
        for (int i = 0; i < size; ++i) {
            // If dev is greater than Threshold, add the anomaly report
            if (isAnomaly(firstVec[i], secondVec[i], correlatedFeature)) {
                result.emplace_back(correlatedFeature.feature1 + "-" + correlatedFeature.feature2, i + 1);
            }
        }
    }

    return result;
}

bool SimpleAnomalyDetector::isAnomaly(float x, float y, const correlatedFeatures &correlatedFeature) {
    Point point(x, y);
    float curDev = dev(point, correlatedFeature.lin_reg);

    if (curDev >= correlatedFeature.threshold)
        return true;
    return false;
}

float SimpleAnomalyDetector::getPearsonThreshold() const {
    return pearsonThreshold;
}

void SimpleAnomalyDetector::setPearsonThreshold(float pearsonThreshold) {
    SimpleAnomalyDetector::pearsonThreshold = pearsonThreshold;
}
