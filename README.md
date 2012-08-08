# Override function Extension for PHP #

This extension allows overrides functions.

## Function ##

* override\_function — Overrides functions

### override\_function — Overrides functions ###

#### Description ####

bool **override\_function** ( string _$name_ , string _$args_ , string _$code_ [ , string _$origin_ ] )

Overrides functions by replacing them in the symbol table.

#### Pameters ####

* _name_

  The function to override.

* _args_

  The function arguments, as a comma separated string.

* _code_

  The new code for the function.

* _origin_

  Original function to rename.

#### Return Values ####

Returns TRUE on success or FALSE on failure.

## Examples ##

    override_function('test', '$a,$b', 'echo "TEST"; return $a * $b;');
    override_function('test', '$a,$b', 'echo "TEST"; return $a * $b;', 'origin_test');
