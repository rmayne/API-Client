<?php
require_once('../include/TbsApi.inc');

class TbsApiTest extends PHPUnit_Framework_TestCase
{

	function testGetInstance(){
		$this->TbsApi = TbsApi::getInstance();
		$this->assertInstanceOf('TbsApi', $this->TbsApi, 'Singleton behavior not functional.');
		return $this->TbsApi;
	}

	/**
	* @depends testGetInstance
	*/
	function testGetBasesList($TbsApi){
		$this->assertEquals('array', gettype($TbsApi->getBasesList()), "Failed to retrieve list of bases");
	}

	function testGetResource(){}

	function testPost(){}

	function testPut(){}

	function testLookupPlanType(){}

	function testLookupSimplePLan(){}

	function testLookupInvoiceCategory(){}

	function testLookupTransClass(){}

	function testLookupTaxTransType(){}

	function testLookupTaxServType(){}

	function testGetAspDate(){}

	function testGetHumanDate(){}

	/*
	* FUTURE TESTS
	*/

	//testDateConversion functions
	//function testLookupProductTypes(){}

	//test filter functions

	//function testLookupCountryCodes(){}

	//function testLookupAddressClasses(){}

	//function testLookupLineTypes(){}

	//function testLoopupServiceTypes(){}

	//function testLookupProductDetails(){}

	//function testLookupProducts(){}
	
}
?>