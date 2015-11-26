// A program that attempts to perform multi-assignment, which is not
// permitted by the P language.
//
// Expected behavior:
//  *  Produces a compiler error on line 13 due to the comma.

begin
    var i : int
 ;  var j : int
 ;  i := 3
 ;  j := 4
 
 ;  i, j := j, i    // ERROR
end
