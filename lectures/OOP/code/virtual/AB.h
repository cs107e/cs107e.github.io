class Base {       
  public:      
    int x;
    int y;
    virtual int f(int) = 0;
};

class A : public Base {
  public:
    int f(int);
};

class B : public Base {
  public:
    int f(int);
};

