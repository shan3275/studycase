from cmd import Cmd
import os
import sys

class Cli(Cmd):
    def __init(self):
        Cmd.__init__(self)

    def do_hello(self,line):
        print "hello",line

    def do_bye(self,line):
        print "Bye Bye",line    

if __name__ == '__main__':
    cli = Cli()
    cli.cmdloop()
