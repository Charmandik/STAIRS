#ifndef TERMINALBLOCK_H
#define TERMINALBLOCK_H
#include <QString>
#include <QWidget>

class TerminalBlock: public QWidget
{
public:
    TerminalBlock();
    ~TerminalBlock();
    QString terminalBlockName;
    double price = 0;
    unsigned int count = 0;
//signals:
//    void deleteFromVector();

};

#endif // TERMINALBLOCK_H
