class A(object):
  def __init__(self, x, y):
    self.x = x
    self.y = y   

a = A(1,2)

print(a)
print(type(a))
print(type(A))
print(type(type))

print(isinstance(a, A))
print(isinstance(A, type))
print(issubclass(A, object))
