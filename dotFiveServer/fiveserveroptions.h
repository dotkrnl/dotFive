#ifndef FIVESERVEROPTIONS_H
#define FIVESERVEROPTIONS_H

#include <QObject>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QString>

#include "fiveconstants.h"

class FiveServerOptions : public QObject
{
    Q_OBJECT

public:
    explicit FiveServerOptions(QCoreApplication *app,
            QObject *parent = 0);

    const QString get(const QString &name) const {
        return m_parser.value(name);
    }

protected:
    QCommandLineParser m_parser;

signals:

public slots:

};

#endif // FIVESERVEROPTIONS_H
