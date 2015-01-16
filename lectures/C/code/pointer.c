int main()
{
   int *p, *q;
   int n;

   n = 0x20200000;
   p = n;
   q = p + 1;
   q = 2 * p;
   n = q - p;
   if( p == q )
       ;
   if( p != q )
       ;
   if( p < q ) 
       ;
   if( p == 0 )
       ;
   if( p == 1 )
       ;
   q = p & (~3);
   q = p | q;
   n = q;
   p = &(n+1);
   n = *&n;
   p = &*p;
}

