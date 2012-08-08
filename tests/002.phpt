--TEST--
override_function() function: internal function
--FILE--
<?php
if (!extension_loaded('override')) {
    dl('override.' . PHP_SHLIB_SUFFIX);
}

echo phpversion(), "\n";
$ret = override_function('phpversion','','return "PHP VERSION";');
var_dump($ret);
echo phpversion(), "\n";
--EXPECTF--
5.%d.%d
bool(true)
PHP VERSION
