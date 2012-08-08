--TEST--
override_function() function
--FILE--
<?php
if (!extension_loaded('override')) {
    dl('override.' . PHP_SHLIB_SUFFIX);
}

function override_test($a) {
    echo "a is $a\n";
}

override_test(1);
$ret = override_function('override_test','$b','echo "b is $b\n";');
var_dump($ret);
override_test(2);
--EXPECT--
a is 1
bool(true)
b is 2
