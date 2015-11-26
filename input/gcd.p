// Compute greatest common divisor using Euclid's algorithm
//
// Expected behavior: prints the GCD of a and b (18)

begin
   var a : int;
   var b : int;

   a := 54;
   b := 36;
   while a != b do
      if a > b then a := a - b
      else b := b - a
      fi
   od;
   print a
end