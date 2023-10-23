#ifndef CONSTVARIABLES_H
#define CONSTVARIABLES_H

#include <QString>

class Constants {
public:
    static QString localDirectoryConfigName() {
        return QString("application/local_path");
    };
    static QString domainConfigName() {
        return QString("server/domain");
    };
    static QString clientNameConfigName() {
        return QString("server/client_name");
    };
    static QString clientKeyConfigName() {
        return QString("server/client_key");
    };
};

#endif // CONSTVARIABLES_H
