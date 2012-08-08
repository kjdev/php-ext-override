--TEST--
override_function() function: multi
--FILE--
<?php
if (!extension_loaded('override')) {
    dl('override.' . PHP_SHLIB_SUFFIX);
}

function override_test($a) {
    echo "a is $a\n";
}

function override_sample($a) {
    echo "sample(a) is $a\n";
}

override_test(1);
override_sample(10);
$ret = override_function('override_test','$b','echo "b is $b\n";');
var_dump($ret);
$ret = override_function('override_sample','$b','echo "sample(b) is $b\n";');
var_dump($ret);
override_test(2);
override_sample(20);
--EXPECT--
a is 1
sample(a) is 10
bool(true)
bool(true)
b is 2
sample(b) is 20
