/*
 * Created by:
 * Shaked Cohen, Shakedc159@gmail.com.
 * Noam Cohen, cohennoam48@gmail.com.
*/

#include "HybridAnomalyDetector.h"



HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

Point **HybridAnomalyDetector::toPoints(vector<float> x, vector<float> y) {
    Point **ps = new Point *[x.size()];
    for (size_t i = 0; i < x.size(); i++) {
        ps[i] = new Point(x[i], y[i]);
    }
    return ps;
}

void HybridAnomalyDetector::addCF(const TimeSeries &ts, const int &firstId, const int &secondId, const float &pearson) {
    // if no correlation
    if (pearson < 0.5) {
        return;
    }

    // if line evaluation
    if (pearsonThreshold <= pearson) {
        SimpleAnomalyDetector::addCF(ts, firstId, secondId, pearson);
        return;
    }

    // circle evaluation

    // get the columns
    const std::vector<float> &firstVec = ts.getColumn(firstId);
    const std::vector<float> &secondVec = ts.getColumn(secondId);
    Point **points = toPoints(firstVec, secondVec);

    // adding correlated Feature
    string firstName = ts.getColumnName(firstId);
    string secondName = ts.getColumnName(secondId);
    Circle circle = findMinCircle(points, firstVec.size());

    // adding the cf
    cf.emplace_back(firstName, secondName, pearson, Line(),
                    circle.center, circle.radius * 1.1f);
}

bool HybridAnomalyDetector::isAnomaly(float x, float y, const correlatedFeatures &cf) {
    // if the simple evaluation
    if (pearsonThreshold <= cf.corrlation) {
        return SimpleAnomalyDetector::isAnomaly(x, y, cf);
    }

    // if Circle evaluation
    bool isInside = Point(x, y).dist(cf.circleCenter) < cf.threshold;
    if (!isInside)
        return true;
    return false;
}