/*
 * Created by:
 * Shaked Cohen, Shakedc159@gmail.com.
 * Noam Cohen, cohennoam48@gmail.com.
*/

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
    DefaultIO* dio;
    vector<Command*> commands;
    // you can add data members
public:
    CLI(DefaultIO* dio);
    void start();
    virtual ~CLI();
};

#endif /* CLI_H_ */
