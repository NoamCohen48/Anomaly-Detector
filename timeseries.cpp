/*
 * Created by:
 * Shaked Cohen, Shakedc159@gmail.com.
 * Noam Cohen, cohennoam48@gmail.com.
*/

#include "timeseries.h"

/**
 * Read a CSV file and fill a table with the info.
 * @param fileName the file name.
 * @return a table of the csv file.
 */
std::vector<std::pair<std::string, std::vector<float>>> TimeSeries::readCsv(const std::string &fileName) {

    // Create a vector of pairs to store the results
    std::vector<std::pair<std::string, std::vector<float>>> result;

    // Open input stream
    std::ifstream myFile(fileName);

    // Check that the open succeed
    if (!myFile.is_open()) throw std::runtime_error("Could not open file");

    std::string line, columnName;

    // IF there is column
    if (myFile.good()) {

        // Get the first line in the file
        std::getline(myFile, line);

        // Create a stringstream
        std::stringstream stringStream(line);

        // Get all the columns name
        while (std::getline(stringStream, columnName, ',')) {

            // Initialize and add the pairs to result
            result.emplace_back(columnName, std::vector<float>{});
        }
    }

    // Read line
    while (std::getline(myFile, line)) {

        // Create a stringstream of the current line
        std::stringstream stringStream(line);

        // Get the current column index
        int columnId = 0;
        float val;

        // Get the data
        while (stringStream >> val) {

            // Add the current data to the column's values vector
            result.at(columnId).second.push_back(val);

            // If the next token is a comma, ignore it and move on (empty tile)
            if (stringStream.peek() == ',') stringStream.ignore();

            // Increment the column index
            columnId++;
        }
    }

    // Close the file
    myFile.close();

    return std::move(result);
}

/**
 * Get a column from table.
 * @param columnName the column title - name.
 * @return the column as vector.
 */
const std::vector<float> &TimeSeries::getColumn(const std::string &columnName) const {

    // Run throw all the tables
    for (const auto &pair: table) {

        // Check if this is the column we need
        if (pair.first == columnName) {
            return pair.second;
        }
    }
    throw std::runtime_error("column name wasn't found");
}

/**
 * Get the amount of columns.
 * @return unsigned long long that represent the amount of columns.
 */
unsigned long long int TimeSeries::getColumnsSize() const {
    return table.size();
}

/**
 * Get the amount of rows.
 * @return unsigned long long that represent the amount of rows.
 */
unsigned long long int TimeSeries::getRowsSize() const {

    // Check if there is no column
    if (getColumnsSize() <= 0) {
        return 0;
    }

    // Else return the first column size (all the columns have the same size)
    return table[0].second.size();
}

/**
 * Get a column from table by ID.
 * @param id the column number.
 * @return the column as vector.
 */
const std::vector<float> &TimeSeries::getColumn(int id) const {

    // Check if there is no id
    if (id < 0 || getColumnsSize() <= id) {

        throw std::runtime_error("Id is incorrect");
    }
    return table[id].second;
}

/**
 * Get the title of a column by ID.
 * @param id the column id - number.
 * @return the name of the column.
 */
std::string TimeSeries::getColumnName(int id) const {

    // Check if there is no id
    if (id < 0 || getColumnsSize() <= id) {

        throw std::runtime_error("Id is incorrect");
    }
    return table[id].first;
}

