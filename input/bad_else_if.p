// A sample program that attempts to use an "else if" syntax, which is not
// permitted by the grammar. Instead, the grammar requires that each `if`
// only be paired with one `else`, and the nested `if` in the `else if` must
// be written and treated as a separate `if-then-else` structure.
//
// Expected behavior:
//  *  Compiler error on line 18 due to a missing `fi` (to match the `if` on
//     line 11).

begin
    if 1 = 2 then
        print 1
    else if 1 = 3 then
        print 2
    else 
        print 3
    fi
end
