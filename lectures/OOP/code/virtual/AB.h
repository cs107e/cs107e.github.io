class Base {       
  public:      
    int x;
    int y;
    virtual int f(int);
};

class A : public Base {
  public:
    int f(int);
};

class B : public Base {
  public:
    int f(int);
};

