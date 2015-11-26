// A simple program that is missing the `var` in the variable declaration.
//
// Expected behavior:
//  * Compiler error on line 9: Missing the `var`, so likely an
//    unexpected identifier symbol

begin
    begin
        x : int;
        skip
    end
end

