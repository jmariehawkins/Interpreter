// A program that prints out the first 100 natural numbers and whether each is
// prime.
// 
// Expected behavior:
//  *  Compiles without error
//  *  For the first 100 natural numbers (starting from 2):
//     *  Print the number
//     *  Print whether the number is prime or not
// 

begin
    var START : int     // The starting point 
 ;  var COUNT : int     // How many numbers to process 
 
 ;  var prime : bool    // Is the current value prime? 
 ;  var num   : int     // The current number 
 ;  var i     : int     // Our current-number iteration variable 
 ;  var j     : int     // Our "check if prime" iteration variable 


 ;  START := 2
 ;  COUNT := 100

 ;  num := START
 ;  i   := 0
 ;  while i != COUNT do
               print num

            ;  prime := true
            ;  j     := 2
            ;  while j * j =< num  and  prime do
                       prime := not num mod j = 0
                    ;  j      := j + 1
               od
            ;  print prime
            ;  num := num + 1
            ;  i   := i + 1
    od
end

