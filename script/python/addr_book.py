#!/usr/bin/python
#Filename : addr_book.py
#Function : This program can add, modify, del and search your contact
#	    persons.Personal information include phone number.
#History  : 2013/04/10     shan      V0.1

import cPickle as p
import os
import sys

Version = 'V0.1'
Help    = '''\
This program prints files to the standard output
Any number of files can be specified.
Options include:
--version: Prints the version number
--help: Display this help
add name number
del name
modify name number
search name
For example:
	python addr_book.py add shan 15915491613
	python addr_book.py del shan
	python addr_book.py modify meng 13433333333
	python search meng'''

#First stat personal information class
class Person:
	'''represents person information'''
	def __init__(self, name, number):
		self.name   = name
		self.number = number
		print 'Initialized person: %s, number: %d' %s(self.name, self.number)

#create a null dictionary
'''
addr_book_test = {'ab'  :32232333234,
	     'shan':15915492613,
	     'meng':13568873833}
'''
addr_book_dict = {}
#address book database
addr_book_file = 'addr_book.database'

#test write dict to file
#f = file(addr_book_file, 'w')
#p.dump(addr_book_test, f)
#f.close
#first reload addr_book.database data
if os.path.isfile(addr_book_file):
	f = file(addr_book_file)
	addr_book_dict = p.load(f)
#	for name, number in addr_book_dict.items():
#		print 'Person %s, number %d' %(name, number)
#else:
#	print 'no reload'

if len(sys.argv) < 2:
	print 'No action specified.'
	sys.exit()

if sys.argv[1].startswith('--'):
	option = sys.argv[1][2:]
	#fetch sys.argv[1], but without the first two characters
	if option == 'version':
		print Version
	elif option == 'help' :
		print Help
	else:
		print 'Unknown option.'
	sys.exit()
elif len(sys.argv) == 3:
	option = sys.argv[1]
	if option == 'del':
		del addr_book_dict[sys.argv[2]]
		print 'Delete %s' % sys.argv[2]
	elif option == 'search':
		print 'Person %s, number %d' %(sys.argv[2], addr_book_dict[sys.argv[2]])
	else:
		print 'Unknown option'
	#sys.exit()
elif len(sys.argv) == 4:
	option = sys.argv[1]
	if option == 'add':
		addr_book_dict[sys.argv[2]] = int(sys.argv[3])
	elif option == 'modify':
		addr_book_dict[sys.argv[2]] = int(sys.argv[3])
	else:
		print 'Unknown option'
	#sys.exit()
else:
	print 'Error parameter'
	print Help

f = file(addr_book_file, 'w')
p.dump(addr_book_dict, f)
f.close

