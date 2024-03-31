n = 10 # number of fibonacci terms
f0 = 0 # fibonacci number (1st term)
f1 = 1 # fibonacci number (2nd term)
while n > 0:
    print(f0) # print the current term
    nth = f0 + f1 # the nth term
    f0 = f1  # update first
    f1 = nth # update second
    n -= 1; # decrement number of terms left
