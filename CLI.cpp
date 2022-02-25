/*
 * Created by:
 * Shaked Cohen, Shakedc159@gmail.com.
 * Noam Cohen, cohennoam48@gmail.com.
*/

#include "CLI.h"

CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
    commands.push_back(new uploadCSVCommand(this->dio));
    commands.push_back(new algorithmSettingsCommand(this->dio));
    commands.push_back(new detectAnomaliesCommand(this->dio));
    commands.push_back(new displayResultsCommand(this->dio));
    commands.push_back(new uploadAnomaliesCommand(this->dio));
    commands.push_back(new exitCommand(this->dio));
}

void CLI::start() {
    Database db;

    int nextCommand = 0;
    do {
        // Print the menu.
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");

        int commandNum = 1;
        for (auto command: commands) {
            dio->write(to_string(commandNum) + "." + command->description);
            commandNum++;
        }

        string input = dio->read();

        // Get the next command from the user.
        nextCommand = input[0] - '0' - 1;

        // Check if the command valid.
        if (nextCommand >= 0 && nextCommand < commands.size()) {

            // Execute the command.
            commands[nextCommand]->execute(db);
        }
    } while (nextCommand != 5);

}


CLI::~CLI() {

    // Clear all the commands
    commands.clear();
}

