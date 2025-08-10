import unittest
import task1

class TestTask1(unittest.TestCase):
    def test_no_emotions(self):
        data = 'nobody hi'
        result = 0
        print(task1.solve(data))
        self.assertEqual(result, task1.solve(data))

    def test_one_emotions(self):
        data = 'X-\\'
        result = 1
        print(task1.solve(data))
        self.assertEqual(result, task1.solve(data))

    def test_many_emotions(self):
        data = 'X-\\X-\\X-\\X-\\X-\\X-\\'
        result = 6
        print(task1.solve(data))
        self.assertEqual(result, task1.solve(data))

    def test_emotions_with_splits(self):
        data = '[]X-\\ hi XXX-\\\\\\     X-\\X-\\X-\\--\\'
        result = 5
        print(task1.solve(data))
        self.assertEqual(result, task1.solve(data))

    def test_broken_emotions(self):
        data = 'Hello X- \\ X-0 X -\\'
        result = 0
        print(task1.solve(data))
        self.assertEqual(result, task1.solve(data))


if __name__ == '__main__':
    unittest.main()