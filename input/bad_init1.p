// A test for checking whether a variable is initialized before use.
//
// Expected behavior: 
//  *  Compiler error on line 12: `i` is used uninitialized
begin
     var i : int;
     var j : int;
     var b : bool;

     j := 1;

     j := i;
     b := i < j;
     i := j
end

