<?php

//require_once 'vendor/autoload.php';
require_once "/Users/hhdtemp12/Sites/devdesktop/drupal-7.34/sites/all/libraries/sausage/vendor/autoload.php";

define('SAUCE_HOST', 'nd49:8f872f51-97a0-4f6a-ba30-f67ef68e3912@ondemand.saucelabs.com');

class WebTest extends PHPUnit_Extensions_Selenium2TestCase
{
 protected $start_url = 'http://saucelabs.com/test/guinea-pig';

 public static $browsers = array(
        array(
            'browserName' => 'firefox',
            'host' => SAUCE_HOST,
            'port' => 80,
            'desiredCapabilities' => array(
                'version' => '15',
                'platform' => 'Windows 2012',
                'record-screenshots' => FALSE,
                'record-video' => FALSE
            )
        )
    );

    protected function setUp()
    {
        $this->setBrowserUrl('');  
    }

    public function testTitle()
    {
        //echo print_r($_SERVER['argv'], TRUE);
        $param = $_SERVER['argv'];
        $url = $param[2];
        $this->url($url);
        $this->assertContains("I am a page title", $this->title());
        //$this->takeScreenshot();
        $this->currentScreenshot();
    }
}
