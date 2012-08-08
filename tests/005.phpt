--TEST--
override_function() function: internal copy origin
--FILE--
<?php
if (!extension_loaded('override')) {
    dl('override.' . PHP_SHLIB_SUFFIX);
}

echo phpversion(), "\n";
$ret = override_function('phpversion','','return "PHP VERSION => " . origin_phpversion();', 'origin_phpversion');
var_dump($ret);
echo phpversion(), "\n";
echo origin_phpversion(), "\n";
--EXPECTF--
5.%d.%d
bool(true)
PHP VERSION => 5.%d.%d
5.%d.%d
