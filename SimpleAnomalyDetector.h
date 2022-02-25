/*
 * Created by:
 * Shaked Cohen, Shakedc159@gmail.com.
 * Noam Cohen, cohennoam48@gmail.com.
*/


#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <utility>
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures {
    string feature1, feature2;
    float corrlation;
    Line lin_reg;
    Point circleCenter;
    float threshold;

    correlatedFeatures(string feature1, string feature2, float corrlation, Line lin_reg, Point circleCenter,
                       float threshold) :
            feature1(std::move(feature1)), feature2(std::move(feature2)), corrlation(corrlation), lin_reg(lin_reg),
            circleCenter(circleCenter), threshold(threshold) {}
};

class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {


protected:

    vector<correlatedFeatures> cf;
    float pearsonThreshold = 0.9;

public:
    float getPearsonThreshold() const;

    void setPearsonThreshold(float pearsonThreshold);

protected:
    /**
     * Calculate max deviation of a given 2 columns which represent x and y values and a line.
     * @param firstVec, secondVec x value and y value of the points as vec.
     * @param size the size of the vectors.
     * @param regLine the line to calculate max deviation.
     * @return the max deviation.
     */
    static float calculateDeviation(const std::vector<float> &firstVec, const std::vector<float> &secondVec, int size,
                                    const Line &regLine);

    /**
    * Add correlated feature to the correlated features vector.
    * @param firstColumn, secondColumn the columns name as string.
    * @param pearson the correlation of the two columns.
    * @param regLine the regression line.
    * @param maxDev the max deviation of the two columns.
    */
    virtual void addCF(const TimeSeries &ts, const int &firstId, const int &secondId, const float &pearson);

public:

    /**
     * Constructor
     */
    SimpleAnomalyDetector() = default;

    /**
     * Deconstruct
     */
    virtual ~SimpleAnomalyDetector() = default;

    /**
     * Train the model on a given table.
     * @param ts TimeSeries on which we learn
     */
    virtual void learnNormal(const TimeSeries &ts);

    /**
     * Detect false points on a given table.
     * @param ts the table to detect false points on.
     * @return a vector of all the false points (AnomalyReport).
     */
    virtual vector<AnomalyReport> detect(const TimeSeries &ts);

    /**
     * @return The correlated feature
     */
    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }


    virtual bool isAnomaly(float x, float y, const correlatedFeatures &correlatedFeature);
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
