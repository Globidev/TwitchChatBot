#include "stdafx.h"

#include "ChatBot.h"
#include "ChatRoom.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ChatBot globibot("globibot", 
                     "32g35w5dtbptxveuuovuo82as753mb3",
                     "ouf5m8taz9e5vo0rk6fxgw6e2qxma5g");

    ChatRoom room(globibot, "globinette");
    

    QLineEdit lineEdit;
    QObject::connect(&lineEdit, &QLineEdit::returnPressed, [&]
    {
        room.sendPrivateMessage(lineEdit.text());
        lineEdit.clear();
    });
    lineEdit.show();
    
    return a.exec();
}
