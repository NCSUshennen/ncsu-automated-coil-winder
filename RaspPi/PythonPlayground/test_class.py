#file  -- test_class.py --
class MyClass:
    helloCount = 0

    def __init__(self):
       self.helloCount = 1

    def test_func(self, givenHelloCount):
        for i in range(0,givenHelloCount):
            print("Hello!")