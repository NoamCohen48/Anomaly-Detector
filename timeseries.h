/*
 * Created by:
 * Shaked Cohen, Shakedc159@gmail.com.
 * Noam Cohen, cohennoam48@gmail.com.
*/


#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <string>
#include <fstream>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream

using namespace std;

class TimeSeries {
private:
    std::string CSVfileName;
    std::vector<std::pair<std::string, std::vector<float>>> table;

    /**
     * Read a CSV file and fill a table with the info.
     * @param fileName the file name.
     * @return a table of the csv file.
     */
    static std::vector<std::pair<std::string, std::vector<float>>> readCsv(const std::string &fileName);

public:

    explicit TimeSeries(const std::string &CSVfileName) {
        this->CSVfileName = CSVfileName;
        table = readCsv(CSVfileName);
    }

    /**
     * Get a column from table by ID.
     * @param id the column number.
     * @return the column as vector.
     */
    const std::vector<float> &getColumn(int id) const;

    /**
     * Get a column from table.
     * @param columnName the column title - name.
     * @return the column as vector.
     */
    const std::vector<float> &getColumn(const std::string &columnName) const;

    /**
     * Get the title of a column by ID.
     * @param id the column id - number.
     * @return the name of the column.
     */
    std::string getColumnName(int id) const;

    /**
     * Get the amount of columns.
     * @return unsigned long long that represent the amount of columns.
     */
    unsigned long long int getColumnsSize() const;

    /**
     * Get the amount of rows.
     * @return unsigned long long that represent the amount of rows.
     */
    unsigned long long int getRowsSize() const;
};


#endif /* TIMESERIES_H_ */
