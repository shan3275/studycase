#!/usr/bin/python
# Filename:hello.py
import sys
from PyQt4.QtGui import *
 
class TestWidget(QWidget):
    def __init__(self):
        QWidget.__init__(self, windowTitle=u"A Simple Example for PyQt.")
        self.outputArea=QTextBrowser(self)
        self.helloButton=QPushButton(self.trUtf8("Greetings(&S)"), self)
        self.setLayout(QVBoxLayout())
        self.layout().addWidget(self.outputArea)
        self.layout().addWidget(self.helloButton)
 
        self.helloButton.clicked.connect(self.sayHello)
 
    def sayHello(self):
        yourName, okay=QInputDialog.getText(self, self.trUtf8("What's your name?"), self.trUtf8(b"name"))
        if not okay or yourName==u"": 
            self.outputArea.append(self.trUtf8("Hello,stranger!"))
        else:
            self.outputArea.append(self.trUtf8("Hello,<b>%1</b>.").arg(yourName))
 
app=QApplication(sys.argv)
testWidget=TestWidget()
testWidget.show()
sys.exit(app.exec_())

