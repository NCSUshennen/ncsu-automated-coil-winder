#file -- use_test_class.py --

#import everything from given file test_class.py then use that class
from test_class import *

testMyClass = MyClass()
testMyClass.test_func(2)

#import everything from given file test_class.py
#from test_class import *
#test_func() #prints "Hello"
#print(myvar)      #prints 42

#import the file test_class.py and use it to call things in that file
#different syntax
#import test_class
#test_class.test_func()  #prints "Hello!"
#print (test_class.myvar)  #prints 42